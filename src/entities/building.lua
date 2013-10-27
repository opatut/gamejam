require("util/helper")
require("util/vector")
require("scene/entity")

Building = class("Building", Entity)

function Building:__init()
    Entity.__init(self)
    self.z = -100
    self.size = Vector(1, 1)
    self.physicsObject = {}

    -- self.particleSystem = love.graphics.newParticleSystem(resources.images.particle, 128)
    -- self.particleSystem:start()
    -- self.particleSystem:setSizes(0.2, 0.9)
    -- self.particleSystem:setColors(
    --     240, 250, 50, 250,
    --     250, 10, 10, 0)
    -- self.particleSystem:setEmissionRate(100)
    -- self.particleSystem:setParticleLife(0.5)
    -- self.particleSystem:setSpread(0.5)
end

function Building:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "static")
    self.physicsObject.shape = love.physics.newRectangleShape(self.size.x * GRIDSIZE, self.size.y * GRIDSIZE)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    self.physicsObject.fixture:setCategory(PHYSICS_GROUPS.WORLD)
end

function Building:onUpdate(dt)
    -- self.particleSystem:update(dt)
    -- self.particleSystem:setPosition(self:getPosition().x, self:getPosition().y)
end

function Building:onDraw()
    love.graphics.setColor(50, 50, 50)
    local s = self.size * GRIDSIZE
    love.graphics.rectangle("fill", self.position.x - s.x/2, self.position.y - s.y/2, s.x, s.y)
end
