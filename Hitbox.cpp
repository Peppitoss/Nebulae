#include"Hitbox.h"

Hitbox::Hitbox()
{
	pCenterOfRotation = 0;
}

void Hitbox::DrawHitBox(SDL_Renderer* renderer, const Coord pos, double heading) const
{

}

SDL_Rect Hitbox::ToSLDRect() const
{
	SDL_Rect result = { this->GetTopLeft().x, this->GetTopLeft().y, this->GetBoundary().x, this->GetBoundary().y };
	return result;
}

bool Hitbox::IsCollision(const SDL_Rect& rect)
{
	//SDL_Rect intersect = geometry::GetRectangleIntersect(this->GetTopLeft(), this->GetBoundary().x, this->GetBoundary().y, Coord(rect.x,rect.y), rect.w, rect.h);
	//return intersect.x > 0 && intersect.y > 0;

	return geometry::BoxIntersectRectangle_inclusive(this->GetTopLeft(), this->GetBoundary().x, this->GetBoundary().y, Coord(rect.x, rect.y), rect.w, rect.h);
}