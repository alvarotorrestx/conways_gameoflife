#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"

class SettingsDialog : public wxDialog
{
private:
	// Pointer towards the settings object
	Settings* pSettings = nullptr;
	// Living Cells Color Pointer for color picker
	wxColourPickerCtrl* livingCellsColorPicker = nullptr; 
	// Dead Cells Color Pointer for color picker
	wxColourPickerCtrl* deadCellsColorPicker = nullptr; 
	// Grid color pointer for the color picker
	wxColourPickerCtrl* gridColorPicker = nullptr;
	// Thick Grid color pointer for the color picker
	wxColourPickerCtrl* thickGridColorPicker = nullptr;
	// Grid Size Pointer for spinner
	wxSpinCtrl* gridSizeSpinner = nullptr; 
	// Time Interval Pointer for spinner
	wxSpinCtrl* timeIntervalSpinner = nullptr; 

public:
	// Constructor
	SettingsDialog(wxWindow* parent, Settings* settings);
	// Deconstructor
	~SettingsDialog() {}

	// Methods
	void SetSettings(Settings* settings);
	void OnSaveButtonClick(wxCommandEvent& event);
	void OnCancelButtonClick(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();
};

