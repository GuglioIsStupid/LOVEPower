#pragma once

#include <tuple>
#include <wiiuse/wpad.h>

namespace love {
    namespace wiimote {
        struct WiimoteController {
            int index;
            int status;
            WPADData* data;
            WPADData* prev_data;

            WiimoteController(int idx);

            bool isConnected() const;
            void update();

            float getX() const;
            float getY() const;
            float getDX() const;
            float getDY() const;
            std::tuple<float, float> getPosition() const;
            float getAngle() const;
        };
    }
}
