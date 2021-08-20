
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${FLAGS}")
    include_directories("/System/Library/Frameworks/")
    find_library(APP_SERVICES_LIBRARY ApplicationServices)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

file(GLOB SOURCE_FILES
    "init.cc"
    "Canvas.cc"
    "CanvasGradient.cc"
    "CanvasPattern.cc"
    "CanvasRenderingContext2D.cc"
    "FontManager.cc"
    "helpers.cc"
    "Image.cc"
    "ImageData.cc"
    "StyleParser.cc"
    "TextMetrics.cc"
    "W3CSkColorParser.cc"
)

execute_process(COMMAND which node
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_BIN_PATH
)

string(REPLACE "\n" "" NODE_BIN_PATH ${NODE_BIN_PATH})

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC} "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" ${NODE_INCLUDE_PATH})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} skia skparagraph skshaper harfbuzz icu sfntly ${APP_SERVICES_LIBRARY})
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
