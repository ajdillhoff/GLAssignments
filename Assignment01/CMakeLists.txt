cmake_minimum_required( VERSION 2.6 )
project( Assignment0 )

set( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )

link_directories(lib/)

find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )
include_directories( ${GLFW_INCLUDE_DIRS} )

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIR} )
find_package( GLEW REQUIRED )
include_directories( ${GLEW_INCLUDE_DIR} )

if( APPLE )
    include_directories( /System/Library/Frameworks )
    find_library( COCOA_LIBRARY Cocoa )
    find_library( CORE_VIDEO_LIBRARY CoreVideo )
    find_library( OpenGL_LIBRARY OpenGL )
    find_library( IOKIT_LIBRARY IOKit )
    mark_as_advanced( COCOA_LIBRARY
                      CORE_VIDEO_LIBRARY
                      OpenGL_LIBRARY
                      IOKIT_LIBRARY )
    set( EXTRA_LIBS ${COCOA_LIBRARY} ${CORE_VIDEO_LIBRARY} ${OpenGL_LIBRARY}
                      ${IOKIT_LIBRARY} )
endif( APPLE )

# add executable
add_executable( Assignment0 buildTest.cpp shader.cpp )

target_link_libraries( Assignment0 ${EXTRA_LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES} SOIL )
