#pragma once
#include "iapp.hpp"

class SDLApp : public IApp {

public:
	SDLApp();
	virtual ~SDLApp();

	int Setup();
	int MainLoop();

};
