#define LIVING_CELLS_COLOR_PICKER_ID 10001
#define DEAD_CELLS_COLOR_PICKER_ID 10002
#define GRID_SIZE_SPINNER_ID 10003
#define TIME_INTERVAL_SPINNER_ID 10004
#define GRID_COLOR_PICKER_ID 10005
#define THICK_GRID_COLOR_PICKER_ID 10006

#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
	EVT_BUTTON(wxID_OK, SettingsDialog::OnSaveButtonClick)
	EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancelButtonClick)
wxEND_EVENT_TABLE();

SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settings) : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxSize(300, 355)), pSettings(settings)
{
	// Main Modal Box Sizer
	wxBoxSizer* mainSettingsSizer = new wxBoxSizer(wxVERTICAL);
	// Setting Main Sizer
	SetSizer(mainSettingsSizer);

	// Living Cells Color Setting Sizer
	wxBoxSizer* livingCellsColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* livingCellsColorLabel = new wxStaticText(this, wxID_ANY, "Living Cells Color");
	livingCellsColorPicker = new wxColourPickerCtrl(this, LIVING_CELLS_COLOR_PICKER_ID); 
	livingCellsColorPicker->SetColour(pSettings->GetLivingCellsColor()); 
	livingCellsColorSizer->Add(livingCellsColorLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 50);
	livingCellsColorSizer->Add(livingCellsColorPicker, 1, wxEXPAND);

	// Dead Cells Color Setting Sizer
	wxBoxSizer* deadCellsColorSizer = new wxBoxSizer(wxHORIZONTAL);	
	wxStaticText* deadCellsColorLabel = new wxStaticText(this, wxID_ANY, "Dead Cells Color");
	deadCellsColorPicker = new wxColourPickerCtrl(this, DEAD_CELLS_COLOR_PICKER_ID);
	deadCellsColorPicker->SetColour(pSettings->GetDeadCellsColor());
	deadCellsColorSizer->Add(deadCellsColorLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 55);
	deadCellsColorSizer->Add(deadCellsColorPicker, 1, wxEXPAND);

	// Grid Color Setting Sizer
	wxBoxSizer* gridColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* gridColorLabel = new wxStaticText(this, wxID_ANY, "Grid Color");
	gridColorPicker = new wxColourPickerCtrl(this, GRID_COLOR_PICKER_ID);
	gridColorPicker->SetColour(pSettings->GetGridColor());
	gridColorSizer->Add(gridColorLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 88);
	gridColorSizer->Add(gridColorPicker, 1, wxEXPAND);
	
	// Thick Grid Color Setting Sizer
	wxBoxSizer* thickGridColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* thickGridColorLabel = new wxStaticText(this, wxID_ANY, "10x10 Grid Color");
	thickGridColorPicker = new wxColourPickerCtrl(this, THICK_GRID_COLOR_PICKER_ID);
	thickGridColorPicker->SetColour(pSettings->GetThickGridColor());
	thickGridColorSizer->Add(thickGridColorLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 55);
	thickGridColorSizer->Add(thickGridColorPicker, 1, wxEXPAND);

	// Grid Size Setting Sizer
	wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);	
	wxStaticText* gridSizeLabel = new wxStaticText(this, wxID_ANY, "Grid Size");
	gridSizeSpinner = new wxSpinCtrl(this, GRID_SIZE_SPINNER_ID);
	gridSizeSpinner->SetValue(pSettings->GetGridSize());
	gridSizeSizer->Add(gridSizeLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 98);
	gridSizeSizer->Add(gridSizeSpinner, 1, wxEXPAND);

	// Time Interval Setting Sizer
	wxBoxSizer* timeIntervalSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* timeIntervalLabel = new wxStaticText(this, wxID_ANY, "Time Interval");
	timeIntervalSpinner = new wxSpinCtrl(this, TIME_INTERVAL_SPINNER_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT, 25, 1000);
	timeIntervalSpinner->SetValue(pSettings->GetTimeInterval());
	timeIntervalSizer->Add(timeIntervalLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 75);
	timeIntervalSizer->Add(timeIntervalSpinner, 1, wxEXPAND);

	// Ok and Cancel buttons for end of settings save/discard
	wxSizer* saveSettingsButtons = CreateButtonSizer(wxOK | wxCANCEL);

	// Add choices of living cells color, dead cells color, grid size, and time interval to the main sizer
	// With Save / Cancel Button
	mainSettingsSizer->Add(livingCellsColorSizer, 0, wxALL | wxEXPAND, 10);
	mainSettingsSizer->Add(deadCellsColorSizer, 0, wxALL | wxEXPAND, 10);
	mainSettingsSizer->Add(gridColorSizer, 0, wxALL | wxEXPAND, 10);
	mainSettingsSizer->Add(thickGridColorSizer, 0, wxALL | wxEXPAND, 10);
	mainSettingsSizer->Add(gridSizeSizer, 0, wxALL | wxEXPAND, 10);
	mainSettingsSizer->Add(timeIntervalSizer, 0, wxALL | wxEXPAND, 10);
	mainSettingsSizer->Add(saveSettingsButtons, 0, wxALL | wxALIGN_CENTER, 10);
}

void SettingsDialog::SetSettings(Settings* settings)
{
	pSettings = settings;
}

void SettingsDialog::OnSaveButtonClick(wxCommandEvent& event)
{
	// Save changes to Settings object
	pSettings->SetLivingCellsColor(livingCellsColorPicker->GetColour());
	pSettings->SetDeadCellsColor(deadCellsColorPicker->GetColour());
	pSettings->SetGridColor(gridColorPicker->GetColour());
	pSettings->SetThickGridColor(thickGridColorPicker->GetColour());
	pSettings->SetGridSize(gridSizeSpinner->GetValue());
	pSettings->SetTimeInterval(timeIntervalSpinner->GetValue());

	// Close modal
	EndModal(wxID_OK);
}

void SettingsDialog::OnCancelButtonClick(wxCommandEvent& event)
{
	// Close modal
	EndModal(wxID_CANCEL);
}
