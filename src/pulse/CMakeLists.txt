add_executable (
    pulse
        core.cpp
        hero_controller.cpp
        main.cpp
        renderer.cpp
        timer.cpp
)

target_link_libraries (pulse SDL2::SDL2)

target_include_directories (pulse PRIVATE ${SDL2_GFX_INCLUDE_DIR})
target_link_libraries (pulse ${SDL2_GFX_LIBRARY})
