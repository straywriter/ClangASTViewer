
# glad

set(GLADVersion )

set(GLADDir ${CMAKE_CURRENT_SOURCE_DIR}/GL45GLES30)

set(GLADIncludeDir ${GLADDir}/include)
set(GLADSourceDir  ${GLADDir}/src)

# default opengl 4.5 opengles 3.0

add_library(glad)
target_include_directories(glad PUBLIC ${GLADIncludeDir})
target_sources(glad PUBLIC ${GLADSourceDir}/glad.c)
set_target_properties(glad PROPERTIES FOLDER "External")