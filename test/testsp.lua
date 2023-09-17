package.cpath="./?.dll"

local g_serial= require "G_luaSerialPort"

for i=1,1 do
    local code,msg= g_serial.g_sp_open("COM1",9600)
    print(code,msg)
    local code,msg=g_serial.g_sp_write("123",3,100);
    print(code,msg)
    local code,msg=g_serial.g_sp_set_baudrate(115200);
    print(code,msg)
    -- local code,msg=g_serial.g_sp_set_parity(1);
    -- print(code,msg)
    -- local code,msg=g_serial.g_sp_set_parity(0);
    -- print(code,msg)
    -- local code,msg=g_serial.g_sp_set_rts(0);
    -- print(code,msg)
    -- local code,msg=g_serial.g_sp_set_rts(1);
    -- print(code,msg)
    -- local code,msg=g_serial.g_sp_set_dtr(1);
    -- print(code,msg)
    -- local code,msg=g_serial.g_sp_set_dtr(0);
    -- print(code,msg)
    local msg=g_serial.g_sp_read(100,10000);
    print(msg)
    g_serial.g_sp_close();
end


print("closed")