const { Gio, GObject } = imports.gi;
const GLib = imports.gi.GLib;

class TypewriterProcess {
    constructor(path) {
        this._subprocessInput = null;
        
        this._command_kb_reader = 'pkexec ' + path + '/kb_read';
        this._command_typewriter_sound = path + '/tw_snd';
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
        return this._subprocessInput != null;
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
        let status, bytes_written;
        [status, bytes_written] = this._subprocessInput.write_all('exit\n', null);
        this._subprocessInput.flush(null);
        this._subprocessInput.close(null);
        log('[Typewriter-kb]  Stopping process: ' + status);
    }

    _startProcesses() {
        let success, argv, pid, stdin, stdout, stderr;

        [success, argv] = GLib.shell_parse_argv(this._command_kb_reader);
        [success, pid, stdin, stdout, stderr] = GLib.spawn_async_with_pipes(
            null, argv, null, GLib.SpawnFlags.SEARCH_PATH | GLib.SpawnFlags.DO_NOT_REAP_CHILD, null);
            
        GLib.close(stderr);
        this._subprocessInput = new Gio.UnixOutputStream({ fd: stdin, close_fd: true });

        this._onProcessStarted('kb_read', success, pid);

        [success, argv] = GLib.shell_parse_argv(this._command_typewriter_sound);
        [success, pid] = GLib.spawn_async_with_fds(
            null, argv, null, GLib.SpawnFlags.SEARCH_PATH | GLib.SpawnFlags.DO_NOT_REAP_CHILD,
            null, stdout, -1, -1);

        this._onProcessStarted('tw_snd', success, pid);
    }

}