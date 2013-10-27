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

    self.charges = {}
    for i, k in ipairs(COLORNAMES) do self.charges[k] = 0.5 end
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
    local mouse = states.game:mouse()
    local diff = mouse - self.position
    return diff:normalized()
end

function Tank:onUpdate(dt)
    local dir = states.game:getKeyboardVector()

    local currentVelocity = Vector(self.physicsObject.body:getLinearVelocity())
    local targetVelocity = dir * 300
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
    love.graphics.setColor(40, 40, 40)
    love.graphics.rectangle("fill", -9, -5, 17, 10)
    love.graphics.setColor(100, 100, 100)
    love.graphics.rectangle("fill", -10, -7, 20, 2)
    love.graphics.rectangle("fill", -10,  5, 20, 2)
    love.graphics.pop()

    love.graphics.push()
    love.graphics.translate(self.position.x, self.position.y)
    love.graphics.rotate(self:getBarrelVector():angle())
    love.graphics.setColor(255, 255, 255)
    love.graphics.rectangle("fill", -2, -2, 12, 4)
    love.graphics.pop()
end

function Tank:shoot()
    -- local c = 0.005
    if self.shootTimeout > 0 then return end
    local bullet = Bullet()
    bullet.position = self.position
    bullet.velocity = self:getBarrelVector() * 500 + self.velocity
    self.world:add(bullet)
    self.shootTimeout = 0.25
    -- self:charge("yellow", -c)
end

function Tank:charge(color, diff)
    self.charges[color] = math.min(1, math.max(0, self.charges[color] + diff))
end

function Tank:onCollide(other)
    if other.__name == "Blob" then
        other.age = 0
        other.lifetime = 0.1
        other.physicsObject.body:destroy()
        other.physicsObject = nil
        self:charge(other.color, 0.2)
    elseif other.__name == "Enemy" then
        other:kill()
        self:kill()
        states.game:reset()
    end
end
