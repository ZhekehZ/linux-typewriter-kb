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

### Building and installing gnome extension
```sh
make gnome-extension # or just download and extract existing one
ln -s "$( pwd )"/build/typewriter-*.com ~/.local/share/gnome-shell/extensions/ 

# Restart gnome: `Alt+F2` and then `r`
# Enable extension via https://extensions.gnome.org/local/
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

### Extension UI (the bottom one slider) 
![image](https://user-images.githubusercontent.com/14273096/127714813-5ef0f4a7-28a3-435c-997b-f7d534e09a65.png)  
 

