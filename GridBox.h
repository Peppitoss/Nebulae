#pragma once
#include"TextureObject.h"
#include"GridBoxTextureCollection.h"
#include<list>

enum enBoxState
{
	STANDARD = 0,
	HIGHLIGHTED,
	HIDE,
	INVALID,
	UNKNOWN //continue here

};

class GridBox
{

public:
	
	GridBox();
	GridBox( const GridBox& );
	GridBox( GridBox&& );
	//GridBox(imagehandling::IMGToSurface* );
	GridBox(const imagehandling::IMGToSurface* );
	GridBox(const ColorRGB& );

	virtual void setState( enBoxState set);
	enBoxState getBoxState() { return this->State; };
	static const int BOXSIZE = 32; //change to 32??
	Coord position;
	virtual SDL_Texture* getStandardTexture( SDL_Renderer* renderer ) { return pStandardTxt->getTexture(renderer); };
	virtual SDL_Texture* getIconTexture( SDL_Renderer* renderer ) { return pIconTxt->getTexture(renderer); };

	bool IsBoxRedFlagged() { return this->pRedFlag; };
	void SetBoxRedFlag( bool set ) { this->pRedFlag = set; };

	GridBox & operator=( const GridBox& );
	GridBox & operator=( GridBox&& );

	void RefreshColor(const ColorRGB&);
	void Modulate(const ColorRGB&);

	virtual void OverlayIcon( const imagehandling::IMGToSurface* icon );
	virtual void RemoveIcon();

	bool ShowIcon() { return this->pShowIcon; };

	~GridBox() { pStandardTxt.reset(); };

protected:

	bool pRedFlag; //used to tag the box, doesn't implicitly affect the texture of the box, (often used in if(....) sentences ), when invalid and destroyed it has red flag, + more
	enBoxState State;
	std::unique_ptr<imagehandling::TextureObject_2> pStandardTxt;

	virtual void setStandardTexture(const imagehandling::IMGToSurface*);
	virtual void setStandardTexture(const ColorRGB&  );

	void init();
	
private:

	bool pShowIcon;

	std::shared_ptr<imagehandling::TextureObject_2> pIconTxt;

};
