#include <string>
#include "audio.hpp"

#include <sol/sol.hpp>
#include <aesndlib.h>
#include <fat.h>
#include <audiogc/audiogc.hpp>
#include "classes/Source.hpp"

namespace love {
    namespace audio {
        void __init(sol::state &luastate) {
            AESND_Init();
            fatInitDefault();

            __registerTypes(luastate);
        }

        void __registerTypes(sol::state &luastate) {
            luastate.new_usertype<love::audio::Source>(
                "Source",
                sol::no_constructor,
                "play", &love::audio::Source::play,
                "stop", &love::audio::Source::stop,
                "pause", &love::audio::Source::pause,

                "isPlaying", &love::audio::Source::isPlaying,
                "seek", sol::overload(
                    static_cast<void(love::audio::Source::*)(double)>(&love::audio::Source::seek_time),
                    static_cast<void(love::audio::Source::*)(double, std::string)>(&love::audio::Source::seek_time_unit)
                ),
                "tell", &love::audio::Source::tell,

                "setLooping", &love::audio::Source::setLooping,
                "isLooping", &love::audio::Source::isLooping,

                "setPitch", &love::audio::Source::setPitch,
                "getPitch", &love::audio::Source::getPitch,

                "setVolume", &love::audio::Source::setVolume,
                "getVolume", &love::audio::Source::getVolume
            );
        }

        love::audio::Source newSource_file_type(std::string file, std::string type) {
            return love::audio::Source(file, type);
        }
        
        double getVolume(love::audio::Source* source) {
            return source->getVolume();
        }
        void setVolume(love::audio::Source* source, double volume) {
            source->setVolume(volume);
        }
        void play(love::audio::Source* source) {
            source->play();
        }
        void stop(love::audio::Source* source) {
            source->stop();
        }
        void pause(love::audio::Source* source) {
            source->pause();
        }
    }
}
