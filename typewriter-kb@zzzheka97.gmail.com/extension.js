
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
        Main.panel.statusArea.aggregateMenu.menu.addMenuItem(this._indicator.menu);
    }
    
    disable() {
        this._stopTypewriter();
        this._indicator.destroy();
        this._indicator = null;
    }

    _stopTypewriter() {
        GLib.spawn_command_line_sync('typewriter-kb stop');
    }

    _startTypewriter() {
        // GLib.spawn_command_line_sync('typewriter-kb start'); // TODO: ask password and run
    }
}


function init() {
    return new Extension();
}