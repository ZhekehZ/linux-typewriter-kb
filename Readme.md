## Linux typewriter-sound keyboard 

### Prerequirements (only for build)

```sh 
sudo apt-get install libsdl2-dev libsdl2-mixer-dev xxd
```

### Building and installing .deb package
```sh
make deb # or just download existing one
sudo apt install ./build/typewriter_keyboard.deb
```

### Help
```
Usage:
    typewriter-kb start         -- start typewriter 
    typewriter-kb stop          -- stop typewriter
    typewriter-kb [OPTION]      -- configure typewriter

Options:
    -h, --help                show this help
    -s, --setvolume [VALUE]   set typewriter volume (from 0 to 100)
    -g, --getvolume           returns current typewriter volume
```

### Experimental: gnome extension installation
1. install extension
    ```sh
    ln -s $( pwd )/typewriter-kb@zzzheka97.gmail.com ~/.local/share/gnome-shell/extensions/
    ```
2. Restart gnome: `Alt+F2` and then `r`