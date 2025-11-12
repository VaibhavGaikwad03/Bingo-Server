cmake_minimum_required(VERSION 3.31)
project(BingoServer)

set(CMAKE_CXX_STANDARD 23)

add_executable(BingoServer main.cpp
        src/utils/time.cpp
        src/server.cpp
        src/authenticator.cpp
        src/utils/logger.cpp
        src/message_processor.cpp
        include/nlohmann/json.hpp
        include/authenticator.h
        include/utils/logger.h
        include/message_processor.h
        include/message_structures.h
        include/thread_safe_queue.h
        include/server.h
        include/utils/time.h
        include/utils.h
        include/error_codes.h
        include/message_types.h
        include/user_session_manager.h
        src/user_session_manager.cpp
        include/user_session.h
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
        src/MessageResponseFactory/SearchUserRequestMessageResponse.cpp
        include/MessageResponseFactory/SearchUserRequestMessageResponse.h
        include/MessageResponseFactory/ChangePasswordMessageResponse.h
        src/MessageResponseFactory/ChangePasswordMessageResponse.cpp
        include/MessageResponseFactory/UserProfileMessage.h
        src/MessageResponseFactory/UserProfileMessage.cpp
        include/MessageResponseFactory/PendingFriendRequests.h
        src/MessageResponseFactory/PendingFriendRequests.cpp
        src/SignalHandler/handle_signals.cpp
        include/SignalHandler/handle_signals.h
        src/utils/token_generator.cpp
        include/utils/token_generator.h
        include/MessageResponseFactory/ReconnectMessageResponse.h
        src/MessageResponseFactory/ReconnectMessageResponse.cpp
        include/MessageResponseFactory/FriendsListMessage.h
        src/MessageResponseFactory/FriendsListMessage.cpp
        src/utils/config_reader.cpp
        include/utils/config_reader.h
        include/file_paths.h
        src/MessageResponseFactory/UpdateProfileResponse.cpp
        include/MessageResponseFactory/UpdateProfileResponse.h
        include/MessageResponseFactory/TokenLoginResponse.h
        src/MessageResponseFactory/TokenLoginResponse.cpp
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