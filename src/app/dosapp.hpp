#pragma once


#include "iapp.hpp"
#include "mainui.hpp"



	DOSApp();
	virtual ~DOSApp();

	int Setup();
	int MainLoop();

private:
	void RenderMainScreen();

	MainUI m_mainUI;
	bool m_hasMouse;

};


void app_init();
