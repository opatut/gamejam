require("util/helper")
require("util/vector")

require("scene/entity")
require("entities/bullet")

Tank = class("Tank", Entity)

function Tank:__init()
    Entity.__init(self)
    self.z = -1000
    self.physicsObject = {}

    self.shootTimeout = 0
end

function Tank:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newRectangleShape(20, 15)
    self.physicsObject.body:setLinearDamping(0)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    self.physicsObject.fixture:setCategory(PHYSICS_GROUPS.PLAYER)
end

function Tank:getBarrelVector()
    local mouse = Vector(love.mouse.getPosition())
    local diff = mouse - self.position
    return diff:normalized()
end

function Tank:onUpdate(dt)
    local dir = states.game:getKeyboardVector()

    local currentVelocity = Vector(self.physicsObject.body:getLinearVelocity())
    local targetVelocity = dir * dt * 200 * 100
    local factor = dt * 10
    local velocity = currentVelocity * (1-factor) + targetVelocity * factor
    self.physicsObject.body:setLinearVelocity(velocity.x, velocity.y)
    self.physicsObject.body:setAngle(velocity:angleTo(Vector(1, 0)))

    self.position = Vector(self.physicsObject.body:getX(), self.physicsObject.body:getY())
    self.rotation = self.physicsObject.body:getAngle()

    self.shootTimeout = self.shootTimeout - dt
    if love.mouse.isDown("l") then
        self:shoot()
    end
end

function Tank:onDraw()
    love.graphics.push()
    love.graphics.translate(self.position.x, self.position.y)
    love.graphics.rotate(self.rotation)
    love.graphics.setColor(255, 255, 255)
    local s = Vector(20, 15)
    love.graphics.rectangle("fill", -s.x/2, -s.y/2, s.x, s.y)
    love.graphics.pop()
end

function Tank:shoot()
    if self.shootTimeout > 0 then return end
    local bullet = Bullet()
    bullet.position = self.position
    bullet.velocity = self:getBarrelVector() * 1500
    self.world:add(bullet)
    self.shootTimeout = 0.06
end
