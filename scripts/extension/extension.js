const ExtensionUtils = imports.misc.extensionUtils;
const Me = ExtensionUtils.getCurrentExtension();
const Main = imports.ui.main;

const Indicator = Me.imports.volumeSlider.Indicator;
const TypewriterProcess = Me.imports.typewriterProcess.TypewriterProcess;


class Extension {
    constructor() {
        this._indicator = null;    
        this._typewriter = null;
    }
    
    enable() {
        log('[Typewriter-kb]  Starting extension ... ');
        this._typewriter = new TypewriterProcess();
        if (this._typewriter.start()) {
            this._indicator = new Indicator(this._typewriter.subprocessInput());            
            this._indicator.connect(Main.panel.statusArea.aggregateMenu.menu, 2);
        }
        log('[Typewriter-kb]  Extension started');
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
    }
}


function init() {
    return new Extension();
}
