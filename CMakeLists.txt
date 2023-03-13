cmake_minimum_required(VERSION 3.0)
project(Asteroides)
add_executable(Nave nave.c)
include_directories(allegro/include)
link_directories(allegro/lib)
target_link_libraries(Nave allegro allegro_font allegro_image allegro_primitives allegro_audio allegro_acodec allegro_ttf)
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}) # copy resources folder to output dir
include_directories(resources)