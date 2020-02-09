#pragma once
#include <SDL.h>
#include <map>
#include <list>
#include <random>
#include <memory>
#include <new>
#include"Coord.h"

//missing copy constrct, swap and == operator
//enum enAIModel
//{
//	NOACTION = 0,
//	//HOLDPOSITION,
//	MOVETOTARGETCOORD,
//	//MOVETOTARGETENTITY,
//	AIPROBER,
//	//TRACKTARGETENTITY,
//	//TEMPLATEFUNCTION,
//
//};

enum enumAllegiance
{
	aENEMY = 1,
	aFRIENDLY = 2
};


namespace primitives {

	Uint8 intToHex( int value );
	double limitDouble( double value, double limit );
	//void limitDouble(double value, double limit);



	





	//template <typename  T> bool RemoveItemFromVector(std::vector<T>& v, T item);
	//template <typename  T> bool RemoveItemFromVector(std::vector<T>& v, int index);

	bool RectAEqualsToB(const SDL_Rect& A, const SDL_Rect& B);

	class vector
	{

		public:
			vector() {x = 0; y = 0;};

			vector( const vector & other ) {x=other.x, y=other.y; };
			vector( const Coord & other ) {x=other.x, y=other.y; };
			vector(double X, double Y) {x = X,y = Y;};
			vector(double XY) {x = XY,y = XY;};

			vector operator+(const vector& b) { return vector(this->x + b.x, this->y + b.y); };
			vector & vector::operator+=(const vector &rhs) { this->x += rhs.x; this->y += rhs.y;  return *this; };

			vector operator-(const vector& b) { return vector(this->x - b.x, this->y - b.y); };
			vector & vector::operator-=(const vector &rhs) { this->x -= rhs.x; this->y -= rhs.y;  return *this; };
	

			vector& operator=(const vector& other) { this->x = other.x; this->y = other.y; return *this; };

			vector vector::operator*(const double &rhs) const { return vector(this->x * rhs, this->y * rhs); };
			vector & vector::operator*=(const double &rhs) { this->x *= rhs; this->y *= rhs;  return *this; };

			vector vector::operator/(const double &rhs) { return vector(this->x / rhs, this->y / rhs); };
			vector & vector::operator/=(const double &rhs) { this->x /= rhs; this->y /= rhs;  return *this; };

			bool vector::operator==(const vector& rhs) const { return (this->x == rhs.x && this->y == rhs.y); };

			bool operator<(const vector& other) const;
			bool operator>(const vector& other) const;

			bool xORyLargerThan( double val );
			bool xORySmallerThan( double val );

			bool xANDyLargerThan( double val );
			bool xANDySmallerThan( double val );

			void xANDyLimit( double limit );

			vector negate() const { return primitives::vector( -x, -y ); };
			vector xyswap() const { return primitives::vector(y,x); };
			vector yflip() const { return primitives::vector(x,-y); };
			vector xflip() const { return primitives::vector(-x,y); };
			vector yadd(double amount ) const { return primitives::vector(x,y+amount); };
			vector xadd(double amount ) const { return primitives::vector(x+amount,y); };

			virtual void clear() { x,y = 0;};

			double x;
			double y;

			Coord toCoord() const { return Coord( x,y ); };

			double ABS();
			double DotProduct();
			vector Normalize();
			static vector Normalize(vector);
			static vector ScaleByFactor(vector, double ratio);
			vector Scale( double length );
			vector ScaleByFactor( double ratio );
			void Rotate(double heading);

			SDL_Rect ToSDLRect(const vector& size) const;
			SDL_Rect ToSDLRect(double w, double h) const;
	};





	void NormalDistribution( double average_x, double average_y, double standardDev_x, double standardDev_y, int size, std::list<primitives::vector>& listOut, std::default_random_engine& engine );
	void NormalDistribution( double average_x, double average_y, double standardDev_x, double standardDev_y, primitives::vector& Out, std::default_random_engine& engine );

	void NormalDistribution( int average_x, int average_y, int standardDev_x, int standardDev_y, int size, std::list<Coord>& listOut, std::default_random_engine& engine );
	void NormalDistribution( int average, int standardDeviation, int size, std::list<int>& listOut, std::default_random_engine& engine );

	struct PIDSettings
	{

		PIDSettings();
		PIDSettings(double P, double I, double D);
		PIDSettings(double P, double I, double D, double ph, double Ih, double Dh);

		double P;
		double I;
		double D;
		double Pheading;
		double Iheading;
		double Dheading;

		void Reset();

	};

	struct waypoint
	{
		waypoint()
		{
			pos = Coord();
			circleRadius = 1;
			//isActive = false;
		}

		waypoint(Coord setPos)
		{
			pos = setPos;
			circleRadius = 1;
			finalHeading = 0;
			//isActive = false;
		}

		waypoint(int x, int y)
		{
			pos = Coord(x, y);
			circleRadius = 1;
			finalHeading = 0;
			//isActive = false;
		}

		waypoint( Coord setPos, double radius)
		{
			pos = setPos;
			circleRadius = radius;
			finalHeading = 0;
			//isActive = false;
		}

		waypoint( Coord setPos, double radius, double finalHeading )
		{
			pos = setPos;
			circleRadius = radius;
			this->finalHeading = finalHeading;
			//isActive = false;
		}

		waypoint(Coord setPos, primitives::vector velocity, double radius, double finalHeading)
		{
			pos = setPos;
			circleRadius = radius;
			this->finalHeading = finalHeading;
			this->vel = velocity;
			//isActive = false;
		}
		
		bool positionWithinWaypoint( Coord pos ) const;

		//bool isActive;
		Coord pos;
		primitives::vector vel;
		double circleRadius;	//area around Coord to indicate if the Coord has been reached
		double finalHeading;
		
	};

	bool positionWithinWaypoint( primitives::waypoint wp, Coord pos );

	//enum EnumSpriteSize		// see convertEnumToInt(...) for conversion to int values
	//{
	//	x0 = 0,
	//	x1 = 1,
	//	x2 = 2,
	//	x4 = 4,
	//	x8 = 8,
	//	x16 = 16,
	//	x23 = 23,
	//	x24 = 24,
	//	x31 = 31,
	//	x32 = 32,
	//	x48 = 48,
	//	x64 = 64,
	//	x96 = 96,
	//	x128 = 128,
	//	x192 = 192,
	//	x256 = 256,
	//	x384 = 384,
	//	x512 = 512,
	//	x640 = 640,
	//	x768 = 768,
	//	x896 = 896,
	//	x1024 = 1024,
	//	xMAX = 10000,
	//};




	

	SDL_Rect SDLRect(int x, int y, int w, int h);

	std::vector<std::string> Split(const std::string &s,  char delim );
	std::vector<std::string> SplitNewLine(const std::string &s);
	std::vector<std::string> SplitNewLine(const std::string &s, int size, std::string font, int maxWidth);
	std::string GetLargestString(const std::vector<std::string>& );
	Coord GetTextDimensions(std::string text, int size, std::string font);

	struct TargetState
	{
		TargetState();
		TargetState(Coord pos, primitives::vector vel, double Inheading);
		TargetState(const TargetState & other);
		TargetState(const TargetState && other);

		Coord Position;
		primitives::vector Velocity;
		double Heading;
	};

}

namespace geometry {
	
	const static int MAXDISTANCE = 999999;
	const double PI_2 = 6.28318530718;
	const double PI = 3.14159265359;
	const double PI_div2 = 1.5707963268;

	



	bool pointWithinBoundary_inclusive( int point, int lowLimit, int highLimit );
	bool pointWithinBoundary_exclusive( int point, int lowLimit, int highLimit );

	bool pointWithinSquare_inclusive( const Coord&, const Coord& squareTopLeft, int squareThickness );
	bool pointWithinSquare_exclusive( const Coord&, const Coord& squareTopLeft, int squareThickness );

	bool pointWithinRectangle_inclusive( const Coord&, const Coord& squareTopLeft, int rect_x, int rect_y );
	bool pointWithinRectangle_exclusive( const Coord& point, const Coord& squareTopLeft, int rect_x, int rect_y );

	bool pointWithinRectangle_inclusive( const Coord& point, const Coord& squareTopLeft, int rect_x, int rect_y );
	bool pointWithinRectangle_inclusive( const Coord& point, const Coord& squareTopLeft, const Coord& squareBottomright );
	bool pointWithinRectangle_exclusive( const Coord& point, const Coord& squareTopLeft, int rect_x, int rect_y );

	bool BoxIntersectRectangle_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopLeft, int rect_x, int rect_y );
	bool BoxWithinRectangle_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopLeft, int rect_x, int rect_y );
	
	bool BoxIntersectSquare_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopLeft, int squareThickness );
	bool BoxWithinSquare_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopLeft, int rect_x, int rect_y );



	int Sign( double );

	SDL_Rect GetRectangleIntersect( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopLeft, int rect_x, int rect_y );

	double angleBetweenTwoPoints( const Coord&, const Coord& ); //range between [0, 2pi] 
	Coord pointAlongLine( const Coord& linePoint, double angle, double distanceAlongLine );
	double lineFunction( const Coord&, double, double ); //not tested
	double rateFromAngle( double angle ); //range between [0, 2pi] not tested
	std::list<Coord> bresenham( Coord, Coord, std::list<Coord> );
	double vectorHeading( primitives::vector );
	double vectorHeading( Coord );
	primitives::vector angleToVector( double angle );
	double shortestAngleDifference( double xTarget, double ySource);
	
	Coord pixelDistance( Coord, Coord );
	double realDistance( Coord, Coord );
	double realDistance(Coord);
	double realDistanceQuadratic( Coord A, Coord B);
	double realDistanceToYAxis( double Y, Coord B);
	primitives::vector OrthogonalVector( primitives::vector inVector );
	double ProjectVectorOntoVector( primitives::vector inVector, primitives::vector projection );
	bool PointWithinRange( Coord reference, Coord point, double range ); 

	bool pixelAreNeighbours( Coord, Coord );

	Coord rotateCoord( Coord input, double heading );
	primitives::vector rotateVector( primitives::vector input, double heading );
	double radToDegrees( double rad ); 
	double DegreesToRad( double deg );

	double SetHeadingToQaudrant(double heading, int quadrant);
	bool isInFirstQuadrant( double rad );
	bool isInSecondQuadrant( double rad );
	bool isInThirdQuadrant( double rad );
	bool isInFourthQuadrant( double rad );

	double dotProduct( const primitives::vector& A, const primitives::vector& B );
	double dotProduct(const Coord& A);
	double angleBetweenVectors( primitives::vector A, primitives::vector B );
	double angleBetweenVectors( Coord A, Coord B );
	double radLimit2PI( double rad );
	double radLimitPI(double rad);

	Coord GetPointFromCircle( const Coord& center, double distance, double angle );
	bool CoordWIthinRectangle(const SDL_Rect& rect, const Coord&);

	

}

namespace imagehandling
{
	// don't make a larger canvas, see optimization suggestion in main
	//obsolete
	static const char* canvas1024CYAN = "../PictureFiles/Canvas/blank1024.png";
	static const char* canvas512CYAN = "../PictureFiles/Canvas/blank512.png";
	static const char* canvas256CYAN = "../PictureFiles/Canvas/blank256.png";
	static const char* canvas128CYAN = "../PictureFiles/Canvas/blank128.png";
	static const char* canvas64CYAN = "../PictureFiles/Canvas/blank64.png";
	static const char* canvas32CYAN = "../PictureFiles/Canvas/blank32.png";
	static const char* canvas16CYAN = "../PictureFiles/Canvas/blank16.png";
	static const char* canvas8CYAN = "../PictureFiles/Canvas/blank8.png";
	static const char* canvas1CYAN = "../PictureFiles/Canvas/blank1.png";

	



	// implement a method to differentiate between const IMGToSurface and not const... if needed

	//class TextureObject_2 
	//{
	//public:
	//	TextureObject_2();
	//	TextureObject_2( const TextureObject_2& );
	//	TextureObject_2( TextureObject_2&& other);
	//	TextureObject_2( const imagehandling::IMGToSurface* );
	//	//TextureObject_2( imagehandling::IMGToSurface* );

	//	virtual void generateTexture( SDL_Renderer* );
	//	

	//	int getSize() const { return this->pSurface->getSize(); };
	//	spriteSize getDimension() const { return this->pSurface->GetSpriteSize(); };
	//	EnumColorKey getColorKey() const { return pSurface->get_colorKey(); };

	//	SDL_Texture* getTexture( SDL_Renderer* renderer ); //guarantee a texture
	//	SDL_Texture* getTexture() { return pTexture.get(); };

	//	void RefreshTexture();

	//	enumTextureModulation getModulation() { return modulationFlag; }
	//	void setModulation(enumTextureModulation set);
	//	void setOpacity( int value, bool changeBaseOpacity );
	//	void setBlendMode(SDL_BlendMode mode);
	//	void modulate( ColorRGB );

	//	void PaintTextureAtCoord( Coord, ColorRGB );
	//	void PaintTextureAtCoords( const std::vector<CoordWithColor>& canvas );
	//	void PaintTextureAtCoords( const CoordWithColor canvas[], int  size );
	//	void PaintTextureAtCoords( const CoordWithCollisionData canvas[], int  size  );
	//	void SetCropBase( std::shared_ptr<IMGToSurface> );
	//	Coord GetAxisOfRotation() const { return this->pSurface->getAxisOfRotation(); }
	//	
	//	TextureObject_2 & operator=( const TextureObject_2& );
	//	TextureObject_2 & operator=( TextureObject_2&& );
	//	
	//	void SetSurfaceClip( const SDL_Rect& );
	//	int GetFrames() const { return this->pSpriteBase->getFrames(); };
	//	void ChangeFrame(int frame);
	//	void IncrementFrameNumber();
	//	int GetCurrentFrameNumber() const;
	//	void MakeUnique(); // not tested

	//	ColorRGB GetCustomModulation() const { return this->pCustomModulation; };
	//	void SetCustomModulation( ColorRGB col );

	//	void SetFlipOnDraw( bool set );
	//	SDL_RendererFlip GetFlipOnDraw() { return this->pFlipOnDraw; };

	//	enSurfaceFrameDimension GetFrameDimension() const { return this->pSurface->GetSurfaceDimension(); };
	//	//friend void swap(TextureObject_2& first, TextureObject_2& second);
	//	~TextureObject_2();

	//protected:

	//	const imagehandling::IMGToSurface* getSDLImageToSurface() const { return pSurface.get(); };
	//	enumTextureModulation modulationFlag;
	//	SDL_Rect surfaceClipping;

	//	virtual void CropTexture( SDL_Renderer* , const SDL_Rect& crop );

	//private:
	//	
	//	bool pNeedsRefresh;
	//	bool pNeedsClipping;
	//	SDL_RendererFlip pFlipOnDraw;
	//	ColorRGB pCustomModulation;
	//	SDL_BlendMode pBlendMode;

	//	void ChangeBaseOpacity(int newValue);

	//	std::unique_ptr<SDL_Texture,void(*)(SDL_Texture*)> pTexture;
	//	std::unique_ptr<imagehandling::IMGToSurface> pSurface;
	//	std::shared_ptr<imagehandling::IMGToSurface> pSpriteBase;
	//	//std::shared_ptr<imagehandling::IMGToSurface> pCanvasLayer; //make a merge function, with canvas and layer

	//	bool CompareClipping( const SDL_Rect& crop );

	//	//TextureObject_2( const TextureObject_2 && );
	//	//TextureObject_2 & operator=( const TextureObject_2 && );

	//	int pBaseOpacity;

	//	void ModulateTexture();

	//	void Initialize();
	//};

}



namespace optimization
{
	double DIV( double numerator, int value );	//provides accelerated division for the division of number below 50
}

namespace std
{
	template<>
	struct hash<Coord> {
    size_t operator()(const Coord &Coord) const {
        return std::hash<int>()(Coord.x) ^ std::hash<int>()(Coord.y);
    }
	};

}
