#pragma once
#include "Graphics.h"
#include "Sprite.h"
#include"AnimationCollection.h"



class Animator
{

public:

	//animation keeps track of time for each framestrip, gets out the current frame for drawing
	Animator();
	//Animator(imagehandling::TextureCollection* spriteCollector);

	void AddFrameStrip( std::shared_ptr<FrameStrip> );
	void AddFrameStrip(FrameStripReader reader);

	void RunFrames( int stepTimeMs );
	void RunFrame(FrameStripReader&, int stepTimeMs);

	std::vector<FrameStripReader>::iterator const begin() { return frameStripList.begin(); };
	std::vector<FrameStripReader>::iterator const end() { return frameStripList.end(); };

	void generateHitAnimation( enHitAnimation, Coord pos, bool repeat );
	void generateMissileExplosion( Coord pos, bool repeat );
	void generateCannonHit( Coord pos, bool repeat );
	void generateHECannonHit(Coord pos, bool repeat);
	void generateHighExplosiveHit(Coord pos, bool repeat);
	void generateTorpedoExplosiveHit(Coord pos, bool repeat);
	void generateShrapnelExplosiveHit(Coord pos, bool repeat);
	void generateRailExplosiveHit(Coord pos, bool repeat);

private:
	

	//add a draw method from draw class to draw animation
	std::vector<FrameStripReader> frameStripList;
	AniMissileExplosions pMissileExplosionSet;
	AniCannonHit pCannonHitSet;
	AniHECannonHit pHECannonHitSet;
	AniHighExplosiveHit pHighExplosiveHitSet;
	AniTorpedoExplosiveHit pTorpedoExplosiveHitSet;
	AniShrapnelExplosiveHit pShrapnelExplosiveHitSet;
	AniRailExplosiveHit pRailExplosiveHitSet;

	Animator( const Animator& );
	Animator( Animator&& );
	Animator operator=(const Animator& );
	Animator operator=(Animator&&);
};



