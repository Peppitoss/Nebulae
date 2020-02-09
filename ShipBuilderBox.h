#pragma once
#include"DynamicGridBox.h"
#include"GridObjectCollection.h"
#include"BorderData.h"

enum enBoxCondition
{
	NORMAL_CONDITION = 0,
	DESTROYED_CONDITION

};

class ShipBuilderBox : public DynamicGridBox
{
public:

	//ShipBuilderBox();
	ShipBuilderBox();
	ShipBuilderBox( const imagehandling::IMGToSurface* surface, spriteSize size );
	ShipBuilderBox( const imagehandling::IMGToSurface* surface, std::shared_ptr<imagehandling::IMGToSurface> floorBase, std::shared_ptr<imagehandling::IMGToSurface> wallBase, spriteSize size );
	//ShipBuilderBox( const GridObjectTexturePack );
	//ShipBuilderBox( const GridObjectTexturePack, enBoxType type );
	ShipBuilderBox( const ColorRGB& );
	
	void SetColorModulation( ColorRGB col );
	void UpdateBorder( const BorderData& data );

	enBoxType GetBoxType() { return this->pType; };
	void setType( enBoxType set );
	

	void MakeFloor();
	void MakeWall();
	void MakeOuter();

	bool IsBoxDestroyed() { return pBoxCondition == enBoxCondition::DESTROYED_CONDITION; };
	void SetBoxDestroyed() { pBoxCondition = enBoxCondition::DESTROYED_CONDITION; };

	~ShipBuilderBox() {};

private:

	enBoxCondition pBoxCondition;

	void Init();
	//void UpdateInvalidTexture();
	void setStandardTexture(const imagehandling::IMGToSurface* surf) override;
	void setStandardTexture(const ColorRGB& surf);

	std::shared_ptr<imagehandling::IMGToSurface> pWallBase;
	std::shared_ptr<imagehandling::IMGToSurface> pFloorBase;

	//BorderData pPreviousBorderData;
	spriteSize pCropSize;
	enBoxType pType;
	//enBoxOrientation pOrientation;

	ShipBuilderBox( const ShipBuilderBox& );
	ShipBuilderBox & operator=( const ShipBuilderBox& );
	ShipBuilderBox & operator=( ShipBuilderBox&& );
	ShipBuilderBox( ShipBuilderBox&& );
};