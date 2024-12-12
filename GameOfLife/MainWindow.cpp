#define TOOLBAR_PLAY_ICON 10001
#define TOOLBAR_PAUSE_ICON 10002
#define TOOLBAR_NEXT_ICON 10003
#define TOOLBAR_TRASH_ICON 10004
#define GAME_TIMER_ID 10005
#define MENU_OPTIONS_SETTINGS_ID 10006
#define MENU_VIEW_DISPLAY_NEIGHBORS_ID 10007
#define MENU_OPTIONS_RANDOMIZE_ID 10008
#define MENU_OPTIONS_RANDOMIZE_WITH_SEED_ID 10009
#define MENU_FILE_NEW_ID 10010
#define MENU_FILE_OPEN_ID 10011
#define MENU_FILE_SAVE_ID 10012
#define MENU_FILE_SAVE_AS_ID 10013
#define MENU_FILE_EXIT_ID 10014
#define MENU_VIEW_FINITE_ID 10015
#define MENU_VIEW_TOROIDAL_ID 10016
#define MENU_OPTIONS_RESET_SETTINGS_ID 10017
#define MENU_FILE_IMPORT_ID 10018
#define MENU_VIEW_DISPLAY_GRID_ID 10019
#define MENU_VIEW_DISPLAY_THICK_GRID_ID 10020
#define MENU_VIEW_DISPLAY_HUD_ID 10021

#include "MainWindow.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"
#include "play.xpm"
#include "next.xpm"
#include "pause.xpm"
#include "trash.xpm"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChanged)
	EVT_MENU(TOOLBAR_PLAY_ICON, MainWindow::OnPlayButtonClick)
	EVT_MENU(TOOLBAR_PAUSE_ICON, MainWindow::OnPauseButtonClick)
	EVT_MENU(TOOLBAR_NEXT_ICON, MainWindow::OnNextButtonClick)
	EVT_MENU(TOOLBAR_TRASH_ICON, MainWindow::OnClearButtonClick)
	EVT_MENU(MENU_OPTIONS_SETTINGS_ID, MainWindow::OnSettingsOptionClick)
	EVT_MENU(MENU_OPTIONS_RESET_SETTINGS_ID, MainWindow::OnResetSettingsOptionClick)
	EVT_MENU(MENU_VIEW_DISPLAY_NEIGHBORS_ID, MainWindow::OnViewDisplayNeighborsCheck)
	EVT_MENU(MENU_VIEW_FINITE_ID, MainWindow::OnViewFiniteCheck)
	EVT_MENU(MENU_VIEW_TOROIDAL_ID, MainWindow::OnViewToroidalCheck)
	EVT_MENU(MENU_VIEW_DISPLAY_GRID_ID, MainWindow::OnViewDisplayGrid)
	EVT_MENU(MENU_VIEW_DISPLAY_THICK_GRID_ID, MainWindow::OnViewDisplayThickGrid)
	EVT_MENU(MENU_VIEW_DISPLAY_HUD_ID, MainWindow::OnViewDisplayHUD)
	EVT_MENU(MENU_OPTIONS_RANDOMIZE_ID, MainWindow::OnOptionsRandomizeClick)
	EVT_MENU(MENU_OPTIONS_RANDOMIZE_WITH_SEED_ID, MainWindow::OnOptionsRandomizeWithSeedClick)
	EVT_MENU(MENU_FILE_NEW_ID, MainWindow::OnFileNewClick)
	EVT_MENU(MENU_FILE_OPEN_ID, MainWindow::OnFileOpenClick)
	EVT_MENU(MENU_FILE_IMPORT_ID, MainWindow::OnFileImportClick)
	EVT_MENU(MENU_FILE_SAVE_ID, MainWindow::OnFileSaveClick)
	EVT_MENU(MENU_FILE_SAVE_AS_ID, MainWindow::OnFileSaveAsClick)
	EVT_MENU(MENU_FILE_EXIT_ID, MainWindow::OnFileExitClick)
	EVT_TIMER(GAME_TIMER_ID, MainWindow::TriggerTimer)
wxEND_EVENT_TABLE();

MainWindow::MainWindow() 
	: wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(250,250), wxSize(500,500)), pStatusBar(CreateStatusBar(2)), 
	pToolBar(CreateToolBar()), pTimer(new wxTimer(this, GAME_TIMER_ID))
{
	// Instantiate the DrawingPanel pointer and use MainWindow as the parent
	pGraphicsPanel = new DrawingPanel(this, wxSize(750, 500), vGameBoard, vNeighborCount, mLivingCells, mGenerations, this);

	// Load game settings if any on start of game
	mSettings.LoadGameSettings();

	// Passing the Settings object to the DrawingPanel
	pGraphicsPanel->SetSettings(&mSettings);

	// Menu Bar pointer and instantiation
	wxMenuBar* pMenuBar = new wxMenuBar();
	// Set pMenuBar and the menu bar
	SetMenuBar(pMenuBar);

	// File Menu create
	wxMenu* pFileMenu = new wxMenu();
	// File Menu item: New
	pFileMenu->Append(MENU_FILE_NEW_ID, "New");
	// File Menu item: Open
	pFileMenu->Append(MENU_FILE_OPEN_ID, "Open");
	// File Menu item: Import
	pFileMenu->Append(MENU_FILE_IMPORT_ID, "Import");
	// File Menu item: Save
	pFileMenu->Append(MENU_FILE_SAVE_ID, "Save");
	// File Menu item: Save As
	pFileMenu->Append(MENU_FILE_SAVE_AS_ID, "Save As");
	// File Menu item: Exit
	pFileMenu->Append(MENU_FILE_EXIT_ID, "Exit");

	// Add the file menu to the main menu bar
	pMenuBar->Append(pFileMenu, "File");

	// View Menu create
	wxMenu* pViewMenu = new wxMenu();
	// Create Show Neighbor Count menu item
	pDisplayNeighbors = new wxMenuItem(pViewMenu, MENU_VIEW_DISPLAY_NEIGHBORS_ID, "Show Neighbor Count", wxEmptyString, wxITEM_CHECK);
	// Allow the menu item to be checkable
	pDisplayNeighbors->SetCheckable(true);
	// Add the show display neighbors check to the view menu
	pViewMenu->Append(pDisplayNeighbors);
	// Set the saved or default state of Show Neighbor Count to the checked value
	pDisplayNeighbors->Check(mSettings.GetShowNeighborCount());

	// Finite menu item
	pFiniteView = new wxMenuItem(pViewMenu, MENU_VIEW_FINITE_ID, "Finite", wxEmptyString, wxITEM_CHECK);
	pFiniteView->SetCheckable(true);
	// Toroidal menu item
	pToroidalView = new wxMenuItem(pViewMenu, MENU_VIEW_TOROIDAL_ID, "Toroidal", wxEmptyString, wxITEM_CHECK);
	pToroidalView->SetCheckable(true);

	// Add the Finite and Toroidal options to the view menu
	pViewMenu->Append(pFiniteView);
	pViewMenu->Append(pToroidalView);

	// Display Grid view menu option
	pDisplayGrid = new wxMenuItem(pViewMenu, MENU_VIEW_DISPLAY_GRID_ID, "Show Grid", wxEmptyString, wxITEM_CHECK);
	pDisplayGrid->SetCheckable(true);
	// Add the display grid option to the view menu
	pViewMenu->Append(pDisplayGrid);
	// Set the check status to the saved state from settings of display grid
	pDisplayGrid->Check(mSettings.GetDisplayGrid());
	
	// Display Grid view menu option
	pDisplayThickGrid = new wxMenuItem(pViewMenu, MENU_VIEW_DISPLAY_THICK_GRID_ID, "Show 10x10 Grid", wxEmptyString, wxITEM_CHECK);
	pDisplayThickGrid->SetCheckable(true);
	// Add the display 10x10 grid option to the view menu
	pViewMenu->Append(pDisplayThickGrid);
	// Set the check status to the saved state from settings of display grid
	pDisplayThickGrid->Check(mSettings.GetDisplayThickGrid());

	// Show HUD view menu option
	pDisplayHUD = new wxMenuItem(pViewMenu, MENU_VIEW_DISPLAY_HUD_ID, "Show HUD", wxEmptyString, wxITEM_CHECK);
	// Add display hud option to view menu
	pViewMenu->Append(pDisplayHUD);
	// Set the check status to the saved state from settings of display hud
	pDisplayHUD->Check(mSettings.GetDisplayHUD());

	// Set the initial state of the menu items based on the saved Universe view
	pFiniteView->Check(mSettings.GetUniverseView());
	pToroidalView->Check(!mSettings.GetUniverseView());

	// Add the view menu to the main menu bar
	pMenuBar->Append(pViewMenu, "View");

	// Randomize menu create
	wxMenu* pRandomizeMenu = new wxMenu();
	pRandomizeMenu->Append(MENU_OPTIONS_RANDOMIZE_ID, "Randomize (Time)");
	pRandomizeMenu->Append(MENU_OPTIONS_RANDOMIZE_WITH_SEED_ID, "Randomize (Seed)");

	// Add the Randomize menu to the menu bar
	pMenuBar->Append(pRandomizeMenu, "Randomize");

	
	// Options menu create
	wxMenu* pOptionsMenu = new wxMenu();
	// Add a Settings option to the options menu
	pOptionsMenu->Append(MENU_OPTIONS_SETTINGS_ID, "Settings");
	pOptionsMenu->Append(MENU_OPTIONS_RESET_SETTINGS_ID, "Reset to Default Settings");

	// Add the Option menu to the menu bar
	pMenuBar->Append(pOptionsMenu, "Options");

	// Bitmap class of each icon for toolbar
	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	/// Add icons to toolbar
	pToolBar->AddTool(TOOLBAR_PLAY_ICON, "Play", playIcon);
	pToolBar->AddTool(TOOLBAR_PAUSE_ICON, "Pause", pauseIcon);
	pToolBar->AddTool(TOOLBAR_NEXT_ICON, "Next", nextIcon);
	pToolBar->AddTool(TOOLBAR_TRASH_ICON, "Clear", trashIcon);
	// Set icons to 16x16 px
	pToolBar->SetToolBitmapSize(wxSize(16, 16));
	// Resize and reposition window
	pToolBar->Realize();

	// Initalizes gameboard with status bar
	GridInit();

	// Display/Update status text for status bar
	UpdateStatusText();
	
	// Resolves non visible status bar on load
	this->Layout();
}

MainWindow::~MainWindow()
{
	// Free allocated memory
	delete pTimer;

	// Save the game settings on exit of game
	mSettings.SaveGameSettings();
}

void MainWindow::OnSizeChanged(wxSizeEvent& event)
{
	if (pGraphicsPanel != nullptr) {
		// Calls the SetPanelSize and updates the size to the window size by event listening
		pGraphicsPanel->SetPanelSize(event.GetSize());

		// Prevents additional events from triggering - prevents errors
		event.Skip();
	}
}

void MainWindow::UpdateStatusText()
{
	// String to display current generation value
	std::string generationStatus = "Generations: " + std::to_string(mGenerations);
	// String to display current living cells value
	std::string livingCellsStatus = "Living Cells: " + std::to_string(mLivingCells);

	// Update the status text
	this->SetStatusText(wxString(generationStatus), 0);
	this->SetStatusText(wxString(livingCellsStatus), 1);
}

void MainWindow::OnPlayButtonClick(wxCommandEvent& event)
{
	// If time is not running, start the timer/continue
	if (!pTimer->IsRunning()) {
		pTimer->Start(mSettings.GetTimeInterval());
	}
}

void MainWindow::OnNextButtonClick(wxCommandEvent& event)
{
	// If timer is not running, allow next cell generation
	if (!pTimer->IsRunning()) {
		// Call next cell generation
		CreateNextCellGeneration();
	}
}

void MainWindow::OnPauseButtonClick(wxCommandEvent& event)
{
	// If timer is running, stop the timer
	if (pTimer->IsRunning()) {
		pTimer->Stop();
	}
}

void MainWindow::OnClearButtonClick(wxCommandEvent& event)
{
	// If timer is running, don't allow clearing until stopped
	if (!pTimer->IsRunning()) {
		// Clear and reset gameboard
		ResetGameboard();
	}
}

void MainWindow::OnSettingsOptionClick(wxCommandEvent& event)
{
	// Create SettingsDialog object and pass the settings object
	SettingsDialog settingsModal(this, &mSettings);
	
	// Display the modal and store the result
	if (settingsModal.ShowModal() == wxID_OK) {
		// Reinitialize the grid
		GridInit();
		// Refresh the panel
		this->Refresh();
		// Save the game settings if changes made and hit ok
		mSettings.SaveGameSettings();
	}

}

void MainWindow::OnResetSettingsOptionClick(wxCommandEvent& event)
{
	// Call Reset Settings method from the settings struct
	mSettings.ResetSettings();

	// Ensure checkmarks are updated on reset for View options
	pDisplayNeighbors->Check(mSettings.GetShowNeighborCount());
	pFiniteView->Check(true);
	pToroidalView->Check(false);
	pDisplayGrid->Check(true);
	pDisplayThickGrid->Check(true);
	pDisplayHUD->Check(true);

	// Reinit the grid
	GridInit();

	// Save the update settings
	mSettings.SaveGameSettings();

	// Refresh panel
	this->Refresh();
}

void MainWindow::OnViewDisplayNeighborsCheck(wxCommandEvent& event)
{
	// Update View Menu check toggle based on Show Neighbor Count setting state
	mSettings.SetShowNeighborCount(!mSettings.GetShowNeighborCount());

	// Save the settings
	mSettings.SaveGameSettings();

	this->Refresh();
}

void MainWindow::OnViewFiniteCheck(wxCommandEvent& event)
{
	// If Finite is already active, switch to Toroidal
	if (mSettings.GetUniverseView())
	{
		// Set view to Toroidal
		mSettings.SetUniverseView(false);
		// Uncheck Finite
		pFiniteView->Check(false);
		// Check Toroidal
		pToroidalView->Check(true);
	}
	else {
		// Set view to Finite
		mSettings.SetUniverseView(true);
		// Check Finite
		pFiniteView->Check(true);
		// Uncheck Toroidal
		pToroidalView->Check(false);
	}

	// Recalculate neighbor counts for the new mode
	for (size_t i = 0; i < vGameBoard.size(); ++i) {
		for (size_t j = 0; j < vGameBoard[i].size(); ++j) {
			vNeighborCount[i][j] = GetLivingNeighborCount(i, j);
		}
	}

	// Save settings
	mSettings.SaveGameSettings();

	// Refresh panel
	this->Refresh();
}

void MainWindow::OnViewToroidalCheck(wxCommandEvent& event)
{
	// If Toroidal is already active, switch to Finite
	if (!mSettings.GetUniverseView())
	{
		// Set view to Finite
		mSettings.SetUniverseView(true);
		// Check Finite
		pFiniteView->Check(true);
		// Uncheck Toroidal
		pToroidalView->Check(false);
	}
	else {
		// Set view to Toroidal
		mSettings.SetUniverseView(false);
		// Uncheck Finite
		pFiniteView->Check(false);
		// Check Toroidal
		pToroidalView->Check(true);
	}

	// Recalculate neighbor counts for the new mode
	for (size_t i = 0; i < vGameBoard.size(); ++i) {
		for (size_t j = 0; j < vGameBoard[i].size(); ++j) {
			vNeighborCount[i][j] = GetLivingNeighborCount(i, j);
		}
	}

	// Save settings
	mSettings.SaveGameSettings();

	// Refresh panel
	this->Refresh();
}

void MainWindow::OnViewDisplayGrid(wxCommandEvent& event)
{
	// Update the display grid setting to the opposite, true/false
	mSettings.SetDisplayGrid(!mSettings.GetDisplayGrid());

	// Save the game settings
	mSettings.SaveGameSettings();

	// Refresh panel
	this->Refresh();
}

void MainWindow::OnViewDisplayThickGrid(wxCommandEvent& event)
{
	// Update the display thick grid setting to the opposite, true/false
	mSettings.SetDisplayThickGrid(!mSettings.GetDisplayThickGrid());

	// Save the game settings
	mSettings.SaveGameSettings();

	// Refresh panel
	this->Refresh();
}

void MainWindow::OnViewDisplayHUD(wxCommandEvent& event)
{
	// Update the display hud setting to the opposite, true/false
	mSettings.SetDisplayHUD(!mSettings.GetDisplayHUD());

	// Save the game settings
	mSettings.SaveGameSettings();

	// Refresh panel
	this->Refresh();
}

void MainWindow::OnOptionsRandomizeClick(wxCommandEvent& event)
{
	// Call RandomizeGrid method with time of null for srand
	RandomizeGrid(time(NULL));
}


void MainWindow::OnOptionsRandomizeWithSeedClick(wxCommandEvent& event)
{
	// Get seed input from user and save
	int seed = wxGetNumberFromUser("Seed", "Enter Seed", wxEmptyString, LONG_MAX/2, LONG_MIN, LONG_MAX);

	// If cancelled, exit, no randomize
	if (seed == -1) {
		return;
	}

	// Call RandomizeGrid method with the inputted time for srand
	RandomizeGrid(seed);

	// Refreshes and causes the OnPaint event to trigger
	this->Refresh();
}

void MainWindow::OnFileNewClick(wxCommandEvent& event)
{
	// Clear memory saved filepath
	mCurrentFilePath.clear();

	// Resets gameboard, generations, living cells, updates status text, and refreshes panel
	ResetGameboard();

	event.Skip();
}

void MainWindow::OnFileImportClick(wxCommandEvent& event)
{
	// Reset living cell count
	mLivingCells = 0;

	// File Dialog to Import the file
	wxFileDialog fileDialog(this, "Import Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	// If user decides not to save and cancels
	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	// String to hold read lines from file
	std::string buffer;

	// Stream to open the file
	std::ifstream fileStream;

	// Temporary vectors to store the imported game
	std::vector<std::vector<bool>> importedGame;
	int gameRows = 0;
	int gameColumns = 0;

	// Open the file that was chosen in the fileDialog
	fileStream.open((std::string)fileDialog.GetPath());

	// If the file is open
	if (fileStream.is_open()) {

		mCurrentFilePath = (std::string)fileDialog.GetPath();

		// Loop while not at the end of file
		while (!fileStream.eof()) {

			// Add to the buffer string
			std::getline(fileStream, buffer);

			// If buffer is empty, break loop
			if (buffer.size() == 0) break;

			// Resize the imported game rows
			importedGame.resize(gameRows + 1);
			gameColumns = buffer.size();

			// Resize the current row in the imported game
			importedGame[gameRows].resize(buffer.size());

			// Loop through buffer to update live/dead cells
			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*') { // If buffer is a live cell
					importedGame[gameRows][i] = true;
				}
				else { // If buffer is a dead cell
					importedGame[gameRows][i] = false;
				}
			}
			gameRows++;
		}

		// Close the file
		fileStream.close();
	}

	// Calculate the top-left starting position for centering
	int startRow = (vGameBoard.size() - gameRows) / 2;
	int startColumn = (vGameBoard.size() - gameColumns) / 2;

	// Clear the game board
	ResetGameboard();

	// Place the imported game onto the gameboard
	for (int i = 0; i < gameRows; i++)
	{
		for (int j = 0; j < gameColumns; j++)
		{
			if ((startRow + i) < vGameBoard.size() && (startColumn + j) < vGameBoard.size())
			{
				vGameBoard[startRow + i][startColumn + j] = importedGame[i][j];
			}
		}
	}

	// Recalculate neighbor counts for all cells
	for (size_t i = 0; i < vGameBoard.size(); i++) {
		for (size_t j = 0; j < vGameBoard[i].size(); j++) {
			vNeighborCount[i][j] = GetLivingNeighborCount(i, j);

			// If cell is alive
			if (vGameBoard[i][j]) {
				mLivingCells++; // Increment living cells count
			}
		}
	}

	// Update the status text to reflect the new count of living cells
	UpdateStatusText();

	// Refresh paint on panel
	this->Refresh();

	event.Skip();
}

void MainWindow::OnFileOpenClick(wxCommandEvent& event)
{
	// Reset living cell count
	mLivingCells = 0;

	// File Dialog to Open the file
	wxFileDialog fileDialog(this, "Open Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	// If user decides not to save and cancels
	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;
	
	// Clear and resize cells of game board and neighborcount
	for (int i = 0; i < vGameBoard.size(); i++)
	{
		vGameBoard[i].clear();
		vGameBoard[i].resize(0); 
		vNeighborCount[i].clear();
		vNeighborCount[i].resize(0);
	}

	// Clear and resize game board and neighborcount
	vGameBoard.clear();
	vGameBoard.resize(0);
	vNeighborCount.clear();
	vNeighborCount.resize(0);

	// String to hold read lines from file
	std::string buffer;

	// Stream to open the file
	std::ifstream fileStream;

	int ndx = 0;

	// Open the file that was chosen in the fileDialog
	fileStream.open((std::string)fileDialog.GetPath());

	// If the file is open
	if (fileStream.is_open()) {

		mCurrentFilePath = (std::string)fileDialog.GetPath();

		// Loop while not at the end of file
		while (!fileStream.eof()) {

			// Add to the buffer string
			std::getline(fileStream, buffer);

			// If buffer is empty, break loop
			if (buffer.size() == 0) break;

			// Resize the gameboard and neighborcount to buffer size
			if (vGameBoard.size() == 0) {
				vGameBoard.resize(buffer.size());
				vNeighborCount.resize(buffer.size());
			}

			// Resize the current row in the gameboard and neighborcount
			vGameBoard[ndx].resize(buffer.size());
			vNeighborCount[ndx].resize(buffer.size());

			// Loop through buffer to update live/dead cells
			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*') { // If buffer is a live cell
					vGameBoard[ndx][i] = true;
				}
				else { // If buffer is a dead cell
					vGameBoard[ndx][i] = false;
				}
			}
			// Increment index
			ndx++;
		}

		// Close the file
		fileStream.close();
	}

	// Update grid size to match open file buffer size
	pGraphicsPanel->SetGridSize(vGameBoard.size());

	// Recalculate neighbor counts for all cells
	for (size_t i = 0; i < vGameBoard.size(); i++) {
		for (size_t j = 0; j < vGameBoard[i].size(); j++) {
			vNeighborCount[i][j] = GetLivingNeighborCount(i, j);

			// If cell is alive
			if (vGameBoard[i][j]) {
				mLivingCells++; // Increment living cells count
			}
		}
	}

	// Update the status text to reflect the new count of living cells
	UpdateStatusText();

	// Refresh paint on panel
	this->Refresh();

	event.Skip();
}

void MainWindow::OnFileSaveClick(wxCommandEvent& event)
{
	if (!mCurrentFilePath.empty()) {

		// Stream to open the file
		std::ofstream fileStream;

		// Open the file that was chosen in the fileDialog
		fileStream.open(mCurrentFilePath);

		// If the file is open
		if (fileStream.is_open()) {

			// Loop through the gameboard
			for (size_t i = 0; i < vGameBoard.size(); i++)
			{
				for (size_t j = 0; j < vGameBoard[i].size(); j++)
				{
					// If cell is alive, add * to file
					if (vGameBoard[i][j]) {
						fileStream << '*';
					}
					else { // If cell is dead add a . to file
						fileStream << '.';
					}
				}
				// Next row new line
				fileStream << '\n';
			}

			// Close the file
			fileStream.close();
		}
	}
	else {
		OnFileSaveAsClick(event);
	}

	event.Skip();
}

void MainWindow::OnFileSaveAsClick(wxCommandEvent& event)
{
	// File Dialog to Save or Overwrite file
	wxFileDialog fileDialog(this, "Save Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	// If user decides not to save and cancels
	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	// Stream to open the file
	std::ofstream fileStream;

	// Open the file that was chosen in the fileDialog
	fileStream.open((std::string)fileDialog.GetPath());

	// If the file is open
	if (fileStream.is_open()) {

		mCurrentFilePath = (std::string)fileDialog.GetPath();

		// Loop through the gameboard
		for (size_t i = 0; i < vGameBoard.size(); i++)
		{
			for (size_t j = 0; j < vGameBoard[i].size(); j++)
			{
				// If cell is alive, add * to file
				if (vGameBoard[i][j]) {
					fileStream << '*';
				}
				else { // If cell is dead add a . to file
					fileStream << '.';
				}
			}
			// Next row new line
			fileStream << '\n';
		}

		// Close the file
		fileStream.close();
	}


	event.Skip();
}

void MainWindow::OnFileExitClick(wxCommandEvent& event)
{
	// Close the game
	this->Close();
}

void MainWindow::RandomizeGrid(int seed)
{
	// Reset living cell count
	mLivingCells = 0;

	// Set srand seed time
	srand(seed);

	// Loop rows of the game board
	for (int i = 0; i < vGameBoard.size(); i++)
	{
		for (int j = 0; j < vGameBoard[i].size(); j++) // loop through columns of the gameboard
		{
			// Get random number on iteration
			int num = rand();

			// 1/3 Randomize board calculation
			if (num % 3 == 0) {
				vGameBoard[i][j] = true;
			}
			else {
				vGameBoard[i][j] = false;
			}
		}

	}

	// Recalculate neighbor counts for all cells
	for (size_t i = 0; i < vGameBoard.size(); i++) {
		for (size_t j = 0; j < vGameBoard[i].size(); j++) {
			vNeighborCount[i][j] = GetLivingNeighborCount(i, j);

			// If cell is alive
			if (vGameBoard[i][j]) {
				mLivingCells++; // Increment living cells count
			}
		}
	}

	// Update the status text to reflect the new count of living cells
	UpdateStatusText();

	// Refreshes and causes the OnPaint event to trigger
	this->Refresh();
}

int MainWindow::GetLivingNeighborCount(int row, int column)
{
	// Count variable to hold amt of living cells
	int count = 0;

	// Loop for rows -> x
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++) // Loop for columns -> y
		{
			// Skip the entered cell
			if (i == 0 && j == 0) continue;

			// Variable to hold neighbor row
			int neighborRow = row + i;
			// Variable to hold neighbor column
			int neighborColumn = column + j;

			// If Finite View
			if (mSettings.GetUniverseView()) {
				// Ensures neighborRow and neighborColumn is not out of bounds
				if (neighborRow < 0 || neighborRow >= vGameBoard.size() || neighborColumn < 0 || neighborColumn >= vGameBoard.size()) {
					continue;
				}
			}
			else { // Toroidal View - Wrap rows and columns around the grid
				// Wrap top to bottom
				if (neighborRow < 0) neighborRow = vGameBoard.size() - 1;
				// Wrap bottom to top
				if (neighborRow >= vGameBoard.size()) neighborRow = 0;
				// Wrap left to right
				if (neighborColumn < 0) neighborColumn = vGameBoard.size() - 1;
				// Wrap right to left
				if (neighborColumn >= vGameBoard.size()) neighborColumn = 0;
			}

			// If the neighbor cell is living, increment count
			if (vGameBoard[neighborRow][neighborColumn]) {
				count++;
			}
		}
	}

	// Return total count
	return count;
}

void MainWindow::CreateNextCellGeneration()
{
	// Reset living cell count for the new generation
	mLivingCells = 0;

	// Vector for temporary gameboard
	std::vector<std::vector<bool>> sandbox;
	// Set size of vector to gridsize
	sandbox.resize(mSettings.GetGridSize());

	// Loop through sub-vector in GameBoard and set size to grid size
	for (size_t i = 0; i < vGameBoard.size(); i++)
	{
		// Set sub-vector size and init the bool to false
		sandbox[i].resize(mSettings.GetGridSize(), false);
	}

	// Loop through each cell in the active gameboard
	// Rows -> X
	for (size_t i = 0; i < vGameBoard.size(); i++)
	{
		for (size_t j = 0; j < vGameBoard[i].size(); j++) // Columns -> Y
		{
			// Get number of living neighbors for the current cell
			int livingNeighborsCount = GetLivingNeighborCount(i, j);

			// If cell is alive
			if (vGameBoard[i][j]) {
				// Living cells with 2 or 3 living neighbors live in the next generation.
				if (livingNeighborsCount == 2 || livingNeighborsCount == 3) {
					sandbox[i][j] = true; // Keep cell alive in next generation
					mLivingCells++; // Increment living cells count
				} 
			}
			else {
				// Dead cells with exactly 3 living neighbors live in the next generation.
				if (livingNeighborsCount == 3) {
					sandbox[i][j] = true; // Make cell alive in next generation
					mLivingCells++; // Increment living cells count
				}
			}
		}
	}

	// Update current gameboard with the copied sandbox gameboard
	std::swap(vGameBoard, sandbox);

	// Loop through each cell in the active gameboard (Re-loop for Neighbor Count)
	// Rows -> X
	for (size_t i = 0; i < vGameBoard.size(); i++)
	{
		for (size_t j = 0; j < vGameBoard[i].size(); j++) // Columns -> Y
		{
			// Get number of living neighbors for the current cell
			int livingNeighborsCount = GetLivingNeighborCount(i, j);
			// Set neighbor count values to living neighbors count
			vNeighborCount[i][j] = livingNeighborsCount;
		}
	}

	// Increment the generation
	mGenerations++;

	// Update the status text to reflect the new count of living cells
	UpdateStatusText();

	// Refreshes and causes the OnPaint event to trigger
	this->Refresh();
}

void MainWindow::ResetGameboard()
{
	// Resize the game board to match the grid size
	vGameBoard.resize(mSettings.GetGridSize());
	vNeighborCount.resize(mSettings.GetGridSize());

	for (size_t i = 0; i < vGameBoard.size(); i++)
	{
		vGameBoard[i].resize(mSettings.GetGridSize());
		vNeighborCount[i].resize(mSettings.GetGridSize());
	}

	// Loop through the gameboard vector
	for (size_t i = 0; i < vGameBoard.size(); i++)
	{
		for (size_t j = 0; j < vGameBoard[i].size(); j++)
		{
			// Set each cell to false in Game Board
			vGameBoard[i][j] = false;
			// Set neighbor count values to 0 to no longer display
			vNeighborCount[i][j] = 0;
		}
	}

	// Set living cells to 0
	mLivingCells = 0;
	// Set generations to 0
	mGenerations = 0;

	// Reset status text to default
	UpdateStatusText();

	// Refreshes and causes the OnPaint event to trigger
	this->Refresh();
}

void MainWindow::TriggerTimer(wxTimerEvent& event)
{
	// Advances the game by one generation on each timer tick (Time Interval -> mSettings.GetTimeInterval())
	CreateNextCellGeneration();
}

void MainWindow::GridInit()
{
	// Set main vector size to the grid size
	vGameBoard.resize(mSettings.GetGridSize());
	vNeighborCount.resize(mSettings.GetGridSize());

	// Loop through sub-vector in GameBoard and set size to grid size
	for (size_t i = 0; i < vGameBoard.size(); i++)
	{
		// Set sub-vector size and init the bool to false
		vGameBoard[i].resize(mSettings.GetGridSize(), false);
		vNeighborCount[i].resize(mSettings.GetGridSize(), 0);
	}

	// Pass the grid size to the Drawing Panel
	pGraphicsPanel->SetGridSize(mSettings.GetGridSize());
}
