#include"Animation.h"


Animator::Animator() : pMissileExplosionSet(), pCannonHitSet()
{
}

void Animator::AddFrameStrip( std::shared_ptr<FrameStrip> frmStrip )
{
	frameStripList.push_back(FrameStripReader(frmStrip));
}

void Animator::AddFrameStrip(FrameStripReader reader)
{
	if(reader.Strip.get())
		frameStripList.push_back(reader);
}

void Animator::RunFrames( int stepTimeMs )
{
	for( auto it = frameStripList.begin(); it != frameStripList.end();  )
	{
		FrameStripReader& ptr = *it;
		ptr.IncrementFrameTime( stepTimeMs );
		if( ptr.GetFrameRunTime() > ptr.GetMsPerFrame() )
		{

			if( ptr.CurrentFrameIsLastFrame() )
			{
				if( ptr.IsRepeat() )
				{
					ptr.RestartFrame();
					++it;
				}
				else
				{
					if(ptr.DeleteAfterUse())
					{
						ptr.ClearData();
						it = frameStripList.erase(it);
					}
				}
			}
			else
			{
				ptr.IncrementFrame();
				++it;
			}
		}
		else
		{
			++it;
		}
	}

}

void Animator::RunFrame(FrameStripReader& frame, int stepTimeMs)
{
	frame.IncrementFrameTime(stepTimeMs);
	if (frame.GetFrameRunTime() > frame.GetMsPerFrame())
	{
		if (frame.CurrentFrameIsLastFrame())
		{

			if (frame.IsRepeat())
				frame.RestartFrame();
			else
			{
				if (frame.DeleteAfterUse())
				{
					frame.ClearData();
				}
			}
		}
		else
			frame.IncrementFrame();
	}
}

void Animator::generateHitAnimation( enHitAnimation type, Coord pos, bool repeat )
{
	switch( type )
	{
	case(enHitAnimation::MISSILEEXPLOSION):
		generateHighExplosiveHit(pos, repeat); // generateMissileExplosion(pos, repeat);   //
	break;
	case(enHitAnimation::CANNONHIT):
		generateCannonHit(pos, repeat);
		break;
	case(enHitAnimation::HECANNONHIT):
		generateHECannonHit(pos, repeat);
		break;
	case(enHitAnimation::SIEGECANNONHIT):
		generateHECannonHit(pos, repeat);
		break;
	case(enHitAnimation::CLUSTEREXPLOSION):
			break;
	case(enHitAnimation::ELECTRICEXPLOISION):
			break;
	case(enHitAnimation::LARGEEXPLOSION):
		generateTorpedoExplosiveHit(pos, repeat);
			break;
	case(enHitAnimation::LARGEMISSILEEXPLOSION):
		generateHighExplosiveHit(pos, repeat);
			break;
	case(enHitAnimation::LASEREXPLOSION):
			break;
	case(enHitAnimation::NOANIMATION):
			break;
	case(enHitAnimation::NUCLEAREXPLOSION):
			break;
	case(enHitAnimation::SHRAPNEL):
		generateShrapnelExplosiveHit(pos, repeat);
		break;
	case(enHitAnimation::RAILEXPLOSION):
		generateRailExplosiveHit(pos, repeat);
		break;
	case(enHitAnimation::SHIPDESTRUCTION):
		generateHighExplosiveHit(pos, repeat);
		break;
	}
		
}

void Animator::generateMissileExplosion( Coord pos, bool repeat )
{
	this->AddFrameStrip(this->pMissileExplosionSet.GetRandomStrip( pos, repeat ));
}

void Animator::generateCannonHit( Coord pos, bool repeat )
{
	this->AddFrameStrip(this->pCannonHitSet.GetRandomStrip( pos, repeat));
}

void Animator::generateHECannonHit(Coord pos, bool repeat)
{
	this->AddFrameStrip(this->pHECannonHitSet.GetRandomStrip(pos, repeat));
}

void Animator::generateHighExplosiveHit(Coord pos, bool repeat) //
{
	this->AddFrameStrip(this->pHighExplosiveHitSet.GetRandomStrip(pos, repeat));
}

void Animator::generateTorpedoExplosiveHit(Coord pos, bool repeat)
{
	this->AddFrameStrip(this->pTorpedoExplosiveHitSet.GetRandomStrip(pos, repeat));
}

void Animator::generateShrapnelExplosiveHit(Coord pos, bool repeat)
{
	this->AddFrameStrip(this->pShrapnelExplosiveHitSet.GetRandomStrip(pos, repeat));
}

void Animator::generateRailExplosiveHit(Coord pos, bool repeat)
{
	this->AddFrameStrip(this->pRailExplosiveHitSet.GetRandomStrip(pos, repeat));
}

