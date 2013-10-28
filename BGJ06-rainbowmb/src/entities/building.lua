require("util/helper")
require("util/vector")
require("scene/entity")

Building = class("Building", Entity)

function Building:__init(x, y, width, height)
    Entity.__init(self)
    self.z = -100
    self.size = Vector(width or 1, height or 1)
    self.position = Vector(x * GRIDSIZE, y*GRIDSIZE)
    self.x = x
    self.y = y
    self.physicsObject = {}

end

function Building:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "static")
    self.physicsObject.shape = love.physics.newRectangleShape(self.size.x * GRIDSIZE, self.size.y * GRIDSIZE)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    self.physicsObject.fixture:setCategory(PHYSICS_GROUPS.WORLD)
end

function Building:onUpdate(dt)
end

function Building:onDraw()
    love.graphics.setColor(10, 10, 10)
    local s = self.size * GRIDSIZE
    love.graphics.rectangle("fill", self.position.x - s.x/2, self.position.y - s.y/2, s.x, s.y)

    love.graphics.setColor(255, 255, 255)
    if not states.game:hasBuildingAt(self.x+1, self.y+0) then
        love.graphics.line( self.position.x + GRIDSIZE/2, self.position.y - GRIDSIZE/2,
                            self.position.x + GRIDSIZE/2, self.position.y + GRIDSIZE/2 )
    end
    if not states.game:hasBuildingAt(self.x-1, self.y+0) then
        love.graphics.line( self.position.x - GRIDSIZE/2, self.position.y - GRIDSIZE/2,
                            self.position.x - GRIDSIZE/2, self.position.y + GRIDSIZE/2 )
    end
    if not states.game:hasBuildingAt(self.x+0, self.y+1) then
        love.graphics.line( self.position.x + GRIDSIZE/2, self.position.y + GRIDSIZE/2,
                            self.position.x - GRIDSIZE/2, self.position.y + GRIDSIZE/2 )
    end
    if not states.game:hasBuildingAt(self.x+0, self.y-1) then
        love.graphics.line( self.position.x + GRIDSIZE/2, self.position.y - GRIDSIZE/2,
                            self.position.x - GRIDSIZE/2, self.position.y - GRIDSIZE/2 )
    end
end
