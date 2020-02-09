#include"GridBox.h"
#include"PointColor.h"

GridBox::GridBox() : pStandardTxt( nullptr )
{
	init();
}

GridBox::GridBox( const GridBox& other ) : pStandardTxt( new imagehandling::TextureObject_2( *other.pStandardTxt.get() ) ), pIconTxt( new imagehandling::TextureObject_2( *other.pIconTxt.get() ) )
{
	this->State = other.State;
	this->pRedFlag = other.pRedFlag;
	this->pShowIcon = other.pShowIcon;
}

GridBox::GridBox( GridBox&& other ) : pStandardTxt( std::move( other.pStandardTxt ) ), pIconTxt( std::move( other.pIconTxt ) )
{
	this->State = std::move(other.State);
	this->pRedFlag = std::move(other.pRedFlag);
	this->pShowIcon = std::move(other.pShowIcon);
}

GridBox::GridBox(const ColorRGB& col ) : pStandardTxt( new PointColor( col ) )
{
}

//GridBox::GridBox(imagehandling::IMGToSurface* surf ) : pStandardTxt( new imagehandling::TextureObject_2( surf ) )
//{
//	init();
//}

GridBox::GridBox(const imagehandling::IMGToSurface* surf) : pStandardTxt( new imagehandling::TextureObject_2( surf ) )
{
	init();
}

void GridBox::init()
{
	State = enBoxState::STANDARD;
	pRedFlag = false;
	pShowIcon = false;
}

GridBox & GridBox::operator=( const GridBox& other)
{
	this->State = other.State;
	this->pRedFlag = other.pRedFlag;
	this->pStandardTxt.reset( new imagehandling::TextureObject_2( *other.pStandardTxt.get() ) );
	this->pIconTxt.reset( new imagehandling::TextureObject_2( *other.pIconTxt.get() ) );
	this->pShowIcon = other.pShowIcon;
	return *this;
}
GridBox & GridBox::operator=( GridBox&& other)
{	
	this->State = std::move(other.State);
	this->pRedFlag = std::move(other.pRedFlag);
	this->pStandardTxt = std::move(other.pStandardTxt);
	this->pIconTxt = std::move( other.pIconTxt );
	this->pShowIcon = std::move( other.pShowIcon );
	return *this;
}

void GridBox::setStandardTexture(const imagehandling::IMGToSurface* surf)
{
	this->pStandardTxt.reset( new imagehandling::TextureObject_2( surf ) );
}
void GridBox::setStandardTexture(const ColorRGB& col )
{
	this->pStandardTxt.reset( new PointColor( col ) );
}

void GridBox::Modulate(const ColorRGB& col)
{
	this->pStandardTxt.get()->modulate(col);
}

void GridBox::RefreshColor(const ColorRGB& col)
{
	((PointColor*)this->pStandardTxt.get())->SetColor(col); //.reset( new PointColor( col ) );
}

void GridBox::OverlayIcon( const imagehandling::IMGToSurface* icon )
{
	if( pShowIcon == false && icon->GetSpriteSize().GetDimension() == spriteSize::GetSize16().GetDimension() )
	{
		this->pIconTxt.reset( new imagehandling::TextureObject_2( icon ) );
		pShowIcon = true;
	}
}
void GridBox::RemoveIcon()
{
	pShowIcon = false;
}

void GridBox::setState( enBoxState set)
{
	switch (set)
	{
	case STANDARD:
		this->State = set;
		break;
	case HIGHLIGHTED:
		this->State = set;
		break;
	case HIDE:
		this->State = set;
		break;
	case INVALID:
		this->State = set;
		break;
	case UNKNOWN:
		this->State = set;
		break;
	default:
		break;
	}

}
