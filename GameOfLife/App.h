#pragma once

#include "wx/wx.h"
#include "MainWindow.h"

class App : public wxApp
{
private:
	// MainWindow pointer for Game window
	MainWindow* pGameWindow = nullptr;
public:
	App();
	~App();
	virtual bool OnInit();
};

