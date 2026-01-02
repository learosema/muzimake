#if defined(__DOS__) || defined(__WATCOMC__)
#include "app/dosapp.hpp"
#else
#include "app/sdlapp.hpp"
#endif
#include "log.h"

int main()
{
	APP_LOG("Main App started.");
#if defined(__DOS__) || defined(__WATCOMC__)
	DOSApp app;
	app.Setup();
	app.MainLoop();
	app_destroy();
#else
	SDLApp app;
	app.Setup();
	app.MainLoop();
#endif
	APP_LOG("Main App finished.");
	return 0;
}
