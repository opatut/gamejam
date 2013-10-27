require("util/helper")
require("util/vector")

require("scene/entity")
require("entities/explosion")

Bullet = class("Bullet", Entity)

function Bullet:__init()
    Entity.__init(self)
    self.z = -1000
    self.physicsObject = {}
    self.radius = 1
    self.lifetime = 1
end

function Bullet:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(self.radius)
    self.physicsObject.body:setBullet(true)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    self.physicsObject.fixture:setCategory(PHYSICS_GROUPS.BULLET)
    self.physicsObject.fixture:setMask(PHYSICS_GROUPS.PLAYER)

    self.physicsObject.body:setLinearVelocity(self.velocity:unpack())
end

function Bullet:onRemove()
    self.world:add(Explosion(self.position))
end

function Bullet:onCollide(other)
    self:kill()
end

function Bullet:onUpdate(dt)
    self.position = Vector(self.physicsObject.body:getX(), self.physicsObject.body:getY())
end

function Bullet:onDraw()
    love.graphics.setColor(255, 255, 200)
    local p = self.position - self.velocity:normalized() * 3
    local e = self.position + self.velocity:normalized() * 3
    love.graphics.line(p.x, p.y, e.x, e.y)

    love.graphics.setBlendMode("additive")
    love.graphics.setColor(240, 250, 60, 150)
    love.graphics.draw(resources.images.particle, self.position.x, self.position.y, 0, 0.3, 0.3,
        resources.images.particle:getWidth()/2, resources.images.particle:getHeight()/2)
    love.graphics.setBlendMode("alpha")
end
