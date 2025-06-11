cmake_minimum_required(VERSION 3.31)
project(BingoServer)

set(CMAKE_CXX_STANDARD 23)

add_executable(BingoServer main.cpp
        src/server.cpp
        include/server.h
        include/nlohmann/json.hpp
        include/mutex_queue.h
        src/log.cpp
        include/log.h
        include/utils.h
        include/time.h
        src/time.cpp
        include/message_structures.h
        include/message_processor.h
        src/message_processor.cpp)

# Link the required libraries
target_link_libraries(BingoServer
        /usr/local/lib/uSockets.a   # static uSockets library
        z                           # zlib
        ssl                         # OpenSSL
        crypto                      # OpenSSL
        uv                          # libuv
        pthread
)