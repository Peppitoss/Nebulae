#pragma once
#include"Graphics.h"
#include"Globals.h"

enum enZoomLevel
{
	ZoomCLOSE = 0,
	ZoomNORMAL = 1,
	ZoomFAR = 2
};

class GameCamera
{

public:

	GameCamera();
	GameCamera(SDL_Rect Boundary);


	double GetZoomGlobalPositionModifier();
	double GetZoomDrawPositionModifier();
	static double GetZoomGlobalPositionModifier(enZoomLevel);
	static double GetZoomDrawPositionModifier(enZoomLevel);
	void SetZoomLevel(enZoomLevel set);
	void ResetZoomLevel(enZoomLevel set);
	void IncreaseZoomLevel(int amount);
	void DecreaseZoomLevel(int amount);

	void SetCameraCenterPoint(const Coord & newPoint);
	void ScaleCameraPoint(double scaling);
	void SetCameraScreenSize(enZoomLevel newSize);

	int PlayerSize() const { return DEFAULT_PLAYER_SIZE; }
	Coord GetCameraPos() { return Coord(this->x(), y()); }

	SDL_Rect& GetBoundary() { return pCameraBoundary; }
	int x() { return pCameraBoundary.x; }
	int y() { return pCameraBoundary.y; }

	int w() { return pCameraBoundary.w; }
	int h() { return pCameraBoundary.h; }

	Coord GetCameraCenterModifier() const { return this->pCameraCenterModifier; }
	enZoomLevel GetZoomLevel() const { return this->pZoomLevel; }

private:

	SDL_Rect pCameraBoundary;

	enZoomLevel pZoomLevel;
	Coord pCameraCenterModifier;

	void UpdateZoomCameraCenterModifier(int sHeight, int sWidth);
	void UpdateZoomCameraCenterModifier(int sHeight, int sWidth, double modifier);

	void Initialize();
};