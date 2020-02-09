#include"DynamicGridBox.h"
#include"GridBoxTextureCollection.h"
#include"BorderGraphicHandler.h"
#include"WhiteBorderPoints.h"

DynamicGridBox::DynamicGridBox() : GridBox( GBTextureCollection::ImportGrid_Standard.get() )  , pInvalidTxt(nullptr)
{
	//this->pStandardTxt.get()->SetCropBase( GBTextureCollection::ImportGrid_Standard );
	Init();
}

DynamicGridBox::DynamicGridBox(const imagehandling::IMGToSurface* surf ) : GridBox( surf ) , pInvalidTxt(nullptr)
{
	Init();
}

DynamicGridBox::DynamicGridBox( const DynamicGridBox& other ) : GridBox(other), 
	pHighlightTxt( new imagehandling::TextureObject_2( *other.pHighlightTxt.get() ) ),
	pInvalidTxt( new imagehandling::TextureObject_2( *other.pInvalidTxt.get() ) )
{
}

DynamicGridBox::DynamicGridBox( DynamicGridBox&& other ) : GridBox(other), 
	pHighlightTxt( std::move( other.pHighlightTxt ) ),
	pInvalidTxt( std::move( other.pInvalidTxt ) )
{
}

DynamicGridBox::DynamicGridBox(const ColorRGB& col) : GridBox( col ) , pInvalidTxt(nullptr)
{
	Init();
	//setHighlightTexture( &imagehandling::whiteParticle );
}


void DynamicGridBox::Init()
{
	State = enBoxState::STANDARD;
	auto highlightTexture = new imagehandling::TextureObject_2(*this->pStandardTxt.get());
	highlightTexture->setModulation(imagehandling::enumTextureModulation::HIGHMOD);
	pHighlightTxt.reset(highlightTexture);
	UpdateInvalidTexture();
}

DynamicGridBox & DynamicGridBox::operator=( const DynamicGridBox& other)
{
	(GridBox)*this = (GridBox)other;
	this->pHighlightTxt.reset( new imagehandling::TextureObject_2( *other.pHighlightTxt.get() ) );
	this->pInvalidTxt.reset( new imagehandling::TextureObject_2( *other.pInvalidTxt.get() ) );
	return *this;
}
DynamicGridBox & DynamicGridBox::operator=( DynamicGridBox&& other)
{	
	(GridBox)*this = (GridBox)other;
	this->pHighlightTxt = std::move(other.pHighlightTxt);
	this->pInvalidTxt = std::move(other.pInvalidTxt);
	return *this;
}

SDL_Texture* DynamicGridBox::GetTextureByState(SDL_Renderer* renderer)
{
	SDL_Texture* result = NULL;
	
	switch( this->State )
	{
	case( enBoxState::STANDARD ) :
		result = this->getStandardTexture(renderer);
		break;
	case( enBoxState::HIGHLIGHTED ) :
		result = this->getHighlightTexture(renderer);
		break;
	case( enBoxState::INVALID ) :
		result = this->getInvalidTexture(renderer);
		break;
	default:
		result = this->getStandardTexture(renderer);
		break;
	};
	
	return result;	
}

SDL_Texture* DynamicGridBox::getStandardTexture( SDL_Renderer* renderer )
{
	bool needsRefresh = false;
	if( pStandardTxt->getTexture() == nullptr )
		needsRefresh = true;

	SDL_Texture* result = pStandardTxt->getTexture(renderer);

	if( needsRefresh && result != nullptr )
	{
		UpdateInvalidTexture();
		//also possibly update highlighttexture
	}
	return result;
}

SDL_Texture* DynamicGridBox::getHighlightTexture(SDL_Renderer* renderer) 
{ 
	return pHighlightTxt->getTexture(renderer); 
}

SDL_Texture* DynamicGridBox::getInvalidTexture(SDL_Renderer* renderer) 
{
	return pInvalidTxt->getTexture(renderer); 
}

void DynamicGridBox::setHighlightTexture(const imagehandling::IMGToSurface* surf)
{
	pHighlightTxt.reset( new imagehandling::TextureObject_2( surf ) );
}

void DynamicGridBox::UpdateInvalidTexture()
{
	//is code run often????????????? check this
	pInvalidTxt.reset( new imagehandling::TextureObject_2( *this->pStandardTxt.get() ));
	pInvalidTxt->setModulation(imagehandling::enumTextureModulation::REDMOD);
}

void DynamicGridBox::Refresh()
{
	this->pStandardTxt.get()->ResetTexture();
	this->pHighlightTxt.get()->ResetTexture();
	this->pInvalidTxt.get()->ResetTexture();
}