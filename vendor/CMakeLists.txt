include(FetchContent)

add_subdirectory(greatest)
add_subdirectory(cp437)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
  GIT_TAG release-3.2.x
  EXCLUDE_FROM_ALL
)
set(SDL_JOYSTICK OFF)
set(SDL_HAPTIC OFF)
set(SDL_CAMERA OFF)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(SDL_HIDAPI OFF)
set(SDL_POWER OFF)
set(SDL_OPENGL OFF)
set(SDL_SENSOR OFF)
set(SDL_DIALOG OFF)
set(SDL_HIDAPI_JOYSTICK OFF)
set(SDL_HIDAPI_LIBUSB OFF)
set(SDL_HIDAPI_LIBUSB_SHARED OFF)
set(SDL_VIRTUAL_JOYSTICK OFF)
FetchContent_MakeAvailable(SDL3)