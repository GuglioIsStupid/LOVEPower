# LoveWii

A "port" of LÖVE2D to the Wii. Based heavily off of [WiiLove](https://github.com/HTV04/WiiLove)


PR's are heavily appreaciated! I'm still new to this stuff

# Compiling

TODO

When compiling, its recommended to do the first build with ONLY `make`. After that you may do `make -j#`

# TODO
- Implement Mii data (love.mii)
- Implement transforms (love.graphics)
- Implement quads (love.graphics)
- Fix freezing with some graphical functions (love.graphics)
- Audio (love.audio)

Not in a specific order.

# Supported
- Wiimote (love.wiimote. Based off of love.joystick)
- Basic graphics (love.graphics)
- Basic filesystem (love.filesystem)
- Basic timer (love.timer)
- Basic math (love.math)
- Basic event (love.event)

# Unsupported
Any keyboard, mouse, touch, joystick (except wiimote), window, physics functions.