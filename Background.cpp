#include"ShipImageDefinitions.h"
#include"Background.h"
#include"Debug.h"

Background::Background() : TextureObject_2(ImportImage_background_grey)
{
	this->pBaseDistance = 0;
}

Background::Background( const imagehandling::IMGToSurface* bk_surf, unsigned int distance) : TextureObject_2(bk_surf)
{
	//this->pRenderer = renderer;
	//this->pBaseTexture = SDL_CreateTextureFromSurface(pRenderer, bk_surf->getSurface() );

	//int w, h;
	//SDL_QueryTexture(this->pTexture, NULL, NULL, &w, &h);

	//Coord size(0,0);
	//size.x = w;
	//size.y = h;

	//this->pBaseSize = size;

	if( distance > MAX_DISTANCE )
		distance = MAX_DISTANCE;

	this->pBaseDistance = distance;

}


void Background::addTexture( SDL_Texture* bk_text, unsigned int distance, bool renderWithBase )
{

	this->pLayeredTexture.push_back( std::unique_ptr<SDL_Texture,void(*)(SDL_Texture*)>(bk_text, Debug::DestroyTexture) );

	int w, h;
	SDL_QueryTexture(getTexturePointer(this->getListSize()-1), NULL, NULL, &w, &h);
	Debug::SDLErrorCheck();

	Coord size(0,0);
	size.x = w;
	size.y = h;

	this->pTextureDimensions.push_back( size );

	if( distance > MAX_DISTANCE )
		distance = MAX_DISTANCE;

	this->pRenderWithBase.push_back( renderWithBase );
	this->pTextureDistance.push_back( distance );
}

void Background::ResetTextures()
{
	for (int i = 0; i < this->pLayeredTexture.size(); i++)
	{
		this->pLayeredTexture[i].reset();
	}
	this->pLayeredTexture.clear();
}

Background::~Background()
{
	ResetTextures();
	TextureObject_2::~TextureObject_2();
}