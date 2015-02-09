include("/boot/core.lua");

screen = {};
color = {};

color.BLACK   = 0;
color.RED     = 1;
color.GREEN   = 2;
color.YELLOW  = 3;
color.BLUE    = 4;
color.MAGENTA = 5;
color.CYAN    = 6;
color.WHITE   = 7;

color.DEFAULT = 0;
color.BRIGHT  = 1;
color.DIM     = 2;

function color.isEnabled()
    local c = loader.getenv("loader_color");
    if c ~= nil then
        if c:lower() == "no"  or c == "0" then
            return false;
        end
    end
    return not core.bootserial();
end

color.disabled = not color.isEnabled();


function color.escapef(c)
    if color.disabled then return c; end
    return "\027[3"..c.."m";
end

function color.escapeb(c)
    if color.disabled then return c; end
    return "\027[4"..c.."m";
end

function color.escape(fg, bg, att)
    if color.disabled then return ""; end
    if not att then att = "" else att = att..";"; end
    return "\027["..att.."3"..fg..";4"..bg.."m";
end

function color.default()
    if color.disabled then return ""; end
    return "\027[0;37;40m";
end

function color.highlight(str)
    if color.disabled then return str; end
    return "\027[1m"..str.."\027[0m";
end

function screen.clear()
    if core.bootserial() then return; end
    print("\027[H\027[J");
end

function screen.setcursor(x, y)
    if core.bootserial() then return; end
    print("\027["..y..";"..x.."H");
end

function screen.setforeground(c)
    if color.disabled then return c; end
    print("\027[3"..c.."m");
end

function screen.setbackground(c)
    if color.disabled then return c; end
    print("\027[4"..c.."m");
end

function screen.defcolor()
    print(color.default());
end

function screen.defcursor()
    if core.bootserial() then return; end
    print("\027[25;0H");
end