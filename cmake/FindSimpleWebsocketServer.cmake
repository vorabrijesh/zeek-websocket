# - Try to find SimpleWebsocketServer include dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(SimpleWebsocketServer)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  SimpleWebsocketServer_ROOT_DIR          Set this variable to the root installation of
#                            SimpleWebsocketServer if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  SimpleWebsocketServer_FOUND             System has SimpleWebsocketServer, include and library dirs found
#  SimpleWebsocketServer_INCLUDE_DIR       The SimpleWebsocketServer include directories.
#  SimpleWebsocketServer_LIBRARIES         The SimpleWebsocketServer libraries.


find_path(SimpleWebsocketServer_ROOT_DIR
    NAMES include/simple-websocket-server/client_ws.hpp
    NAMES include/simple-websocket-server/server_ws.hpp
)

find_path(SimpleWebsocketServer_INCLUDE_DIR
    NAMES .
    HINTS ${SimpleWebsocketServer_ROOT_DIR}
)

find_library(SimpleWebsocketServer_LIBRARIES
    NAMES .
    HINTS ${SimpleWebsocketServer_ROOT_DIR}
)


set(SimpleWebsocketServer_LIBRARIES ${SimpleWebsocketServer_ROOT_DIR}
    CACHE STRING "SimpleWebsocketServer libraries" FORCE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SimpleWebsocketServer DEFAULT_MSG
    SimpleWebsocketServer_LIBRARIES
    SimpleWebsocketServer_INCLUDE_DIR
)

mark_as_advanced(
    SimpleWebsocketServer_ROOT_DIR
    SimpleWebsocketServer_INCLUDE_DIR
    SimpleWebsocketServer_LIBRARIES
)
