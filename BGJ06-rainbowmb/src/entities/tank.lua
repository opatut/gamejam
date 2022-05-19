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
    self.specials = {}
    for i, k in ipairs(COLORNAMES) do
        self.charges[k] = 0.5
        -- self.specials[k] = false
    end

    self.canvas = love.graphics.newCanvas(128, 128)

    self.specialEnabled = false
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
    self.specialEnabled = love.keyboard.isDown("lshift")

    local dir = states.game:getKeyboardVector()

    if self.specials.blue then
        self:charge("blue", -0.1*dt)
    end
    local currentVelocity = Vector(self.physicsObject.body:getLinearVelocity())
    local targetVelocity = dir * 300 * (self.specials.blue and 2 or 1)
    local factor = dt * 10
    local velocity = currentVelocity * (1-factor) + targetVelocity * factor
    self.physicsObject.body:setLinearVelocity(velocity.x, velocity.y)
    self.physicsObject.body:setAngle(velocity:angleTo(Vector(1, 0)))

    self.position = Vector(self.physicsObject.body:getX(), self.physicsObject.body:getY())
    self.rotation = self.physicsObject.body:getAngle()

    self.shootTimeout = self.shootTimeout - dt
    if love.mouse.isDown(1) then
        self:shoot()
    end

    -- hacky before translate etc.
    love.graphics.setCanvas(self.canvas)
        love.graphics.setBackgroundColor(0, 0, 0)
        love.graphics.clear()
        love.graphics.setColor(255, 255, 255)
        love.graphics.draw(resources.images.tank_bottom, 64, 64, math.pi/2+self.rotation, 1, 1,
            resources.images.tank_bottom:getWidth()/2, resources.images.tank_bottom:getHeight()/2)
        love.graphics.draw(resources.images.tank_top, 64, 64, math.pi/2+self:getBarrelVector():angle(), 1, 1,
            resources.images.tank_top:getWidth()/2, resources.images.tank_top:getHeight()/2)
    love.graphics.setCanvas()
end

function Tank:onDraw()
    love.graphics.setColor(255, 255, 255)

    love.graphics.setBlendMode("add")
    love.graphics.draw(self.canvas, self.position.x, self.position.y, 0, 0.25, 0.25, 64, 64)
    love.graphics.setBlendMode("alpha")
end

function Tank:shoot()
    -- local c = 0.005
    if self.shootTimeout > 0 then return end

    local n = self.specials.violet and 1 or 0
    for i=-n,n do
        local bullet = Bullet(self.specials.indigo and 3 or 1)
        bullet.position = self.position
        bullet.velocity = self:getBarrelVector() * 500 + self.velocity
        bullet.velocity:rotate(i*0.1)
        self.world:add(bullet)
    end

    self.shootTimeout = self.specials.orange and 0.05 or 0.25
    -- self:charge("yellow", -c)

    if self.specials.indigo then
        self:charge("indigo", -0.03)
    end
    if self.specials.violet then
        self:charge("violet", -0.03)
    end
    if self.specials.orange then
        self:charge("orange", -0.01)
    end
end

function Tank:charge(color, diff)
    self.charges[color] = math.min(1, math.max(0, self.charges[color] + diff))
    if diff < 0 and self.charges[color] <= 0 then self.specials[color] = false end
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
        states.game:died()
    end
end

function Tank:special()
    if self.specials.red then
        print("mine")
    end
end
