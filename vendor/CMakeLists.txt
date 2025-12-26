include(FetchContent)

add_subdirectory(greatest)
add_subdirectory(cp437)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-3.2.x
)

FetchContent_MakeAvailable(SDL3)