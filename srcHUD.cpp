#include"scrHUD.h"
#include"StringBuilder.h"
#include"ScreenButton.h"
#include"GameManager.h"
#include"StringBuilder.h"
#include"BackgroundCollection.h"

string PlayerHUD::pShipPositionTxt = "Ship position: ";
string PlayerHUD::pShipVelocityTxt = "Ship velocity: ";
string PlayerHUD::pShipHeadingTxt = "Ship heading: ";
string PlayerHUD::pLocationText = "Location name: ";

PlayerHUD::PlayerHUD( int screen_height, int screen_width, Coord pos )
{
	screenPosition = pos;

	pScreenWidth = screen_width;
	pScreenHeight = screen_height;

	//int spacing = screen_height/10;
	int x_coord = 64;
	int y_coord = 16;
	int rect_h = STANDARD_BUTTON_HEIGHT;
	int rect_w = 171; //9 * 19

	int y_shift = 0;
	int x_shift = 0;

	auto menu = std::shared_ptr<Widget>(new ScreenButton( Coord(x_coord+pos.y+x_shift, y_coord+pos.x+y_shift),  4*13, 28, BtnText("Menu")));
	menu->OnWidgetClicked = [](Widget*) {GameManager::Screens.openPlayerMenu(); };
	this->Control()->AddWidget(menu);
	x_coord += 60; 

	auto inventory = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord+pos.y+x_shift, y_coord+pos.x+y_shift), 9*9, 28 , BtnText("Inventory")));
	inventory->OnWidgetClicked = [](Widget*) {GameManager::Screens.openPlayerInventory(); };
	this->Control()->AddWidget(inventory);

	x_coord += 89;
	auto map = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord+pos.y+x_shift, y_coord+pos.x+y_shift), 9*9, 28 , BtnText("Starmap")));
	map->OnWidgetClicked = [](Widget*) {GameManager::Screens.openCampaign(); };
	this->Control()->AddWidget(map); 

	x_coord += 89;
	auto shop = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord + pos.y + x_shift, y_coord + pos.x + y_shift), 4 * 13, 28, BtnText("Shop")));
	shop->OnWidgetClicked = [](Widget*) {GameManager::Screens.openShopScreen(false); };
	this->Control()->AddWidget(shop);

	auto hudexpandRight = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord + 945 - 293 + pos.y + x_shift, pos.y + 500 +162), ImportImage_HUDExpandButton, "hudrightexpand"));
	hudexpandRight->OnWidgetClick = [&](Widget*) 
	{ 
		if(this->RightExpandButtonClicked)
			this->RightExpandButtonClicked(); 
	};
	this->Control()->AddWidget(hudexpandRight);

	auto hudexpandLeft = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord - 4 + pos.y + x_shift, pos.y + 500 + 162), ImportImage_HUDExpandButton, "hudleftexpand"));
	hudexpandLeft->OnWidgetClick = [&](Widget*) 
	{
		if(this->LeftExpandButtonClicked)
			this->LeftExpandButtonClicked();
	};
	this->Control()->AddWidget(hudexpandLeft);

	pShipPositionResult = PlayerHUD::pShipPositionTxt;
	pShipVelocityResult = PlayerHUD::pShipVelocityTxt;
	pShipHeadingResult = PlayerHUD::pShipHeadingTxt;
	pLocationTextResult = PlayerHUD::pLocationText;

	pShipPositionLink = std::shared_ptr<Widget>( new Link( Text(PlayerHUD::pShipPositionTxt), 14, Coord(pScreenWidth-256,56+56+28), 24, 64+16 ));
	pShipPositionLink.get()->SetWidgetMode( WIDGET_MODE::WL_NON_INTERACTIVE );
	this->Control()->AddWidget( pShipPositionLink );

	pShipVelocityLink = std::shared_ptr<Widget>( new Link( Text(PlayerHUD::pShipVelocityTxt), 14, Coord(pScreenWidth-256,56+56), 24, 64+16 ));
	pShipVelocityLink.get()->SetWidgetMode( WIDGET_MODE::WL_NON_INTERACTIVE );
	this->Control()->AddWidget( pShipVelocityLink );

	pShipHeadingLink = std::shared_ptr<Widget>( new Link( Text(PlayerHUD::pShipHeadingTxt), 14, Coord(pScreenWidth-256,56+28), 24, 64+16 ));
	pShipHeadingLink.get()->SetWidgetMode( WIDGET_MODE::WL_NON_INTERACTIVE );
	this->Control()->AddWidget( pShipHeadingLink );

	pLocationLink = std::shared_ptr<Widget>(new Link( Text(PlayerHUD::pLocationText), 14, Coord(pScreenWidth - 256, 56), 24, 64 + 16));
	pLocationLink.get()->SetWidgetMode(WIDGET_MODE::WL_NON_INTERACTIVE);
	this->Control()->AddWidget(pLocationLink);

	pGameSpeed = std::shared_ptr<Link>(new Link(Text("Average loop time:"), 14, Coord(pScreenWidth - 256, 28), 28, 128));
	pGameSpeed.get()->SetWidgetMode(WIDGET_MODE::WL_NON_INTERACTIVE);
	this->Control()->AddWidget(pGameSpeed);

	this->pStretchBackgroundTextures = false;
	this->pBackground.SetAllTextureHasSameDimension(false);
	this->SetBackground(ImportImage_HUDBottom, 255);
	this->AddBackgroundLayer(ImportImage_HUDTop, Coord(0,-35), 200);
}

void PlayerHUD::render( SDL_Renderer& renderer )
{
	RenderBackground(renderer, Coord(this->screenPosition.x, this->screenPosition.y + this->pScreenHeight - 64 + 36));

	for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
	{
		std::string txt = (*it)->ID();
		if (txt == PlayerHUD::pShipPositionTxt)
			RenderShipPositionTxt(renderer);
		else if (txt == PlayerHUD::pShipVelocityTxt)
			RenderShipVelocityTxt(renderer);
		else if (txt == PlayerHUD::pShipHeadingTxt)
			RenderShipHeadingTxt(renderer);
		else if (txt == PlayerHUD::pLocationText)
			RenderLocationTxt(renderer);
			
		(*it)->Render(renderer);
	}

	double averageTick = GameManager::CurrentInstanceManager()->GetIntegrator()->GetAverageTick();
	this->pGameSpeed->RefreshText("Average loop time: " + DoubleToString(averageTick,2));

	//this->pTargetList.render( renderer );
	//this->pLauncherList.render( renderer );

	// Indicate that this is the active screen
	//this->Control()->SetScreenState(SCREEN_ACTIVE, false);
}

void PlayerHUD::RenderLocationTxt(SDL_Renderer& renderer)
{
	this->pLocationTextResult = PlayerHUD::pLocationText + GameManager::CurrentLocationName();

	Link* linkLocation = ((Link*)this->pLocationLink.get());
	linkLocation->RenewText(this->pLocationTextResult.c_str());
}

void PlayerHUD::RenderShipHeadingTxt( SDL_Renderer& renderer )
{
	this->pShipHeadingResult = PlayerHUD::pShipHeadingTxt + DoubleToString(GameManager::CurrentInstanceManager()->GetCurrentPlayer()->AsUnitEntity()->getGlobalHeading(),2);

	Link* linkShipHeading = ((Link*)this->pShipHeadingLink.get());
	//Screen::RenderLetter(pShipHeadingResult, linkShipHeading->GetPosition().xadd(10), renderer); //comment out because GetLetterTexture was slow
	//linkShipHeading->RenewText( this->pShipHeadingResult.c_str() );
}

void PlayerHUD::RenderShipVelocityTxt( SDL_Renderer& renderer )
{
	this->pShipVelocityResult = PlayerHUD::pShipVelocityTxt + IntToString(GameManager::CurrentInstanceManager()->GetCurrentPlayer()->AsUnitEntity()->getVelocity().ABS());

	Link* linkShipVelocity = ((Link*)this->pShipVelocityLink.get());
	//Screen::RenderLetter(pShipVelocityResult, linkShipVelocity->GetPosition().xadd(10), renderer); //comment out because GetLetterTexture was slow
	//linkShipVelocity->RenewText( this->pShipVelocityResult.c_str() ); 
}

void PlayerHUD::RenderShipPositionTxt( SDL_Renderer& renderer )
{
	this->pShipPositionResult = PlayerHUD::pShipPositionTxt + GameManager::CurrentInstanceManager()->GetCurrentPlayer()->AsUnitEntity()->getPosition().ToString();

	Link* linkShipPosition = ((Link*)this->pShipPositionLink.get());
	//Screen::RenderLetter(pShipPositionResult, linkShipPosition->GetPosition().xadd(10), renderer); //comment out because GetLetterTexture was slow

	//linkShipPosition->RenewText( this->pShipPositionResult.c_str() );
}

void PlayerHUD::ResetHUDForInstance()
{
}

void PlayerHUD::HandleEvent( SDL_Event& e)
{
	Screen::HandleEvent( e );
	//this->pTargetList.HandleEvent( e );
	//this->pLauncherList.HandleEvent( e );
}

//void PlayerHUD::HandleActiveWidget( SDL_Event& e, Widget* widget )
//{
//	Screen::HandleActiveWidget(e,widget);
//}