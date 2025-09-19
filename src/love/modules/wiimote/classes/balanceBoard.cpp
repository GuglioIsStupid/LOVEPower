#include "balanceBoard.hpp"

#include <wiiuse/wiiuse.h>
#include <wiiuse/wpad.h>

#include <tuple>
#include <string>

namespace love {
    namespace wiimote {

        #pragma region General
        BalanceBoard::BalanceBoard(int idx) {
            index = idx + 4; // Balance boards are 4-7
            status = WPAD_Probe(index, nullptr);
            data = nullptr;

            if (status == WPAD_ERR_NONE) {
                data = WPAD_Data(index);
                wb = data->exp.wb;
            }
        }

        bool BalanceBoard::isConnected() const {
            return status == WPAD_ERR_NONE && data && data->exp.type == WPAD_EXP_WIIBOARD;
        }
        
        void BalanceBoard::update() {
            status = WPAD_Probe(index, nullptr);
            if (status == WPAD_ERR_NONE) {
                data = WPAD_Data(index);
                wb = data->exp.wb;
            }

            /* updateTemperature(); */
        }
        
        int BalanceBoard::getID() const {
            return index;
        }

        std::string BalanceBoard::getName() const {
            if (!isConnected()) return "Unknown";
            return "Balance Board";
        }

        float BalanceBoard::getBatteryLevel() const {
            if (!data) return -1.0f;
            return static_cast<float>(data->battery_level) / 255.0f;
        }

        #pragma region Weights

        float BalanceBoard::getTopLeftWeight() const {
            if (!isConnected()) return 0.0f;
            return wb.tl;
        }
        float BalanceBoard::getTopRightWeight() const {
            if (!isConnected()) return 0.0f;
            return wb.tr;
        }
        float BalanceBoard::getBottomLeftWeight() const {
            if (!isConnected()) return 0.0f;
            return wb.bl;
        }
        float BalanceBoard::getBottomRightWeight() const {
            if (!isConnected()) return 0.0f;
            return wb.br;
        }
        std::tuple<float, float, float, float> BalanceBoard::getWeights() const {
            if (!isConnected()) return std::make_tuple(0.0f, 0.0f, 0.0f, 0.0f);
            return std::make_tuple(wb.tl, wb.tr, wb.bl, wb.br);
        }
        float BalanceBoard::getTotalWeight() const {
            if (!isConnected()) return 0.0f;
            return wb.tl + wb.tr + wb.bl + wb.br;
        }

        float BalanceBoard::getRawTopLeftWeight() const {
            if (!isConnected()) return 0.0f;
            return static_cast<float>(wb.rtl);
        }
        float BalanceBoard::getRawTopRightWeight() const {
            if (!isConnected()) return 0.0f;
            return static_cast<float>(wb.rtr);
        }
        float BalanceBoard::getRawBottomLeftWeight() const {
            if (!isConnected()) return 0.0f;
            return static_cast<float>(wb.rbl);
        }
        float BalanceBoard::getRawBottomRightWeight() const {
            if (!isConnected()) return 0.0f;
            return static_cast<float>(wb.rbr);
        }
        std::tuple<short, short, short, short> BalanceBoard::getRawWeightsInt() const {
            if (!isConnected()) return std::make_tuple(0, 0, 0, 0);
            return std::make_tuple(wb.rtl, wb.rtr, wb.rbl, wb.rbr);
        }
        float BalanceBoard::getRawTotalWeight() const {
            if (!isConnected()) return 0.0f;
            return static_cast<float>(wb.rtl + wb.rtr + wb.rbl + wb.rbr);
        }

        #pragma region Balance

        float BalanceBoard::getBalanceX() const {
            if (!isConnected()) return 0.0f;
            return wb.x;
        }

        float BalanceBoard::getCenterOfBalanceY() const {
            if (!isConnected()) return 0.0f;
            return wb.y;
        }

        std::tuple<float, float> BalanceBoard::getBalance() const {
            if (!isConnected()) return std::make_tuple(0.0f, 0.0f);
            return std::make_tuple(wb.x, wb.y);
        }
        
        #pragma region Calibration
        std::tuple<std::tuple<short, short, short>, 
                    std::tuple<short, short, short>, 
                    std::tuple<short, short, short>, 
                    std::tuple<short, short, short>
        > BalanceBoard::getCalibrationData() const {
            if (!isConnected()) return std::make_tuple(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0), 
                                        std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));

            return std::make_tuple(
                std::make_tuple(wb.ctl[0], wb.ctl[1], wb.ctl[2]),
                std::make_tuple(wb.ctr[0], wb.ctr[1], wb.ctr[2]),
                std::make_tuple(wb.cbl[0], wb.cbl[1], wb.cbl[2]),
                std::make_tuple(wb.cbr[0], wb.cbr[1], wb.cbr[2])
            );
        }
        
        /* #pragma region Temperature
        // Okay so wiiuse/wpad.h doesn't implement the temperature reading for balance boards
        // It sucks! But whatever,, I thought it would have been cool (and funny) to have
        // I might attempt to implement it in the future.
        // ^ I'm sure I can get around it with straight wiiuse data reading, but like my brain is NOT working rn
        void BalanceBoard::updateTemperature() {
            if (!isConnected()) return;

            static ubyte buffer[11];
            wiiuse_read_data(data->wm, buffer, 0x00, 11, [](struct wiimote_t* wm, ubyte* buf) {
                tempRaw = buf[8];

                tempC = 20.0f + (static_cast<int>(buf[8]) - 0x1A);
            });
        }

        float BalanceBoard::getTemperature() const {
            if (!isConnected()) return 0.0f;
            return tempC;
        }

        ubyte BalanceBoard::getTemperatureRaw() const {
            if (!isConnected()) return 0;
            return tempRaw;
        }

        float BalanceBoard::getTemperatureCorrectedWeight() const {
            if (!isConnected()) return 0.0f;
            float total = getTotalWeight();
            return total * (1.0f - 0.0007f * (tempRaw - 0x19));
        } */

        #pragma endregion

    }
}