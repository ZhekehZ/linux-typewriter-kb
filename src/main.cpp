#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "keyboard/kb_linux_fs.hpp"
#include "sound/snd_sdl_mixer.hpp"
#include "sound/snd_wav_sound.hpp"

#include <thread>

int main(){
    snd::SDLRAIIContextManager manager(44100, 8, 1024);
    

    if (!manager) {
        std::cerr << manager.get_error_message() << std::endl;
        return EXIT_FAILURE;
    }

    auto dss = kb::linux_filesystem::open_and_get_all_keyboards();

    if (dss.empty()) {
        std::cerr << "No kb found!" << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "Found " << dss.size() << " keyboard(s)" << std::endl;
    }

    // snd::WAVSound ss[] = {
    //     snd::WAVSound("assets/tap1.wav", 50),
    //     snd::WAVSound("assets/tap2.wav", 50),
    //     snd::WAVSound("assets/tap3.wav", 50)};

    snd::WAVSound down("assets/regular_btn/down/1.wav", 100);
    snd::WAVSound up("assets/regular_btn/up/1.wav", 50);

    snd::WAVSound hold("tap.wav", 50);

    kb::event_reader reader(std::move(dss));
    while (auto next = reader.next()) {
        if (next->kind == kb::Event::DOWN) {
            // int c = rand() % 3;
            // ss[c].play();
            down.play();
        }
        if (next->kind == kb::Event::UP) {
            up.play();
        }
        if (next->kind == kb::Event::PRESSED) {
            
            // hold.play();
        }
    }

    return EXIT_SUCCESS;    
}
