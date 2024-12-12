#include "App.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {
	
}

bool App::OnInit() {
	// Instantiate the MainWindow pointer
	pGameWindow = new MainWindow();
	// Call the Show() method on the pointer
	pGameWindow->Show();

	return true;
}