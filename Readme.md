<img src="https://raw.githubusercontent.com/ZhekehZ/linux-typewriter-kb/main/assets/typewriter.svg" 
  width="32" height="32" alt="Icon">
Linux typewriter-sound keyboard 
------
> (Icons made by [Freepik](https://www.freepik.com)  from [Flaticon](https://www.flaticon.com/))  
>   
> [![CMake](https://github.com/ZhekehZ/linux-typewriter-kb/actions/workflows/cmake.yml/badge.svg)](https://github.com/ZhekehZ/linux-typewriter-kb/actions/workflows/cmake.yml) 

### Prerequirements (only for build)

```sh 
sudo apt-get install libsdl2-dev libsdl2-mixer-dev xxd
```

### Building and installing .deb package
```sh
mkdir -p build 
cmake -S . -B build
cmake --build build --target DEB_PACKAGE

sudo apt install ./build/*.deb
```

### Building and installing gnome extension
```sh
mkdir -p build 
cmake -S . -B build
cmake --build build --target GNOME_EXTENSION

ln -s "$( pwd )"/build/*.com ~/.local/share/gnome-shell/extensions/ 
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

### Extension UI (the bottom slider) 
![image](https://user-images.githubusercontent.com/14273096/129383204-f8e67c2d-6000-46ef-b319-e9144497ae6a.png)
 

