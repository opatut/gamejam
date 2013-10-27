require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/tank")
require("entities/building")

Game = class("Game", GameState)

function Game:__init()
    self.paused = false
    self:reset()
end

function Game:reset()
    self.world = World()
    self.tank = Tank()
    self.tank.position = Vector(200, 200)
    self.world:add(self.tank)

    local b = Building()
    b.position = Vector(300, 400)
    self.world:add(b)
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
end

function Game:onDraw()
    love.graphics.setBackgroundColor(10, 10, 10)
    love.graphics.clear()

    self.world:draw()
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
