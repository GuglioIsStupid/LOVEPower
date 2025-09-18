#pragma once

#include <string>

#include <sol/sol.hpp>
#include "classes/Source.hpp"

// TODO: Seperate this into a seperate class file
namespace love {
    namespace audio {
        void __init(sol::state &luastate);
        void __registerTypes(sol::state &luastate);

        love::audio::Source newSource_file_type(std::string file, std::string type);

        double getVolume(love::audio::Source* source);
        void setVolume(love::audio::Source* source, double volume);

        void play(love::audio::Source* source);
        void stop(love::audio::Source* source);
        void pause(love::audio::Source* source);
    }
}