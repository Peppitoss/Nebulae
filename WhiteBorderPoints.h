//#pragma once
//#include"Graphics.h"
//#include"PointColor.h"
//#include"BorderGraphicHandler.h"
//
//

//
//class WhiteBorderPoints : public PointColor, public BorderGraphicHandler
//{
//
//public:
//
//	WhiteBorderPoints();
//	WhiteBorderPoints( primitives::EnumSpriteSize );
//	WhiteBorderPoints( primitives::EnumSpriteSize, const ColorRGB& col);
//
//	SDL_Texture* GetTexture( SDL_Renderer* renderer, const BorderData& data);
//	//void UpdateBorders( SDL_Renderer* renderer, const BorderData& );
//
//	void Initialize();
//
//	
//
//protected:
//
//	bool pForceUpdate;
//	void CropTexture( SDL_Renderer* , const SDL_Rect& );
//	void UpdateSize( primitives::EnumSpriteSize newSize );
//};