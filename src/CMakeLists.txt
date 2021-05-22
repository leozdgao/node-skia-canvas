
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${FLAGS}")
    include_directories("/System/Library/Frameworks/")
    find_library(APP_SERVICES_LIBRARY ApplicationServices)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

file(GLOB SOURCE_FILES
    "init.cc"
    "Canvas.cc"
    "CanvasRenderingContext2D.cc"
    "color.cc"
    "helpers.cc"
)


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC} "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" "/Users/leozdgao/.nvm/versions/node/v14.16.1/include/node")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} skia harfbuzz icu sfntly ${APP_SERVICES_LIBRARY})
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/skia/out/Debug")

execute_process(COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
)

string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

include_directories(${CMAKE_JS_INC})
include_directories("${CMAKE_SOURCE_DIR}/skia")
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

add_definitions(-DNAPI_VERSION=6)
