#include "wiimoteController.hpp"

#include <wiiuse/wiiuse.h>
#include <wiiuse/wpad.h>
#include <tuple>
#include <string>

namespace love {
    namespace wiimote {

        #pragma region General
        WiimoteController::WiimoteController(int idx) {
            index = idx;
            status = WPAD_Probe(index, nullptr);
            isMotionPlusEnabled = false;
            //wm = wiiuse_get_by_id(index);
            data = nullptr;
            prev_data = nullptr;

            if (status == WPAD_ERR_NONE) {
                data = WPAD_Data(index);
            }
        }

        bool WiimoteController::isConnected() const {
            return status == WPAD_ERR_NONE;
        }
        
        void WiimoteController::update() {
            status = WPAD_Probe(index, nullptr);
            if (status == WPAD_ERR_NONE) {
                prev_data = data;
                data = WPAD_Data(index);
            }
        }
        
        int WiimoteController::getID() const {
            return index;
        }

        std::string WiimoteController::getName() const {
            if (!data) return "Unknown";

            std::string name = "Wiimote";
            if (data->exp.type == EXP_NUNCHUK) {
                name += " + Nunchuk";
            } else if (data->exp.type == EXP_CLASSIC) {
                name += " + Classic Controller";
            } else if (data->exp.type == EXP_GUITAR_HERO_3) {
                name = "Guitar Hero 3 Controller";
            } else if (data->exp.type == EXP_MOTION_PLUS) {
                name += " + Motion Plus";
            }

            return name;
        }

        float WiimoteController::getBatteryLevel() const {
            if (!isConnected() || !data) return -1.0f;
            return static_cast<float>(data->battery_level) / 255.0f;
        }
        
        #pragma region IR Data

        float WiimoteController::getX() const {
            return (isConnected() && data) ? data->ir.x : 0.0f;
        }
        float WiimoteController::getY() const {
            return (isConnected() && data) ? data->ir.y : 0.0f;
        }
        float WiimoteController::getZ() const {
            return (isConnected() && data) ? data->ir.z : 0.0f;
        }

        float WiimoteController::getRawX() const {
            return (isConnected() && data) ? data->ir.ax : 0.0f;
        }
        float WiimoteController::getRawY() const {
            return (isConnected() && data) ? data->ir.ay : 0.0f;
        }

        float WiimoteController::getSmoothX() const {
            return (isConnected() && data) ? data->ir.sx : 0.0f;
        }
        float WiimoteController::getSmoothY() const {
            return (isConnected() && data) ? data->ir.sy : 0.0f;
        }

        float WiimoteController::getDistance() const {
            return (isConnected() && data) ? data->ir.distance : 0.0f;
        }

        float WiimoteController::getDX() const {
            return (isConnected() && data && prev_data) ? data->accel.x - prev_data->accel.x : 0.0f;
        }
        float WiimoteController::getDY() const {
            return (isConnected() && data && prev_data) ? data->accel.y - prev_data->accel.y : 0.0f;
        }
        float WiimoteController::getDZ() const {
            return (isConnected() && data && prev_data) ? data->accel.z - prev_data->accel.z : 0.0f;
        }

        float WiimoteController::getAngle() const {
            return (isConnected() && data) ? data->ir.angle : 0.0f;
        }

        float WiimoteController::getRoll() const {
            return (isConnected() && data) ? data->orient.roll : 0.0f;
        }
        float WiimoteController::getPitch() const {
            return (isConnected() && data) ? data->orient.pitch : 0.0f;
        }
        float WiimoteController::getYaw() const {
            return (isConnected() && data) ? data->orient.yaw : 0.0f;
        }

        float WiimoteController::getAbsoluteRoll() const {
            return (isConnected() && data) ? data->orient.a_roll : 0.0f;
        }
        float WiimoteController::getAbsolutePitch() const {
            return (isConnected() && data) ? data->orient.a_pitch : 0.0f;
        }

        float WiimoteController::getGforceX() const {
            return (isConnected() && data) ? data->gforce.x : 0.0f;
        }
        float WiimoteController::getGforceY() const {
            return (isConnected() && data) ? data->gforce.y : 0.0f;
        }
        float WiimoteController::getGforceZ() const {
            return (isConnected() && data) ? data->gforce.z : 0.0f;
        }

        std::tuple<float, float> WiimoteController::getPosition() const {
            return std::make_tuple(getX(), getY());
        }
        std::tuple<float, float> WiimoteController::getRawPosition() const {
            return std::make_tuple(getRawX(), getRawY());
        }
        std::tuple<float, float> WiimoteController::getSmoothPosition() const {
            return std::make_tuple(getSmoothX(), getSmoothY());
        }

        void WiimoteController::setMotionPlus(bool enabled) {
            if (isConnected()) {
                WPAD_SetMotionPlus(index, enabled ? 1 : 0);
                isMotionPlusEnabled = enabled;
            }
        }

        bool WiimoteController::getMotionPlus() const {
            return isMotionPlusEnabled;
        }

        #pragma region Nunchuk

        bool WiimoteController::hasNunchuk() const {
            return isConnected() && data && data->exp.type == EXP_NUNCHUK;
        }

        float WiimoteController::getNunchukXRaw() const {
            return hasNunchuk() ? data->exp.nunchuk.accel.x : 0.0f;
        }
        float WiimoteController::getNunchukYRaw() const {
            return hasNunchuk() ? data->exp.nunchuk.accel.y : 0.0f;
        }
        float WiimoteController::getNunchukZRaw() const {
            return hasNunchuk() ? data->exp.nunchuk.accel.z : 0.0f;
        }

        float WiimoteController::getNunchukX() const {
            return hasNunchuk() ? data->exp.nunchuk.accel_calib.cal_g.x : 0.0f;
        }
        float WiimoteController::getNunchukY() const {
            return hasNunchuk() ? data->exp.nunchuk.accel_calib.cal_g.y : 0.0f;
        }
        float WiimoteController::getNunchukZ() const {
            return hasNunchuk() ? data->exp.nunchuk.accel_calib.cal_g.z : 0.0f;
        }
        
        std::tuple<float, float, float> WiimoteController::getNunchukPositionRaw() const {
            return std::make_tuple(getNunchukXRaw(), getNunchukYRaw(), getNunchukZRaw());
        }
        std::tuple<float, float, float> WiimoteController::getNunchukPosition() const {
            return std::make_tuple(getNunchukX(), getNunchukY(), getNunchukZ());
        }

        float WiimoteController::getNunchukRoll() const {
            return hasNunchuk() ? data->exp.nunchuk.orient.roll : 0.0f;
        }
        float WiimoteController::getNunchukPitch() const {
            return hasNunchuk() ? data->exp.nunchuk.orient.pitch : 0.0f;
        }
        float WiimoteController::getNunchukYaw() const {
            return hasNunchuk() ? data->exp.nunchuk.orient.yaw : 0.0f;
        }

        float WiimoteController::getNunchukGforceX() const {
            return hasNunchuk() ? data->exp.nunchuk.gforce.x : 0.0f;
        }
        float WiimoteController::getNunchukGforceY() const {
            return hasNunchuk() ? data->exp.nunchuk.gforce.y : 0.0f;
        }
        float WiimoteController::getNunchukGforceZ() const {
            return hasNunchuk() ? data->exp.nunchuk.gforce.z : 0.0f;
        }

        float WiimoteController::getNunchukJoystickRawX() const {
            return hasNunchuk() ? data->exp.nunchuk.js.pos.x : 0.0f;
        }
        float WiimoteController::getNunchukJoystickRawY() const {
            return hasNunchuk() ? data->exp.nunchuk.js.pos.y : 0.0f;
        }

        float WiimoteController::getNunchukJoystickX() const {
            if (!hasNunchuk()) return 0.0f;
            float x = static_cast<float>(data->exp.nunchuk.js.pos.x - data->exp.nunchuk.js.center.x);
            float range = static_cast<float>(data->exp.nunchuk.js.max.x - data->exp.nunchuk.js.center.x);
            return range != 0.0f ? x / range : 0.0f;
        }
        float WiimoteController::getNunchukJoystickY() const {
            if (!hasNunchuk()) return 0.0f;
            float y = static_cast<float>(data->exp.nunchuk.js.pos.y - data->exp.nunchuk.js.center.y);
            float range = static_cast<float>(data->exp.nunchuk.js.max.y - data->exp.nunchuk.js.center.y);
            return range != 0.0f ? y / range : 0.0f;
        }

        std::tuple<float, float> WiimoteController::getNunchukJoystickAxisRaw() {
            return std::make_tuple(getNunchukJoystickRawX(), getNunchukJoystickRawY());
        }
        std::tuple<float, float> WiimoteController::getNunchukJoystickAxis() {
            return std::make_tuple(getNunchukJoystickX(), getNunchukJoystickY());
        }

        #pragma endregion
    }
}
