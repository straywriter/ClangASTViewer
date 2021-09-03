

add_library(dirent INTERFACE)
target_include_directories(
  dirent
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)
