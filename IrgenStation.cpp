//#include"IrgenStation.h"
//
//IrgenStation::IrgenStation()
//	: LocationData()
//{
//	this->SetInterior(InteriorModel("Irgen", true));
//	this->SetName("Irgen Carrier");
//
//
//
//
//	Coord pIrgenPos = Coord(0, 0);
//
//	ShipModel andrej = ShipModel("andrej", 1, Coord(1015, -835));
//	andrej.AddLink("24,0:Andrej:40,23:d_andrej");
//
//	ShipModel oleus = ShipModel("oleus", 2, Coord(1460, -500));
//	oleus.AddLink("24,0:Oleus:40,23:d_oleus");
//
//	ShipModel guard = ShipModel("orange suit", 3, Coord(1105, -860));
//	guard.AddLink("24,0:Guard:40,23:d_guard");
//
//	ShipModel commander = ShipModel("commander", 4, Coord(550, -650));
//	commander.AddLink("20,40:Commander:73,23:d_commander");
//
//	ShipModel officer = ShipModel("officer", 5, Coord(570, -650));
//	ShipModel osuit = ShipModel("orange suit", 6, Coord(590, -680));
//
//	//a_1.SetBehaviour(enActions::MOVETOTARGETCOORD); // (std::shared_ptr<BehaviorTree>(BAITree::generateTravelToCoordTree(primitives::waypoint(pIrgenPos, 50))));
//	//a_2.SetBehaviour(enActions::MOVETOTARGETCOORD);
//	//a_3.SetBehaviour(enActions::MOVETOTARGETCOORD);
//	//a_4.SetBehaviour(enActions::MOVETOTARGETCOORD);
//	//andrej.SetTargetPosition(primitives::waypoint(pIrgenPos, 1100, -0.2));
//	//a_2.SetTargetPosition(primitives::waypoint(pIrgenPos, 1200, -0.3));
//	//a_3.SetTargetPosition(primitives::waypoint(pIrgenPos, 800));
//	//a_4.SetTargetPosition(primitives::waypoint(pIrgenPos, 1100, 1.2));
//
//	ShipModel irgen = ShipModel("IRGEN", 20, pIrgenPos);
//
//
//	this->AddVessel(andrej, enumAllegiance::aFRIENDLY);
//	this->AddVessel(oleus, enumAllegiance::aFRIENDLY);
//	this->AddVessel(guard, enumAllegiance::aFRIENDLY);
//	this->AddVessel(commander, enumAllegiance::aFRIENDLY);
//	this->AddVessel(officer, enumAllegiance::aFRIENDLY);
//	this->AddVessel(osuit, enumAllegiance::aFRIENDLY);
//	//this->AddVessel(a_2, enumAllegiance::aENEMY);
//	//this->AddVessel(a_3, enumAllegiance::aENEMY);
//	//this->AddVessel(a_4, enumAllegiance::aENEMY);
//	//this->AddVessel(a_5, enumAllegiance::aENEMY);
//	////this->AddVessel(ShipModel("SHEPHERD", 19, Coord(0, -200)), enumAllegiance::aENEMY);
//	//this->AddVessel(irgen, enumAllegiance::aFRIENDLY);
//}
//
