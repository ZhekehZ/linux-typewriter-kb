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
 
    --gnome                   use gnome auth dialogue
```

### Experimental: gnome extension installation
0. Install .deb package
1. Install extension
    ```sh
    ln -s "$( pwd )/typewriter-kb@zzzheka97.gmail.com" ~/.local/share/gnome-shell/extensions/
    ```
2. Restart gnome: `Alt+F2` and then `r`
3. Change volume with the bottom slider  
![image](https://user-images.githubusercontent.com/14273096/126871481-0bacf5b9-c6c5-40c4-bca8-632bd525e4e6.png)
