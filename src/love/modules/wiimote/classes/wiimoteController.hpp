#pragma once

#include <tuple>
#include <wiiuse/wpad.h>
#include <wiiuse/wiiuse.h>

#include <string>

#include <sol/sol.hpp>

namespace love {
    namespace wiimote {
        struct WiimoteController {
            #pragma region General
            int index;
            int status;
            bool isMotionPlusEnabled;
            WPADData* data;
            WPADData* prev_data;
            wiimote_t* wm;

            WiimoteController(int idx);

            bool isConnected() const;
            void update();

            float getBatteryLevel() const;
            int getID() const;

            #pragma region IR Data

            float getX() const;
            float getY() const;
            float getZ() const;
            float getRawX() const;
            float getRawY() const;
            float getSmoothX() const;
            float getSmoothY() const;

            float getDX() const;
            float getDY() const;
            float getDZ() const;

            float getAngle() const;
            float getDistance() const;
            
            std::tuple<float, float> getPosition() const;
            std::tuple<float, float> getRawPosition() const;
            std::tuple<float, float> getSmoothPosition() const;

            float getRoll() const;
            float getPitch() const;
            float getYaw() const;
            float getAbsoluteRoll() const;
            float getAbsolutePitch() const;

            float getGforceX() const;
            float getGforceY() const;
            float getGforceZ() const;

            std::string getName() const;

            void setMotionPlus(bool enabled);
            bool getMotionPlus() const;

            #pragma region Nunchuk
            bool hasNunchuk() const;
            float getNunchukX() const;
            float getNunchukY() const;
            float getNunchukZ() const;

            float getNunchukXRaw() const;
            float getNunchukYRaw() const;
            float getNunchukZRaw() const;

            std::tuple<float, float, float> getNunchukPositionRaw() const;
            std::tuple<float, float, float> getNunchukPosition() const;

            float getNunchukRoll() const;
            float getNunchukPitch() const;
            float getNunchukYaw() const;

            float getNunchukGforceX() const;
            float getNunchukGforceY() const;
            float getNunchukGforceZ() const;
            
            float getNunchukJoystickRawX() const;
            float getNunchukJoystickRawY() const;
            float getNunchukJoystickX() const;
            float getNunchukJoystickY() const;
            std::tuple<float, float> getNunchukJoystickAxisRaw();
            std::tuple<float, float> getNunchukJoystickAxis();

            #pragma region Classic

            bool hasClassic() const;

            float getClassicLeftJoystickRawX() const;
            float getClassicLeftJoystickRawY() const;
            float getClassicLeftJoystickX() const;
            float getClassicLeftJoystickY() const;
            std::tuple<float, float> getClassicLeftJoystickRaw() const;
            std::tuple<float, float> getClassicLeftJoystick() const;

            float getClassicRightJoystickRawX() const;
            float getClassicRightJoystickRawY() const;
            float getClassicRightJoystickX() const;
            float getClassicRightJoystickY() const;
            std::tuple<float, float> getClassicRightJoystickRaw() const;
            std::tuple<float, float> getClassicRightJoystick() const;

            float getClassicLeftShoulder() const;
            float getClassicRightShoulder() const;
            std::tuple<sol::object, sol::object> getClassicAxisRaw(sol::state_view lua) const;
            std::tuple<sol::object, sol::object> getClassicAxis(sol::state_view lua) const;

            #pragma endregion

            bool checkButton(const std::string& buttonName) const {
                if (!data) return false;
                std::string lowerButtonName = buttonName;
                std::transform(lowerButtonName.begin(), lowerButtonName.end(), lowerButtonName.begin(), ::tolower);

                if (lowerButtonName == "a")     return data->btns_h & WPAD_BUTTON_A;
                if (lowerButtonName == "b")     return data->btns_h & WPAD_BUTTON_B;
                if (lowerButtonName == "1")     return data->btns_h & WPAD_BUTTON_1;
                if (lowerButtonName == "2")     return data->btns_h & WPAD_BUTTON_2;
                if (lowerButtonName == "plus")  return data->btns_h & WPAD_BUTTON_PLUS;
                if (lowerButtonName == "minus") return data->btns_h & WPAD_BUTTON_MINUS;
                if (lowerButtonName == "home")  return data->btns_h & WPAD_BUTTON_HOME;
                if (lowerButtonName == "up")    return data->btns_h & WPAD_BUTTON_UP;
                if (lowerButtonName == "down")  return data->btns_h & WPAD_BUTTON_DOWN;
                if (lowerButtonName == "left")  return data->btns_h & WPAD_BUTTON_LEFT;
                if (lowerButtonName == "right") return data->btns_h & WPAD_BUTTON_RIGHT;
                if (hasNunchuk()) {
                    if (lowerButtonName == "c")     return data->exp.nunchuk.btns & WPAD_NUNCHUK_BUTTON_C;
                    if (lowerButtonName == "z")     return data->exp.nunchuk.btns & WPAD_NUNCHUK_BUTTON_Z;
                } else if (hasClassic()) {
                    if (lowerButtonName == "classic_a")      return data->btns_h & WPAD_CLASSIC_BUTTON_A;
                    if (lowerButtonName == "classic_b")      return data->btns_h & WPAD_CLASSIC_BUTTON_B;
                    if (lowerButtonName == "classic_x")      return data->btns_h & WPAD_CLASSIC_BUTTON_X;
                    if (lowerButtonName == "classic_y")      return data->btns_h & WPAD_CLASSIC_BUTTON_Y;
                    if (lowerButtonName == "classic_zl")     return data->btns_h & WPAD_CLASSIC_BUTTON_ZL;
                    if (lowerButtonName == "classic_zr")     return data->btns_h & WPAD_CLASSIC_BUTTON_ZR;
                    if (lowerButtonName == "classic_minus")  return data->btns_h & WPAD_CLASSIC_BUTTON_MINUS;
                    if (lowerButtonName == "classic_plus")   return data->btns_h & WPAD_CLASSIC_BUTTON_PLUS;
                    if (lowerButtonName == "classic_up")     return data->btns_h & WPAD_CLASSIC_BUTTON_UP;
                    if (lowerButtonName == "classic_down")   return data->btns_h & WPAD_CLASSIC_BUTTON_DOWN;
                    if (lowerButtonName == "classic_left")   return data->btns_h & WPAD_CLASSIC_BUTTON_LEFT;
                    if (lowerButtonName == "classic_right")  return data->btns_h & WPAD_CLASSIC_BUTTON_RIGHT;
                    if (lowerButtonName == "classic_l")      return data->btns_h & WPAD_CLASSIC_BUTTON_FULL_L;
                    if (lowerButtonName == "classic_r")      return data->btns_h & WPAD_CLASSIC_BUTTON_FULL_R;
                    if (lowerButtonName == "classic_home")   return data->btns_h & WPAD_CLASSIC_BUTTON_HOME;
                }

                return false;
            }

        };
    }
}
