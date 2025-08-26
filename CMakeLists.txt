cmake_minimum_required(VERSION 3.31)
project(BingoServer)

set(CMAKE_CXX_STANDARD 23)

add_executable(BingoServer main.cpp
        src/time.cpp
        src/server.cpp
        src/authenticator.cpp
        src/log.cpp
        src/message_processor.cpp
        include/nlohmann/json.hpp
        include/authenticator.h
        include/log.h
        include/message_processor.h
        include/message_structures.h
        include/mutex_queue.h
        include/server.h
        include/time.h
        include/utils.h
        include/error_codes.h
        include/message_types.h
        include/session_manager.h
        src/session_manager.cpp
        include/session.h
        include/debug.h
        src/debug.cpp
        include/message_handler.h
        src/message_handler.cpp
        include/message_parser.h
        src/message_parser.cpp
        include/message_keys.h
        include/typedefs.h
        include/MessageResponseFactory/IMessageResponse.h
        src/MessageResponseFactory/LoginMessageResponse.cpp
        include/MessageResponseFactory/LoginMessageResponse.h
        src/MessageResponseFactory/LogoutMessageResponse.cpp
        include/MessageResponseFactory/LogoutMessageResponse.h
        src/MessageResponseFactory/SignupMessageResponse.cpp
        include/MessageResponseFactory/SignupMessageResponse.h
        src/MessageResponseFactory/SearchUserRequestResponse.cpp
        include/MessageResponseFactory/SearchUserRequestResponse.h
        include/MessageResponseFactory/ChangePasswordResponse.h
        src/MessageResponseFactory/ChangePasswordResponse.cpp
        include/MessageResponseFactory/UserProfileMessage.h
        src/MessageResponseFactory/UserProfileMessage.cpp
        include/MessageResponseFactory/PendingFriendRequests.h
        src/MessageResponseFactory/PendingFriendRequests.cpp
)

# Link the required libraries
target_link_libraries(BingoServer
        ${CMAKE_SOURCE_DIR}/lib/uSockets.a   # static uSockets library
        z                           # zlib
        ssl                         # OpenSSL
        crypto                      # OpenSSL
        uv                          # libuv
        pthread
        mysqlcppconnx
)