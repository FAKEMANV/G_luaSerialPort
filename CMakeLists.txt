
cmake_minimum_required (VERSION 3.8)

project ("G_luaSerialPort" VERSION 1.0.0.1 DESCRIPTION "lua serialport" )

add_subdirectory(libserialport)
add_subdirectory(lua)
add_library (${PROJECT_NAME} SHARED "G_luaSerialPort.c" )
target_link_libraries(${PROJECT_NAME} PUBLIC liblua libserialport)

add_library (G_luaSerialPort_S STATIC "G_luaSerialPort.c" )
target_link_libraries(G_luaSerialPort_S PUBLIC liblua libserialport)

get_target_property(libs ${PROJECT_NAME} LINK_LIBRARIES)
message("${PROJECT_NAME} libs :${libs}")

