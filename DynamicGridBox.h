#pragma once
#include"GridBox.h"
#include"BorderData.h"


enum enBoxType
{
	EMPTY=0,
	WALL,
	//CORNERWALL,
	OUTER,
};

//enum enBoxOrientation
//{
//	NORTH=0,
//	EAST,
//	SOUTH,
//	WEST,
//};

class DynamicGridBox : public GridBox
{
public:

	DynamicGridBox();
	DynamicGridBox( const DynamicGridBox& );
	DynamicGridBox( DynamicGridBox&& );

	DynamicGridBox(const imagehandling::IMGToSurface* );
	DynamicGridBox(const ColorRGB& );

	virtual SDL_Texture* GetTextureByState( SDL_Renderer* renderer);
	virtual SDL_Texture* getStandardTexture( SDL_Renderer* renderer );
	SDL_Texture* getHighlightTexture(SDL_Renderer* renderer);
	SDL_Texture* getInvalidTexture(SDL_Renderer* renderer);
	void setHighlightTexture(const imagehandling::IMGToSurface*);
	void Refresh();

	//~DynamicGridBox() { delete pHighlightTxt; }
	DynamicGridBox & operator=( const DynamicGridBox& );
	DynamicGridBox & operator=( DynamicGridBox&& );

protected:

	void UpdateInvalidTexture();
	std::unique_ptr<imagehandling::TextureObject_2> pHighlightTxt;
	std::unique_ptr<imagehandling::TextureObject_2> pInvalidTxt;

private:

	virtual void Init();


};
