require("util/helper")
require("util/vector")

require("scene/entity")

Explosion = class("Explosion", Entity)

function Explosion:__init(position, size)
    Entity.__init(self)
    self.position = position
    self.size = size or 1
    self.lifetime = 0.2*self.size

    self.particleSystem = love.graphics.newParticleSystem(resources.images.particle, 128)
    self.particleSystem:start()
    self.particleSystem:setSizes(0.4, 0.25*self.size, 0.1*self.size, 0.0)
    self.particleSystem:setColors(
        240, 250, 50, 250,
        250, 10, 10, 100,
        250, 250, 250, 0)
    self.particleSystem:setEmissionRate(10000)
    self.particleSystem:setParticleLife(self.lifetime * 0.6, self.lifetime)
    self.particleSystem:setSpread(2*math.pi)
    local s = math.log(self.size+1)
    self.particleSystem:setSpeed(200*s, 400*s)
    self.particleSystem:setRadialAcceleration(-200 / self.lifetime)
end

function Explosion:onUpdate(dt)
    self.particleSystem:setPosition(self.position:unpack())
    self.particleSystem:update(dt)
    self.particleSystem:stop()
end

function Explosion:onDraw()
    love.graphics.setBlendMode("additive")
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")
end
