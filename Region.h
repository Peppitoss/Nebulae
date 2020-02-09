#pragma once
#include"Shapes.h"
#include<string>

class Region
{
public:

	Region();
	Region(std::shared_ptr<Shapes::Shape> shape);
	Region(int x, int y, int w, int h);
	Region(const Coord& pos, const Coord& size);

	bool IsPointInRegion(const Coord pos) { return pShape->IsPointWithinShape(pos); };

	void SetName(std::string set) { this->pName = set; }
	std::string GetName() const { return this->pName; }

	bool IsEmpty() const { return !pShape.get();  }

	bool IsActive() const { return this->pIsActive; }
	void SetActiveState(bool isActive ) { this->pIsActive = isActive; }

private:

	void Initialize();

	std::string pName;
	std::shared_ptr<Shapes::Shape> pShape;
	bool pIsActive;

};

class CollRegion : public std::vector<Region>
{
public:

	CollRegion();
	void AddRegion(Region region) { this->push_back(region); };

	Region GetRegion(std::string name);
	Region GetRegion(const Coord pos);
	bool IsRegionInPoint(const Coord pos);


};