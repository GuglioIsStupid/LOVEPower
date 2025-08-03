#include <string>
#include "audio.hpp"

namespace love {
    namespace audio {
        AudioSource newSource_file_type(std::string file, std::string type) {
            AudioSource source;
            source.file = file;
            source.type = type;
            return source;
        }

        AudioSource newSource_file(std::string file) {
            return newSource_file_type(file, "static");
        }
    }

    void AudioSource::play() {
    }

    bool AudioSource::isPlaying() const {
        return false;
    }
}
