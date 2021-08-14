const ExtensionUtils = imports.misc.extensionUtils;
const Me = ExtensionUtils.getCurrentExtension();
const Main = imports.ui.main;

const Indicator = Me.imports.volumeSlider.Indicator;
const TypewriterProcess = Me.imports.typewriterProcess.TypewriterProcess;
const Logger = Me.imports.logger.Logger;

const GLib = imports.gi.GLib;

class Extension {
    constructor() {
        this._indicator = null;    
        this._typewriter = null;
    }
    
    enable() {
        Logger.logInfo('Enabling extension ... ');

        this._typewriter = new TypewriterProcess();
        if (this._typewriter.start()) {
            this._indicator = new Indicator(this._typewriter.subprocessInput());            
            this._indicator.connect(Main.panel.statusArea.aggregateMenu.menu, 2);
        }

        Logger.logInfo('Extension enabled');
    }
    
    disable() {
        if (this._typewriter) {
            this._typewriter.stop();
            this._typewriter = null;
        }

        if (this._indicator) {
            this._indicator.disconnect();
            this._indicator.destroy();
            this._indicator = null;
        }

        Logger.logInfo('Extension disabled');
    }
}


function init() {
    return new Extension();
}
