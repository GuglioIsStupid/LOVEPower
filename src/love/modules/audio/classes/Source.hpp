#pragma once

#include <string>
#include <memory>

#include <audiogc/audiogc.hpp>

#include "../../../common/Object.h"

namespace love {
    namespace audio {
        struct Source {
            Source(std::string file, std::string mode);

            Source(const Source&) = delete;
            Source& operator=(const Source&) = delete;

            Source(Source&&) noexcept = default;
            Source& operator=(Source&&) noexcept = default;

            ~Source() = default;

            void play();
            void stop();
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

            std::unique_ptr<audiogc::player> audiogcPlayer;

            std::string file;
            std::string type;
        };
    }
}
