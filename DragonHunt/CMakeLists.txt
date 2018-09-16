# Define an executable
file(GLOB dragonhunt_SRC
    "src/*.h"
    "src/*.cpp"
)

add_executable(dragonhunt
    ${dragonhunt_SRC})

# Define the libraries this project depends upon
target_link_libraries(dragonhunt
    tinyxml2)

message(STATUS "include: " ${tp_include_dir})

target_include_directories(dragonhunt PUBLIC ${tp_include_dir})