cmake_minimum_required(VERSION 3.28)

project(ParticleLab LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(
  main
  src/Main.cpp
  inc/Application.h
  src/Application.cpp
  inc/engine/Aircraft.h
  inc/engine/Animation.h
  inc/engine/Button.h
  inc/engine/BloomEffect.h
  inc/engine/Category.h
  inc/engine/Command.h
  inc/engine/CommandQueue.h
  inc/engine/Component.h
  inc/engine/Container.h
  inc/engine/DataTables.h
  inc/engine/EmitterNode.h
  inc/engine/Entity.h
  inc/engine/Foreach.h
  inc/states/GameOverState.h
  inc/states/GameState.h
  inc/engine/KeyBinding.h
  inc/engine/Label.h
  inc/states/MenuState.h
  inc/engine/MusicPlayer.h
  inc/engine/Particle.h
  inc/engine/ParticleNode.h
  inc/states/PauseState.h
  inc/engine/Pickup.h
  inc/engine/Player.h
  inc/engine/PostEffect.h
  inc/engine/Projectile.h
  inc/engine/ResourceHolder.h
  inc/engine/ResourceHolder.inl
  inc/engine/ResourceIdentifiers.h
  inc/engine/SceneNode.h
  inc/states/SettingsState.h
  inc/engine/SpriteNode.h
  inc/engine/TextNode.h
  inc/engine/SoundNode.h
  inc/engine/SoundPlayer.h
  inc/states/State.h
  inc/states/StateIdentifiers.h
  inc/states/StateStack.h
  inc/states/TitleState.h
  inc/engine/Utility.h
  inc/engine/Utility.inl
  inc/engine/World.h
  src/engine/Aircraft.cpp
  src/engine/Animation.cpp
  src/engine/Button.cpp
  src/engine/BloomEffect.cpp
  src/engine/Command.cpp
  src/engine/CommandQueue.cpp
  src/engine/Component.cpp
  src/engine/Container.cpp
  src/engine/DataTables.cpp
  src/engine/EmitterNode.cpp
  src/engine/Entity.cpp
  src/states/GameOverState.cpp
  src/states/GameState.cpp
  src/engine/KeyBinding.cpp
  src/engine/Label.cpp
  src/states/MenuState.cpp
  src/engine/MusicPlayer.cpp
  src/states/PauseState.cpp
  src/engine/ParticleNode.cpp
  src/engine/Pickup.cpp
  src/engine/Player.cpp
  src/engine/PostEffect.cpp
  src/engine/Projectile.cpp
  src/engine/SceneNode.cpp
  src/states/SettingsState.cpp
  src/engine/SpriteNode.cpp
  src/engine/TextNode.cpp
  src/engine/SoundNode.cpp
  src/engine/SoundPlayer.cpp
  src/states/State.cpp
  src/states/StateStack.cpp
  src/states/TitleState.cpp
  src/engine/Utility.cpp
  src/engine/World.cpp
  inc/particles/Quark.h
  src/particles/Quark.cpp)

target_link_libraries(main PRIVATE sfml-system sfml-graphics sfml-window
                                   sfml-audio)

find_package(ReactPhysics3D REQUIRED)

if(WIN32)
  list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\ReactPhysics3D")
elseif(APPLE)
  list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/ReactPhysics3D")
endif()

target_link_libraries(main PRIVATE ReactPhysics3D::ReactPhysics3D)

target_compile_features(main PRIVATE cxx_std_17)

install(DIRECTORY media DESTINATION ParticleLab)

add_custom_command(
  TARGET main
  COMMENT "Formatting sources"
  POST_BUILD
  COMMAND cmake-format -c ${CMAKE_CURRENT_SOURCE_DIR}/cmake-format.json -i
          ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
  COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/inc -iname '*.h' -o -iname '*.cpp' |
          xargs clang-format -style=file -i
  COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/src -iname '*.h' -o -iname '*.cpp' |
          xargs clang-format -style=file -i)

if(WIN32)
  add_custom_command(
    TARGET main
    COMMENT "Copy OpenAL DLL"
    PRE_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy
      ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
      $<TARGET_FILE_DIR:main>
    VERBATIM)
endif()
