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
        -- Main.lua exists, so lets check for a conf.lua
        local t = {
            --identity = nil, -- Not supported in LoveWii
            appendidentity = false, -- Sources in the save directory first if true
            version = "0.1", -- TODO: Implement version checking
            --accelerometerjoystick = false, -- Not supported in LoveWii
            --externalstorage = false, -- Not supported in LoveWii
            --gammacorrect = false, -- Not supported in LoveWii

            audio = {
                --mic = false, -- Not supported in LoveWii
                --mixwithsystem = false -- Not supported in LoveWii
            },

            window = {}, -- Not supported, but here for compatibility

            modules = {
                audio = true,
                data = true,
                event = true,
                font = true,
                graphics = true,
                image = true,
                --joystick = false, -- No love.joystick module in LoveWii
                --keyboard = false, -- No love.keyboard module in LoveWii
                wiimote = true, -- LoveWii specific module
                mii = true, -- LoveWii specific module
                math = true,
                mouse = false, -- No love.mouse module in LoveWii
                physics = true,
                sound = true,
                system = true,
                thread = true,
                timer = true,
                --touch = false, -- No love.touch module in LoveWii
                --video = false, -- No love.video module in LoveWii
                --window = false -- No love.window module in LoveWii
            }
        }

        if love.filesystem.exists("conf.lua") then
            local conf = love.filesystem.load("conf.lua")
            if conf then
                local ok, res = pcall(conf, t)
                if not ok then
                    writeLog("Error in conf.lua: " .. tostring(res))
                end
            end
        end

        if not t.modules.audio then love.audio = nil end
        if not t.modules.data then love.data = nil end
        if not t.modules.event then love.event = nil end
        if not t.modules.font then love.font = nil end
        if not t.modules.graphics then love.graphics = nil end
        if not t.modules.image then love.image = nil end
        --if not t.modules.joystick then love.joystick = nil end
        --if not t.modules.keyboard then love.keyboard = nil end
        if not t.modules.wiimote then love.wiimote = nil end
        if not t.modules.mii then love.mii = nil end
        if not t.modules.math then love.math = nil end
        --if not t.modules.mouse then love.mouse = nil end
        if not t.modules.physics then love.physics = nil end
        if not t.modules.sound then love.sound = nil end
        if not t.modules.system then love.system = nil end
        if not t.modules.thread then love.thread = nil end
        if not t.modules.timer then love.timer = nil end
        --if not t.modules.touch then love.touch = nil end
        --if not t.modules.video then love.video = nil end
        --if not t.modules.window then love.window = nil end

        if love.filesystem and t.appendidentity then
            love.filesystem.preferSaveDirectory(true)
        end

        -- TODO: Implement version checking

        --local min, maj, rev = love.getVersion()

        local version_ok = true -- TODO: Set to false if version is not compatible
        if not version_ok then
            print("Warning: Game version is not compatible with this version of LoveWii. Issues may occur.")
        end

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