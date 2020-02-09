#include"BorderGraphicHandler.h"



//
//BorderGraphicHandler::BorderGraphicHandler()
//{
//	this->pCropSize = spriteSize( primitives::EnumSpriteSize::x1 );
//}
//BorderGraphicHandler::BorderGraphicHandler( primitives::EnumSpriteSize size )
//{
//	this->pCropSize = spriteSize(size);
//}
//
//void BorderGraphicHandler::UpdateSize( primitives::EnumSpriteSize newSize )
//{
//	pCropSize = spriteSize(newSize);
//}
//
////void BorderGraphicHandler::Initialize()
////{
////	this->pPreviousCrop.x = -10000;
////}
//
//void BorderGraphicHandler::UpdateTextureCrop( SDL_Renderer* renderer, const BorderData& data)
//{
//	this->CropTexture(renderer, GenerateCropRect(data) );
//}

Coord BorderGraphicHandler::GetSurfaceDiagonalCutPosition(const BorderData& data)
{
	int corners = data.GetDiagonalCornerBorders();

	if (corners == 0)
		return Coord(0);
	if (corners == 4)
		return Coord(1, 0);
	if (corners == 3)
		return ThreeDiagonalCornersGetCoord(data);
	if (corners == 2)
		return TwoDiagonalCornersGetCoord(data);
	if (corners == 1)
		return OneDiagonalCornersGetCoord(data);
	return Coord(0);
}


Coord BorderGraphicHandler::ThreeDiagonalCornersGetCoord(const BorderData& data)
{
	if (!data.upleft)
		return Coord(2, 4);
	if (!data.upright)
		return Coord(1, 4);
	if (!data.downleft)
		return Coord(0, 4);
	if (!data.downright)
		return Coord(3, 4);

	return Coord(0);
}

Coord BorderGraphicHandler::TwoDiagonalCornersGetCoord(const BorderData& data)
{
	if (!data.upleft && !data.upright)
		return Coord(0, 4);
	if (!data.downleft && !data.downright)
		return Coord(1, 4);
	if (!data.downleft && !data.upright)
		return Coord(0, 5);
	if (!data.upright && !data.downright)
		return Coord(0, 5);	//1,5
	if (!data.downright && !data.upleft)
		return Coord(0, 5);
	if (!data.upleft && !data.downleft)
		return Coord(0, 5); //2,5

	return Coord(0);

}

Coord BorderGraphicHandler::OneDiagonalCornersGetCoord(const BorderData& data)
{
	return Coord(0);
}

Coord BorderGraphicHandler::GetSurfaceCutPosition( const BorderData& data )
{
	int corners = data.GetStraightCornerBorders();
	
	if( corners == 0 )
		return Coord(0);
	if( corners == 4 )
		return Coord(1,0);
	if( corners == 3 )
		return ThreeCornersGetCoord( data );
	if( corners == 2 )
		return TwoCornersGetCoord( data );
	if( corners == 1 )
		return OneCornersGetCoord( data );
	return Coord(0);

}

Coord BorderGraphicHandler::ThreeCornersGetCoord( const BorderData& data)
{
	if( !data.left )
		return Coord(2,1);
	if( !data.up )
		return Coord(1,1);
	if( !data.right )
		return Coord(0,1);
	if( !data.down )
		return Coord(3,1);

	return Coord(0);
}

Coord BorderGraphicHandler::TwoCornersGetCoord( const BorderData& data)
{
	if( !data.left && !data.right)
		return Coord(0,2);
	if( !data.up && !data.down)
		return Coord(1,2);
	if( !data.up && !data.right )
		return Coord(5,2);
	if( !data.right && !data.down  )
		return Coord(4,2);
	if( !data.down && !data.left  )
		return Coord(3,2);
	if( !data.left && !data.up  )
		return Coord(2,2);

	return Coord(0);

}

Coord BorderGraphicHandler::OneCornersGetCoord( const BorderData& data)
{
	if( data.down )
		return Coord(0,3);
	if( data.left )
		return Coord(3,3);
	if( data.up )
		return Coord(2,3);
	if( data.right )
		return Coord(1,3);

	return Coord(0);
}

//bool BorderGraphicHandler::UpdateRequired(const BorderData& data) const 
//{ 
//	return this->pPreviousData != data; 
//}
//
//SDL_Rect BorderGraphicHandler::GetPreviousCrop() const
//{
//	return this->pPreviousCrop;
//}
//
SDL_Rect BorderGraphicHandler::GenerateCropRect( const BorderData& data, int cropSize )
{
	Coord cutPosition;

	if(data.GetStraightCornerBorders() == 4)
		cutPosition = GetSurfaceDiagonalCutPosition(data);
	else
		cutPosition = GetSurfaceCutPosition( data );

	SDL_Rect result = SDL_Rect();

	result.x = cutPosition.x*cropSize;
	result.y = cutPosition.y*cropSize;
	result.h = cropSize;
	result.w = cropSize;

	return result;
}
