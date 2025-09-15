#include <sol/sol.hpp>
#include <wiiuse/wpad.h>

#include "classes/wiimoteController.hpp"

#include <vector>

#include "wiimote.hpp"

namespace {
    std::vector<love::wiimote::WiimoteController*> wiimotes(4);
}

namespace love {
    namespace wiimote {
        void __init(sol::state &luastate) {
            WPAD_Init();
            WPAD_SetVRes(0, 640, 480); // TODO: Support other resolutions? (Should add canvas' support first)
            WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
            WPAD_ScanPads();

            for (int i = 0; i < 4; ++i) {
                wiimotes[i] = new love::wiimote::WiimoteController(i);
            }

            __registerTypes(luastate);

            sol::table wiimote = luastate.create_table_with(
                "update", update,
                "getWiimote", getWiimote
            );

            luastate["love"]["wiimote"] = wiimote;
        }

        void __registerTypes(sol::state &luastate) {
            luastate.new_usertype<love::wiimote::WiimoteController>(
                "WiimoteController",
                sol::no_constructor,
                "isConnected", &love::wiimote::WiimoteController::isConnected,
                "update", &love::wiimote::WiimoteController::update,
                "getX", &love::wiimote::WiimoteController::getX,
                "getY", &love::wiimote::WiimoteController::getY,
                "getDX", &love::wiimote::WiimoteController::getDX,
                "getDY", &love::wiimote::WiimoteController::getDY,
                "getPosition", &love::wiimote::WiimoteController::getPosition,
                "getAngle", &love::wiimote::WiimoteController::getAngle,
                "getID", &love::wiimote::WiimoteController::getID,
                "getName", &love::wiimote::WiimoteController::getName,
                "isDown", [](WiimoteController& wm, sol::variadic_args va) {
                    if (!wm.isConnected() || !wm.data) return false;

                    for (sol::variadic_args::iterator it = va.begin(); it != va.end(); ++it) {
                        std::string button = (*it).as<std::string>();
                        if (!wm.checkButton(button)) {
                            return false;
                        }
                    }
                    return true;
                }
            );
        }

        void update() {
            WPAD_ScanPads();
            
            for (size_t i = 0; i < wiimotes.size(); ++i) {
                wiimotes[i]->update();
            }
        }

        WiimoteController* getWiimote(int index) {
            if (index < 1 || index > 4) {
                return nullptr;
            }

            return wiimotes[index - 1];
        }
    }
}