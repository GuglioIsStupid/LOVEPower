local quit
package.path = "game/?.lua; game/?/init.lua"

require("love.callbacks")

function love.run()
    if love.math then
        love.math.setRandomSeed(os.time())
    end
    if love.load then love.load() end
    if love.timer then love.timer.step() end

    local dt = 0

    while true do
        --[[ if love.event then
            love.event.pump()
            for name, a, b, c, d, e, f in love.event.poll() do
                if name == "quit" then
                    if not love.quit or not love.quit() then
                        return a or 0
                    end
                end
                love.handlers[name](a, b, c, d, e, f)
            end
        end ]]
        if love.timer then dt = love.timer.step() end
        if love.wiimote then love.wiimote.update() end
        if love.update then love.update(dt) end

        if love.graphics and love.graphics.isActive and love.graphics.isActive() then
            love.graphics.origin()
            --love.graphics.clear(love.graphics.getBackgroundColor()) -- TODO: Figure out why this freezes the game

            if love.draw then love.draw() end

            love.graphics.present()
        end

        love.timer.sleep(1)
    end
end

local function writeLog(msg)
    local f = io.open("sd:/love_error_lua.log", "a")
    if f then
        f:write(msg, "\n")
        f:close()
    end
end

function love.errorhandler(err)
    local msg = tostring(err) .. "\n" ..
                (debug and debug.traceback and debug.traceback("", 2) or "")
    writeLog(msg)

    love.graphics.setFont(love.graphics.newFont(14))
    while true do
        love.graphics.origin()
        love.graphics.setColor(1, 1, 1)
        love.graphics.print("Error:\n" .. msg, 30, 30)
        love.graphics.present()
    end
end

local function main()
    if love.filesystem.exists("main.lua") then
        local chunk = love.filesystem.load("main.lua")
        if chunk then chunk() end
    else
        require("love.nogame")
        love.nogame()
    end

    love.run()
end

local ok, err = xpcall(main, function(e)
    return debug and debug.traceback and debug.traceback(e, 2) or tostring(e)
end)

if not ok then
    love.errorhandler(err)
end