include("/boot/core.lua");
password = {};

function password.read()
    local str = "";
    local n = 0;
    
    repeat
        ch = io.getchar();
        if ch == 13 then break; end
        
        if ch == 8 then 
            if n > 0 then
                n = n - 1;
                print("\008");
                str = string.sub(str, 1, n);
            end
        else 
            print("*");
            str = str .. string.char(ch);
            n = n + 1;
        end
    until n == 16
    return str;
end

function password.check()
    local boot_pwd = loader.getenv("bootlock_password");
    if boot_pwd ~= nil then
        while true do
            print("Boot password: ");
            if boot_pwd == password.read() then break; end
            print("\nloader: incorrect password!\n");
            loader.delay(3*1000*1000);
        end
    end
    
    local pwd = loader.getenv("password");
    if (pwd == nil) then return; end
    
    core.autoboot();
    
    while true do
        print("Password: ");
        if pwd == password.read() then break; end
        print("\nloader: incorrect password!\n");
        loader.delay(3*1000*1000);
    end
end