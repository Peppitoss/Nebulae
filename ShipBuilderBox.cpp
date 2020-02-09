#include"ShipBuilderBox.h"
#include"GridBoxTextureCollection.h"
#include"BorderGraphicHandler.h"

ShipBuilderBox::ShipBuilderBox() : DynamicGridBox() 
{
	Init();
}

ShipBuilderBox::ShipBuilderBox( const imagehandling::IMGToSurface* surface, spriteSize size ) : DynamicGridBox(surface) 
{
	pCropSize = size;
	Init();
}

ShipBuilderBox::ShipBuilderBox( const imagehandling::IMGToSurface* surface, std::shared_ptr<imagehandling::IMGToSurface> floorBase, std::shared_ptr<imagehandling::IMGToSurface> wallBase, spriteSize size ) : DynamicGridBox(surface)
{
	this->pFloorBase = floorBase;
	this->pWallBase = wallBase;
	this->pStandardTxt.get()->SetCropBase( floorBase );
	this->setType( enBoxType::EMPTY );
	pCropSize = size;
	Init();
}

void ShipBuilderBox::UpdateBorder( const BorderData& data )
{
	this->pStandardTxt.get()->SetSurfaceClip( BorderGraphicHandler::GenerateCropRect( data, pCropSize.GetSize() ) );
	this->pInvalidTxt.get()->SetSurfaceClip( BorderGraphicHandler::GenerateCropRect( data, pCropSize.GetSize() ) );
}

ShipBuilderBox::ShipBuilderBox( const ColorRGB& col ) : DynamicGridBox(col) 
{
	this->setType(enBoxType::OUTER);
}

void ShipBuilderBox::Init()
{
	pType = enBoxType::EMPTY;
	pBoxCondition = enBoxCondition::NORMAL_CONDITION;
}

void ShipBuilderBox::SetColorModulation( ColorRGB col )
{
	this->pStandardTxt.get()->SetCustomModulation( col );
}

void ShipBuilderBox::setStandardTexture(const imagehandling::IMGToSurface* surf)
{
	DynamicGridBox::setStandardTexture(surf);
}

void ShipBuilderBox::setStandardTexture(const ColorRGB& col)
{
	DynamicGridBox::setStandardTexture(col);
}

void ShipBuilderBox::MakeOuter()
{
	this->pStandardTxt.get()->SetCropBase(pFloorBase);
}

void ShipBuilderBox::MakeFloor()
{
	this->pStandardTxt.get()->SetCropBase( pFloorBase );
}

void ShipBuilderBox::MakeWall()
{
	this->pStandardTxt.get()->SetCropBase( pWallBase );
}

void ShipBuilderBox::setType( enBoxType set )
{
	switch (set)
	{
	case(enBoxType::EMPTY) :
		MakeFloor();
		break;
	case(enBoxType::OUTER) :
		MakeOuter();
		break;
	case(enBoxType::WALL):
		MakeWall();
		break;
	default:
		break;
	}
	pType = set;
}
