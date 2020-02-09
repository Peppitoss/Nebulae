#include"TextureObject.h"
#include"Wrapper.h"
#include"Graphics.h"
#include"Debug.h"

namespace imagehandling
{

	TextureObject_2::TextureObject_2() : pTexture(nullptr, Debug::DestroyTexture)
	{
		Initialize();
	}
	TextureObject_2::TextureObject_2(const imagehandling::IMGToSurface* surface) : pSurface(new imagehandling::IMGToSurface(*surface)), pTexture(nullptr, Debug::DestroyTexture)
	{
		Initialize();
	}

	TextureObject_2::TextureObject_2(const imagehandling::IMGToSurface& surface) : pSurface(new imagehandling::IMGToSurface(surface)), pTexture(nullptr, Debug::DestroyTexture)
	{
		Initialize();
	}

	TextureObject_2::TextureObject_2(const TextureObject_2& other) : pSurface(new imagehandling::IMGToSurface(*other.pSurface.get())), pTexture(nullptr, Debug::DestroyTexture)
	{
		this->modulationFlag = other.modulationFlag;
		this->surfaceClipping = other.surfaceClipping;
		this->pNeedsRefresh = other.pNeedsRefresh;
		this->pNeedsClipping = other.pNeedsClipping;
		//this->pCanvasLayer = other.pCanvasLayer; //correct??
		this->pSpriteBase = other.pSpriteBase;
		this->pCustomModulation = other.pCustomModulation;
		this->pBaseOpacity = other.pBaseOpacity;
		this->pFlipOnDraw = other.pFlipOnDraw;
		this->pAllowTextureFiltering = other.pAllowTextureFiltering;
	}

	TextureObject_2::TextureObject_2(TextureObject_2&& other) : pSurface(std::move(other.pSurface)), pTexture(nullptr, Debug::DestroyTexture)
	{
		this->modulationFlag = std::move(other.modulationFlag);
		this->surfaceClipping = std::move(other.surfaceClipping);
		//this->pCanvasLayer = std::move(other.pCanvasLayer);
		this->pNeedsRefresh = std::move(other.pNeedsRefresh);
		this->pNeedsClipping = std::move(other.pNeedsClipping);
		this->pCustomModulation = std::move(other.pCustomModulation);
		this->pBaseOpacity = std::move(other.pBaseOpacity);
		this->pFlipOnDraw = std::move(other.pFlipOnDraw);
		this->pAllowTextureFiltering = std::move(other.pAllowTextureFiltering);
	}

	TextureObject_2 & TextureObject_2::operator=(const TextureObject_2& other)
	{
		this->pTexture.reset();
		this->pSurface.reset(new imagehandling::IMGToSurface(*other.pSurface.get()));
		//this->pCanvasLayer.reset( new imagehandling::IMGToSurface( std::vector<CoordWithColor>(), this->getDimension() ) );
		this->modulationFlag = other.modulationFlag;
		this->pSpriteBase = other.pSpriteBase;
		this->pCustomModulation = other.pCustomModulation;
		this->pBaseOpacity = other.pBaseOpacity;
		this->pFlipOnDraw = other.pFlipOnDraw;
		this->pAllowTextureFiltering = other.pAllowTextureFiltering;
		//swap(*this,other);
		return *this;
	}

	TextureObject_2 & TextureObject_2::operator=(TextureObject_2&& other)
	{
		this->pTexture.reset();
		this->pSurface = std::move(other.pSurface); //.reset( new imagehandling::IMGToSurface( other.pSurface.get() ) );
													//this->pCanvasLayer = std::move( other.pCanvasLayer );
		this->modulationFlag = std::move(other.modulationFlag);
		this->pSpriteBase = std::move(other.pSpriteBase);
		this->pCustomModulation = std::move(other.pCustomModulation);
		this->pBaseOpacity = std::move(other.pBaseOpacity);
		this->pFlipOnDraw = std::move(other.pFlipOnDraw);
		this->pAllowTextureFiltering = std::move(other.pAllowTextureFiltering);
		//swap(*this,other);
		return *this;
	}

	//void TextureObject_2::RenderTexture(SDL_Renderer* renderer, const Coord& atLocation) 
	//{
	//	this->RenderTexture(renderer, atLocation);
	//}

	void TextureObject_2::RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle) 
	{
		if (angle == 0)
			RenderTexture(renderer, atLocation);
		this->RenderTexture(renderer, atLocation, angle, this->GetDimension().Center());
	}

	void TextureObject_2::RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle, const Coord& rotationAxis ) 
	{
		if (angle == 0)
			RenderTexture(renderer, atLocation);

		SDL_RendererFlip flip = this->GetFlipOnDraw() ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE;
		this->RenderTexture(renderer, atLocation, angle, this->GetDimension().Center(), flip);
	}

	void TextureObject_2::RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, double angle, const Coord& rotationAxis, SDL_RendererFlip flip) 
	{
		int w, h;
		SDL_QueryTexture(this->getTexture(renderer), NULL, NULL, &w, &h); 
		
		//note that w and h of texture should also equal to pSize.x + 2*this->pScrRect.x, and pSize.y + 2*this->pScrRect.x

		SDL_Rect renderClip = { 0, 0, w, h };
		this->RenderTexture(renderer, atLocation, renderClip, angle, this->GetDimension().Center(), flip);
	}

	void TextureObject_2::RenderTexture(SDL_Renderer* renderer, const Coord& atLocation, const SDL_Rect& clip, double angle, const Coord& rotationAxis, SDL_RendererFlip flip) 
	{
		auto texture = this->getTexture(renderer);
		SDL_Point axisPoint = { rotationAxis.x, rotationAxis.y };
		angle = geometry::radToDegrees(angle)*-1;

		//SDL_Rect src = { this->pScrRect.x , this->pScrRect.y, this->GetDimension().x, GetDimension().y };

		SDL_Rect renderClip = { clip.x , clip.y , clip.w, clip.h };
		SDL_Rect renderQuad = { atLocation.x - this->pScrRect.x, atLocation.y - this->pScrRect.y, clip.w, clip.h };



		if(pAllowTextureFiltering)
			SDL_RenderCopyEx(renderer, texture, &renderClip, &renderQuad, angle, &axisPoint, flip);
		else
		{
			char interpolation = *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
			auto res = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
			SDL_RenderCopyEx(renderer, texture, &renderClip, &renderQuad, angle, &axisPoint, flip);
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &interpolation);
		}
		Debug::SDLErrorCheck();
	}

	void TextureObject_2::RenderTexture(SDL_Renderer* renderer, const Coord& destination)
	{
		auto texture = this->getTexture(renderer);

		int w, h;
		SDL_QueryTexture(this->getTexture(renderer), NULL, NULL, &w, &h);
		SDL_Rect renderClip = { 0, 0, w, h };

		RenderTexture(renderer, renderClip, destination);
	}

	void TextureObject_2::RenderTexture(SDL_Renderer* renderer, const SDL_Rect& renderClip, const Coord& destination)
	{
		auto texture = this->getTexture(renderer);
		SDL_Rect renderQuad = { destination.x - this->pScrRect.x, destination.y - this->pScrRect.y, renderClip.w, renderClip.h };

		if (pAllowTextureFiltering)
			SDL_RenderCopy(renderer, texture, &renderClip, &renderQuad);
		else
		{
			char interpolation = *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
			auto res = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
			SDL_RenderCopy(renderer, texture, &renderClip, &renderQuad);
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &interpolation);
		}
		Debug::SDLErrorCheck();
	}

	void TextureObject_2::generateTexture(SDL_Renderer* renderer)
	{
		if (SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY) == nullptr || *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY) == '0')
		{
			this->pScrRect = { 0,0,0,0 };
		}
		else if (*SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY) == '1')
		{
			this->pScrRect = { 1,1,0,0 };
		}

		if (this->pNeedsClipping)
		{
			CropTexture(renderer, this->surfaceClipping);
			this->pNeedsClipping = false;
		}
		else
		{
			if (this->modulationFlag == imagehandling::HIGHMOD)
			{
				this->pNeedsRefresh = true;
				int intensity = 90;
				SDL_Surface* surfLighten = sdlWrapper.CreateSquare(ColorRGB(intensity, intensity, intensity).ToSDL_Color(), pSurface->GetRealSurfaceDimensions());
				SDL_Surface* highlightsurf = sdlWrapper.CreateSquare(ColorRGB(0, 0, 0, 0).ToSDL_Color(), pSurface->GetRealSurfaceDimensions());
				//SDL_SetColorKey(highlightsurf, 1, ColorRGB(0, 255, 255).ToSDL_Map(*highlightsurf->format));

				SDL_SetSurfaceBlendMode(surfLighten, SDL_BlendMode::SDL_BLENDMODE_ADD);
				Debug::SDLErrorCheck();
				SDL_BlitSurface(pSurface->GetSurface().get(), nullptr, highlightsurf, nullptr);
				Debug::SDLErrorCheck();
				//SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
				//SDL_FillRect(highlightsurf, nullptr, ColorRGB(20, 20, 20).ToSDL_Map(*highlightsurf->format));
				//SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_NONE);
				SDL_BlitSurface(surfLighten, nullptr, highlightsurf, nullptr);
				Debug::SDLErrorCheck();


				//Todo, this ignores pAllowFiltering!!!!!!!!!!!!!
				pTexture = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(SDL_CreateTextureFromSurface(renderer, highlightsurf), Debug::DestroyTexture);
				Debug::SDLErrorCheck();
			}
			else
			{
				//pTexture.reset();
				pTexture = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(pSurface->GenerateTextureFromSurface(renderer, this->pAllowTextureFiltering), Debug::DestroyTexture);
			}

		}
	}

	void TextureObject_2::CropTexture(SDL_Renderer* renderer, const SDL_Rect& crop)
	{
		if (this->pSpriteBase.get() == nullptr)
			return;

		this->pSurface.get()->ReplaceSurface(this->pSpriteBase.get()->GenerateSurfaceFromSurface(crop), this->pSpriteBase.get()->get_colorKey());
		pTexture.reset(pSurface->GenerateTextureFromSurface(renderer, this->pAllowTextureFiltering));
	}

	void TextureObject_2::ResetTexture()
	{
		this->pNeedsRefresh = true;

		//if (this->pNeedsRefresh)
		//{
		//	this->pTexture.reset();
		//	this->pNeedsRefresh = false;
			//if (this->surfaceClipping.h*this->surfaceClipping.w > 0)	// Why is this placed here? don't auto clip!
			//	this->pNeedsClipping = true;
		//}
	}

	void TextureObject_2::PaintTextureAtCoord(Coord Coord, ColorRGB color)
	{
		this->pSurface->putpixel(Coord, color);
		this->pNeedsRefresh = true;
	}

	void TextureObject_2::PaintTextureAtCoords(const std::vector<CoordWithColor>& canvas)
	{
		this->pSurface->putpixel(canvas);
		this->pNeedsRefresh = true;
	}

	void TextureObject_2::PaintTextureAtCoords(const CoordWithColor canvas[], int  size)
	{
		this->pSurface->putpixel(canvas, size);
		this->pNeedsRefresh = true;
	}

	void TextureObject_2::PaintTextureAtCoords(const CoordWithCollisionData canvas[], int  size)
	{
		this->pSurface->putpixel(canvas, size);
		this->pNeedsRefresh = true;
	}

	void TextureObject_2::SetCropBase(std::shared_ptr<IMGToSurface> base)
	{
		this->pSpriteBase = base;
		if (this->pSurface.get() == false)
		{
			this->pSurface = std::unique_ptr<IMGToSurface>(new IMGToSurface());
		}
	}

	//SDL_Texture* TextureObject_2::getTexture( SDL_Renderer* renderer, const SDL_Rect& crop)
	//{
	//	if(!pTexture)
	//	{
	//		generateTexture(renderer,crop);
	//		ModulateTexture();
	//	}
	//	return getTexture();
	//}

	SDL_Texture* TextureObject_2::getTexture(SDL_Renderer* renderer)
	{
		if (!pTexture || this->pNeedsRefresh)
		{
			this->pTexture.reset();
			generateTexture(renderer);
			this->pNeedsRefresh = false;

			ModulateTexture();
			//setBlendMode(this->pBlendMode);
			int result = SDL_SetTextureAlphaMod(this->pTexture.get(), this->pBaseOpacity);
			Debug::SDLErrorCheck();


		}
		return getTexture();
	}

	void TextureObject_2::ModulateTexture()
	{
		switch (this->modulationFlag)
		{
		case imagehandling::NOMOD:
			DeModulate(this->getTexture());
			break;
		case imagehandling::REDMOD:
			Redlight(this->getTexture());
			break;
		case imagehandling::HIGHMOD:
			DeModulate(this->getTexture());
			break;
		case imagehandling::CUSTOMMOD:
			modulate(this->pCustomModulation);
			break;
		default:
			break;
		}
	}

	bool TextureObject_2::CompareClipping(const SDL_Rect& crop)
	{
		return (surfaceClipping.x == crop.x && surfaceClipping.y == crop.y && surfaceClipping.h == crop.h && surfaceClipping.w == crop.w);
	}

	void TextureObject_2::MakeUnique() //not tested :)
	{
		this->pSurface.get()->ReplaceSurface();
		this->pNeedsRefresh = true;
	}

	void TextureObject_2::SetSurfaceClip(const SDL_Rect& rect)
	{
		if (!CompareClipping(rect) && rect.h*rect.w > 0)
		{
			surfaceClipping = SDL_Rect(rect);
			pNeedsClipping = true;
			this->pNeedsRefresh = true;
		}
	}

	void TextureObject_2::IncrementFrameNumber()
	{
		int currentFrame = GetCurrentFrameNumber();
		if (this->pSpriteBase->getFrames() < currentFrame)
			ChangeFrame(currentFrame++);
	}

	int TextureObject_2::GetCurrentFrameNumber() const
	{
		enSurfaceFrameDimension baseFrameDim = this->pSpriteBase->GetSurfaceDimension();
		auto clipSize = this->pSpriteBase->GetSpriteSize();

		switch (baseFrameDim)
		{
		case(enSurfaceFrameDimension::SingleDimension):
			return 1;
			break;
		case(enSurfaceFrameDimension::VerticalFrames):
			return surfaceClipping.y / clipSize.height() + 1;
			break;
		case(enSurfaceFrameDimension::HorizontalFrames):
			return surfaceClipping.x / clipSize.width() + 1;
		case(enSurfaceFrameDimension::SquareFrames):
			throw new std::exception("Not implemented!");
			break;
		default:
			return 1;
			break;
		}
	}

	bool TextureObject_2::ChangeFrame(int frame)
	{
		int currentFrame = GetCurrentFrameNumber();
		if (frame == currentFrame)
			return false;

		enSurfaceFrameDimension baseFrameDim = this->pSpriteBase->GetSurfaceDimension();

		if (baseFrameDim != enSurfaceFrameDimension::SingleDimension &&
			baseFrameDim != enSurfaceFrameDimension::ArbitraryFrames)
		{
			int totFrames = this->pSpriteBase->getFrames();
			auto clipSize = this->pSpriteBase->GetSpriteSize();
			SDL_Rect clip = { 0,0,clipSize.width() ,clipSize.height() };

			switch (baseFrameDim)
			{
			case(enSurfaceFrameDimension::VerticalFrames):
				if (totFrames < frame)
					frame = totFrames;
				clip.y = clipSize.height()*(frame - 1);
				break;
			case(enSurfaceFrameDimension::HorizontalFrames):
				if (totFrames < frame)
					frame = totFrames;
				clip.x = clipSize.width()*(frame - 1);
				break;
			case(enSurfaceFrameDimension::SquareFrames):
				throw new std::exception("Not implemented!");
				break;
			default:
				break;
			}
			this->SetSurfaceClip(clip);
		}
		return true;

	}

	void TextureObject_2::Initialize()
	{
		this->modulationFlag = enumTextureModulation::CUSTOMMOD;
		this->surfaceClipping.w = 0;
		this->surfaceClipping.h = 0;
		this->pCustomModulation = ColorRGB(255, 255, 255);
		this->pNeedsClipping = false;
		this->pAllowTextureFiltering = true;
		this->pSpriteBase.reset();
		//this->pCanvasLayer.reset( new imagehandling::IMGToSurface( std::vector<CoordWithColor>(), this->getDimension() ) );
		this->pBaseOpacity = 255;
		this->pFlipOnDraw = SDL_RendererFlip::SDL_FLIP_NONE;
		this->pBlendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
	}

	void TextureObject_2::modulate(ColorRGB col)
	{
		SDL_SetTextureColorMod(this->getTexture(), col.red(), col.green(), col.blue());
		Debug::SDLErrorCheck();
	}

	void TextureObject_2::setBlendMode(SDL_BlendMode mode)
	{
		SDL_SetTextureBlendMode(this->getTexture(), mode);
		Debug::SDLErrorCheck();
		this->pBlendMode = mode;
	}

	void TextureObject_2::setModulation(enumTextureModulation set)
	{
		this->modulationFlag = set;

		switch (this->modulationFlag)
		{
		case imagehandling::HIGHMOD:
			this->pNeedsRefresh = true;
			break;
		}
	}

	void TextureObject_2::setOpacity(int value, bool changeBaseOpacity)
	{
		if (changeBaseOpacity)
			ChangeBaseOpacity(value);
		else
		{
			SDL_SetTextureAlphaMod(this->getTexture(), value);
			Debug::SDLErrorCheck();
		}
	}

	void TextureObject_2::ChangeBaseOpacity(int newValue)
	{
		if (this->pBaseOpacity == newValue)
			return;
		this->pBaseOpacity = newValue;
		this->pNeedsRefresh = true;
	}

	void TextureObject_2::SetCustomModulation(ColorRGB col)
	{
		if (this->pCustomModulation == col)
			return;
		else
		{
			this->pCustomModulation = col;
			this->pNeedsRefresh = true;
		}
	};

	void TextureObject_2::SetFlipOnDraw(bool set)
	{
		if (set)
			this->pFlipOnDraw = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		else
			this->pFlipOnDraw = SDL_RendererFlip::SDL_FLIP_NONE;
	};

	TextureObject_2::~TextureObject_2()
	{
		this->pTexture.reset();
		this->pSpriteBase.reset();
		this->pSurface.reset();
		//this->pCanvasLayer.reset();
	}

	SDL_Texture* Highlight(TextureObject_2* text)
	{
		if (text->getModulation() != enumTextureModulation::HIGHMOD)
		{
			//SDL_SetTextureColorMod(text->getTexture(),255,255,255);
			text->setModulation(enumTextureModulation::HIGHMOD);
		}
		return text->getTexture();
	}
	SDL_Texture* Redlight(TextureObject_2* text)
	{
		if (text->getModulation() != enumTextureModulation::REDMOD)
		{
			SDL_SetTextureColorMod(text->getTexture(), 100, 20, 20);
			Debug::SDLErrorCheck();
			text->setModulation(enumTextureModulation::REDMOD);
		}

		return text->getTexture();
	}
	SDL_Texture* DeModulate(TextureObject_2* text)
	{
		if (text->getModulation() != enumTextureModulation::NOMOD)
		{
			SDL_SetTextureColorMod(text->getTexture(), 255, 255, 255);
			Debug::SDLErrorCheck();
			text->setModulation(enumTextureModulation::NOMOD);
		}

		return text->getTexture();
	}

	//SDL_Texture* Highlight(SDL_Texture* txt)
	//{
	//	SDL_SetTextureColorMod(txt,255,255,255);
	//	return txt;
	//}
	SDL_Texture* Redlight(SDL_Texture* txt)
	{
		SDL_SetTextureColorMod(txt, 200, 20, 20);
		Debug::SDLErrorCheck();
		return txt;
	}
	SDL_Texture* DeModulate(SDL_Texture* txt)
	{
		SDL_SetTextureColorMod(txt, 255, 255, 255);
		Debug::SDLErrorCheck();
		return txt;
	}

}