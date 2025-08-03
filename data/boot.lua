local quit
package.path = "game/?.lua; game/?/init.lua"

local function debug_print(msg)
    if love and love.graphics and love.graphics.print then
        love.graphics.clear(0, 0, 0)
        love.graphics.origin()
        love.graphics.print(tostring(msg), 10, 10)
        love.graphics.present()
    else
        print(msg)
    end
end

local debug = true
function love.run()
    if love.load then love.load() end
    if love.timer then love.timer.step() end

    while true do
        local dt = 0

        if love.timer then dt = love.timer.step() end

        if love.update then love.update(dt) end

        if love.graphics and love.graphics.isActive and love.graphics.isActive() then
            love.graphics.origin()
            --love.graphics.clear(love.graphics.getBackgroundColor()) -- TODO: Figure out why this freezes the game

            if love.draw then love.draw() end

            if debug then -- REMOVE ME
                love.graphics.setColor(1, 1, 1)
                love.graphics.print("FPS: " .. tostring(love.timer.getFPS()), 10, 30)
            end

            love.graphics.present()
        end

        love.timer.sleep(1)
    end
end

function love.errorhandler(err)
    local msg = "Error\n\n" ..
	            tostring(err) ..
				"\n\n\n" ..
				string.gsub(string.gsub(debug.traceback(), "\t", ""), "stack traceback:", "Traceback\n") ..
				"\n\n\nPress HOME to return to loader\n"
	local msgTable = {}

	for line in string.gmatch(msg, "([^\n]*)\n") do
        local cleaned = line:gsub("\r", "")
        cleaned = cleaned:gsub("\n", "")
        cleaned = cleaned:gsub("\t", "")

		table.insert(msgTable, cleaned)
	end

	love.graphics.setFont(love.graphics.newFont(14))

	while true do
		--[[ love.event.pump()

		for name, a,b,c,d,e,f in love.event.poll do
			if name == "homepressed" then
				return 1
			end
		end ]]

		-- love.graphics.clear(89, 157, 220)
        -- TEMP UNTIL CLEAR IS FIXED
        --[[ love.graphics.setColor(89/255, 157/255, 220/255)
        love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())
        love.graphics.setColor(1, 1, 1) ]]

		for i, line in ipairs(msgTable) do
			love.graphics.print(line, 70, 60 + i * 18)
		end

		love.graphics.present()

        love.timer.sleep(1)
	end
end


local function main()
    local chunk = love.filesystem.load("main.lua")
    if chunk then chunk() end
    love.run()
end

-- Capture error with a proper traceback
local ok, err = xpcall(main, function(e)
    return debug and debug.traceback and debug.traceback(e, 2) or tostring(e)
end)

if not ok then
    -- Now call your error handler with the full traceback/error string
    love.errorhandler("test")
end