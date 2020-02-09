#include"BorderData.h"
#include"DiscreteRotations.h"
#include <algorithm>    // std::swap

BorderData::BorderData()
{
	this->initialize();
}

BorderData::BorderData( const PixelBox& box, bool neighbourPositive)
{
	initialize(box,neighbourPositive);
	this->Rotation = enObjectDiscreteRotation::goNoRotation;
}

BorderData::BorderData( const PixelBox& box, bool neighbourPositive, enObjectDiscreteRotation rotation )
{
	initialize(box,neighbourPositive);
	this->Rotation = rotation;
}

int BorderData::GetStraightCornerBorders() const
{
	return up + down + left + right;
}

int BorderData::GetDiagonalCornerBorders() const
{
	return upleft + upright + downleft + downright;
}

void BorderData::RotateData()
{
	RotateData(Rotation);
}

void BorderData::FlipData(enObjectFlipped flip)
{
	switch (flip)
	{
	case goNotFlipped:
		break;
	case goIsFlipped:
		std::swap(left, right);
		std::swap(upleft, upright);
		std::swap(downleft, downright);
		//temp_left = left;
		//left = right;
		//right = temp_left;


		break;
	default:
		break;
	}
}

void BorderData::RotateData(enObjectDiscreteRotation rotation)
{
	bool temp_up;
	bool temp_left;
	bool temp_right;

	bool temp_upleft;
	bool temp_upright;

	switch (rotation)
	{
	case(enObjectDiscreteRotation::goNoRotation) :
		break;
	case(enObjectDiscreteRotation::go90Rotation) :
		temp_up = up;

		up = right;//
		right = down;
		down = left;//
		left = temp_up;

		temp_upleft = upleft;

		upleft = downright;
		downright = downleft;
		downleft = upright;
		upright = temp_upleft;

		break;
	case(enObjectDiscreteRotation::go180Rotation) :
		temp_up = up;
		temp_left = left;

		up = down;
		left = right;
		down = temp_up;
		right = temp_left;

		temp_upleft = upleft;
		temp_upright = upright;

		upleft = downleft;
		upright = downright;
		downleft = temp_upleft;
		downright = temp_upright;
		break;
	case(enObjectDiscreteRotation::go270Rotation) :
		temp_up = up;
		temp_right = right;
		up = left;
		left = down;
		down = right;
		right = temp_up;

		temp_upleft = upleft;

		upleft = upright;
		upright = downleft;
		downleft = downright;
		downright = temp_upleft;
		break;
	default:
		break;
	}
}


void BorderData::initialize()
{
	this->up = false;
	this->down = false;
	this->left = false;
	this->right = false;
	
	this->upleft = false;
	this->upright = false;
	this->downleft = false;
	this->downright = false;
}

void BorderData::initialize(const PixelBox& box, bool neighbourPositive)
{
	this->up = box.up == neighbourPositive;
	this->down = box.down == neighbourPositive;
	this->left = box.left == neighbourPositive;
	this->right = box.right == neighbourPositive;

	this->upleft = box.upleft == neighbourPositive;;
	this->upright = box.upright == neighbourPositive;;
	this->downleft = box.downright == neighbourPositive;;	//downleft = downright, is needed to correct drawing, the reason of this error is unknown
	this->downright = box.downleft == neighbourPositive;;  //downright = downleft, is needed to correct drawing, the reason of this error is unknown
}

bool BorderData::operator==(const BorderData& rhs) const
{
	return this->Equal(rhs);
}

bool BorderData::operator!=(const BorderData& rhs) const
{
	return !this->Equal(rhs);
}

bool BorderData::Equal( const BorderData& rhs ) const
{
	return (this->up == rhs.up &&
		this->down == rhs.down &&
		this->left == rhs.left &&
		this->right == rhs.right &&
		this->upleft == rhs.upleft &&
		this->upright == rhs.upright &&
		this->downleft == rhs.downleft &&
		this->downright == rhs.downright);
}