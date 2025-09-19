#pragma once

#include <tuple>

#include <wiiuse/wpad.h>
#include <wiiuse/wiiuse.h>

#include <string>

#include <sol/sol.hpp>

namespace love {
    namespace wiimote {
        struct BalanceBoard {
            #pragma region General

            int index;
            int status;
            wii_board_t wb;
            WPADData* data;
            /* wiimote_t* wm;

            ubyte tempRaw;
            float tempC; */

            BalanceBoard(int idx);
            bool isConnected() const;
            void update();

            float getBatteryLevel() const;
            int getID() const;
            std::string getName() const;

            #pragma region Weights
            
            float getTopLeftWeight() const;
            float getTopRightWeight() const;
            float getBottomLeftWeight() const;
            float getBottomRightWeight() const;
            std::tuple<float, float, float, float> getWeights() const;
            float getTotalWeight() const;

            float getRawTopLeftWeight() const;
            float getRawTopRightWeight() const;
            float getRawBottomLeftWeight() const;
            float getRawBottomRightWeight() const;
            std::tuple<short, short, short, short> getRawWeightsInt() const;
            float getRawTotalWeight() const;

            #pragma region Balance
            float getBalanceX() const;
            float getCenterOfBalanceY() const;
            std::tuple<float, float> getBalance() const;

            #pragma region Calibration
            std::tuple<std::tuple<short, short, short>, 
                        std::tuple<short, short, short>, 
                        std::tuple<short, short, short>, 
                        std::tuple<short, short, short>
            > getCalibrationData() const;

            /* #pragma region Temperature
            void updateTemperature();
            float getTemperature() const;
            ubyte getTemperatureRaw() const;
            float getTemperatureCorrectedWeight() const; */
        };
    }
}