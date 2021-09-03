
cmake_minimum_required(VERSION 3.15)

project(ClangASTViewer)


add_subdirectory(External)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/External/LibClang/CMake")

if(MSVC)
  add_compile_options($<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MT>
                      $<$<CONFIG:Release>:/MT>)
endif()

set(LibClangIncludeDir
    ${CMAKE_CURRENT_SOURCE_DIR}/External/LibClang/Include/
    CACHE STRING "" FORCE)

if(NOT
   (MSVC
    OR APPLE
    OR UNIX))
  message(
    FATAL_ERROR
      "This build currenly works only with macOS, Microsoft Visual Studio and Linux."
  )
endif()
if(APPLE OR UNIX)
  find_program(CMAKE_LIBTOOL libtool)
  if(NOT CMAKE_LIBTOOL)
    message(FATAL_ERROR "'libtool' is necessary for building static archives")
  endif()
  include(LinuxMacosBuild)
else()
  include(MSVCBuild)
endif()


file(GLOB HeadFile ${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h)
file(GLOB SourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp)

message( ${HeadFile})
message(  ${SourceFile})

add_executable(ClangASTViewer)
target_sources(ClangASTViewer PUBLIC ${HeadFile} ${SourceFile})
target_include_directories(ClangASTViewer PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
target_link_libraries(ClangASTViewer PUBLIC glfw glad imgui libclang)


if(MSVC)
  add_compile_options($<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MT>
                      $<$<CONFIG:Release>:/MT>)
endif()


add_custom_command(
  TARGET ClangASTViewer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${FontDir}
          $<TARGET_FILE_DIR:ClangASTViewer>/Font)


