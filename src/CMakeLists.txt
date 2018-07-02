set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

find_package (SDL2 REQUIRED)

find_file (SDL2_GFX_INCLUDE_DIRECTORY "SDL2/SDL2_gfxPrimitives.h")
find_library (SDL2_GFX_LIBRARY SDL2_gfx)

add_subdirectory (pulse)
