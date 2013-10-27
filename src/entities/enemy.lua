require("util/helper")
require("util/vector")

require("scene/entity")
require("entities/bullet")

Enemy = class("Enemy", Entity)

function Enemy:__init(position)
    Entity.__init(self)
    self.z = -1000
    self.physicsObject = {}
    self.position = position

    self.shootTimeout = 0

    self.charges = {}
    for i, k in ipairs(COLORNAMES) do self.charges[k] = 0.5 end
end

function Enemy:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(16)
    self.physicsObject.body:setLinearDamping(0)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    self.physicsObject.fixture:setCategory(PHYSICS_GROUPS.ENEMY)
end

function Enemy:onRemove()
    self.world:add(Blob(self.position))
    self.world:add(Explosion(self.position, 2))
end

function Enemy:onUpdate(dt)
    local dir = (states.game.tank.position - self.position):normalized()

    local currentVelocity = Vector(self.physicsObject.body:getLinearVelocity())
    local targetVelocity = dir * 200
    local factor = dt * 10
    local velocity = currentVelocity * (1-factor) + targetVelocity * factor
    self.physicsObject.body:setLinearVelocity(velocity.x, velocity.y)
end

function Enemy:onDraw()
    love.graphics.setColor(255, 0, 0)
    love.graphics.circle("fill", self.position.x, self.position.y, 16)
end

function Enemy:onCollide(other)
    if other.__name == "Bullet" then
        self:kill()
    end
end
