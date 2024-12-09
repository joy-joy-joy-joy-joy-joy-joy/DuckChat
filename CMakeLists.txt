cmake_minimum_required(VERSION 3.27)
project(GateServer)

# 基本配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 静态链接配置
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# Boost 配置
set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)
set(BOOST_ROOT "${PROJECT_SOURCE_DIR}/include/boost-1_86")
set(BOOST_INCLUDEDIR "${PROJECT_SOURCE_DIR}/include/boost-1_86")
set(BOOST_LIBRARYDIR "${PROJECT_SOURCE_DIR}/lib")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_NO_SYSTEM_PATHS ON)

# 手动指定 Boost 库文件
set(BOOST_FILESYSTEM_LIBRARY "${PROJECT_SOURCE_DIR}/lib/libboost_filesystem-mgw13-mt-x64-1_86.a")
set(BOOST_SYSTEM_LIBRARY "${PROJECT_SOURCE_DIR}/lib/libboost_system-mgw13-mt-x64-1_86.a")
set(BOOST_THREAD_LIBRARY "${PROJECT_SOURCE_DIR}/lib/libboost_thread-mgw13-mt-x64-1_86.a")
set(BOOST_CHRONO_LIBRARY "${PROJECT_SOURCE_DIR}/lib/libboost_chrono-mgw13-mt-x64-1_86.a")
set(BOOST_ATOMIC_LIBRARY "${PROJECT_SOURCE_DIR}/lib/libboost_atomic-mgw13-mt-x64-1_86.a")

# MSYS2 包配置
set(gRPC_DIR "D:/MSYS2/mingw64/lib/cmake/grpc")
set(Protobuf_DIR "D:/MSYS2/mingw64/lib/cmake/protobuf")
set(utf8_range_DIR "D:/MSYS2/mingw64/lib/cmake/utf8_range")
set(absl_DIR "D:/MSYS2/mingw64/lib/cmake/absl")
set(re2_DIR "D:/MSYS2/mingw64/lib/cmake/re2")
set(c-ares_DIR "D:/MSYS2/mingw64/lib/cmake/c-ares")
set(REDIS_PLUS_PLUS_LIBRARY "D:/MSYS2/mingw64/lib/libredis++_static.a")
set(HIREDIS_LIBRARY "D:/MSYS2/mingw64/lib/libhiredis.a")
set(MARIADB_INCLUDE_DIR "D:/MSYS2/mingw64/include/mariadb")
set(MARIADB_LIBRARY "D:/MSYS2/mingw64/lib/libmariadb.dll.a")

# 使用 MSYS2 的 JsonCpp
set(JSONCPP_ROOT "D:/MSYS2/mingw64")
set(JSONCPP_INCLUDE_DIR "${JSONCPP_ROOT}/include")
set(JSONCPP_LIBRARY "${JSONCPP_ROOT}/lib/libjsoncpp.dll.a")

# 全局预处理器定义
add_definitions(
        -DJSON_DLL=0
        -DJSON_STATIC=1
        -DJSON_API=
        -DJSONCPP_STATIC_DEFINE  # 添加这个定义
)

# Proto 库
add_library(proto_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/protos/verify.pb.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/protos/verify.grpc.pb.cc
)

# Proto 库链接 JsonCpp
target_link_libraries(proto_lib
        PRIVATE
        ${JSONCPP_LIBRARY}
)

# 包含目录
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/Sources/Headers
        ${BOOST_INCLUDEDIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Sources/protos
        ${JSONCPP_INCLUDE_DIR}
        "D:/MSYS2/mingw64/include"
        ${MARIADB_INCLUDE_DIR}
)

# 其他依赖
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(re2 CONFIG REQUIRED)
find_package(c-ares CONFIG REQUIRED)

# JsonCpp 配置
find_package(jsoncpp CONFIG REQUIRED)

# 主可执行文件
add_executable(GateServer
        Sources/main.cpp
        Sources/CServer.cpp
        Sources/HttpConnection.cpp
        Sources/LoginSystem.cpp
        Sources/VerifyGrpcClient.cpp
        Sources/ConfigMgr.cpp
        Sources/headers/ConfigMgr.h
        Sources/AsioIOServicePool.cpp
        Sources/headers/AsioIOServicePool.h
        Sources/RedisMgr.cpp
        Sources/headers/RedisMgr.h
        Sources/RedisConPool.cpp
        Sources/headers/RedisConPool.h
        Sources/MariaPool.cpp
        Sources/headers/MariaPool.h
        Sources/headers/MariaDao.h
        Sources/MariaDao.cpp
)

# 目标特定的预处理器定义
target_compile_definitions(GateServer PRIVATE
        BOOST_ALL_NO_LIB
        BOOST_ALL_STATIC_LINK
        PROTOBUF_USE_STATIC_LIBS
        GRPC_STATIC_DEFINE
)

# 链接配置
target_link_libraries(GateServer
        PRIVATE
        ${MARIADB_LIBRARY}
        curl
        zstd
        proto_lib
        protobuf::libprotobuf
        gRPC::grpc++
        gRPC::grpc++_reflection
        ${JSONCPP_LIBRARY}
        ${BOOST_FILESYSTEM_LIBRARY}
        ${BOOST_SYSTEM_LIBRARY}
        ${BOOST_THREAD_LIBRARY}
        ${BOOST_CHRONO_LIBRARY}
        ${BOOST_ATOMIC_LIBRARY}
        -static
        -static-libgcc
        -static-libstdc++
        ws2_32
        mswsock
        secur32
        crypt32
        bcrypt
        shlwapi
        iphlpapi
        ${REDIS_PLUS_PLUS_LIBRARY}
        ${HIREDIS_LIBRARY}
)