const { GObject, St } = imports.gi;
const GLib = imports.gi.GLib;

const PanelMenu = imports.ui.panelMenu;
const PopupMenu = imports.ui.popupMenu;
const Slider = imports.ui.slider;

var Indicator = GObject.registerClass(
    class Indicator extends PanelMenu.SystemIndicator {
        _init() {
            super._init();
            
            this._item = new PopupMenu.PopupBaseMenuItem({ activate: false });
            this.menu.addMenuItem(this._item);
    
            this._slider = new Slider.Slider(0);
            this._sliderChangedId = this._slider.connect('drag-end',
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
            let percent = Math.floor(this._slider.value * 100);
            GLib.spawn_command_line_async('/opt/typewriter_keyboard/tw_config setvolume ' + percent);

            C.open(0, true);
            GLib.spawn_command_line_async('firefox');
        }
    
        _changeSlider(value) {
            this._slider.block_signal_handler(this._sliderChangedId);
            this._slider.value = value;
            this._slider.unblock_signal_handler(this._sliderChangedId);
        }
    });