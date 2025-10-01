#include <sol/sol.hpp>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>

#include "filesystem.hpp"
extern "C" {
    #include <lua.h>
}

namespace love {
    namespace filesystem {
        void __init(sol::state &luastate, int argc, char **argv) {
            createDirectories("sd:/LOVEPower");
            std::filesystem::current_path("sd:/LOVEPower");

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
            std::string path;

            if (doesPreferSaveDirectory) {
                if (std::filesystem::exists("save/" + file)) {
                    path = "save/" + file;
                } else {
                    path = "game/" + file;
                }
            } else {
                if (std::filesystem::exists("game/" + file)) {
                    path = "game/" + file;
                } else {
                    path = "save/" + file;
                }
            }

            return path;
        }

        void init(std::string identity) { // Lua accesible
            // identity is the new path to use for save and game files
            if (identity.empty()) {
                identity = "sd:/LOVEPower";
            }
            std::filesystem::current_path(identity);

            createDirectories("game");
            createDirectories("save");
        }

        bool fileExists(const std::string& file) {
            std::string path = getFilePath(file);
            return std::filesystem::exists(path);
        }

        sol::protected_function load(const std::string& file, sol::this_state state) {
            sol::state_view luastate(state);

            return luastate.load_file(getFilePath(file)).get<sol::protected_function>();
        }

        sol::table getInfo(const std::string& file, sol::this_state s) {
            sol::state_view lua(s);
            sol::table info = lua.create_table();

            std::string path = getFilePath(file);
            if (!std::filesystem::exists(path)) {
                return info; // return empty table instead of nil
            }

            info["filetype"] = std::filesystem::is_directory(path) ? "directory" : "file";
            info["size"] = static_cast<double>(std::filesystem::file_size(path));

            auto ftime = std::filesystem::last_write_time(path);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now());
            info["modtime"] = static_cast<double>(std::chrono::system_clock::to_time_t(sctp));

            return info;
        }

        bool exists(const std::string& file, sol::this_state lua) {
            std::string path = getFilePath(file);
            return std::filesystem::exists(path);
        }

        void preferSaveDirectory(const bool preferSave) {
            doesPreferSaveDirectory = preferSave;
        }
    }
}

int luaopen_love_filesystem(lua_State *L)  {
    sol::state_view luastate(L);

    luastate["love"]["filesystem"] = luastate.create_table_with(
        "init", love::filesystem::init,
        "load", love::filesystem::load,
        "getInfo", love::filesystem::getInfo,
        "exists", love::filesystem::exists,
        "preferSaveDirectory", love::filesystem::preferSaveDirectory
    );

    return 1;
}