--[[
Copyright (c) 2006-2024 LOVE Development Team

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
--]]

-- Make sure love exists.
local love = require("love")

function love.nogame()
	function love.draw()
		love.graphics.clear(0.2, 0.2, 0.2)
		love.graphics.setColor(1, 1, 1)
		love.graphics.printf("No game loaded.\n\nDrag and drop a .love file or folder onto this window to start playing!", 0, love.graphics.getHeight() / 2 - 40, love.graphics.getWidth(), "center")
	end
end

return love.nogame
