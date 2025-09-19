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

            u8 getBatteryLevel() const;

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

            int getID() const;
            // TODO: Add extentions to the name
            std::string getName() const;

            void setMotionPlus(bool enabled);
            bool getMotionPlus() const;

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

                return false;
            }

        };
    }
}
