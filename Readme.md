## linux typewriter-sound keyboard 

### Build and run

```sh 
sudo apt-get install libsdl2-dev libsdl2-mixer-dev xxd
<<<<<<< HEAD
make app
sudo ./build/run
```
*__Note__* : if you cannot play sound as a superuser, you can assemble the parts of the application separately and run as follows:  
```
make separate
sudo ./build/kb_read | ./build/tw_snd
```
=======
make
sudo ./build/run
```
>>>>>>> f6457f03f580f0a187c248195db5ccab955f1cd6
