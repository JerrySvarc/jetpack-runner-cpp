cmake_minimum_required(VERSION 3.21)
project(JetpackRunner LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(JetpackRunner
        Source/main.cpp
        Source/Game.cpp
        Source/Player.cpp
        Source/Entity.cpp
        Source/Zapper.cpp
        Source/Items.cpp
)
target_link_libraries(JetpackRunner PRIVATE sfml-graphics)
target_compile_features(JetpackRunner PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET JetpackRunner POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS JetpackRunner)
