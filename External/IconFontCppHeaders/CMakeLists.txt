

add_library(IconFontCppHeaders INTERFACE)
target_include_directories(
  IconFontCppHeaders
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)
