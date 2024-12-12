#pragma once
#include "wx/wx.h"
#include "Settings.h"

class MainWindow;

class DrawingPanel : public wxPanel
{
private:
	// Member fields 
	std::vector<std::vector<bool>>& vGameBoard;
	// Vector to hold referenced neighbor count values
	std::vector<std::vector<int>>& vNeighborCount;
	// Pointer for the Settings object
	Settings* pSettings = nullptr;
	// Reference to generations in MainWindow
	int& mLivingCells;
	// Reference to living cells in MainWindow
	int& mGenerations;
	MainWindow* pMainWindow = nullptr;

public:
	// Constructor
	DrawingPanel(wxFrame* parent, wxSize size, std::vector<std::vector<bool>>& rGameBoard, std::vector<std::vector<int>>& rNeighborCount, int& rLivingCells, int& rGenerations, MainWindow* mainWindow);
	// Deconstructor
	~DrawingPanel();
	// Methods
	void OnPaint(wxPaintEvent& event);
	void SetPanelSize(const wxSize& resize);
	void SetGridSize(const int& size);
	void OnMouseUp(wxMouseEvent& event);
	void SetSettings(Settings* settings);

	wxDECLARE_EVENT_TABLE();
};

