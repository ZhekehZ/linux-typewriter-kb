const { GObject, St } = imports.gi;
const GLib = imports.gi.GLib;

const PanelMenu = imports.ui.panelMenu;
const PopupMenu = imports.ui.popupMenu;
const Slider = imports.ui.slider;

var Indicator = GObject.registerClass(
    class Indicator extends PanelMenu.SystemIndicator {
        _init(output) {
            super._init();

            this._output = output;
            this._lastUpdate = 0;
            this._item = new PopupMenu.PopupBaseMenuItem({ activate: false });
            this.menu.addMenuItem(this._item);
    
            this._slider = new Slider.Slider(0.5);
            this._sliderChangedId = this._slider.connect('notify::value',
                this._sliderChanged.bind(this));
            this._slider.accessible_name = _("Typewriter keyboard volume");
    
            let icon = new St.Icon({ icon_name: 'process-working-symbolic',
                                     style_class: 'popup-menu-icon' });

            this._item.add(icon);
            this._item.add_child(this._slider);

            this._item.connect('button-press-event', (actor, event) => {
                return this._slider.startDragging(event);
            });
            this._item.connect('key-press-event', (actor, event) => {
                return this._slider.emit('key-press-event', event);
            });
            this._item.connect('scroll-event', (actor, event) => {
                return this._slider.emit('scroll-event', event);
            });
        }
    
        _sliderChanged() {
            let currentTime = Date.now();
            let delta = currentTime - this._lastUpdate;
            if (0 <= currentTime && currentTime < 200) {
                return;
            } 

            this._lastUpdate = currentTime;
            let percent = Math.floor(this._slider.value * 100);
            let status, bytes_written;
            [status, bytes_written] = this._output.write_all('v' + percent + '\n', null);
            this._output.flush(null);
        }
    
        _changeSlider(value) {
            this._slider.block_signal_handler(this._sliderChangedId);
            this._slider.value = value;
            this._slider.unblock_signal_handler(this._sliderChangedId);
        }
    });