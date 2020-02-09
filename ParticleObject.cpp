#include"ParticleObject.h"

ParticleObject::ParticleObject()
{
	//pFigureExterior = PixelFigure(1,1,1);
	//pCenter = Coord(0,0);
	//pHeading = 0;
}

ParticleObject::ParticleObject( const BoxList& list)
{
	//pFigureExterior = PixelFigure(list, list.GetCenter() );
	//pCenter = list.GetCenter();
	//pHeading = 0;
}


//SingleParticleObject::SingleParticleObject( ColorRGB color )
//{
//	pFigureExterior = PixelFigure(1,1,1);
//	pCenter = Coord(0,0);
//	pHeading = 0;
//	pMassParticleList.push_back( std::shared_ptr<MassParticle>( new MassParticle( ColorRGB(100,100,100,100 )) ) );
//}