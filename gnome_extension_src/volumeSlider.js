const { GObject, St, Gio } = imports.gi;
const GLib = imports.gi.GLib;

const PanelMenu = imports.ui.panelMenu;
const PopupMenu = imports.ui.popupMenu;
const Slider = imports.ui.slider;

const ExtensionUtils = imports.misc.extensionUtils;
const Me = ExtensionUtils.getCurrentExtension();
const Logger = Me.imports.logger.Logger;

const CONFIG_PIPE_PATH = '/tmp/typewriter-kb-pipe';
const WRITE_FREQUENCY = 100;

var Indicator = GObject.registerClass(
    class Indicator extends PanelMenu.SystemIndicator {
        _init(output) {
            super._init();

            this._lastUpdate = 0;
            this._output = output;
            
            this._item = new PopupMenu.PopupBaseMenuItem({ activate: false });
            this._item.add(this._loadIcon());
            this.menu.addMenuItem(this._item);

            this._slider = new Slider.Slider(0.5);
            this._slider.accessible_name = _("Typewriter keyboard volume");
            this._item.add_child(this._slider);
        }

        connect(menu, position) {
            menu.addMenuItem(this.menu, position);

            this._sliderChangedId = this._slider.connect('notify::value',
                        this._sliderChanged.bind(this));
            this._sliderReleaseId = this._slider.connect('button-release-event', 
                        this._sliderChangedNow.bind(this));

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

        _loadIcon() {
            let iconPath = `${Me.path}/typewriter-symbolic.svg`;
            let gicon = Gio.icon_new_for_string(iconPath);

            let icon = new St.Icon({ icon_name: 'process-working-symbolic',
                                     style_class: 'popup-menu-icon' });

            icon.set_gicon(gicon);            
            return icon;
        }

        disconnect() {
            if (this._slider) {
                this._slider.destroy();
                this._slider = null;
            }
            if (this._item) {
                this._item.destroy();
                this._item = null;
            }
            this._output = null;
        }

        _sliderChanged() {
            let currentTime = Date.now();
            let delta = currentTime - this._lastUpdate;
            if (delta > WRITE_FREQUENCY) {
                this._lastUpdate = currentTime;
                this._sendChangeVolumeMessage();
            } 
        }

        _sliderChangedNow() {
            this._sendChangeVolumeMessage();
            let percent = Math.floor(this._slider.value * 100);
            Logger.logInfo(`Current volume: ${percent}%`);
        }

        _sendChangeVolumeMessage() {
            if (this._output) {
                let percent = Math.floor(this._slider.value * 100);
                this._output.write_all(`v${percent}\n`, null);
                this._output.flush(null);
            }
        }

        _changeSlider(value) {
            this._slider.block_signal_handler(this._sliderChangedId);
            this._slider.block_signal_handler(this._sliderReleaseId);
            value = value < 0 ? 0 : value > 1 ? 1 : value;
            this._slider.value = value;
            this._slider.unblock_signal_handler(this._sliderReleaseId);
            this._slider.unblock_signal_handler(this._sliderChangedId);
        }
    });