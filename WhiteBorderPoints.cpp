//#include"WhiteBorderPoints.h"
//

//
//
//WhiteBorderPoints::WhiteBorderPoints() : PointColor( &ImportImage_border_template_sprite ), BorderGraphicHandler()
//{
//	this->pScale = spriteSize(primitives::EnumSpriteSize::x8);
//	Initialize();
//}
//
//WhiteBorderPoints::WhiteBorderPoints( primitives::EnumSpriteSize size ) : PointColor( &ImportImage_border_template_sprite ), BorderGraphicHandler(size)
//{
//	this->pScale = spriteSize(size);
//	Initialize();
//}
//
//WhiteBorderPoints::WhiteBorderPoints( primitives::EnumSpriteSize size, const ColorRGB& col) : PointColor( &ImportImage_border_template_sprite ) , BorderGraphicHandler(size)
//{
//	this->pScale = spriteSize(size);
//	Initialize();
//	this->Color = col;
//}
//
//void WhiteBorderPoints::UpdateSize( primitives::EnumSpriteSize newSize )
//{
//	BorderGraphicHandler::UpdateSize(newSize);
//	this->pScale = BorderGraphicHandler::GetCropSize();
//}
//
//SDL_Texture* WhiteBorderPoints::GetTexture( SDL_Renderer* renderer, const BorderData& data)
//{
//	UpdateTextureCrop(renderer, data);
//	return getTexture( renderer );
//}
//
//void WhiteBorderPoints::CropTexture( SDL_Renderer* renderer, const SDL_Rect& rect )
//{
//	TextureObject_2::CropTexture( renderer, rect );
//	this->modulate( this->Color );
//}
//
//void WhiteBorderPoints::Initialize()
//{
//	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
//	this->pForceUpdate = true;
//}