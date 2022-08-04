
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZeekPluginWebsocket)

include(ZeekPlugin)
find_package(SimpleWebsocketServer)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_compile_options(-pthread -lpthread)
find_package(Threads REQUIRED)

if (SIMPLEWEBSOCKETSERVER_FOUND)
    include_directories(BEFORE ${SimpleWebsocketServer})
    zeek_plugin_begin(Ennetix Websocket ${ZEEK_PLUGIN_BEGIN_OPTS})
    zeek_plugin_cc(src/WebsocketWriter.cc)
    zeek_plugin_cc(src/Plugin.cc)
    zeek_plugin_bif(src/websocket.bif)
    zeek_plugin_dist_files(README CHANGES COPYING VERSION)
    # zeek_plugin_scripts(
    #         scripts/__load__.zeek
    #         scripts/__preload__.zeek
    #         scripts/types.zeek
    #         scripts/Ennetix/Websocket/__load__.zeek)
    zeek_plugin_link_library(${SimpleWebsocketServer_LIBRARIES})
    zeek_plugin_end()

elseif (NOT SIMPLEWEBSOCKETSERVER_FOUND)
    message(FATAL_ERROR "SIMPLEWEBSOCKETSERVER not found.")
endif()

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION LIMIT_COUNT 1)

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    # Allows building rpm/deb packages via "make package" in build dir.
    include(ConfigurePackaging)
    ConfigurePackaging(${VERSION})
endif ()
