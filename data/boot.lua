local love = require("love")

require("love.arg")
require("love.callbacks")

local noGameCode = false
local invalidGamePath = nil

function love.boot()
    require("love.filesystem")
    local arg0 = love.arg.getLow(arg) or "sd:/LOVEPower/"
    if type(arg0) == "string" and arg0 ~= "" then
        -- Dolpin or something like that
        arg0 = "sd:/LOVEPower/"
    else
        arg0 = arg0:match("^(.-)[^/\\]-$") -- Gets the directory of the executable
        if arg0 == nil or arg0 == "" then -- Fall back to default
            arg0 = "sd:/LOVEPower/"
        end
    end
    love.filesystem.init(arg0)

    local canHasGame = true
    -- TODO: Implement fusing

    local isFusedGame = canHasGame
    --love.filesystem.setFused(isFusedGame)

    love.setDeprecationOutput(true)

    if canHasGame and not (love.filesystem.exists("main.lua") or love.filesystem.exists("conf.lua")) then
        noGameCode = true
        canHasGame = false
    end

    if not canHasGame then
        local nogame = require("love.nogame")
        nogame()
    end
end

function love.init()
    local c = {
        --title = "LOVEPower", -- Not used in LOVEPower
        version = "0.1", -- TODO: Implement version checking
        window = {}, -- Not supported, but here for compatibility
        modules = {
            audio = true,
            data = true,
            event = true,
            font = true,
            graphics = true,
            image = true,
            --joystick = false, -- No love.joystick module in LOVEPower
            --keyboard = false, -- No love.keyboard module in LOVEPower
            wiimote = true, -- LOVEPower specific module
            mii = true, -- LOVEPower specific module
            math = true,
            mouse = false, -- No love.mouse module in LOVEPower
            physics = true,
            sound = true,
            system = true,
            timer = true,
            --touch = false, -- No love.touch module in LOVEPower
            --video = false, -- No love.video module in LOVEPower
            window = false -- Dummy module available
        },
        audio = {
            --mic = false, -- Not supported in LOVEPower
            --mixwithsystem = false -- Not supported in LOVEPower
        },
        --console = false, -- Not supported in LOVEPower
        --identity = nil, -- Not supported in LOVEPower
        appendidentity = false, -- Sources in the save directory first if true
        --accelerometerjoystick = false, -- Not supported in LOVEPower
        --externalstorage = false, -- Not supported in LOVEPower
        --gammacorrect = false, -- Not supported in LOVEPower
    }

    local openedconsole = false

    local confok, conferr
    if (not love.conf) and love.filesystem and love.filesystem.exists("conf.lua") then
        confok, conferr = pcall(function()
            local chunk, err = love.filesystem.load("conf.lua")
            if chunk then
                chunk(c)
            else
                error(err)
            end
        end)
    end

    if love.conf then
        confok, conferr = pcall(love.conf, c)
    end

    for _,v in ipairs{
        "data",
        "timer",
        "event",
        --"sound",
        "system",
        "audio",
        --"image",
        --"font",
        "window",
        "graphics",
        "math",
        "physics",
        "wiimote",
        "mii"
    } do
        if c.modules[v] then
            require("love." .. v)
        end
    end

    if love.event then
		love.createhandlers()
	end

    -- VERSION CHECKING.... TODO

    if not confok and conferr then
        error(conferr)
    end

    if love.timer then love.timer.step() end

    if love.filesystem then
        if love.filesystem.getInfo("main.lua") then
            local mainok, mainerr = pcall(function()
                local chunk, err = love.filesystem.load("main.lua")
                if chunk then chunk()
                else error(err) end
            end)
        end
    end

    if noGameCode then
        error("No code to run\nYour game might be packaged incorrectly.\nMake sure main.lua is at the top level of the zip.")
    elseif invalidGamePath then
        error("Cannot load game at path '" .. invalidGamePath .. "'\nMake sure a folder exists at the specified path.")
    end
end

local print, debug, tostring = print, debug, tostring

local inerror = false

local function error_printer(msg, layer)
    print((debug.traceback("Error: " .. tostring(msg), 1 + (layer or 1)):gsub("\n[^\n]+$", "")))
end

local function deferErrHand(...)
    local errhand = love.errorhandler or love.errhand
    local handler = (not inerror and errhand) or error_printer
    inerror = true
    func = handler(...)
end

local function safe_call(f)
    local ok, err = pcall(f, deferErrHand)
    if not ok then
        return false, err
    end
    return true
end

local ok = safe_call(love.boot)
if not ok then return end

ok = safe_call(love.init)
if not ok then return end

if love.run then
    safe_call(love.run)
end