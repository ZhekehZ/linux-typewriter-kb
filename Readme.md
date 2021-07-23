## Linux typewriter-sound keyboard 

### Prerequirements

```sh 
sudo apt-get install libsdl2-dev libsdl2-mixer-dev xxd
```

### Building and installing .deb package
```sh
make deb
sudo apt install ./build/typewriter_keyboard.deb
```

### Help
```sh
    typewriter-kb start         -- start typewriter 
    typewriter-kb stop          -- stop typewriter
    typewriter-kb [OPTION]      -- configure typewriter

Options:
    -h, --help                show this help
    -s, --setvolume [VALUE]   set typewriter volume (from 0 to 100)
    -g, --getvolume           returns current typewriter volume
```
