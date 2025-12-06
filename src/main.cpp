#include "app/dosapp.hpp"
#include "log.h"

int main()
{
	APP_LOG("Main App started.");
	DOSApp app;
	app.Setup();
	app.MainLoop();
	APP_LOG("Main App finished.");
	return 0;
}
