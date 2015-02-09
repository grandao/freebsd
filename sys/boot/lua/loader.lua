LOADED = {};

function include(filename)
    if LOADED[filename] == nil then
        loader.include(filename);
        LOADED[filename] = true;
    end
end


include("/boot/password.lua");
include("/boot/config.lua");

config.load();
password.check();

include("/boot/menu.lua");
menu.run();
