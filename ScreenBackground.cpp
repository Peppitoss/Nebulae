#include"ScreenBackground.h"
#include"BackgroundCollection.h"
#include"ShipImageDefinitions.h"
#include"Debug.h"

ScreenBackground::ScreenBackground()
{
}

ScreenBackground::ScreenBackground( const imagehandling::IMGToSurface* bk_surf )
{
	if(bk_surf != nullptr)
		this->AddTexture( bk_surf );
}

imagehandling::TextureObject_2& ScreenBackground::getTexture(int index )
{
	return *pLayeredTexture[index].Object.get();
};

void ScreenBackground::Render( SDL_Renderer& renderer, const SDL_Rect& dim )
{
	SDL_Rect renderQuad = { dim.x, dim.y, dim.w, dim.h };
	RenderLayeredTextures(renderer, renderQuad);
}

void ScreenBackground::Render( SDL_Renderer& renderer, int x, int y )
{
	Render( renderer, Coord(x,y) );
}

void ScreenBackground::Render( SDL_Renderer& renderer, const Coord& pos )
{
	//Coord dimension = Coord(this->pDimension.w, this->pDimension.h);
	//if (pAllTexturesHaveSameDimensionAsBase == false)
	//	dimension = Coord(0);

	SDL_Rect renderQuad = { pos.x, pos.y, this->pDimension.w, this->pDimension.h };
	RenderLayeredTextures(renderer, renderQuad);
}

void ScreenBackground::Render( SDL_Renderer& renderer )
{
	RenderLayeredTextures(renderer, this->pDimension);
}

void ScreenBackground::RenderLayeredTextures(SDL_Renderer& renderer, SDL_Rect dim)
{
	for (int i = 0; i < this->pLayeredTexture.size(); i++)
	{
		Coord position = this->pLayeredTexture[i];

		Coord dimension = Coord(this->pDimension.w, this->pDimension.h);
		if (pAllTexturesHaveSameDimensionAsBase == false)
			dimension = this->pLayeredTexture[i].Object->GetDimension().GetDimension();
		 
		SDL_Rect renderQuad = { dim.x + position.x, dim.y + position.y, dimension.x, dimension.y };
		getTexture(i).RenderTexture(&renderer, Coord::GetRectPosition(renderQuad));
		/*SDL_RenderCopy(&, getTexture(i).getTexture(&renderer), NULL, &renderQuad);*/ //
		Debug::SDLErrorCheck();
	}
}

void ScreenBackground::AddTexture( const imagehandling::IMGToSurface* bk_surf )
{
	AddTexture(bk_surf,255);
}

void ScreenBackground::AddTexture( std::shared_ptr<imagehandling::TextureObject_2> bk_text)
{
	AddTexture(bk_text,255);
}

void ScreenBackground::AddTexture(const imagehandling::IMGToSurface* bk_surf, int opacity)
{
	AddTexture(bk_surf, Coord(0), opacity);
}

void ScreenBackground::AddTexture( const imagehandling::IMGToSurface* bk_surf, const Coord& pos, int opacity)
{
	this->pLayeredTexture.push_back( CoordWithTexture( pos, std::shared_ptr<imagehandling::TextureObject_2>( new imagehandling::TextureObject_2(bk_surf))) );
	this->pLayeredTexture.back().Object->setOpacity(opacity,true);
}

void ScreenBackground::AddTexture( std::shared_ptr<imagehandling::TextureObject_2> bk_text, int opacity)
{
	this->pLayeredTexture.push_back( bk_text );
	this->pLayeredTexture.back().Object->setOpacity(opacity, true);
}

void ScreenBackground::AddTexture(std::shared_ptr<imagehandling::TextureObject_2> bk_text, const Coord& pos, int opacity)
{
	this->pLayeredTexture.push_back(CoordWithTexture(pos,bk_text));
	this->pLayeredTexture.back().Object->setOpacity(opacity, true);
}

void ScreenBackground::ResetTextures()
{
	for( int i = 0; i < this->pLayeredTexture.size(); i++ )
		this->pLayeredTexture[i].Object.reset();
	this->pLayeredTexture.clear();
}

ScreenBackground::~ScreenBackground()
{
	ResetTextures();
}