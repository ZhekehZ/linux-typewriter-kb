const { Gio, GObject } = imports.gi;
const GLib = imports.gi.GLib;

var TypewriterProcess = 
    class TypewriterProcess {
        constructor() {
            this._subprocessInput = null;
            
            this._kb_reader = '/opt/typewriter_keyboard/kb_read';
            this._typewriter_sound = '/opt/typewriter_keyboard/tw_snd';
        }
        
        start() {
            try { 
                this._startProcesses(); 
            } catch(e) {
                log('[Typewriter-kb]  ' + e);
            } 
            return this._isRunning();
        }

        stop() {
            if (this._isRunning()) {
                try { 
                    this._stopProcesses(); 
                } catch(e) {
                    log('[Typewriter-kb]  mb it\'s ok, but ' + e);
                } 
            }
            this._subprocessInput = null;
        }

        subprocessInput() {
            return this._subprocessInput;
        }

        _isRunning() {
            if (this._subprocessInput) return true;
            return false;
        }
        
        _onProcessStarted(name, success, pid) {
            log('[Typewriter-kb]  Starting ' + name + ' process: ' + success + ', pid = ' + pid);
                    
            if (!success) return;
            
            GLib.child_watch_add( GLib.PRIORITY_DEFAULT, pid,
                function(pid, status) {
                    GLib.spawn_close_pid(pid);
                    log('[Typewriter-kb]  Subprocess ' + name + ' finished, status = ' + status);
                });
        }

        _stopProcesses() {
            if (this._subprocessInput) {
                let status, bytes_written;
                [status, bytes_written] = this._subprocessInput.write_all('exit\n', null);
                this._subprocessInput.flush(null);
                this._subprocessInput.close(null);
                log('[Typewriter-kb]  Stopping process: ' + status);
            }
            this._subprocessInput = null;
        }

        _testFilesExists() {
            return GLib.file_test(this._kb_reader, GLib.FileTest.EXISTS) &&
                GLib.file_test(this._kb_reader, GLib.FileTest.IS_EXECUTABLE) &&
                GLib.file_test(this._typewriter_sound, GLib.FileTest.EXISTS) &&
                GLib.file_test(this._typewriter_sound, GLib.FileTest.IS_EXECUTABLE);
        }

        _startProcesses() {
            if (!this._testFilesExists()) {
                log('[Typewriter-kb]  typewriter-kb not found!');
                return;
            }

            let success, argv, pid, stdin, stdout, stderr;

            [success, pid, stdin, stdout, stderr] = GLib.spawn_async_with_pipes(
                null, ['pkexec', this._kb_reader], null, 
                GLib.SpawnFlags.SEARCH_PATH | GLib.SpawnFlags.DO_NOT_REAP_CHILD, null);
                
            GLib.close(stderr);
            this._subprocessInput = new Gio.UnixOutputStream({ fd: stdin, close_fd: true });

            this._onProcessStarted('kb_read', success, pid);

            [success, pid] = GLib.spawn_async_with_fds(
                null, [this._typewriter_sound], null, 
                GLib.SpawnFlags.SEARCH_PATH | GLib.SpawnFlags.DO_NOT_REAP_CHILD,
                null, stdout, -1, -1);

            this._onProcessStarted('tw_snd', success, pid);
        }

    };
