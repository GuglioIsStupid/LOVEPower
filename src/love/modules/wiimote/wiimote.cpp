#include <sol/sol.hpp>
#include <wiiuse/wpad.h>

#include "classes/wiimoteController.hpp"

#include <vector>

#include "wiimote.hpp"

namespace {
    std::vector<love::wiimote::WiimoteController*> wiimotes(4);
    love::wiimote::BalanceBoard* balanceBoard = nullptr;
}

namespace love {
    namespace wiimote {
        void __init(sol::state &luastate) {
            WPAD_Init();
            WPAD_SetVRes(0, 640, 480); // TODO: Support other resolutions? (Should add canvas' support first)
            WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
            WPAD_ScanPads();

            // 0-3 are wiimotes, 4-7 are balance boards
            for (int i = 0; i < 4; ++i) {
                wiimotes[i] = new love::wiimote::WiimoteController(i);
            }

            balanceBoard = new love::wiimote::BalanceBoard(1);

            __registerTypes(luastate);
        }

        void __registerTypes(sol::state &luastate) {
            luastate.new_usertype<love::wiimote::WiimoteController>(
                "WiimoteController",
                sol::no_constructor,
                "isConnected", &love::wiimote::WiimoteController::isConnected,
                "update", &love::wiimote::WiimoteController::update,
                "getX", &love::wiimote::WiimoteController::getX,
                "getY", &love::wiimote::WiimoteController::getY,
                "getZ", &love::wiimote::WiimoteController::getZ,
                "getRawX", &love::wiimote::WiimoteController::getRawX,
                "getRawY", &love::wiimote::WiimoteController::getRawY,
                "getSmoothX", &love::wiimote::WiimoteController::getSmoothX,
                "getSmoothY", &love::wiimote::WiimoteController::getSmoothY,
                "getDistance", &love::wiimote::WiimoteController::getDistance,
                "getDX", &love::wiimote::WiimoteController::getDX,
                "getDY", &love::wiimote::WiimoteController::getDY,
                "getPosition", &love::wiimote::WiimoteController::getPosition,
                "getRawPosition", &love::wiimote::WiimoteController::getRawPosition,
                "getSmoothPosition", &love::wiimote::WiimoteController::getSmoothPosition,
                "getAngle", &love::wiimote::WiimoteController::getAngle,
                "getRoll", &love::wiimote::WiimoteController::getRoll,
                "getPitch", &love::wiimote::WiimoteController::getPitch,
                "getYaw", &love::wiimote::WiimoteController::getYaw,
                "getAbsoluteRoll", &love::wiimote::WiimoteController::getAbsoluteRoll,
                "getAbsolutePitch", &love::wiimote::WiimoteController::getAbsolutePitch,
                "getGforceX", &love::wiimote::WiimoteController::getGforceX,
                "getGforceY", &love::wiimote::WiimoteController::getGforceY,
                "getGforceZ", &love::wiimote::WiimoteController::getGforceZ,
                "getBatteryLevel", &love::wiimote::WiimoteController::getBatteryLevel,
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
                },
                "setMotionPlus", &love::wiimote::WiimoteController::setMotionPlus,
                "getMotionPlus", &love::wiimote::WiimoteController::getMotionPlus,

                "hasNunchuk", &love::wiimote::WiimoteController::hasNunchuk,
                "getNunchukX", &love::wiimote::WiimoteController::getNunchukX,
                "getNunchukY", &love::wiimote::WiimoteController::getNunchukY,
                "getNunchukZ", &love::wiimote::WiimoteController::getNunchukZ,
                "getNunchukXRaw", &love::wiimote::WiimoteController::getNunchukXRaw,
                "getNunchukYRaw", &love::wiimote::WiimoteController::getNunchukYRaw,
                "getNunchukZRaw", &love::wiimote::WiimoteController::getNunchukZRaw,
                "getNunchukPosition", &love::wiimote::WiimoteController::getNunchukPosition,
                "getNunchukPositionRaw", &love::wiimote::WiimoteController::getNunchukPositionRaw,
                "getNunchukRoll", &love::wiimote::WiimoteController::getNunchukRoll,
                "getNunchukPitch", &love::wiimote::WiimoteController::getNunchukPitch,
                "getNunchukYaw", &love::wiimote::WiimoteController::getNunchukYaw,
                "getNunchukGforceX", &love::wiimote::WiimoteController::getNunchukGforceX,
                "getNunchukGforceY", &love::wiimote::WiimoteController::getNunchukGforceY,
                "getNunchukGforceZ", &love::wiimote::WiimoteController::getNunchukGforceZ,
                "getNunchukJoystickRawX", &love::wiimote::WiimoteController::getNunchukJoystickRawX,
                "getNunchukJoystickRawY", &love::wiimote::WiimoteController::getNunchukJoystickRawY,
                "getNunchukJoystickX", &love::wiimote::WiimoteController::getNunchukJoystickX,
                "getNunchukJoystickY", &love::wiimote::WiimoteController::getNunchukJoystickY,
                "getNunchukJoystickAxisRaw", &love::wiimote::WiimoteController::getNunchukJoystickAxisRaw,
                "getNunchukJoystickAxis", &love::wiimote::WiimoteController::getNunchukJoystickAxis,

                "hasClassic", &love::wiimote::WiimoteController::hasClassic,
                "getClassicLeftJoystickRawX", &love::wiimote::WiimoteController::getClassicLeftJoystickRawX,
                "getClassicLeftJoystickRawY", &love::wiimote::WiimoteController::getClassicLeftJoystickRawY,
                "getClassicLeftJoystickX", &love::wiimote::WiimoteController::getClassicLeftJoystickX,
                "getClassicLeftJoystickY", &love::wiimote::WiimoteController::getClassicLeftJoystickY,
                "getClassicLeftJoystick", &love::wiimote::WiimoteController::getClassicLeftJoystick,
                "getClassicRightJoystickRawX", &love::wiimote::WiimoteController::getClassicRightJoystickRawX,
                "getClassicRightJoystickRawY", &love::wiimote::WiimoteController::getClassicRightJoystickRawY,
                "getClassicRightJoystickX", &love::wiimote::WiimoteController::getClassicRightJoystickX,
                "getClassicRightJoystickY", &love::wiimote::WiimoteController::getClassicRightJoystickY,
                "getClassicRightJoystick", &love::wiimote::WiimoteController::getClassicRightJoystick,
                "getClassicAxisRaw", &love::wiimote::WiimoteController::getClassicAxisRaw,
                "getClassicAxis", &love::wiimote::WiimoteController::getClassicAxis,
                "getClassicLeftShoulder", &love::wiimote::WiimoteController::getClassicLeftShoulder,
                "getClassicRightShoulder", &love::wiimote::WiimoteController::getClassicRightShoulder,
                "getClassicAxisRaw", &love::wiimote::WiimoteController::getClassicAxisRaw,
                "getClassicAxis", &love::wiimote::WiimoteController::getClassicAxis
            );

            luastate.new_usertype<love::wiimote::BalanceBoard>(
                "BalanceBoard",
                sol::no_constructor,
                "isConnected", &love::wiimote::BalanceBoard::isConnected,
                "update", &love::wiimote::BalanceBoard::update,
                "getTopLeftWeight", &love::wiimote::BalanceBoard::getTopLeftWeight,
                "getTopRightWeight", &love::wiimote::BalanceBoard::getTopRightWeight,
                "getBottomLeftWeight", &love::wiimote::BalanceBoard::getBottomLeftWeight,
                "getBottomRightWeight", &love::wiimote::BalanceBoard::getBottomRightWeight,
                "getWeights", &love::wiimote::BalanceBoard::getWeights,
                "getTotalWeight", &love::wiimote::BalanceBoard::getTotalWeight,
                "getRawTopLeftWeight", &love::wiimote::BalanceBoard::getRawTopLeftWeight,
                "getRawTopRightWeight", &love::wiimote::BalanceBoard::getRawTopRightWeight,
                "getRawBottomLeftWeight", &love::wiimote::BalanceBoard::getRawBottomLeftWeight,
                "getRawBottomRightWeight", &love::wiimote::BalanceBoard::getRawBottomRightWeight,
                "getRawWeights", &love::wiimote::BalanceBoard::getRawWeightsInt,
                "getRawTotalWeight", &love::wiimote::BalanceBoard::getRawTotalWeight,
                "getBalanceX", &love::wiimote::BalanceBoard::getBalanceX,
                "getBalanceY", &love::wiimote::BalanceBoard::getCenterOfBalanceY,
                "getBalance", &love::wiimote::BalanceBoard::getBalance,
                "getBatteryLevel", &love::wiimote::BalanceBoard::getBatteryLevel,
                "getID", &love::wiimote::BalanceBoard::getID,
                "getName", &love::wiimote::BalanceBoard::getName,
                "getCalibrationData", &love::wiimote::BalanceBoard::getCalibrationData
            );
        }

        void update() {
            WPAD_ScanPads();
            
            for (size_t i = 0; i < wiimotes.size(); ++i) {
                wiimotes[i]->update();
            }

            if (balanceBoard) {
                balanceBoard->update();
            }
        }

        WiimoteController* getWiimote(int index) {
            if (index < 1 || index > 4) {
                return nullptr;
            }

            return wiimotes[index - 1];
        }

        BalanceBoard* getBalanceBoard() {
            return balanceBoard;
        }
    }
}