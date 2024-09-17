//
//

#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include <queue>
#include<set>
#include<fstream>
#include<algorithm>

using namespace std;

#include "Constant.h"
#include "ChargingStation.h"
#include "Vehicle.h"
#include "DemandGenerator.h"
#include "ChargingAllocation.h"

int main() {
    srand(time(0));
	DemandGenerator generator;
	generator.generateDemands();
	generator.writeDemands();

//    ChargingAllocation allocation;
//    allocation.readDemandList();
////	allocation.displayDemandList();
//    cout <<"\nVechile information:" << endl;
//    allocation.displayVehicleList();
//    cout << endl;
//
//    cout <<"\nCharging station information:" << endl;
//    allocation.displayChargingStations();
//    cout << endl;
//
//    cout <<"\nInitial charging allocation ..." << endl;
//    allocation.displayVehicleList(2);
//    cout << endl;
//    allocation.displayChargingStations(2);
//
////    allocation.balancing();
//    allocation.balancingMultiOriginDestination();
//
//    cout <<"\nCharging allocation after balancing." << endl;
//    allocation.displayVehicleList(2);
//    cout << endl;
//    allocation.displayChargingStations(2);
//    cout << endl;

//	system("pause");
    return 0;
}
