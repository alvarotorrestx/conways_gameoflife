#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include "SettingsDialog.h"

class MainWindow : public wxFrame
{
private:
	// Member fields
	// Variable to hold generations count
	int mGenerations = 0;
	// Variable to hold living cells count
	int mLivingCells = 0;
	wxStatusBar* pStatusBar;
	// DrawingPanel pointer for the GraphicsPanel
	DrawingPanel* pGraphicsPanel = nullptr;
	// GameBoard vector for cells, sub-vectors dead/alive
	std::vector<std::vector<bool>> vGameBoard;
	// wxToolBar pointer for ToolBar
	wxToolBar* pToolBar;
	// wxTimer pointer for the Timer
	wxTimer* pTimer;
	// Settings object access
	Settings mSettings;
	// Menu Item pointer for choice of Show Display Neighbors
	wxMenuItem* pDisplayNeighbors = nullptr;
	// Vector to hold each neighbor count of cell generations
	std::vector<std::vector<int>> vNeighborCount;
	// String to hold current file path of saved/open file
	std::string mCurrentFilePath = "";
	// Menu Item pointer for Finite option
	wxMenuItem* pFiniteView = nullptr;
	// Menu Item pointer for Toroidal option
	wxMenuItem* pToroidalView = nullptr;
	// Menu item pointer for show grid
	wxMenuItem* pDisplayGrid = nullptr;
	// Menu item pointer for show 10x10 grid
	wxMenuItem* pDisplayThickGrid = nullptr;
	// Menu item pointer for show hud
	wxMenuItem* pDisplayHUD = nullptr;

public:
	// Constructor
	MainWindow();
	// Destructor
	~MainWindow();

	//Methods
	void OnSizeChanged(wxSizeEvent& event);
	void GridInit();
	void UpdateStatusText();
	void OnPlayButtonClick(wxCommandEvent& event);
	void OnNextButtonClick(wxCommandEvent& event);
	void OnPauseButtonClick(wxCommandEvent& event);
	void OnClearButtonClick(wxCommandEvent& event);
	void OnSettingsOptionClick(wxCommandEvent& event);
	void OnResetSettingsOptionClick(wxCommandEvent& event);
	void OnViewDisplayNeighborsCheck(wxCommandEvent& event);
	void OnViewFiniteCheck(wxCommandEvent& event);
	void OnViewToroidalCheck(wxCommandEvent& event);
	void OnViewDisplayGrid(wxCommandEvent& event);
	void OnViewDisplayThickGrid(wxCommandEvent& event);
	void OnViewDisplayHUD(wxCommandEvent& event);
	void OnOptionsRandomizeClick(wxCommandEvent& event);
	void OnOptionsRandomizeWithSeedClick(wxCommandEvent& event);
	void OnFileNewClick(wxCommandEvent& event);
	void OnFileImportClick(wxCommandEvent& event);
	void OnFileOpenClick(wxCommandEvent& event);
	void OnFileSaveClick(wxCommandEvent& event);
	void OnFileSaveAsClick(wxCommandEvent& event);
	void OnFileExitClick(wxCommandEvent& event);
	int GetLivingNeighborCount(int row, int column);
	void CreateNextCellGeneration();
	void ResetGameboard();
	void TriggerTimer(wxTimerEvent& event);
	void RandomizeGrid(int seed);

	wxDECLARE_EVENT_TABLE();
};

