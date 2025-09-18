#pragma once

#include <string>
#include <audiogc/audiogc.hpp>

namespace love {
    namespace audio {
        struct Source {
            Source(std::string file, std::string mode);
            
            void play();
            void stop(); // sets time back to 0
            void pause();
            
            bool isPlaying() const;
            void seek_time(double time);
            void seek_time_unit(double time, std::string unit);
            double tell() const;

            void setLooping(bool loop);
            bool isLooping() const;

            void setPitch(double pitch);
            double getPitch() const;

            void setVolume(double volume);
            double getVolume() const;
            
            audiogc::player *audiogcPlayer = nullptr;
            
            std::string file;
            std::string type;
        };
    }
}