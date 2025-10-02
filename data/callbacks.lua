local love = require("love")

function love.createhandlers()
    love.handlers = setmetatable({
        quit = function()
			return
		end,
        lowmemory = function()
            if love.lowmemory then love.lowmemory() end
            collectgarbage()
            collectgarbage()
        end
    }, {
        __index = function(self, name)
            error("Unknown event: " .. name)
        end
    })
end

function love.run()
    if love.load then love.load(love.arg.parseGameArguments(arg), arg) end

    if love.timer then love.timer.step() end

    local dt = 0

    while true do
        if love.event then
            love.event.pump()
            while true do
                local name, a, b, c, d, e, f = love.event.poll()
                if not name then break end
                if name == "quit" then
                    if not love.quit or not love.quit() then
                        return a or 0
                    end
                else
                    love.handlers[name](a, b, c, d, e, f)
                end
            end

            if love.timer then dt = love.timer.step() end
            if love.wiimote then love.wiimote.update() end
            if love.update then love.update(dt) end

            if love.graphics and love.graphics.isActive() then
                love.graphics.origin()
                --love.graphics.clear(love.graphics.getBackgroundColor()) -- TODO: Figure out why this freezes the game
                -- until then, render a rectangle
                local lastColor = {love.graphics.getColor()}
                love.graphics.setColor(love.graphics.getBackgroundColor())
                love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())
                love.graphics.setColor(unpack(lastColor))
                if love.draw then love.draw() end
                love.graphics.present()
            end

            love.timer.sleep(0.001)
            collectgarbage("step")
        end
    end
end

local debug, print, tostring, error = debug, print, tostring, error

function love.threaderror(t, err)
	error("Thread error ("..tostring(t)..")\n\n"..err, 0)
end

local function error_printer(msg, layer)
	print((debug.traceback("Error: " .. tostring(msg), 1+(layer or 1)):gsub("\n[^\n]+$", "")))
end

local function writeLog(msg)
    local f = io.open("sd:/love_error_lua.log", "a")
    if f then
        f:write(msg, "\n")
        f:close()
    end
end

function love.errhand(err)
    local msg = tostring(err) .. "\n" ..
        (debug and debug.traceback and debug.traceback("", 2) or "")
    writeLog(msg)

    love.audio.stop()
    love.graphics.setFont(love.graphics.newFont(14))

    while true do
        love.graphics.origin()
        love.graphics.setColor(1, 1, 1)
        love.graphics.print("Error:", 10, 10)
        -- get a table of all the lines
        local lines = {}
        for line in msg:gmatch("([^\n]*)\n?") do
            table.insert(lines, line)
        end
        local y = 30
        for i, line in ipairs(lines) do
            love.graphics.print(line, 10, y)
            y = y + 20
        end
        love.graphics.present()

        love.event.pump()
        while true do
            local name, a, b, c, d, e, f = love.event.poll()
            if not name then break end
            if name == "quit" then
                return
            end
        end
    end
end