# cmake_minimum_required(VERSION 3.10)

# project(ImGui)

# External Library
# add_subdirectory(External)

if(MSVC)
  add_compile_options($<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MT>
                      $<$<CONFIG:Release>:/MT>)
endif()
# ImGui
file(GLOB_RECURSE  ImGuiHeaderFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/ImGui/*.h)
file(GLOB_RECURSE  ImGuiSourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImGui/*.cpp)

set(FontDir ${CMAKE_CURRENT_SOURCE_DIR}/Font/ CACHE STRING "" FORCE)

# message(${ImGuiHeaderFile})
# message(${ImGuiSourceFile})

add_library(imgui ${ImGuiHeaderFile} ${ImGuiSourceFile})
target_include_directories(
  imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
               ${CMAKE_CURRENT_SOURCE_DIR}/Include/ImGui/)
target_link_libraries(imgui PUBLIC stb dirent freetype IconFontCppHeaders)


# add_subdirectory(Example)
