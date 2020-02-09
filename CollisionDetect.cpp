#include"CollisionDetect.h"
#include"Driver.h"
#


namespace CollisionDetect {

	bool rangeIntersection(int r1_min, int r1_max, int r2_min, int r2_max)
	{
		if (r1_max < r2_min || r2_max < r1_min) //(r2_min < r1_max || r1_min < r2_max)//(r1_max < r2_min || r2_max < r1_min)  // (r2_minX < r1_maxX && r1_minX < r2_maxX)
			return false;
		return true;
	}

	RectSide AngleBetweenRectangle(const SDL_Rect& Target, const SDL_Rect& Source)
	{
		RectSide bottomUp = RectSide::rNone;
		RectSide LeftRight = RectSide::rNone;

		int x_0 = 0;// Target.x;
		int x_1 = x_0 + Target.w;
		int y_0 = 0;// Target.y;
		int y_1 = y_0 + Target.h;

		int sideHits = 0;

		if (rangeIntersection(Source.x, Source.x + Source.w, x_0, x_1))
		{
			if (Source.y < 0)
				bottomUp = RectSide::rBottom;
			else
				bottomUp = RectSide::rTop;
			sideHits++;
		}
		if (rangeIntersection(Source.y, Source.y + Source.h, y_0, y_1))
		{
			if (Source.x < 0)
				LeftRight = RectSide::rRight;
			else
				LeftRight = RectSide::rLeft;
			sideHits++;
		}

		if (sideHits > 1)
		{
			if (LeftRight == RectSide::rRight)
			{
				int dist1 = (Source.x + Source.w) - 0;
				if (bottomUp == RectSide::rBottom)
				{
					int dist2 = (Source.y + Source.h) - 0;
					if (std::abs(dist1) < std::abs(dist2))
						return RectSide::rRight;
					return RectSide::rBottom;
				}
				else
				{
					int dist2 = (Source.y) - (0 + Target.h);
					if (std::abs(dist1) < std::abs(dist2))
						return RectSide::rRight;
					return RectSide::rTop;
				}
			}
			else
			{
				int dist1 = (Source.x) - (0 + Target.w);
				if (bottomUp == RectSide::rBottom)
				{
					int dist2 = (Source.y + Source.h) - 0;
					if (std::abs(dist1) < std::abs(dist2))
						return RectSide::rLeft;
					return RectSide::rBottom;
				}
				else
				{
					int dist2 = (Source.y) - (0 + Target.h);
					if (std::abs(dist1) < std::abs(dist2))
						return RectSide::rLeft;
					return RectSide::rTop;
				}
			}
			//Coord distance = Coord(cTarget.x - cSource.x, cTarget.y - cSource.y );
			//if (std::abs(distance.x) > std::abs(distance.y))
			//	return LeftRight;
			//else
			//	return bottomUp;
		}
		return bottomUp | LeftRight;
	}

	bool projectileWithinBoundary(DynamicEntity& A, Projectile& B)
	{
		if (!A.reqCollisionDetect || !B.reqCollisionDetect)
			return false;

		int ASize = A.GetCachedSize();
		Coord diff = B.getGlobalPos();
		diff += (B.getHitPoint() -= Coord(ASize*0.5)).yflip() -= A.getPosition();//B.getGlobalPos() + B.getHitPoint().yflip() - ( A.getPosition() + Coord(ASize*0.5).yflip() );

		if (std::abs(diff.x) > ASize*0.5 || std::abs(diff.y) > ASize*0.5)
			return false;
		return true;
	}

	colData dynamicEntityHullDetect( DynamicEntity& A, Projectile& B )
	{
		colData result;

		int ASize = A.GetCachedSize();
		Coord diff = B.getGlobalPos();
		diff += (B.getHitPoint() -= Coord(ASize*0.5)).yflip() -= A.getPosition();

		Coord targetFrameHitPoint = (diff - ASize*0.5).negate().FlipAlongXline(ASize*0.5);
		Coord targetFrameHitPointRotated = A.getGraphics()->getGraphicalObject()->transformPositionToObjectFrame(targetFrameHitPoint);//B.getHitPoint());

		CollisionResult cResult = A.getGraphics()->getGraphicalObject()->coordinateCollisionResult(targetFrameHitPointRotated);

		if ((A.reqCollisionDetect && B.reqCollisionDetect) && cResult.IsHitMiss() == false)
		{
			result.isValidCollision = true;
			result.hunterHitPos = targetFrameHitPoint;
			result.vel_A = A.getVelocity();
			result.vel_B = B.getVelocity();
			result.targetHitpos = targetFrameHitPoint;
			//A.reqCollisionDetect = false;
			if (cResult.TakeNoDamage())
				result.collisionType = enCollisionType::ctOnlyAnimation;
			//B.reqCollisionDetect = false;

		}
		return result;
		//if( !A.reqCollisionDetect || !B.reqCollisionDetect )
		//{
		//	result.isValidCollision = false;
		//	return result;
		//}

		//int ASize = A.GetCachedSize();
		//Coord diff = B.getGlobalPos() + (B.getHitPoint() - Coord(ASize*0.5)).yflip() - A.getPosition();//B.getGlobalPos() + B.getHitPoint().yflip() - ( A.getPosition() + Coord(ASize*0.5).yflip() );

		////check distance
		//if (std::abs(diff.x) > ASize*0.5 || std::abs(diff.y) > ASize*0.5)
		//	return result;


		//double entityDistances = geometry::realDistance( Coord(0), diff );//A.getPosition(), B.getGlobalPos() );
		//double boundary = (ASize*0.5 )*1.4142;
		////bool withinCollisionBound = false;

		//if( entityDistances < boundary )
		//{
		//	//withinCollisionBound = true;

		//	Coord targetFrameHitPoint = ( diff - ASize*0.5 ).negate().FlipAlongXline(ASize*0.5 );
		//	Coord targetFrameHitPointRotated = A.getGraphics()->getGraphicalObject()->transformPositionToObjectFrame(targetFrameHitPoint);//B.getHitPoint());

		//	CollisionResult cResult = A.getGraphics()->getGraphicalObject()->coordinateCollisionResult(targetFrameHitPointRotated);

 	//		if( ( A.reqCollisionDetect && B.reqCollisionDetect ) && cResult.IsHitMiss() == false  )
		//	{
		//		result.isValidCollision = true;
		//		result.hunterHitPos = targetFrameHitPoint;
		//		result.vel_A = A.getVelocity();
		//		result.vel_B = B.getVelocity();
		//		result.targetHitpos = targetFrameHitPoint;
		//		//A.reqCollisionDetect = false;
		//		if(cResult.TakeNoDamage() )
		//			result.collisionType = enCollisionType::ctOnlyAnimation;
		//		//B.reqCollisionDetect = false;

		//	}
		//}


	}

	colData ProjectileProjectileCollisionDetect( Projectile& A, Projectile& B_hunter )
	{
		colData result;

		if( !A.reqCollisionDetect || !B_hunter.reqCollisionDetect )
		{
			return result;
		}

		int validDistance = 32;	//this to be based on hunter missile
		
		Coord globalBPosition = B_hunter.getGlobalPos() + B_hunter.getHitPoint();
		Coord globalAPosition = A.getGlobalPos() + A.getHitPoint();

		if( geometry::PointWithinRange( globalBPosition, globalAPosition, validDistance ) )
		{
			result.isValidCollision = true;
			result.hunterHitPos = globalBPosition;
			result.targetHitpos = globalAPosition;
			result.vel_A = A.getVelocity();
			result.vel_B = B_hunter.getVelocity();
		}

		return result;
	}

	colData WalkerInteriorCollision (WalkingEntity& A, InteriorEntity& B)
	{
		colData result;
		if (!A.reqCollisionDetect || !B.reqCollisionDetect)
			return result;

		Coord globalAPosition = A.GetProposedPosition().yflip();
		Coord globalBPosition = B.getPosition();

		//double entityDistances = geometry::realDistanceQuadratic(Coord(0), globalAPosition- globalBPosition);//A.getPosition(), B.getGlobalPos() );
		//double boundary = (A.getSize()*A.getSize()*0.5);//*1.4142;

		//if (entityDistances >= boundary)
		//	return result;

		Coord sizeDiff = A.getSize() - A.getGraphics()->getGraphicalObject()->GetBoundarySize().ScaleToLargestDimension();
		//sizeDiff = geometry::rotateCoord(sizeDiff, A.getUnitHeading());
		Coord pos = sizeDiff / 2;

		//Coord boundPosition = A.getGraphics()->getGraphicalObject()->GetBoundaryPosition();
		////boundPosition = geometry::rotateCoord(boundPosition, -1*A.getUnitHeading());
		//boundPosition.x = globalAPosition.x + pos.x;
		//boundPosition.y = globalAPosition.y +pos.y;

		//SDL_Rect transformedBoundary = A.getGraphics()->getGraphicalObject()->GetBoundary();
		//transformedBoundary.x = boundPosition.x;//globalAPosition.x + pos.x;
		//transformedBoundary.y = boundPosition.y;//pos.y - globalAPosition.y;
		//transformedBoundary.w = transformedBoundary.w*std::cos(A.getUnitHeading()) + transformedBoundary.h*std::sin(A.getUnitHeading());
		//transformedBoundary.h = transformedBoundary.h*std::cos(A.getUnitHeading()) + transformedBoundary.w*std::sin(A.getUnitHeading());

		//double mHeading = A.getUnitHeading()*-1;
		double pHeading = geometry::SetHeadingToQaudrant(A.getUnitHeading(), 1);
		SDL_Rect test2  = A.getGraphics()->getGraphicalObject()->GetBoundaryAsSAquare();
		//double a = std::abs( test2.w*std::cos(pHeading) + test2.h*std::sin(pHeading) );	//can be negative?? should not
		//double b = std::abs( test2.h*std::cos(pHeading) + test2.w*std::sin(pHeading) );	//can be negative?? should not
		//test2.w = a;
		//test2.h = b;

		if (test2.w == 0 || test2.h == 0)
		{
			int abs = 3;
			return result;
		}

		test2.x = globalAPosition.x +A.getSize() / 2;// -test2.w / 2;// -(A.getGraphics()->getGraphicalObject()->GetBoundarySize().x / 2)*std::cos(mHeading) + std::sin(mHeading)*(A.getGraphics()->getGraphicalObject()->GetBoundarySize().y / 2); //wrong
		test2.y = globalAPosition.y +A.getSize() / 2;// -test2.h / 2;// -(A.getGraphics()->getGraphicalObject()->GetBoundarySize().y / 2)*std::cos(mHeading) + std::sin(mHeading)*(A.getGraphics()->getGraphicalObject()->GetBoundarySize().x / 2); //wrong



		CollisionResult collision = B.getGraphics()->getGraphicalObject()->coordinateCollisionResult(test2); //;A.getGraphics()->CoordIsCollision
		if (collision.IsHit())
		{
			auto link = B.getGraphics()->getGraphicalObject()->GetObjectFromID(collision.GetHitIndex());
			

			Coord relativeLinkPos = B.TransformPositionToLinkFrame(Coord(test2.x, test2.y), *link);  // BØG HERE      // -A.getSize() / 2;
			SDL_Rect transformedBoundary = test2;

			//Coord transform = transformPosToLinkObjectFrame(centerpoint, **it) + (*it)->GetBoundarySize() / 2 - Coord(rect.w / 2, rect.h / 2);

			transformedBoundary.x = relativeLinkPos.x + link->GetBoundarySize().x*0 - test2.w/2;// - test2.w/2;
			transformedBoundary.y = relativeLinkPos.y + link->GetBoundarySize().y*0 - test2.h / 2;

			RectSide cResult = AngleBetweenRectangle(link->GetBoundary(), transformedBoundary); //geometry::angleBetweenTwoPoints(*link->relationalZeroHeadingPosition + link->GetBoundarySize()/2, B.TransformPositionToLinkFrame(globalAPosition, *link));
			//if (std::isnan(heading))
			//	return result;
			
			//double headingTransform = heading; //pi2 - heading - pi
			//double moveDirection = A.getGlobalHeading();

			//double diff = geometry::radLimit2PI(geometry::shortestAngleDifference(moveDirection, 0)); // heading));

			
			EntityMovementDrive map = A.GetDiscreteMovementMap();
			bool leftMask = !map.pLeft.IsEnabled();
			bool rightMask = !map.pRight.IsEnabled();
			bool forwardMask = !map.pForward.IsEnabled();
			bool backMask = !map.pBackward.IsEnabled();
			if (cResult & RectSide::rLeft && map.Left())
			{
				leftMask = true;
				result.isValidCollision = true;
			}
			if (cResult & RectSide::rRight && map.Right())
			{
				rightMask = true;
				result.isValidCollision = true;
			}
			if (cResult & RectSide::rBottom && map.Backward())
			{
				backMask = true;
				result.isValidCollision = true;
			}
			if (cResult & RectSide::rTop && map.Forward())
			{
				forwardMask = true;
				result.isValidCollision = true;
			}
			//else
			//	result.isValidCollision = false;
			A.SetMovementMap(forwardMask, backMask, leftMask, rightMask, true);
			//{
			//	result.isValidCollision = true;
			//	result.vel_A = A.getVelocity();
			//	result.vel_B = primitives::vector(0);
			//}
			//else
			//{
			//	int fortesting = 2;
			//}



			//if (std::abs(diff) < std::_Pi_div2)
			//{
			//	result.isValidCollision = true;
			//	result.vel_A = A.getVelocity();
			//	result.vel_B = primitives::vector(0);
			//}
			//else
			//{
			//	int fortesting = 2;
			//}
		}
		return result;
	}
	
}

CollisionHandle::CollisionHandle(Coord targetHitPos, primitives::vector velA, primitives::vector velB, enCollisionType collisiontype)
{
	TargetHitPos = targetHitPos;
	pVelA = velA; 
	pVelB = velB;
	this->pCollisionType = collisiontype;

};

CollisionHandle_PR_PR::CollisionHandle_PR_PR(std::shared_ptr<Projectile> prey, std::shared_ptr<Projectile> hunter, Coord preyhitpoint, Coord hunterhitpoint, primitives::vector velDE, primitives::vector velPR )
	: CollisionHandle( preyhitpoint, velDE,velPR, enCollisionType::ctProjectileImpact  )
{
	PR_prey = prey;
	PR_hunting = hunter;
	this->hunterHitPoint = hunterhitpoint;
}

CollisionHandle_DE_PR::CollisionHandle_DE_PR(std::shared_ptr<DynamicEntity> entity, std::shared_ptr<Projectile> projectile, CollisionDetect::colData& pData )
	: CollisionHandle(pData.targetHitpos, pData.vel_A, pData.vel_B, pData.collisionType  )
{
	DE_A = entity;
	PR_A = projectile;

}

CollisionHandle_DE_DE::CollisionHandle_DE_DE(std::shared_ptr<DynamicEntity> entityA, std::shared_ptr<DynamicEntity> entityB, Coord pos, primitives::vector velDE, primitives::vector velDE2)
	: CollisionHandle( pos, velDE, velDE2, enCollisionType::ctEntityCollision)
{
	DE_A = entityA;
	DE_B = entityB;
}

//CollisionHandle_VE_PR::CollisionHandle_VE_PR(std::shared_ptr<Vessel> entity, std::shared_ptr<Projectile> projectile, Coord pos, primitives::vector velVE, primitives::vector velPR)
//	: CollisionHandle( pos, velVE,velPR  )
//{
//	VE_A = entity;
//	PR_A = projectile;
//}