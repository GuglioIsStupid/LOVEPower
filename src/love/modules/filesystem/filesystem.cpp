#include "../../lib/sol/sol.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>

#include "filesystem.hpp"

namespace love {
    namespace filesystem {
        void __init(sol::state &luastate, int argc, char **argv) {
            createDirectories("sd:/lovewii");
            std::filesystem::current_path("sd:/lovewii");

            createDirectories("game");
            createDirectories("save");
        }

        void createDirectories(const std::string& path) {
            std::filesystem::path dir(path);
            if (!std::filesystem::exists(dir)) {
                std::filesystem::create_directories(dir);
            }
        }

        std::string getFilePath(const std::string& file) {
            if (doesPreferSaveDirectory) {
                if (std::filesystem::exists("save/" + file)) {
                    return "save/" + file;
                } else {
                    return "game/" + file;
                }
            }

            if (std::filesystem::exists("game/" + file)) {
                return "game/" + file;
            } else {
                return "save/" + file;
            }

            return file;
        }

        bool fileExists(const std::string& file) {
            std::string path = getFilePath(file);
            return std::filesystem::exists(path);
        }

        sol::protected_function load(const std::string& file, sol::this_state state) {
            sol::state_view luastate(state);

            return luastate.load_file(getFilePath(file)).get<sol::protected_function>();
        }

        sol::object getInfo(const std::string& file, sol::this_state state) {
            sol::state_view luastate(state);
            std::string path = getFilePath(file);

            if (!std::filesystem::exists(path)) {
                return sol::nil;
            }

            sol::table info = sol::table(luastate);
            info["filetype"] = std::filesystem::is_directory(path) ? "directory" : "file";
            info["size"] = static_cast<double>(std::filesystem::file_size(path));

            auto ftime = std::filesystem::last_write_time(path);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now());
            std::time_t mod_time = std::chrono::system_clock::to_time_t(sctp);
            info["modtime"] = static_cast<double>(mod_time);

            return info;
        }

        void preferSaveDirectory(const bool &preferSave) {
            doesPreferSaveDirectory = preferSave;
        }
    }
}
