local love = require("love")

love.path = {}
love.arg = {}

function love.path.normalslashes(p)
    return p:gsub("\\", "/")
end

function love.path.endslash(p)
    if p:sub(-1) ~= "/" then
        return p .. "/"
    else
        return p
    end
end

function love.path.abs(p)
    local tmp = love.path.normalslashes(p)
    if tmp:find("/") == 1 then
        return true
    end

    if tmp:find("%a:") == 1 then
        return true
    end

    return false
end

function love.path.getFull(p)
    if love.path.abs(p) then
		return love.path.normalslashes(p)
	end

	local cwd = love.filesystem.getWorkingDirectory()
	cwd = love.path.normalslashes(cwd)
	cwd = love.path.endslash(cwd)

	local full = cwd .. love.path.normalslashes(p)

	return full:match("(.-)/%.$") or full
end

function love.path.leaf(p)
    p = love.path.normalslashes(p)

    local a = 1
    local last = p

    while a do
        a = p:find("/", a + 1)

        if a then
            last = p:sub(a + 1)
        end
    end

    return last
end

function love.arg.getLow(a)
	local m = math.huge
	for k, _ in pairs(a) do
		if k < m then
			m = k
		end
	end
	return a[m], m
end

love.arg.optionIndices = {}

function love.arg.parseGameArguments(a)
	local out = {}

	local _, lowindex = love.arg.getLow(a)

	local o = lowindex
	for i = lowindex, #a do
        out[o] = a[i]
        o = o + 1
	end

	return out
end