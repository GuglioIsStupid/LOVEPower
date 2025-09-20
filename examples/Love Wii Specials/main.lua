local defaultColor = {1, 1, 1}
local aPressedColor = {1, 0, 0}
local bPressedColor = {0, 0, 1}
local color = defaultColor
local wiimote = love.wiimote.getWiimote(1)
local board = love.wiimote.getBalanceBoard()
local img = love.graphics.newImage("resized.png")
local quad = love.graphics.newQuad(0, 0, 64, 64, img:getDimensions())

local audio = love.audio.newSource("test.ogg", "static")
audio:setLooping(true)
audio:play()
audio:setVolume(0.05)

function love.update(dt)
    if wiimote:isDown("b") then
        color = bPressedColor
    else
        color = (wiimote:isDown("a") and aPressedColor) or defaultColor
    end

    if wiimote:isDown("home") or wiimote:isDown("classic_home") then
        love.event.quit()
    end
end

local maxBalanceX, maxBalanceY = 0, 0
local minBalanceX, minBalanceY = 0, 0

function love.draw()
    local roll = wiimote:getRoll()
    local x, y = wiimote:getSmoothPosition()
    local z = wiimote:getZ()

    local _, _, w, h = quad:getViewport()
    w, h = img:getDimensions()

    local invertZ = 1 - z
    love.graphics.draw(img, x, y, math.rad(roll), invertZ, invertZ, w/2, h/2)

    love.graphics.setColor(unpack(color))

    love.graphics.print("Wiimote IR XYZ: " .. string.format("%.2f, %.2f, %.2f", x, y, z), 10, 10)
    love.graphics.print("Wiimote IR Distance: " .. string.format("%.2f", wiimote:getDistance()), 10, 30)
    love.graphics.print("Wiimote Battery: " .. wiimote:getBatteryLevel(), 10, 50) -- this is an int
    love.graphics.print("Wiimote Angle: " .. string.format("%.2f", wiimote:getAngle()), 10, 70)
    love.graphics.print("Wiimote Roll: " .. string.format("%.2f", roll), 10, 90)
    love.graphics.print("Wiimote Pitch: " .. string.format("%.2f", wiimote:getPitch()), 10, 110)
    love.graphics.print("Wiimote Yaw: " .. string.format("%.2f", wiimote:getYaw()), 10, 130)
    love.graphics.print("Wiimote Name: " .. wiimote:getName(), 10, 150)
    if wiimote:hasNunchuk() then
        local nrx, nry = wiimote:getNunchukJoystickAxisRaw()
        love.graphics.print("Nunchuk Joystick Raw XY: " .. string.format("%d, %d", nrx, nry), 10, 170)
        local nx, ny = wiimote:getNunchukJoystickAxis()
        love.graphics.print("Nunchuk Joystick XY: " .. string.format("%.2f, %.2f", nx, ny), 10, 190)
        love.graphics.print("Nunchuk Raw XYZ: " .. string.format("%.2f, %.2f, %.2f", wiimote:getNunchukXRaw(), wiimote:getNunchukYRaw(), wiimote:getNunchukZRaw()), 10, 210)
    elseif wiimote:hasClassic() then
        local lx, ly = wiimote:getClassicLeftJoystick()
        local lrx, lry = wiimote:getClassicLeftJoystickRawX(), wiimote:getClassicLeftJoystickRawY()
        love.graphics.print(string.format("Classic Left Joystick Raw XY: %d, %d", lrx, lry), 10, 170)
        love.graphics.print(string.format("Classic Left Joystick XY: %.2f, %.2f", lx, ly), 10, 190)

        local rx, ry = wiimote:getClassicRightJoystick()
        local rrx, rry = wiimote:getClassicRightJoystickRawX(), wiimote:getClassicRightJoystickRawY()
        love.graphics.print(string.format("Classic Right Joystick Raw XY: %d, %d", rrx, rry), 10, 210)
        love.graphics.print(string.format("Classic Right Joystick XY: %.2f, %.2f", rx, ry), 10, 230)

        local lShoulder = wiimote:getClassicLeftShoulder()
        local rShoulder = wiimote:getClassicRightShoulder()
        love.graphics.print(string.format("Classic L/R Shoulder: %.2f, %.2f", lShoulder, rShoulder), 10, 250)

        local buttons = {"classic_a", "classic_b", "classic_x", "classic_y", "classic_zl", "classic_zr", "classic_home"}
        local btnStr = ""
        for _, btn in ipairs(buttons) do
            if wiimote:isDown(btn) then
                btnStr = btnStr .. btn .. " "
            end
        end
        love.graphics.print("Classic Buttons Down: " .. btnStr, 10, 270)

    end

    local bx, by = board:getBalance()
    if bx > maxBalanceX then maxBalanceX = bx end
    if by > maxBalanceY then maxBalanceY = by end
    if bx < minBalanceX then minBalanceX = bx end
    if by < minBalanceY then minBalanceY = by end
    love.graphics.print("Board Name: " .. board:getName(), 300, 10)
    love.graphics.print(string.format("Balance XY: %.2f, %.2f | MinMax: (%.2f, %.2f) (%.2f, %.2f)", bx, by, minBalanceX, minBalanceY, maxBalanceX, maxBalanceY), 300, 30)
    love.graphics.print("Board Weight (Kg): " .. string.format("%.2f", board:getTotalWeight()), 300, 50)
end
