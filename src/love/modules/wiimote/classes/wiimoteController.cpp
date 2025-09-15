#include "wiimoteController.hpp"

#include <wiiuse/wpad.h>
#include <tuple>
#include <string>

namespace love {
    namespace wiimote {

        WiimoteController::WiimoteController(int idx)
            : index(idx), status(WPAD_Probe(idx, nullptr)), data(nullptr), prev_data(nullptr)
        {
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

        float WiimoteController::getX() const {
            return (isConnected() && data) ? data->ir.x : 0.0f;
        }

        float WiimoteController::getY() const {
            return (isConnected() && data) ? data->ir.y : 0.0f;
        }

        float WiimoteController::getDX() const {
            return (isConnected() && data && prev_data) ? data->ir.x - prev_data->ir.x : 0.0f;
        }

        float WiimoteController::getDY() const {
            return (isConnected() && data && prev_data) ? data->ir.y - prev_data->ir.y : 0.0f;
        }

        std::tuple<float, float> WiimoteController::getPosition() const {
            return std::make_tuple(getX(), getY());
        }

        float WiimoteController::getAngle() const {
            return (isConnected() && data) ? data->ir.angle : 0.0f;
        }

        int WiimoteController::getID() const {
            return index;
        }

        std::string WiimoteController::getName() const {
            return "Wiimote";
        }
    }
}
