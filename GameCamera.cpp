#include"GameCamera.h"

GameCamera::GameCamera()
{
	Initialize();
}

GameCamera::GameCamera(SDL_Rect boundary)
{
	Initialize();
	pCameraBoundary = boundary;
}

void GameCamera::Initialize()
{
	pZoomLevel = enZoomLevel::ZoomNORMAL;
	pCameraCenterModifier = Coord(0);
}


double GameCamera::GetZoomGlobalPositionModifier()  //is practically the inverse of GetZoomDrawPositionModifier
{
	return GetZoomGlobalPositionModifier(pZoomLevel);
}

double GameCamera::GetZoomDrawPositionModifier()	//is practically the inverse of GetZoomGlobalPositionModifier
{
	return GetZoomDrawPositionModifier(pZoomLevel);
}

double GameCamera::GetZoomGlobalPositionModifier(enZoomLevel zoomLevel)
{
	switch (zoomLevel)
	{
	case(enZoomLevel::ZoomCLOSE):
		return 0.4;
		break;
	case(enZoomLevel::ZoomNORMAL):
		return 1;
		break;
	case(enZoomLevel::ZoomFAR):
		return 1.67;
		break;
	}
}
double GameCamera::GetZoomDrawPositionModifier(enZoomLevel zoomLevel)
{
	switch (zoomLevel)
	{
	case(enZoomLevel::ZoomCLOSE):
		return 2.5;//1.25;	
		break;
	case(enZoomLevel::ZoomNORMAL):
		return 1;
		break;
	case(enZoomLevel::ZoomFAR):
		return 0.6;
		break;
	}
}


void GameCamera::SetZoomLevel(enZoomLevel set)
{
	if (pZoomLevel != set)
		ResetZoomLevel(set);
}

void GameCamera::ResetZoomLevel(enZoomLevel set)
{
	pZoomLevel = set;
	UpdateZoomCameraCenterModifier(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
}


void GameCamera::UpdateZoomCameraCenterModifier(int sWidth, int sHeight)
{
	UpdateZoomCameraCenterModifier(sHeight, sWidth, GetZoomDrawPositionModifier());
}

void GameCamera::UpdateZoomCameraCenterModifier(int sHeight, int sWidth, double modifier)
{
	pCameraCenterModifier = Coord(
		sWidth / (2 * modifier) - PlayerSize() / 2,
		sHeight / (2 * modifier) - PlayerSize() / 2);
}


void GameCamera::IncreaseZoomLevel(int amount)
{
	if (pZoomLevel + amount > 2)
		SetZoomLevel(enZoomLevel::ZoomFAR);
	else
		SetZoomLevel((enZoomLevel)((int)pZoomLevel + amount));
}
void GameCamera::DecreaseZoomLevel(int amount)
{
	if (pZoomLevel - amount < 0)
		SetZoomLevel(enZoomLevel::ZoomCLOSE);
	else
		SetZoomLevel((enZoomLevel)((int)pZoomLevel - amount));
}


void GameCamera::SetCameraCenterPoint(const Coord & newPoint)
{
	this->pCameraBoundary.x = newPoint.x;
	this->pCameraBoundary.y = newPoint.y;
}

void GameCamera::ScaleCameraPoint(double scaling)
{
	this->pCameraBoundary.x *= scaling;
	this->pCameraBoundary.y *= scaling;
}

void GameCamera::SetCameraScreenSize(enZoomLevel newSize)
{
	switch (newSize)
	{
	case ZoomCLOSE:
		this->pCameraBoundary.w = DEFAULT_SCREEN_WIDTH*0.4;
		this->pCameraBoundary.h = DEFAULT_SCREEN_HEIGHT*0.4;
		break;
	case ZoomNORMAL:
		this->pCameraBoundary.w = DEFAULT_SCREEN_WIDTH;
		this->pCameraBoundary.h = DEFAULT_SCREEN_HEIGHT;
		break;
	case ZoomFAR:
		this->pCameraBoundary.w = DEFAULT_SCREEN_WIDTH*1.67;
		this->pCameraBoundary.h = DEFAULT_SCREEN_HEIGHT*1.67;
		break;
	default:
		break;
	}

}