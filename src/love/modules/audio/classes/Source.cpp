#include "Source.hpp"

#include "../../../modules/filesystem/filesystem.hpp"

#include <string>

namespace love {
    namespace audio {
        Source::Source(std::string file, std::string mode) {
            std::string path = filesystem::getFilePath(file);
            audiogc::mode fileMode;
            if (mode == "static")
                fileMode = audiogc::mode::store;
            else
                fileMode = audiogc::mode::stream;

            audiogcPlayer = new audiogc::player(audiogc::type::detect, path, fileMode);
            
            if (!audiogcPlayer) {
                throw std::runtime_error("Failed to load audio file: " + path);
            }

            this->file = file;
            this->type = mode;
        }

        void Source::play() {
            audiogcPlayer->play();
        }
        
        void Source::stop() {
            audiogcPlayer->stop();
        }
        
        void Source::pause() {
            audiogcPlayer->pause();
        }
        
        bool Source::isPlaying() const {
            return audiogcPlayer->is_playing();
        }

        void Source::seek_time(double time) {
            audiogcPlayer->seek(time);
        }

        void Source::seek_time_unit(double time, std::string unit) {
            // unit type not supported, just use seek_time
            seek_time(time);
        }

        double Source::tell() const {
            return audiogcPlayer->tell();
        }

        void Source::setLooping(bool loop) {
            audiogcPlayer->set_looping(loop);
        }
        
        bool Source::isLooping() const {
            return audiogcPlayer->is_looping();
        }

        void Source::setPitch(double pitch) {
            audiogcPlayer->set_pitch(pitch);
        }

        double Source::getPitch() const {
            return audiogcPlayer->get_pitch();
        }

        void Source::setVolume(double volume) {
            audiogcPlayer->set_volume(volume * 255); // audiogc volume range is 0-255
        }

        double Source::getVolume() const {
            return audiogcPlayer->get_volume() / 255.0; // convert back to 0-1 range. For parity with Love2D
        }
    }
}