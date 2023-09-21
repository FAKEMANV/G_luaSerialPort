package.cpath="./?.dll"



for i=1,10 do
    local g_serial= require "G_luaSerialPort"
    local sp= g_serial.new()
    print(sp)
    local code,msg =sp:open("COM1",9600)
    print(code,msg)
    local code,msg=sp:write("123",100);
    print(code,msg)
    local msg=sp:read(1024,1000);
    print(#msg,msg)
    sp:close();
   
end


-- print("closed")