#include "window.hpp"

extern "C" {
    #include <lua.h>
}

#include <sol/sol.hpp>
#include <string>
#include <tuple>

namespace love {
    namespace window {
        int getDisplayCount() {
            return 1;
        }

        std::string getDisplayName(int displayIndex) {
            if (displayIndex != 0) {
                throw std::out_of_range("Display index out of range");
            }
            return "Default Display";
        }

        std::string getDisplayName_current() {
            return "Default Display";
        }

        bool setMode(int width, int height, const sol::table& settings) {
            return true;
        }

        bool setMode_no_settings(int width, int height) {
            return true;
        }

        bool updateMode(int width, int height, const sol::table& settings) {
            return true;
        }

        bool updateMode_no_settings(int width, int height) {
            return true;
        }

        std::tuple<int, int, sol::table> getMode() {
            sol::state_view lua = sol::state_view(luaL_newstate());
            sol::table settings = lua.create_table();
            settings["width"] = 600;
            settings["height"] = 480;
            settings["fullscreen"] = false;
            settings["vsync"] = true;
            settings["msaa"] = 0;
            settings["display"] = 1;
            settings["resizable"] = false;
            settings["borderless"] = false;
            settings["minwidth"] = 600;
            settings["minheight"] = 480;
            return std::make_tuple(600, 480, settings);
        }

        bool isHighDPIAllowed() {
            return false;
        }

        std::string getDisplayOrientation(int displayIndex) {
            if (displayIndex != 0) {
                throw std::out_of_range("Display index out of range");
            }
            return "landscape";
        }

        std::string getDisplayOrientation_current() {
            return "landscape";
        }

        bool getFullscreen() {
            return true;
        }

        sol::table getFullscreenModes() {
            sol::state_view lua = sol::state_view(luaL_newstate());
            sol::table modes = lua.create_table();
            modes.add(lua.create_table_with("width", 640, "height", 480));
            return modes;
        }

        bool isOpen() {
            return true;
        }

        bool close() {
            return false;
        }

        bool open() {
            return true;
        }

        std::tuple<int, int> getDesktopDimensions(int displayIndex) {
            if (displayIndex != 0) {
                throw std::out_of_range("Display index out of range");
            }
            return std::make_tuple(640, 480);
        }

        std::tuple<int, int> getDesktopDimensions_current() {
            return std::make_tuple(640, 480);
        }

        bool setVSync(bool enable) {
            return true;
        }

        bool getVSync() {
            return true;
        }

        void setDisplaySleepEnabled(bool enable) {
        }

        bool isDisplaySleepEnabled() {
            return true;
        }
        
        bool hasFocus() {
            return true;
        }

        bool isVisible() {
            return true;
        }

        float getDPIScale() {
            return 1.0f;
        }

        float getNativeDPIScale() {
            return 1.0f;
        }

        float toPixels(float dp) {
            return dp * getDPIScale();
        }

        std::tuple<float, float> toPixels_x_y(float x, float y) {
            return std::make_tuple(x * getDPIScale(), y * getDPIScale());
        }

        float toDP(float px) {
            return px / getDPIScale();
        }

        std::tuple<float, float> toDP_x_y(float x, float y) {
            return std::make_tuple(x / getDPIScale(), y / getDPIScale());
        }

        bool isMaximized() {
            return false;
        }

        bool isMinimized() {
            return false;
        }

        bool showMessageBox(const std::string& title, const std::string& message, const sol::table& options) {
            return true;
        }

        bool showMessageBox_no_settings(const std::string& title, const std::string& message) {
            return true;
        }

        bool setIcon(const std::string& filename) {
            return false;
        }

        bool setTitle(const std::string& title) {
            return true;
        }

        void setPosition(int x, int y) {
        }

        std::tuple<int, int> getPosition() {
            return std::make_tuple(0, 0);
        }
    }
}

int luaopen_love_window(lua_State *L) {
    sol::state_view luastate(L);

    luastate["love"]["window"] = luastate.create_table_with(
        "getDisplayCount", love::window::getDisplayCount,
        "getDisplayName", sol::overload(
            love::window::getDisplayName,
            love::window::getDisplayName_current
        ),
        "setMode", sol::overload(
            love::window::setMode,
            love::window::setMode_no_settings
        ),
        "updateMode", sol::overload(
            love::window::updateMode,
            love::window::updateMode_no_settings
        ),
        "getMode", love::window::getMode,
        "isHighDPIAllowed", love::window::isHighDPIAllowed,
        "getDisplayOrientation", sol::overload(
            love::window::getDisplayOrientation,
            love::window::getDisplayOrientation_current
        ),
        "getFullscreen", love::window::getFullscreen,
        "getFullscreenModes", love::window::getFullscreenModes,
        "isOpen", love::window::isOpen,
        "close", love::window::close,
        "open", love::window::open,
        "getDesktopDimensions", sol::overload(
            love::window::getDesktopDimensions,
            love::window::getDesktopDimensions_current
        ),
        "setVSync", love::window::setVSync,
        "getVSync", love::window::getVSync,
        "setDisplaySleepEnabled", love::window::setDisplaySleepEnabled,
        "isDisplaySleepEnabled", love::window::isDisplaySleepEnabled,
        "hasFocus", love::window::hasFocus,
        "isVisible", love::window::isVisible,
        "getDPIScale", love::window::getDPIScale,
        "getNativeDPIScale", love::window::getNativeDPIScale,
        "toPixels", sol::overload(
            love::window::toPixels,
            love::window::toPixels_x_y
        ),
        "toDP", sol::overload(
            love::window::toDP,
            love::window::toDP_x_y
        ),
        "isMaximized", love::window::isMaximized,
        "isMinimized", love::window::isMinimized,
        "showMessageBox", sol::overload(
            love::window::showMessageBox,
            love::window::showMessageBox_no_settings
        ),
        "setIcon", love::window::setIcon,
        "setTitle", love::window::setTitle,
        "setPosition", love::window::setPosition,
        "getPosition", love::window::getPosition
    );

    return 1;
}