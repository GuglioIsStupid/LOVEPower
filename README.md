# LoveWii

A "port" of LÖVE2D to the Wii. Based heavily off of [WiiLove](https://github.com/HTV04/WiiLove)

PR's are heavily appreaciated! I'm still new to this stuff

# Compiling

1. Download, compile and install [Lua for Wii](https://wiibrew.org/wiki/Lua_for_Wii).
2. (Optional) Download and install [libmii](https://wiibrew.org/wiki/Libmii).
3. Compiled with "make". For some reason multiple threads don't work(?). I'll look into it in the near future.

Modified libraries will be available in source code for ease of user compiling

# TODO
- Implement Mii data (love.mii) (Via libmii)
- Implement transforms (love.graphics)
- Implement physics (love.physics) (Via Box2d)
- Fix freezing with some graphical functions (love.graphics)

Not in a specific order.

# Supported
- love.wiimote
- love.graphics
- love.filesystem
- love.timer
- love.math
- love.event
- love.audio

A wiki containing supported functions will be created in the future, for now you can look inside the modules CPP files as your wiki.

# Unsupported
Any keyboard, mouse, touch, joystick and window functions
