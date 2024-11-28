cmake_minimum_required(VERSION 3.28)

project(ParticleLab LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
    FetchContent_MakeAvailable(SFML)

add_executable(
    main
    src/main.cpp
    src/particles/Quark.h
    src/particles/Quark.cpp
    src/Command.h
    src/Command.cpp
    src/CommandQueue.h
    src/CommandQueue.cpp
    src/SpriteContainer.h
    src/SpriteContainer.cpp
    src/utils.h
    src/utils.cpp
    src/utils.inl
)

target_link_libraries(main PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)

find_package(ReactPhysics3D REQUIRED)

if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\ReactPhysics3D")
elseif(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/ReactPhysics3D")
endif()

target_link_libraries(main PRIVATE ReactPhysics3D::ReactPhysics3D)

target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()
