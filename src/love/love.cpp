#include <string>
#include <tuple>

#include <sol/sol.hpp>

#include "love.hpp"

#include "modules/graphics/classes/texture.hpp"
#include "modules/graphics/classes/font.hpp"

#include "modules/wiimote/classes/wiimoteController.hpp"

#include "modules/graphics/graphics.hpp"
#include "modules/filesystem/filesystem.hpp"
#include "modules/data/data.hpp"
#include "modules/timer/timer.hpp"
#include "modules/system/system.hpp"
#include "modules/audio/audio.hpp"
#include "modules/math/math.hpp"
#include "modules/event/event.hpp"
#include "modules/wiimote/wiimote.hpp"

namespace love {
    bool hasDeprecationOutput() {
        return _deprecationOutput;
    }

    void setDeprecationOutput(bool deprecationOutput) {
        _deprecationOutput = deprecationOutput;
    }

    std::tuple<int, int, int, std::string> getVersion() {
        return _version;
    }

    void initialize(sol::state& luastate, int argc, char** argv) {
        // Register love submodules
        love::audio::__init(luastate);
        love::graphics::__init(luastate);
        love::filesystem::__init(luastate, argc, argv);
        love::timer::__init(luastate);
        love::wiimote::__init(luastate);

        // Inject love table
        luastate["love"] = luastate.create_table_with(
            "_console", love::_console,
            "_os", love::_os,
            "hasDeprecationOutput", love::hasDeprecationOutput,
            "setDeprecationOutput", love::setDeprecationOutput,
            "getVersion", love::getVersion,
            "graphics", luastate.create_table_with(
                "setColor", sol::overload(
                    love::graphics::setColor_float4,
                    love::graphics::setColor_float3
                ),
                "getColor", love::graphics::getColor,
                "rectangle", love::graphics::rectangle,
                "draw", sol::overload(
                    love::graphics::draw,
                    love::graphics::draw_x,
                    love::graphics::draw_x_y,
                    love::graphics::draw_x_y_r,
                    love::graphics::draw_x_y_r_sx,
                    love::graphics::draw_x_y_r_sx_sy,
                    love::graphics::draw_x_y_r_sx_sy_ox,
                    love::graphics::draw_x_y_r_sx_sy_ox_oy,

                    love::graphics::draw_quad,
                    love::graphics::draw_quad_x,
                    love::graphics::draw_quad_x_y,
                    love::graphics::draw_quad_x_y_r,
                    love::graphics::draw_quad_x_y_r_sx,
                    love::graphics::draw_quad_x_y_r_sx_sy,
                    love::graphics::draw_quad_x_y_r_sx_sy_ox,
                    love::graphics::draw_quad_x_y_r_sx_sy_ox_oy
                ),
                "setFont", love::graphics::setFont,
                "newFont", sol::overload(
                    love::graphics::newFont,
                    love::graphics::newFont_size,
                    love::graphics::newFont_file,
                    love::graphics::newFont_file_size
                ),
                "print", sol::overload(
                    love::graphics::print,
                    love::graphics::print_x,
                    love::graphics::print_x_y,
                    love::graphics::print_x_y_r,
                    love::graphics::print_x_y_r_sx,
                    love::graphics::print_x_y_r_sx_sy,
                    love::graphics::print_x_y_r_sx_sy_ox,
                    love::graphics::print_x_y_r_sx_sy_ox_oy
                ),
                "newImage", sol::overload(
                    love::graphics::newImage,
                    love::graphics::newImage_data
                ),
                "newQuad", love::graphics::newQuad,
                "setBackgroundColor", sol::overload(
                    love::graphics::setBackgroundColor_float4,
                    love::graphics::setBackgroundColor_float3
                ),
                "getBackgroundColor", love::graphics::getBackgroundColor,
                "clear", sol::overload(
                    love::graphics::clear_float4,
                    love::graphics::clear_float3,
                    love::graphics::clear
                ),
                "isActive", love::graphics::isActive,
                "origin", love::graphics::origin,
                "present", love::graphics::present,
                "getWidth", love::graphics::getWidth,
                "getHeight", love::graphics::getHeight,
                "getDimensions", love::graphics::getDimensions
            ),
            "data", luastate.create_table_with(
                "decode", love::data::decode
            ),
            "filesystem", luastate.create_table_with(
                "load", love::filesystem::load,
                "getInfo", love::filesystem::getInfo,
                "exists", love::filesystem::exists,
                "preferSaveDirectory", love::filesystem::preferSaveDirectory
            ),
            "timer", luastate.create_table_with(
                "sleep", love::timer::sleep,
                "step", love::timer::step,
                "getFPS", love::timer::getFPS
            ),
            "system", luastate.create_table_with(
                "getOS", love::system::getOS,
                "getProcessorCount", love::system::getProcessorCount,
                "getPowerInfo", love::system::getPowerInfo
            ),
            "audio", luastate.create_table_with(
                "newSource", love::audio::newSource_file_type,
                "getVolume", love::audio::getVolume,
                "setVolume", love::audio::setVolume,
                "play", love::audio::play,
                "stop", love::audio::stop,
                "pause", love::audio::pause
            ),
            "math", luastate.create_table_with(
                "setRandomSeed", sol::overload(
                    love::math::setRandomSeed_uint32,
                    love::math::setRandomSeed_parts
                ),
                "random", sol::overload(
                    love::math::random,
                    love::math::random_max,
                    love::math::random_min_max
                ),
                "setRandomState", love::math::setRandomState,
                "getRandomSeed", love::math::getRandomSeed,
                "getRandomState", love::math::getRandomState
            ),
#ifdef USE_LIBMII
            "mii", luastate.create_table(),
#endif
#ifdef USE_PHYSICS
            "physics", luastate.create_table(),
#endif
            "event", luastate.create_table_with(
                "pump", love::event::pump,
                "poll", love::event::poll,
                "push", love::event::push,
                "quit", love::event::quit
            ),
            "wiimote", luastate.create_table_with(
                "update", love::wiimote::update,
                "getWiimote", love::wiimote::getWiimote,
                "getBalanceBoard", love::wiimote::getBalanceBoard
            )
        );
    }
}
