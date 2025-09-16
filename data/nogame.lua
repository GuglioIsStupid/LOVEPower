function love.nogame()
    -- TODO: Make this pretty?
    function love.draw()
        love.graphics.setColor(1, 1, 1)
        love.graphics.print("No game found. Please add a main.lua file to sd:/lovewii/game/", 10, 10)
    end
end