local love = require("love")

function love.nogame()
    -- TODO: Make this pretty?
    function love.draw()
        love.graphics.setColor(1, 1, 1)
        love.graphics.print("No game found. Please add a main.lua file to sd:/LOVEPower/game/", 10, 10)
    end
end