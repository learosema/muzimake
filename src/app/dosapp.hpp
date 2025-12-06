#pragma once


#include "iapp.hpp"
#include "mainui.hpp"

class DOSApp : public IApp {

public:
	DOSApp();
	virtual ~DOSApp();

	int Setup();
	int MainLoop();

private:
	void RenderMainScreen();

	MainUI m_mainUI;
	bool m_hasMouse;

};
