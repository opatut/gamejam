require("util/helper")
require("util/vector")

require("scene/entity")

Bomb = class("Bomb", Entity)

function Bomb:__init(position)
    Entity.__init(self)
    self.position = position
    self.lifetime = 3
    self.radius = 0
    self.z = -1000
end

function Bomb:onUpdate(dt)
    self.radius = math.log(self.age+1, 10) * 5 * 512
    -- get ever enemy inside
    for k, v in pairs(self.world:findByType("Enemy", true)) do
        if Vector.dist(v.position, self.position) <= self.radius*0.8 then
            v:kill()
        end
    end
end

function Bomb:onDraw()
    love.graphics.setBlendMode("additive")
    for i, c in ipairs(COLORNAMES) do
        local w = resources.images.ring:getWidth()
        local r,g,b = unpack(COLORS[c])
        local s = self.radius / 512 * (1+(7-i)*0.04)
        love.graphics.setColor(r, g, b, 255 * (1-self:progress()))
        love.graphics.draw(resources.images.ring, self.position.x, self.position.y, 0, s, s, w/2, w/2)
    end
    love.graphics.setBlendMode("alpha")
end
