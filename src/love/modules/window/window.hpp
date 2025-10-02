#pragma once

extern "C" {
    #include <lua.h>
}

#include <sol/sol.hpp>
#include <string>
#include <tuple>

namespace love {
    namespace window {
        int getDisplayCount();
        std::string getDisplayName(int displayIndex);
        std::string getDisplayName_current();

        bool setMode(int width, int height, const sol::table& settings);
        bool setMode_no_settings(int width, int height);

        bool updateMode(int width, int height, const sol::table& settings);
        bool updateMode_no_settings(int width, int height);

        std::tuple<int, int, sol::table> getMode();

        bool isHighDPIAllowed();

        std::string getDisplayOrientation(int displayIndex);
        std::string getDisplayOrientation_current();

        bool getFullscreen();
        sol::table getFullscreenModes();

        bool isOpen();
        bool close();
        bool open();

        std::tuple<int, int> getDesktopDimensions(int displayIndex);
        std::tuple<int, int> getDesktopDimensions_current();

        bool setVSync(bool enable);
        bool getVSync();

        void setDisplaySleepEnabled(bool enable);
        bool isDisplaySleepEnabled();

        bool hasFocus();
        bool isVisible();

        float getDPIScale();
        float getNativeDPIScale();

        float toPixels(float dp);
        std::tuple<float, float> toPixels_x_y(float x, float y);
        float toDP(float px);
        std::tuple<float, float> toDP_x_y(float x, float y);

        bool isMaximized();
        bool isMinimized();

        bool showMessageBox(const std::string& title, const std::string& message, const sol::table& settings);
        bool showMessageBox_no_settings(const std::string& title, const std::string& message);

        bool setIcon(const std::string& filename);

        bool setTitle(const std::string& title);

        void setPosition(int x, int y);
        std::tuple<int, int> getPosition();
    }
}

int luaopen_love_window(lua_State *L);