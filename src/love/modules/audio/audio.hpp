#pragma once

#include <string>

// TODO: Seperate this into a seperate class file
namespace love {
    struct AudioSource {
        std::string file;
        std::string type;

        // play and isplaying methods
        void play();
        bool isPlaying() const;
    };
    namespace audio {
        AudioSource newSource_file_type(std::string file, std::string type);
        AudioSource newSource_file(std::string file);
    }
}