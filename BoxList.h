#pragma once
#include<list>
#include"Coord.h"
#include"SpriteSize.h"
#include"BorderData.h"

//very simple class to generate list of coordinate points. All points in the list generated overlap, hence
//the list may include dublicate coordinates. The BoxList is meant as a temporary class used to generate GridObjects effectively
//it is not optimized in any way or contains any "smart" algorithms 





class BoxList
	{
	public:

		BoxList();
		BoxList( const BoxList& );
		BoxList( int width, int height );

		void addBox( Coord pos ) { list.push_back( pos ); };

		void copyList(const std::list<Coord>&, const Coord& relativePosition );
		void copyList(const std::vector<Coord>&, const Coord& relativePosition );
		void generateSquare( int size );
		void generateRectangle( int w, int h );

		Coord getLowestPosition() const { return this->lowestPosition; };
		Coord getHighestPosition() const { return Coord( lowestPosition.x + pWidth-1, lowestPosition.y + pHeight-1 ); };
		const std::list<Coord>& getList() const { return this->list; };
		//std::list<CoordWithColor> getList( ColorRGB col ) const;

		int getHeight() const { return pHeight; };
		int getWidth() const { return pWidth; };

		spriteSize GetSize() const;
		
		///  std::map<Coord,EdgeBox*>::const_iterator const begin() const { return flatList.begin(); }
		bool isEdge( const Coord& ) const;
		bool isCorner( const Coord& ) const;
		bool isEmpty() { return pWidth*pHeight == 0; };

		std::list<Coord>::const_iterator const begin() const { return this->list.begin(); };
		std::list<Coord>::const_iterator const end() const { return this->list.end(); };

		Coord  GetCenter() const;
		BorderData GetBorderFromCoord( const Coord& , bool edgePositive ) const;

		void Translate( const Coord& );

		void Clear();

	private:

		int pWidth;
		int pHeight;
		Coord lowestPosition;
		
		bool compare_boxes(const Coord& first, const Coord& second);
		std::list<Coord> list;
		void initialize();

	};
