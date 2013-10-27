require("util/resources")
require("util/gamestack")
require("util/settings")

-- require("states/intro")
require("states/game")

-- global subsystems
resources = Resources("data/")
settings = Settings()
settings:load()

-- global variables
SIZE = Vector(love.graphics.getWidth(), love.graphics.getHeight())
HALFSIZE = SIZE / 2
FULLSCREEN = settings:get("fullscreen", false)
MUTE = settings:get("mute", false)
GRIDSIZE=50

PHYSICS_GROUPS = {}
PHYSICS_GROUPS.PLAYER = 0x0001
PHYSICS_GROUPS.ENEMY  = 0x0002
PHYSICS_GROUPS.BULLET = 0x0004
PHYSICS_GROUPS.WORLD  = 0x0008


function reset()
    -- start game
    states = {}
    -- states.intro = Intro()
    states.game = Game()
    stack = GameStack()
    stack:push(states.game)
end

function love.load()
    math.randomseed(os.time())

    -- setFullscreen(FULLSCREEN)
    -- setMute(MUTE)

    resources:addImage("particle", "particle.png")

    -- load fonts
    resources:addFont("normal", "DejaVuSans.ttf", 20)

    -- load music
    -- resources:addMusic("background", "background-music.ogg")

    -- load sounds
    -- resources:addSound("bell", "bell.wav")

    resources:load()

    reset()

    -- resources.music.background:play()
    -- resources.music.background:setVolume(0.1)
    -- resources.music.background:setLooping(true)
end

function love.update(dt)
    stack:update(dt)
end

function love.draw()
    stack:draw()

    -- love.graphics.setFont(resources.fonts.tiny)
    -- love.graphics.print("FPS: " .. love.timer.getFPS(), 5, 5)
end

function love.keypressed(k, u)
    if k == "f" then
        -- setFullscreen(not FULLSCREEN)
    elseif k == "m" then
        -- setMute(not MUTE)
    end
    stack:keypressed(k, u)
end

function love.mousepressed( x, y, button )
    stack:mousepressed(x, y, button)
end

-- function setFullscreen(fullscreen)
--     _, _, fs = love.graphics.getMode()
--     if fs == fullscreen then return end

--     if fullscreen then
--         modes = love.graphics.getModes()
--         table.sort(modes, function(a, b) return a.width*a.height < b.width*b.height end)
--         love.graphics.setMode(modes[#modes].width, modes[#modes].height, true)
--     else
--         love.graphics.setMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y, false)
--     end

--     local x, y = love.graphics.getMode()
--     SIZE = Vector(x, y)
--     HALFSIZE = SIZE/2

--     FULLSCREEN = fullscreen
--     settings:set("fullscreen", fullscreen)
--     settings:save()
-- end

-- function setMute(mute)
--     love.audio.setVolume(mute and 0 or 1)
--     MUTE = mute
--     settings:set("mute", mute)
--     settings:save()
-- end

function love.quit()
end
