function love.conf(t)
    t.title = "Rainbowmb"
    t.author = "opatut"
    t.identity = "rainbowmb"
    t.version = "11.4" -- Löve version
    t.console = false
    t.release = false
    t.window.width = 1000
    t.window.height = 740
    t.window.fullscreen = false
    t.window.vsync = 1
    t.window.msaa = 0

    t.modules.joystick = false
    t.modules.audio = true
    t.modules.keyboard = true
    t.modules.event = true
    t.modules.image = true
    t.modules.graphics = true
    t.modules.timer = true
    t.modules.mouse = true
    t.modules.sound = true
    t.modules.physics = true
end

