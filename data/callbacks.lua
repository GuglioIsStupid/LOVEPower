love.handlers = setmetatable({
    quit = function()
        return
    end
}, {
    __index = function(t, k)
        return function(...)
            error("Unhandled event: " .. tostring(k) .. " with args: " .. table.concat({...}, ", "))
        end
    end
})