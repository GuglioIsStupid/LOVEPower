#include <string>
#include "audio.hpp"

#include <sol/sol.hpp>

namespace love {
    namespace audio {
        void __init(sol::state &luastate) {
            __registerTypes(luastate);

            sol::table audio = luastate.create_table_with(
                "newSource", sol::overload(
                    newSource_file,
                    newSource_file_type
                )
            );

            luastate["love"]["audio"] = audio;
        }

        void __registerTypes(sol::state &luastate) {
            luastate.new_usertype<AudioSource>(
                "AudioSource",
                sol::no_constructor,
                "isPlaying", &AudioSource::isPlaying,
                "play", &AudioSource::play
            );
        }

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
