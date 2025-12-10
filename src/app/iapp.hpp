#pragma once

class IApp {
	public:
		virtual ~IApp() {}

		virtual int Setup() = 0;
		virtual int MainLoop() = 0;

};
