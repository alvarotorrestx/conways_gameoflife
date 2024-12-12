#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings {
private:
	// Member fields
	// Living cell colors
	unsigned int mLivingCellsRed = 128;
	unsigned int mLivingCellsGreen = 128;
	unsigned int mLivingCellsBlue = 128;
	unsigned int mLivingCellsAlpha = 255;

	// Dead cell colors
	unsigned int mDeadCellsRed = 255;
	unsigned int mDeadCellsGreen = 255;
	unsigned int mDeadCellsBlue = 255;
	unsigned int mDeadCellsAlpha = 255;

	// Default Grid Colors
	unsigned int mGridColorRed = 0;
	unsigned int mGridColorGreen = 0;
	unsigned int mGridColorBlue = 0;
	unsigned int mGridColorAlpha = 255;

	// Default Grid 10x10 Colors
	unsigned int mThickGridColorRed = 0;
	unsigned int mThickGridColorGreen = 0;
	unsigned int mThickGridColorBlue = 0;
	unsigned int mThickGridColorAlpha = 255;

	// Default grid size
	int mGridSize = 15;
	// Default time interval
	int mTimeInterval = 50;

	// Default check value or Show Neighbor Count
	bool mShowNeighborCount = true;

	// Default universe view of True (Finite), switchable for False (Toroidal)
	bool mUniverseView = true;

	// Default value to Display the Grid lines
	bool mDisplayGrid = true;

	// Default value for showing thick grid lines (10x10)
	bool mDisplayThickGrid = true;

	// Default value for showing the hud overlay
	bool mDisplayHUD = true;

public:
	// Getters
	wxColor GetLivingCellsColor() const {
		return wxColor(mLivingCellsRed, mLivingCellsGreen, mLivingCellsBlue, mLivingCellsAlpha);
	}

	wxColor GetDeadCellsColor() const {
		return wxColor(mDeadCellsRed, mDeadCellsGreen, mDeadCellsBlue, mDeadCellsAlpha);
	}

	wxColor GetGridColor() const {
		return wxColor(mGridColorRed, mGridColorGreen, mGridColorBlue, mGridColorAlpha);
	}

	wxColor GetThickGridColor() const {
		return wxColor(mThickGridColorRed, mThickGridColorGreen, mThickGridColorBlue, mThickGridColorAlpha);
	}

	int GetGridSize() const {
		return mGridSize;
	}

	int GetTimeInterval() const {
		return mTimeInterval;
	}

	bool GetShowNeighborCount() const {
		return mShowNeighborCount;
	}

	bool GetUniverseView() const {
		return mUniverseView;
	}

	bool GetDisplayGrid() const {
		return mDisplayGrid;
	}

	bool GetDisplayThickGrid() const {
		return mDisplayThickGrid;
	}

	bool GetDisplayHUD() const {
		return mDisplayHUD;
	}

	// Setters
	void SetLivingCellsColor(const wxColor& color) {
		mLivingCellsRed = color.GetRed();
		mLivingCellsGreen = color.GetGreen();
		mLivingCellsBlue = color.GetBlue();
		mLivingCellsAlpha = color.GetAlpha();
	}

	void SetDeadCellsColor(const wxColor& color) {
		mDeadCellsRed = color.GetRed();
		mDeadCellsGreen = color.GetGreen();
		mDeadCellsBlue = color.GetBlue();
		mDeadCellsAlpha = color.GetAlpha();
	}
	
	void SetGridColor(const wxColor& color) {
		mGridColorRed = color.GetRed();
		mGridColorGreen = color.GetGreen();
		mGridColorBlue = color.GetBlue();
		mGridColorAlpha = color.GetAlpha();
	}

	void SetThickGridColor(const wxColor& color) {
		mThickGridColorRed = color.GetRed();
		mThickGridColorGreen = color.GetGreen();
		mThickGridColorBlue = color.GetBlue();
		mThickGridColorAlpha = color.GetAlpha();
	}

	void SetGridSize(int size) {
		mGridSize = size;
	}

	void SetTimeInterval(int time) {
		mTimeInterval = time;
	}

	void SetShowNeighborCount(bool check) {
		mShowNeighborCount = check;
	}

	void SetUniverseView(bool check) {
		mUniverseView = check;
	}

	void SetDisplayGrid(bool check) {
		mDisplayGrid = check;
	}

	void SetDisplayThickGrid(bool check) {
		mDisplayThickGrid = check;
	}
	
	void SetDisplayHUD(bool check) {
		mDisplayHUD = check;
	}

	// Methods
	void LoadGameSettings() {
		//Load Data
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		// If no file exists or can't be opened, default settings
		if (!file) {
			return;
		}
		file.read((char*)this, sizeof(Settings));
		file.close();
	}
	
	void SaveGameSettings() const {
		//Save Data
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		// If error with file saving
		if (!file) {
			return;
		}
		file.write((char*)this, sizeof(Settings));
		file.close();
	}

	void ResetSettings() {
		// Set living cells color to default
		SetLivingCellsColor(wxColor(128, 128, 128, 255));
		// Set dead cells color to default
		SetDeadCellsColor(wxColor(255, 255, 255, 255));
		// Set grid color to default
		SetGridColor(wxColor(0, 0, 0, 255));
		// Set thick grid color to default
		SetThickGridColor(wxColor(0, 0, 0, 255));
		// Set grid size to default
		SetGridSize(15);
		// Set time interval to default
		SetTimeInterval(50);
		// Set show neighbor count bool to default
		SetShowNeighborCount(true);
		// Set universe view to default
		SetUniverseView(true);
		// Set display grid to default
		SetDisplayGrid(true);
		// Set show thick grid to default
		SetDisplayThickGrid(true);
		// Set display hud to default
		SetDisplayHUD(true);
	}
};