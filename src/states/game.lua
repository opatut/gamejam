require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/tank")
require("entities/building")
require("entities/blob")
require("entities/enemy")

Game = class("Game", GameState)

function Game:__init()
    self.paused = false
    self:reset()
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

function Game:getKeyboardVector()
    local v = Vector()
    if love.keyboard.isDown("a") or love.keyboard.isDown("left")  then v.x = v.x - 1 end
    if love.keyboard.isDown("d") or love.keyboard.isDown("right") then v.x = v.x + 1 end
    if love.keyboard.isDown("w") or love.keyboard.isDown("up")    then v.y = v.y - 1 end
    if love.keyboard.isDown("s") or love.keyboard.isDown("down")  then v.y = v.y + 1 end
    return v:normalized()
end

function Game:onUpdate(dt)
    self.world:update(dt)
    self.offset = Vector(love.graphics.getWidth(), love.graphics.getHeight()) * 0.5 - self.tank.position

    self.enemySpawn = self.enemySpawn - dt
    if self.enemySpawn <= 0 then
        local p
        repeat
            p = Vector(math.random(-MAPSIZE/2, MAPSIZE/2), math.random(-MAPSIZE/2, MAPSIZE/2))
        until Vector.dist(p*GRIDSIZE, self.tank.position) > (SIZE/2):len() and not self.map[p.x][p.y]

        self.world:add(Enemy(p*GRIDSIZE))
        self.enemySpawn = math.random(0.5, 2.0)
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
    for i, c in ipairs(COLORNAMES) do
        local x, y = 20 + (i-1) * 20, SIZE.y - 10
        love.graphics.setColor(255, 255, 255)
        love.graphics.draw(resources.images.container_back, x, y, 0, 1, 1, 0, resources.images.container_back:getHeight())

        love.graphics.setColor(unpack(COLORS[c]))
        local h = 70 * self.tank.charges[c]
        love.graphics.rectangle("fill", x+3, y-5-h, 10, h)

        love.graphics.setColor(255, 255, 255)
        love.graphics.draw(resources.images.container_front, x, y, 0, 1, 1, 0, resources.images.container_front:getHeight())
    end
    love.graphics.setColor(255, 255, 255, 200)
    local h = SIZE.y-15-0.2*70
    love.graphics.line(10, h, 7*20+25, h)

    love.graphics.setColor(255, 255, 255)
    love.graphics.print(love.timer.getFPS(), 10, 10)
    local p = self.tank.position * (1/GRIDSIZE)
    love.graphics.print(math.floor(p.x+0.5) .. "/" .. math.floor(p.y+0.5), 10, 40)
end

function Game:onKeyPressed(k, u)
    if k == "escape" then
        self.paused = not self.paused
    elseif k == "q" then
        stack:pop()
    end
end

function Game:onMousePressed()
    self.tank:shoot()
end
