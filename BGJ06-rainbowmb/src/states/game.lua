require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/tank")
require("entities/building")
require("entities/blob")
require("entities/enemy")
require("entities/bomb")

Game = class("Game", GameState)

function Game:__init()
    self.offset = Vector(0, 0)
    self.paused = false
    self:reset()
    self.restart = 0
end

function Game:died()
    self.world:add(Explosion(self.tank.position, 10))
    self.restart = 1
end

function Game:reset()
    self.world = World()
    self.tank = Tank()
    self.world:add(self.tank)

    -- generate map
    local n = MAPSIZE/2
    self.map = {}
    for i = -n, n do
        self.map[i] = {}
        for j = -n, n do
            self.map[i][j] = (i ~= 0 or j ~= 0)
        end
    end

    local dirs = {"left", "up", "right", "down"}
    local crossings = { {x = 0, y = 0, left = false, right = false, up = false, down = false, count = 0} }
    for i = 0, 500 do
        local dx, dy, c, cnum, dir, oDir, valid, offset

        valid = false

        while not valid do
            cnum, c = 0, nil
            offset = math.random(2, 8) * 1

            -- choose a random crossing that is not complete
            while not c or (c.left and c.right and c.up and c.down) do
                cnum = math.random(1, #crossings)
                c = crossings[cnum]
            end

            dir, oDir = nil, nil
            while not dir or c[dir] do
                local dirnum = math.random(1, #dirs)
                dir = dirs[dirnum]
                oDir = dirs[(dirnum+2-1)%4+1]
            end

            -- get direction
            dx, dy = 0, 0
            if dir == "left"  then dx = 1 end
            if dir == "right" then dx = -1 end
            if dir == "up"    then dy = -1 end
            if dir == "down"  then dy = 1 end

            -- check if valid
            valid = true
            for j=1,math.max(1, offset-1) do
                local mx, my = c.x+j*dx, c.y+j*dy
                if mx >= -n and mx <= n and my >= -n and my <= n then
                    if not self.map[mx][my] then
                        valid = false
                        break
                    end
                end
            end
        end


        local x, y = c.x + offset * dx, c.y + offset * dy
        local nc = {x = x, y = y, left = false, right = false, up = false, down = false, count = 1}
        nc[oDir] = true
        c[dir] = true
        table.insert(crossings, nc)

        -- do the streets
        for j = 1, offset do
            local mx, my = c.x+j*dx, c.y+j*dy
            if mx >= -n and mx <= n and my >= -n and my <= n then
                self.map[mx][my] = false
            end
        end
    end

    for x = -n, n do
        for y = -n, n do
            if self.map[x][y] then
                self.world:add(Building(x, y))
            end
        end
    end

    self.enemySpawn = 0
end

function Game:hasBuildingAt(x, y)
    local n = MAPSIZE / 2
    return (x >= -n and x <= n and y >= -n and y <= n) and self.map[x][y]
end

function Game:getKeyboardVector()
    local v = Vector()
    if love.keyboard.isDown("a") or love.keyboard.isDown("left")  then v.x = v.x - 1 end
    if love.keyboard.isDown("d") or love.keyboard.isDown("right") then v.x = v.x + 1 end
    if love.keyboard.isDown("w") or love.keyboard.isDown("up")    then v.y = v.y - 1 end
    if love.keyboard.isDown("s") or love.keyboard.isDown("down")  then v.y = v.y + 1 end
    return v:normalized()
end

function Game:onUpdate(dt)
    self.restart = self.restart - dt
    self.world:update(dt)
    if not self.tank.dead then
        self.offset = Vector(love.graphics.getWidth(), love.graphics.getHeight()) * 0.5 - self.tank.position

        self.enemySpawn = self.enemySpawn - dt
        if self.enemySpawn <= 0 then
            local p
            repeat
                p = Vector(math.random(-MAPSIZE/2, MAPSIZE/2), math.random(-MAPSIZE/2, MAPSIZE/2))
            until Vector.dist(p*GRIDSIZE, self.tank.position) > (SIZE/2):len() and not self.map[p.x][p.y]

            self.world:add(Enemy(p*GRIDSIZE))
            self.enemySpawn = math.random(0.5, 1.0)
        end
    end
end

function Game:mouse()
    return Vector(love.mouse.getPosition()) - self.offset
end

function Game:onDraw()
    love.graphics.setBackgroundColor(10, 10, 10)
    love.graphics.clear()

    love.graphics.push()
    love.graphics.translate(self.offset:unpack())
    self.world:draw()
    love.graphics.pop()

    -- draw charges
    local space = 60
    local left = (SIZE.x - 7*space)/2
    for i, c in ipairs(COLORNAMES) do
        local x, y =  left + (i-1) * space, SIZE.y - 10
        love.graphics.setColor(255, 255, 255)
        love.graphics.draw(resources.images.container_back, x, y, 0, 1, 1, 0, resources.images.container_back:getHeight())

        love.graphics.setColor(unpack(COLORS[c]))
        local h = 70 * self.tank.charges[c]
        love.graphics.rectangle("fill", x+3, y-5-h, 10, h)

        love.graphics.setColor(255, 255, 255)
        love.graphics.draw(resources.images.container_front, x, y, 0, 1, 1, 0, resources.images.container_front:getHeight())

        --love.graphics.setColor(unpack(COLORS[c]))
        love.graphics.setColor(255, 255, 255, self.tank.specials[c] and 255 or 100)
        love.graphics.draw(resources.images[c], x+10, y - 110, 0, 0.6, 0.6, 27, 27)

        local s = self.tank.specials[c]
        if s then 
            love.graphics.print('[' .. i .. ']', x+4, y-95)
        else
            love.graphics.print(i, x+4, y-95)
        end

    end

    if self.tank.dead and self.restart <= 0 then
        love.graphics.setColor(255, 255, 255)
        love.graphics.setFont(resources.fonts.large)
        love.graphics.printf("Click to restart", SIZE.x/2-150, SIZE.y/2-10, 300, "center")
        love.graphics.setFont(resources.fonts.normal)
    end

    -- love.graphics.setColor(255, 255, 255)
    -- love.graphics.print(love.timer.getFPS(), 10, 10)
    -- local p = self.tank.position * (1/GRIDSIZE)
    -- love.graphics.print(math.floor(p.x+0.5) .. "/" .. math.floor(p.y+0.5), 10, 40)
end

function Game:onKeyPressed(k, u)
    if k == "escape" then
        self.paused = not self.paused
    elseif k == "q" then
        stack:pop()
    elseif k == " " and not self.tank.dead then
        local available = true
        for i, c in ipairs(COLORNAMES) do
            if self.tank.charges[c] < COSTS.bomb then available = false end
        end
        if available then
            self.world:add(Bomb(self.tank.position))
            for i, c in ipairs(COLORNAMES) do
                self.tank:charge(c, -COSTS.bomb)
            end
        end
    elseif k == "1" or k == "2" or k == "3" or k == "4" or k == "5" or k == "6" or k == "7" then
        self.tank.specials[COLORNAMES[tonumber(k)]] = not self.tank.specials[COLORNAMES[tonumber(k)]]
    end
end

function Game:onMousePressed(x, y, button)
    if button == "l" then
        if self.tank.dead and self.restart < 0 then
            self:reset()
        end
    elseif button == "r" then
        if not self.tank.dead then
            self.tank:special()
        end
    end
end
