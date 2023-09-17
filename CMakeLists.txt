
cmake_minimum_required (VERSION 3.8)

project ("G_luaSerialPort" VERSION 1.0.0.1 DESCRIPTION "lua serialport" )

add_subdirectory(libserialport)
add_subdirectory(lua)
add_library (G_luaSerialPort SHARED "G_luaSerialPort.c" )
target_link_libraries(${PROJECT_NAME} PUBLIC lua libserialport)


