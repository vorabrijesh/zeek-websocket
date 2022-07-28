
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZeekPluginWebsocket)

include(ZeekPlugin)
find_package(SimpleWebsocketServer)

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
