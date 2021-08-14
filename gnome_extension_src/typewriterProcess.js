const { Gio, GObject } = imports.gi;
const GLib = imports.gi.GLib;

const ExtensionUtils = imports.misc.extensionUtils;
const Me = ExtensionUtils.getCurrentExtension();
const Logger = Me.imports.logger.Logger;

var TypewriterProcess = 
    class TypewriterProcess {
        constructor() {
            this._subprocessStream = null;
            
            this._kb_reader = '/opt/typewriter_keyboard/kb_read';
            this._typewriter_sound = '/opt/typewriter_keyboard/tw_snd';
        }
        
        start() {
            let success, fd;
            [success, fd] = this._startProcesses();
            if (success) { 
                this._subprocessStream = new Gio.UnixOutputStream({ fd: fd, close_fd: true }); 
            } else {
                Logger.logError('Unable to start subprocess');
            } 
            return success;
        }

        stop() {
            this._stopProcesses(); 
            this._subprocessStream = null;
        }

        subprocessInput() {
            return this._subprocessStream;
        }
        
        _handleNewProcess(name, pid) {
            Logger.logInfo(`Subprocess ${name} started`);
            GLib.child_watch_add( GLib.PRIORITY_DEFAULT, pid,
                function(pid, status) {
                    GLib.spawn_close_pid(pid);
                    Logger.logInfo(`Subprocess ${name} finished, status = ${status}`);
                });
        }

        _stopProcesses() {
            try {
                if (this._subprocessStream) {
                    let status, bytes_written;
                    [status, bytes_written] = this._subprocessStream.write_all('exit\n', null);
                    this._subprocessStream.flush(null);
                    this._subprocessStream.close(null);
                    Logger.logInfo(`Stopping process: ${status}`);
                }
            } catch (e) {
                Logger.logError(e);
            }
            this._subprocessStream = null;
        }

        _testFilesExists() {
            return GLib.file_test(this._kb_reader, GLib.FileTest.EXISTS) &&
                GLib.file_test(this._kb_reader, GLib.FileTest.IS_EXECUTABLE) &&
                GLib.file_test(this._typewriter_sound, GLib.FileTest.EXISTS) &&
                GLib.file_test(this._typewriter_sound, GLib.FileTest.IS_EXECUTABLE);
        }

        _startProcesses() {
            if (!this._testFilesExists()) {
                Logger.logError('typewriter-kb not found!');
                return null;
            }
            
            try {
                
                let success, stdin, stdout;
                [success, stdin, stdout] = this._startKeyboardReader();
                if (success) {
                    success = this._startTypewriterSoundPlayer(stdout);
                }
                return [success, stdin];
            
            } catch (e) {
                Logger.logError(e);
                return null;
            }
        }

        _startKeyboardReader() {
            let keyboard_read_command = ['pkexec', this._kb_reader, '--stdin'];
            
            let success, pid, stdin, stderr, stdout;
            [success, pid, stdin, stdout, stderr] = GLib.spawn_async_with_pipes(
                null, keyboard_read_command, null, 
                GLib.SpawnFlags.SEARCH_PATH | GLib.SpawnFlags.DO_NOT_REAP_CHILD, null);
            
            if (success) {
                GLib.close(stderr);
                this._handleNewProcess('keyboard reader', pid);
                return [true, stdin, stdout];
            } 
            
            return [false, null, null];
        }

        _startTypewriterSoundPlayer(stdin) {
            let typewriter_sound_command = [this._typewriter_sound];
            
            let success, pid;
            [success, pid] = GLib.spawn_async_with_fds(
                null, typewriter_sound_command, null, 
                GLib.SpawnFlags.SEARCH_PATH | GLib.SpawnFlags.DO_NOT_REAP_CHILD, null, stdin, -1, -1);

            if (success) {
                this._handleNewProcess('typewriter sound player', pid);
            }
            return success;
        }

    };
