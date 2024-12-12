#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
	EVT_PAINT(DrawingPanel::OnPaint)
	EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE();

DrawingPanel::DrawingPanel(wxFrame* parent, wxSize size, std::vector<std::vector<bool>>& rGameBoard, std::vector<std::vector<int>>& rNeighborCount, int& rLivingCells, int& rGenerations, MainWindow* mainWindow)
	: wxPanel(parent, wxID_ANY, wxPoint(0, 0), size), vGameBoard(rGameBoard), vNeighborCount(rNeighborCount), mLivingCells(rLivingCells), mGenerations(rGenerations), pMainWindow(mainWindow)
{
	// Control over the rendering of the DrawingPanel
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawingPanel::~DrawingPanel()
{
}

void DrawingPanel::OnPaint(wxPaintEvent& event)
{
	// Prevent some rendering issues in Windows
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	// Drawing surface
	wxGraphicsContext* context = wxGraphicsContext::Create(dc);
	// Test if context failed
	if (!context) { return; }

	// Calculate the cell width, panel width divided by the grid size
	float cellWidth = this->GetSize().x / (float)pSettings->GetGridSize();
	// Calculate the cell height, panel height divided by the grid size
	float cellHeight = this->GetSize().y / (float)pSettings->GetGridSize();

	// Set font size and color for the Neighbor Counts
	context->SetFont(wxFontInfo(8), *wxRED);

	// Loop up to the gridSize for rows
	for (size_t i = 0; i < pSettings->GetGridSize(); ++i)
	{
		// Loop up to the gridSize for columns
		for (size_t j = 0; j < pSettings->GetGridSize(); j++)
		{
			// Variable to hold x-coord calculation, rows * cell width
			float x = i * cellWidth;
			// Variable to hold y-coord calculation, cols * cell height
			float y = j * cellHeight;
			 
			if (vGameBoard[i][j]) { // If cell is alive (true)
				// Set border to Black or transparent depending on show grid setting
				context->SetPen(pSettings->GetDisplayGrid() ? pSettings->GetGridColor() : wxTransparentColor);
				// Set background to saved living cell color
				context->SetBrush(pSettings->GetLivingCellsColor()); 
			}
			else { // If cell is dead (false)
				// Set border to Black or transparent depending on show grid setting
				context->SetPen(pSettings->GetDisplayGrid() ? pSettings->GetGridColor() : wxTransparentColor);
				// Set background to saved dead cell color
				context->SetBrush(pSettings->GetDeadCellsColor());
			}

			// Draw the rectangles
			context->DrawRectangle(x, y, cellWidth, cellHeight);

			// If the neighbor count value is greater than 0
			if (pSettings->GetShowNeighborCount() && vNeighborCount[i][j] > 0) {
				// Display the neighbor count value
				wxString cellText = std::to_string(vNeighborCount[i][j]);
				// Calculate proper centering of value into the cell
				double textWidth, textHeight;
				context->GetTextExtent(cellText, &textWidth, &textHeight);
				context->DrawText(cellText, (i * cellWidth) + cellWidth / 2 - textWidth / 2, (j * cellHeight) + cellHeight / 2 - textHeight / 2);
			}
		}
	}

	// If Show 10x10 grid is enabled
	if (pSettings->GetDisplayThickGrid()) {
		// Calculate lines to draw
		int linesToDraw = pSettings->GetGridSize() / 10;
		
		// Create pen for thick lines
		wxPen thickGridPen(pSettings->GetThickGridColor(), 4);
		dc.SetPen(thickGridPen);

		// Loop to create vertical lines
		for (int i = 1; i <= linesToDraw; i++) {
			wxPoint vStart(cellWidth * (i * 10), 0);
			wxPoint vEnd(cellWidth * (i * 10), this->GetSize().y);
			dc.DrawLine(vStart, vEnd);
		}

		// Loop to create horizontal lines
		for (int i = 1; i <= linesToDraw; i++) {
			wxPoint hStart(0, cellHeight * (i * 10));
			wxPoint hEnd(this->GetSize().x, cellHeight * (i * 10));
			dc.DrawLine(hStart, hEnd);
		}
	}

	// If Show HUD is enabled
	if (pSettings->GetDisplayHUD()) {
		double xHudText, yHudText;
		context->SetFont(wxFontInfo(10), wxColor(255, 0, 0, 255));
		wxString hudText = wxString::Format(
							"Generations: %d\nLiving Cells: %d\nBoundary Type: %s\nGrid Size: %dx%d",
							mGenerations, 
							mLivingCells, 
							(pSettings->GetUniverseView() ? "Finite" : "Toroidal"), 
							pSettings->GetGridSize(),
							pSettings->GetGridSize()
						);

		context->GetTextExtent(hudText, &xHudText, &yHudText);
		context->DrawText(hudText, 7, GetSize().y - yHudText - 5);
	}

	// Free allocated memory
	delete context;
}

void DrawingPanel::SetPanelSize(const wxSize& size)
{
	// Sets the Panel size by referenced size
	this->SetSize(size);

	// Refreshes and causes the OnPaint event to trigger
	this->Refresh();
}

void DrawingPanel::SetGridSize(const int& size)
{
	// Set the grid size to the passed in size
	pSettings->SetGridSize(size);
}

void DrawingPanel::OnMouseUp(wxMouseEvent& event)
{
	// Reset living cell count
	mLivingCells = 0;

	// Get mouse click x coordinate
	float x = (float)event.GetX();
	// Get mouse click y coordinate
	float y = (float)event.GetY();

	// Calculate the cell width, panel width divided by the grid size
	float cellWidth = (float)(this->GetSize().x) / (float)(pSettings->GetGridSize());
	// Calculate the cell height, panel height divided by the grid size
	float cellHeight = (float)(this->GetSize().y) / (float)(pSettings->GetGridSize());

	// Row calculation of x divided by cell width
	int row = x / cellWidth;
	// Column calculation of y divided by cell height
	int column = y / cellHeight;

	// Switch bool value of clicked cell
	// Checks to ensure the row/column and click are in boundaries of the gameboard
	if (row >= 0 && row < vGameBoard.size() && column >= 0 && column < vGameBoard[row].size()) {
		vGameBoard[row][column] = !vGameBoard[row][column];

		// Recalculate neighbor counts for all cells
        for (size_t i = 0; i < vGameBoard.size(); i++) {
            for (size_t j = 0; j < vGameBoard[i].size(); j++) {
                vNeighborCount[i][j] = pMainWindow->GetLivingNeighborCount(i, j);
				
				// If cell is alive
				if (vGameBoard[i][j]) {
					mLivingCells++; // Increment living cells count
				}
            }
        }
	}

	// Update the status text to reflect the new count of living cells
	pMainWindow->UpdateStatusText();

	// Refreshes and causes the OnPaint event to trigger
	this->Refresh();
}

void DrawingPanel::SetSettings(Settings* settings)
{
	pSettings = settings;
}
