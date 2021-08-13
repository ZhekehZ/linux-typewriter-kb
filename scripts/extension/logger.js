const GLib = imports.gi.GLib;
const DEBUG_MODE = GLib.getenv("SHELL_DEBUG");

var Logger = 
    class Logger {
        static logError(message) {
            log(`[Typewriter-kb] [ERROR] ${message}`);
        }

        static logInfo(message) {
            if (DEBUG_MODE) {
                log(`[Typewriter-kb] [INFO] ${message}`);
            }
        }
    };
