cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "src/SocketIOclient.cpp" "src/WebSockets.cpp" "src/WebSocketsClient.cpp" "src/WebSocketsServer.cpp" "src/libb64/cdecode.c" "src/libb64/cencode.c" "src/libsha1/libsha1.c"
                    INCLUDE_DIRS "src/."
                    REQUIRES "arduino" # Library requires Arduino
                    )

project(WebSockets)
