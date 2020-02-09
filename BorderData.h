#pragma once
#include"PixelBox.h"
#include"DiscreteRotations.h"

class BorderData
{
	
public:

	BorderData();
	BorderData( const PixelBox&, bool neighbourPositive ); 
	BorderData( const PixelBox&, bool neighbourPositive, enObjectDiscreteRotation rotation ); 

	bool up;
	bool down;
	bool right;
	bool left;

	bool upleft;
	bool upright;
	bool downleft;
	bool downright;

	enObjectDiscreteRotation Rotation;
	enObjectFlipped Flip;

	void RotateData(enObjectDiscreteRotation);
	void RotateData();
	void FlipData(enObjectFlipped);
	int GetStraightCornerBorders() const;
	int GetDiagonalCornerBorders() const;

	bool operator==(const BorderData& rhs) const;
	bool operator!=(const BorderData& rhs) const;

private:

	bool Equal( const BorderData& rhs ) const;

	void initialize();
	void initialize(const PixelBox&, bool neighbourPositive);

};