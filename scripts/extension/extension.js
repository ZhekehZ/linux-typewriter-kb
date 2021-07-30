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
        this._typewriter = new TypewriterProcess(Me.path);
        if (this._typewriter.start()) {
            this._indicator = new Indicator(this._typewriter.subprocessInput());
            Main.panel.statusArea.aggregateMenu.menu.addMenuItem(this._indicator.menu, 2);
        }
        log('[Typewriter-kb]  Extension started');
    }
    
    disable() {
        this._typewriter.stop();
        this._indicator.menu.destroy();
        this._indicator.destroy();

        this._indicator = null;
        this._typewriter = null;
    }
}


function init() {
    return new Extension();
}
