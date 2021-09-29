# ============= Resolve deps =============

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${FLAGS}")
  include_directories("/System/Library/Frameworks/")
  find_library(APP_SERVICES_LIBRARY ApplicationServices)
endif()

find_package(PkgConfig REQUIRED)

if (PKG_CONFIG_FOUND)
  pkg_check_modules(skia_deps REQUIRED IMPORTED_TARGET harfbuzz harfbuzz-icu icu-uc libpng libwebp libturbojpeg)
endif()

# ============= Source =============

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

# ============= Env check =============

if (DEFINED ENV{SKIA_DEBUG})
  string(APPEND SKIA_BUILD_OUTPUT_DIRECTORY "out/Debug")
  set (CMAKE_BUILD_TYPE "Debug")
else()
  string(APPEND SKIA_BUILD_OUTPUT_DIRECTORY "out/Release")
  set (CMAKE_BUILD_TYPE "Release")
endif()

# ============= Get node exectuable =============

execute_process(COMMAND which node
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_BIN_PATH
)

string(REPLACE "\n" "" NODE_BIN_PATH ${NODE_BIN_PATH})

# ============= Build config =============

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC} "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" ${NODE_INCLUDE_PATH})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

if (DEFINED ENV{SKIA_DEBUG})
  target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} skia skparagraph skshaper harfbuzz icu sfntly ${APP_SERVICES_LIBRARY})
else ()
  target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} skia skparagraph skshaper PkgConfig::skia_deps ${APP_SERVICES_LIBRARY})
endif()

target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/skia/${SKIA_BUILD_OUTPUT_DIRECTORY}")

execute_process(COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
)

string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

include_directories(${CMAKE_JS_INC})
include_directories("${CMAKE_SOURCE_DIR}/skia")
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# ============= Special for napi =============

add_definitions(-DNAPI_VERSION=6)
