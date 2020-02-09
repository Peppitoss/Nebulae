#include"scrGridObjectSelector.h"
#include"SurfaceButton.h"
#include"SelectLink.h"
#include"ModuleInterfaceGlobalList.h"
#include"GameManager.h"




GridObjectSelector::GridObjectSelector() : ListFrame()
{
	init();
}

GridObjectSelector::GridObjectSelector( Coord dimensions, Coord pos ) : ListFrame( dimensions, pos )
{
	Setup(dimensions, pos);
	init();
}

void GridObjectSelector::Setup( Coord dimensions, Coord pos )
{
	Screen::SetBackground("256x256GreyBack.png", EnumColorKey::kCYAN );
	SetupBarPlacement( pos.y, pos.y + dimensions.y );
	ListFrame::AddList();
	this->AddScrollBars();
}

void GridObjectSelector::AddList(const Coord& pos)
{
	std::list<string> nameList = ModuleGlobalList::GetFilteredNameList();
	for( auto name = nameList.begin(); name != nameList.end(); ++name)
	{
		AddItemToList(*name, Coord(0));
	}
}

void GridObjectSelector::init()
{
	this->SelectedGridObjectName = "";
}

void GridObjectSelector::HandleActiveWidget( SDL_Event& e, Widget* widget )
{
	ListFrame::HandleActiveWidget(e,widget);

	std::string txt = widget->ID();
	
	if(SelectLink *button = dynamic_cast<SelectLink*>(widget))
	{
		if( SelectedGridObjectName != txt  && VisibleInsideWindow( button->GetPosition().y ) )
		{
			SelectedGridObjectName = txt;
			GameManager::loadModuleBuilder( txt );
		}
	}
	else
	{
	}
}

std::string GridObjectSelector::GetSelectedGridObjectName()
{
	return this->SelectedGridObjectName;
}