#include"ShipImageDefinitions.h"

//Ship
//const char* frigate_png = "../PictureFiles/Ships/frigate.png";
//const char* ship_png = "../PictureFiles/Ships/ship.png";
//const char* hardim_png = "../PictureFiles/Ships/Hardim.png";
//const char* gibbs_png = "../PictureFiles/Ships/Gibbs.png";
//const char* nemean_png = "../PictureFiles/Ships/Nemean_Fighter.png";
//const char* copernicus_png = "../PictureFiles/Ships/copernicus.png";
//const char* lancaster_png = "../PictureFiles/Ships/lancaster.png";
//const char* serpent_png = "../PictureFiles/Ships/serpent2.png";
//
//const char* arbalest_png = "../PictureFiles/Ships/Arbalest.png";
//const char* irwin_png = "../PictureFiles/Ships/IrwinGunPlatform.png";
//
//// other
//const char* circle_png = "../PictureFiles/Misc/circle.png";
//const char* gunner_png = "../PictureFiles/Misc/gunner.png";
//const char* planck_png = "../PictureFiles/Projectiles/planck/planck.png";
//const char* missile_png = "../PictureFiles/Projectiles/missile/missile.png";
//const char* missile2_png = "../PictureFiles/Projectiles/missile2/missile2.png";
//const char* horny_png = "../PictureFiles/Ships/horny.png";
//const char* nemean_fighter_png = "../PictureFiles/Ships/Nemean_Fighter.png";
//const char* bombardier_png = "../PictureFiles/Ships/Bombardier.png";
//const char* invisible64_png = "../PictureFiles/Misc/64invisible.png";
//const char* dot1_png = "../PictureFiles/Particles/1dot.png";
const char* background_png     = "../PictureFiles/Backgrounds/backSpace.png";
const char* blackMask_png = "../PictureFiles/Backgrounds/blackMask.png";
const char* whiteMask_png = "../PictureFiles/Backgrounds/whiteMask.png";
//const char* background_png_l2     = "../PictureFiles/Backgrounds/backSpace_l2.png";
//const char* background_png_l3     = "../PictureFiles/Backgrounds/backSpace_l3.png";
//const char* backgroundMars_png = "../PictureFiles/Backgrounds/backSurfMars.png";
//const char* backgroundSun_png  = "../PictureFiles/Backgrounds/backSurfSun.png";
//const char* backgroundSat_png  = "../PictureFiles/Backgrounds/satellite.png";
//const char* backgroundStarmap_png  = "../PictureFiles/Backgrounds/starmap.png";
//
const char* backgroundgrey_png  = "../PictureFiles/PlayerMenu/256x256GreyBack.png";
const imagehandling::IMGToSurface*  ImportImage_background_grey = new imagehandling::IMGToSurface( backgroundgrey_png, kCYAN , spriteSize::GetSize256() );

const imagehandling::IMGToSurface*  ImportImage_background_space = new imagehandling::IMGToSurface( background_png, kCYAN , spriteSize::GetMaxSize() );
const imagehandling::IMGToSurface*  ImportImage_blackMask = new imagehandling::IMGToSurface(blackMask_png, kCYAN, spriteSize::GetMaxSize());
const imagehandling::IMGToSurface*  ImportImage_whiteMask = new imagehandling::IMGToSurface(whiteMask_png, kCYAN, spriteSize::GetMaxSize());
//const imagehandling::IMGToSurface*  ImportImage_background_spaceL2 = new imagehandling::IMGToSurface( background_png_l2, kCYAN , spriteSize::GetMaxSize());
//const imagehandling::IMGToSurface*  ImportImage_background_spaceL3 = new imagehandling::IMGToSurface( background_png_l3, kCYAN , spriteSize::GetMaxSize());
//const imagehandling::IMGToSurface*  ImportImage_background_mars = new imagehandling::IMGToSurface( backgroundMars_png, kCYAN , spriteSize::GetMaxSize());
//const imagehandling::IMGToSurface*  ImportImage_background_sun = new imagehandling::IMGToSurface( backgroundSun_png, kCYAN , spriteSize::GetMaxSize());
//const imagehandling::IMGToSurface*  ImportImage_background_sat = new imagehandling::IMGToSurface( backgroundSat_png, kCYAN , spriteSize::GetMaxSize());
//const imagehandling::IMGToSurface*  ImportImage_background_starmap = new imagehandling::IMGToSurface( backgroundStarmap_png, kCYAN , spriteSize::GetMaxSize());
//const imagehandling::IMGToSurface*  ImportImage_frigate = new imagehandling::IMGToSurface( frigate_png, kCYAN , spriteSize::GetSize256());
//const imagehandling::IMGToSurface*  ImportImage_arbalest = new imagehandling::IMGToSurface( arbalest_png, kCYAN , spriteSize::GetSize128() );
//const imagehandling::IMGToSurface*  ImportImage_irwin = new imagehandling::IMGToSurface( irwin_png, kCYAN , spriteSize::GetSize512());
//const imagehandling::IMGToSurface*  ImportImage_ship = new imagehandling::IMGToSurface( lancaster_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_ship2 = new imagehandling::IMGToSurface( ship_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_circle = new imagehandling::IMGToSurface( circle_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_gunner = new imagehandling::IMGToSurface( gunner_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_project = new imagehandling::IMGToSurface(planck_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_missile = new imagehandling::IMGToSurface( missile_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_missile2 = new imagehandling::IMGToSurface( missile2_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_64invisible = new imagehandling::IMGToSurface( invisible64_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_1dot = new imagehandling::IMGToSurface( dot1_png, kCYAN , spriteSize(1));
//const imagehandling::IMGToSurface*  ImportImage_horny = new imagehandling::IMGToSurface( horny_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_nemean_fighter = new imagehandling::IMGToSurface( nemean_fighter_png, kCYAN , spriteSize::GetSize64());
//const imagehandling::IMGToSurface*  ImportImage_bombardier = new imagehandling::IMGToSurface( bombardier_png, kCYAN , spriteSize::GetSize128());

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////NEW ENTITY PICTURES //////////////////////////////////////////
//////////////////////////////////HERE///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


//Missile Launcher Entity
//const char* ml_speare  = "../PictureFiles/Modules/MissileLauncher/Speare/Speare_.png";
//const imagehandling::IMGToSurface*  ImportImage_ml_speare = new imagehandling::IMGToSurface( ml_speare, kCYAN , spriteSize::GetSize16() );
//
////Radar
//const char* rad_LSR = "../PictureFiles/Modules/Radar/LocalScopeArray/LocalScopeArray_.png";
//const imagehandling::IMGToSurface*  ImportImage_rad_LSR = new imagehandling::IMGToSurface( rad_LSR, kCYAN , spriteSize(8));
//
////Engine Entity
//const char* eng_brownian_png  = "../PictureFiles/Modules/Engine/Mechanical/Brownian/Brownian_.png";
//const imagehandling::IMGToSurface*  ImportImage_engine_brownian = new imagehandling::IMGToSurface( eng_brownian_png, kCYAN , spriteSize::GetSize16());
//
////Power Supply
//const char* ps_odeen3k  = "../PictureFiles/Modules/PowerSupply/Odeen3000/Odeen3000_.png";
//const imagehandling::IMGToSurface*  ImportImage_ps_odeen3k = new imagehandling::IMGToSurface( ps_odeen3k, kCYAN , spriteSize(8));
//
////Controller
//const char* cc_odeenCB  = "../PictureFiles/Modules/Controller/Odeen_CB/Odeen_CB_.png";
//const imagehandling::IMGToSurface*  ImportImage_cc_odeenCB = new imagehandling::IMGToSurface( cc_odeenCB, kCYAN , spriteSize(8));
