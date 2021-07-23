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

### Usage
```sh
$ typewriter-kb  # Default volume is 0
[sudo] password for user:
Start in background

$ typewriter-kb-config setvolume 10
Setting volume to 10%

$ typewriter-kb-config shutdown    
Shutting down
```
