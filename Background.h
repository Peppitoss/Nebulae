#pragma once
#include <SDL.h>
#include"TextureObject.h"

class Background : public imagehandling::TextureObject_2
{
public:

	const static unsigned int MAX_DISTANCE = 2000;

	Background();
	Background( const imagehandling::IMGToSurface* bk_surf, unsigned int distance );

	SDL_Texture* getTexturePointer(int index ){ return pLayeredTexture[index].get(); };
	unsigned int getDistance(int index ){ return pTextureDistance[index]; };
	bool getRenderWithBase( int index) { return pRenderWithBase[index]; };
	int getListSize() { return pLayeredTexture.size() ; };

	unsigned int baseDistance() { return pBaseDistance; };

	void addTexture( SDL_Texture* bk_surf, unsigned int distance, bool renderWithBase ); 
	Coord getDimensions( int i ) { return pTextureDimensions[i]; };

	void ResetTextures();

	~Background();

private:

	unsigned int pBaseDistance;

	std::vector<std::unique_ptr<SDL_Texture,void(*)(SDL_Texture*)>> pLayeredTexture;
	std::vector<unsigned int> pTextureDistance;
	std::vector<bool> pRenderWithBase;			// will the texture be stretched to fill the entire size of the base texture? (fill main background picture?)
	std::vector<Coord> pTextureDimensions;

	Background( const Background& );
	Background( Background&& );
	Background operator=( const Background& );
	Background operator=( Background&& );


};	
