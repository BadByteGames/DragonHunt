
# Define an executable
file(GLOB dragonhunt_SRC
    "src/*.h"
    "src/*.cpp"
)

add_executable(dragonhunt
    ${dragonhunt_SRC})

# Define the libraries this project depends upon
target_link_libraries(dragonhunt
    PUBLIC Boost::filesystem tinyxml2)

message(STATUS "include: " ${tinyxml2_INCLUDE_DIR})

target_include_directories(dragonhunt PUBLIC ${tinyxml2_INCLUDE_DIR})