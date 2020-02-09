#include"TextureObject.h"

template class CoordWithObject<std::shared_ptr<imagehandling::TextureObject_2>>;
typedef CoordWithObject<std::shared_ptr<imagehandling::TextureObject_2>> CoordWithTexture;


class ScreenBackground
{
public:

	ScreenBackground();
	ScreenBackground( const imagehandling::IMGToSurface* bk_surf );

	imagehandling::TextureObject_2& getTexture(int index );
	int getListSize() { return pLayeredTexture.size() ; };

	void Render( SDL_Renderer& );
	void Render( SDL_Renderer&, const Coord& position );
	void Render( SDL_Renderer&, int x, int y );
	void Render( SDL_Renderer& , const SDL_Rect& dim );

	void RenderLayeredTextures(SDL_Renderer& renderer, SDL_Rect dim);

	void AddTexture( std::shared_ptr< imagehandling::TextureObject_2> bk_surf );
	void AddTexture( const imagehandling::IMGToSurface* bk_surf );

	void AddTexture( std::shared_ptr< imagehandling::TextureObject_2> bk_surf, const Coord& position, int opacity);
	void AddTexture(std::shared_ptr< imagehandling::TextureObject_2> bk_surf, int opacity);
	void AddTexture( const imagehandling::IMGToSurface* bk_surf, int opacity );
	void AddTexture(const imagehandling::IMGToSurface* bk_surf, const Coord& position, int opacity);

	SDL_Rect GetDimension() const { return this->pDimension; };
	void SetDimension( const SDL_Rect& set ) { this->pDimension = set; };

	void SetAllTextureHasSameDimension(bool set) { this->pAllTexturesHaveSameDimensionAsBase = set; }

	void ResetTextures();

	~ScreenBackground();

private:

	SDL_Rect pDimension;
	bool pAllTexturesHaveSameDimensionAsBase;

	unsigned int pBaseDistance;

	std::vector<CoordWithObject<std::shared_ptr<imagehandling::TextureObject_2>>> pLayeredTexture;

	ScreenBackground( const ScreenBackground& );
	ScreenBackground( ScreenBackground&& );
	ScreenBackground operator=( const ScreenBackground& );
	ScreenBackground operator=( ScreenBackground&& );


};	
