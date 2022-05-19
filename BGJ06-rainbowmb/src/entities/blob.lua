require("util/helper")
require("util/vector")

require("scene/entity")

Blob = class("Blob", Entity)

function Blob:__init(position)
    Entity.__init(self)
    self.position = position
    self.physicsObject = {}
    self.size = 1

    self.color = COLORNAMES[math.random(1, #COLORNAMES)]
end

function Blob:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "static")
    self.physicsObject.shape = love.physics.newCircleShape(10)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    self.physicsObject.fixture:setCategory(PHYSICS_GROUPS.PLAYER)
    self.physicsObject.fixture:setSensor(true)
end

function Blob:onUpdate(dt)
    if self.lifetime ~= 0 then
        self.size =  1 - self:progress()
    end
end

function Blob:onDraw()
    local beat = math.abs(math.sin(self.age))
    -- local points = {}
    -- local c = 10
    -- for i = 0, c do
    --     local phi = 1.0*(i*2+self.age*5)/c*2*math.pi
    --     local d = 1
    --     d = d + 0.1 * math.sin((i+self.age*2)/c*2*math.pi*2)
    --     d = d + 0.05 * math.sin((i-self.age*2)/c*2*math.pi*5)
    --     local v = self.position + Vector(1, 0):rotated(i/c*2*math.pi) * d * 6 * self.size * (beat*0.5+1)
    --     table.insert(points, v.x)
    --     table.insert(points, v.y)
    -- end
    love.graphics.setColor(unpack(COLORS[self.color]))
    love.graphics.setBlendMode("add")
    -- love.graphics.polygon("fill", unpack(points))
    love.graphics.circle("fill", self.position.x, self.position.y, 6*beat)
    love.graphics.draw(resources.images.particle, self.position.x, self.position.y, 0, self.size * (1+beat*0.2), self.size * (1+beat*0.2),
        resources.images.particle:getWidth()/2, resources.images.particle:getHeight()/2)
    love.graphics.setBlendMode("alpha")
end
