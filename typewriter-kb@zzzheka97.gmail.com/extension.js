const GLib = imports.gi.GLib;

const ExtensionUtils = imports.misc.extensionUtils;
const Me = ExtensionUtils.getCurrentExtension();
const Main = imports.ui.main;
const Slider = Me.imports.volumeSlider.Indicator;


class Extension {
    constructor() {
        this._indicator = null;    
    }
    
    enable() {
        this._indicator = new Slider();
        Main.panel.statusArea.aggregateMenu.menu.addMenuItem(this._indicator.menu, 2);
        this._startTypewriter();
    }
    
    disable() {
        this._stopTypewriter();
        this._indicator.menu.destroy();
        this._indicator = null;
    }

    _stopTypewriter() {
        GLib.spawn_command_line_async('typewriter-kb stop');
    }

    _startTypewriter() {
        GLib.spawn_command_line_async('typewriter-kb start --gnome');
    }
}


function init() {
    return new Extension();
}
