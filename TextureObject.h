#pragma once
#include <SDL.h>
#include"IMGToSurface.h" 

namespace imagehandling
{

	enum enumTextureModulation
	{
		NOMOD = 0,
		REDMOD,
		HIGHMOD,
		CUSTOMMOD,
	};

	class ITextureObjectRenderer
	{
	public:
		void virtual RenderTexture(SDL_Renderer*, const Coord& atLocation) = 0;
		void virtual RenderTexture(SDL_Renderer* renderer, const SDL_Rect&renderClip, const Coord& destination) = 0;

		void virtual RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle) = 0;
		void virtual RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle, const Coord& rotationAxis) = 0;
		void virtual RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle, const Coord& rotationAxis, SDL_RendererFlip flip) = 0;
		void virtual RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, const SDL_Rect& clip, double angle, const Coord& rotationAxis, SDL_RendererFlip flip) = 0;
	};

	class TextureObject_2 : public ITextureObjectRenderer
	{
	public:
		TextureObject_2();
		TextureObject_2(const TextureObject_2&);
		TextureObject_2(TextureObject_2&& other);
		TextureObject_2(const imagehandling::IMGToSurface*);
		TextureObject_2(const imagehandling::IMGToSurface& surface);
		//TextureObject_2( imagehandling::IMGToSurface* );

		virtual void generateTexture(SDL_Renderer*);
		void RenderTexture(SDL_Renderer*, const Coord& atLocation) ;
		//void RenderTexture(SDL_Renderer* renderer, const SDL_Rect& destination);
		void RenderTexture(SDL_Renderer* renderer, const SDL_Rect&renderClip, const Coord& destination);

		void RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle);
		void RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle, const Coord& rotationAxis);
		void RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle, const Coord& rotationAxis, SDL_RendererFlip flip);
		void RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, const SDL_Rect& clip, double angle, const Coord& rotationAxis, SDL_RendererFlip flip);
		
		


		int getSize() const { return this->pSurface->getSize(); };
		spriteSize GetDimension() const { return this->pSurface->GetSpriteSize(); };
		EnumColorKey getColorKey() const { return pSurface->get_colorKey(); };

		SDL_Texture* getTexture(SDL_Renderer* renderer); //guarantee a texture
		SDL_Texture* getTexture() { return pTexture.get(); };

		void ResetTexture();

		enumTextureModulation getModulation() { return modulationFlag; }
		void setModulation(enumTextureModulation set);
		void setOpacity(int value, bool changeBaseOpacity);
		void setBlendMode(SDL_BlendMode mode);
		void modulate(ColorRGB);

		void PaintTextureAtCoord(Coord, ColorRGB);
		void PaintTextureAtCoords(const std::vector<CoordWithColor>& canvas);
		void PaintTextureAtCoords(const CoordWithColor canvas[], int  size);
		void PaintTextureAtCoords(const CoordWithCollisionData canvas[], int  size);
		void SetCropBase(std::shared_ptr<IMGToSurface>);
		Coord GetAxisOfRotation() const { return this->pSurface->getAxisOfRotation(); }

		TextureObject_2 & operator=(const TextureObject_2&);
		TextureObject_2 & operator=(TextureObject_2&&);

		void SetSurfaceClip(const SDL_Rect&);
		int GetFrames() const { return this->pSpriteBase->getFrames(); };
		bool ChangeFrame(int frame);
		void IncrementFrameNumber();
		int GetCurrentFrameNumber() const;
		void MakeUnique(); // not tested

		ColorRGB GetCustomModulation() const { return this->pCustomModulation; };
		void SetCustomModulation(ColorRGB col);

		void SetFlipOnDraw(bool set);
		SDL_RendererFlip GetFlipOnDraw() const  { return this->pFlipOnDraw; };

		enSurfaceFrameDimension GetFrameDimension() const { return this->pSurface->GetSurfaceDimension(); };

		SDL_Rect SrcRect();
		//friend void swap(TextureObject_2& first, TextureObject_2& second);
		~TextureObject_2();

	protected:

		const imagehandling::IMGToSurface* getSDLImageToSurface() const { return pSurface.get(); };
		enumTextureModulation modulationFlag;
		SDL_Rect surfaceClipping;
		bool pAllowTextureFiltering;

		virtual void CropTexture(SDL_Renderer*, const SDL_Rect& crop);

	private:

		bool pNeedsRefresh;
		bool pNeedsClipping;
		SDL_RendererFlip pFlipOnDraw;
		ColorRGB pCustomModulation;
		SDL_BlendMode pBlendMode;
		SDL_Rect pScrRect;

		void ChangeBaseOpacity(int newValue);

		std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> pTexture;
		std::unique_ptr<imagehandling::IMGToSurface> pSurface;
		std::shared_ptr<imagehandling::IMGToSurface> pSpriteBase;
		//std::shared_ptr<imagehandling::IMGToSurface> pCanvasLayer; //make a merge function, with canvas and layer

		bool CompareClipping(const SDL_Rect& crop);

		//TextureObject_2( const TextureObject_2 && );
		//TextureObject_2 & operator=( const TextureObject_2 && );

		int pBaseOpacity;

		void ModulateTexture();

		void Initialize();
	};

	SDL_Texture* Highlight(TextureObject_2* text);
	SDL_Texture* Redlight(TextureObject_2* text);
	SDL_Texture* DeModulate(TextureObject_2* text);

	//SDL_Texture* Highlight(SDL_Texture* text);
	SDL_Texture* Redlight(SDL_Texture* text);
	SDL_Texture* DeModulate(SDL_Texture* text);
}