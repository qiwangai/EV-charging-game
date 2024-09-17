#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <iomanip>
#include <queue>
#include <map>
#include <vector>

using namespace std;

#include "ConstantNew.h"
#include "vehicleNetwork.h"
#include "Location.h"
#include "WeightedGraph.h"
#include "EVCharging.h"

int main() {
	srand(time(0));

	EVCharging charging;
    charging.inputVehicle();
    charging.printVehicles();
    charging.printLocations();
    charging.ChargingDemand();
    charging.printVehicles();
//    charging.printLocations();
////
    charging.calculateInitialCosts();
    // to get te EV average cost before balancing.

//    charging.balancing();

////
//    charging.balancing2();

    // to get waiting time for stations and average cost for EVs.

//    charging.balancing3();
    // to get waiting time for stations and average cost for EVs.


//    charging.printVehicles();
//    charging.printLocations();

//    cout << "Choose a task to run: " << endl;
//	cout << "1. Print location information" << endl;
//	cout << "2. Print adjacency matrix" << endl;
//	cout << "3. Task 3 - list all charging stations in ascending order of charging price" << endl;
//	cout << "4. Task 4 - List adjacent charging stations" << endl;
//	cout << "5. Task 5 - Find the adjacent charging station with lowest travel & charging cost" << endl;
//	cout << "6. Task 6 - Find the nearest charging station" << endl;
//	cout << "7. Task 7 - Find the cheapest charging station other than the current location" << endl;
//	cout << "8. Task 8 - Find the cheapest charging station between origin and destination (single charging)" << endl;
//	cout << "9. Task 9 - Find the best way of charging (multiple charging)" << endl;
//	cout << "0. Quit" << endl;
//
//	int choice;
//	cin >> choice;
//	switch (choice) {
//	case 1:
//		charging.printLocations();
//		cout << endl;
//		break;
//	case 2:
//		charging.printAdjacencyMatrix();
//		break;
//	case 3:
//		charging.listPriceofChargingStations(); //Task 3
//		break;
//	case 4:
//		charging.listAdjacentChargingStations(); //Task 4
//		break;
//	case 5:
//		charging.cheapestAdjacentStations(); //Task 5
//		break;
//	case 6:
//		charging.nearestChargingStation(); //Task 6
//		break;
//	case 7:
//		charging.cheapestOtherStations(); //Task 7
//		break;
//	case 8:
//		charging.cheapestChargingPathSingle(); //Task 8
//		break;
//	case 9:
//		charging.cheapestChargingPathMultiple(); //Task 9
//		break;
//	case 0:
//		return 0;
//	default:
//		break;
//	}
//
//	return 0;
}
