/*
 * EVCharging.h
 *
 *  Created on: 29 Apr 2023
 *      Author:
 */

#ifndef EVCHARGING_H_
#define EVCHARGING_H_

class EVCharging {
private:
    map<int, Location> locations;
    int numberOfLocations;
    WeightedGraphType *weightedGraph;

    void writeLine(ofstream &, int, int, int, int, long) const;

    map<int, double> chargingStationPricing;
    map<int, int> vehicleBestSelection;

    vector<Vehicle *> demands;
    double overallWaitingTime;

    int getIndex(string locationName) {
        for (const auto &el : locations)
            if (el.second.locationName == locationName)
                return el.first;
        return -1;
    }

    int getInputofLocation() {
        cout << "Input a location:" << endl;
        string locationName;
        getline(cin, locationName);
        if (locationName.size() == 0)
            getline(cin, locationName);
        int index = getIndex(locationName);

        if (index == -1) {
            cout << "There is no location " << locationName << endl;
        }

        return index;
    }

    int cheapestChargingStation(int origin, int destination, int avoid,
                                int chargingAmount, double &travelCost, double &chargingCost) {
        vector<double> shortestPath1 = weightedGraph->shortestPath(origin);
        vector<double> shortestPath2 = weightedGraph->shortestPath(destination);

        double lowestCost = DBL_MAX;
        int lowestId = -1;

        for (int i = 0; i < shortestPath1.size(); i++) {
            if (i == avoid)
                continue;

            double cost = (shortestPath1[i] + shortestPath2[i]) * 0.1
                          + chargingAmount * locations[i].chargingPrice;

            if (locations[i].numOfChargers > 0 && cost < lowestCost) {
                if (locations[i].chargingPrice > 0 || chargingAmount <= 25) {
                    lowestCost = cost;
                    lowestId = i;
                }
            }
        }

        travelCost = (shortestPath1[lowestId] + shortestPath2[lowestId]) * 0.1;
        chargingCost = chargingAmount * locations[lowestId].chargingPrice;

        return lowestId;
    }

public:
    EVCharging();

    ~EVCharging();

    void inputLocations();

    void inputVehicle();

    void printVehicles();

    void printLocations();

    void ChargingDemand();

    double statistics(vector<Vehicle *> &);

    void balancing();

    void printAdjacencyMatrix();

    void printAdjacencyList();

    void listPriceofChargingStations();

    void listAdjacentChargingStations();

    void cheapestAdjacentStations();

    void nearestChargingStation();

    void cheapestOtherStations();

    void cheapestChargingPathSingle();

    void cheapestChargingPathMultiple();

    void testGraph() {

        stack<int> path = weightedGraph->shortestPath(0, 9);

        for (int i = 0; i < path.size(); i++) {
            cout << locations[path.top()].locationName << ", ";
            path.pop();
        }


        cout << endl;

    }

    //Task 9
    int bestFreeChargingStation(int v1, int v2, int chargingAmount) {
        double lowestCost = DBL_MAX;
        int lowestId = -1;
        vector<double> shortestPathv1 = weightedGraph->shortestPath(v1);
        vector<double> shortestPathv2 = weightedGraph->shortestPath(v2);

        for (int i = 0; i < numberOfLocations; i++) {
            if (locations[i].numOfChargers > 0
                && (locations[i].chargingPrice == 0)) {
                double cost = 0.1 * (shortestPathv1[i] + shortestPathv2[i])
                              + locations[i].chargingPrice * chargingAmount;

                if (cost < lowestCost) {
                    lowestCost = cost;
                    lowestId = i;
                }

            }
        }
        return lowestId;
    }


//    void test();


    void chargingStationPrice();


    void StackelbergVersion2();

    void balancing2();

    void balancing3();

    void calculateInitialCosts();
};

EVCharging::EVCharging() {
    inputLocations();
    weightedGraph = new WeightedGraphType(numberOfLocations);
}

EVCharging::~EVCharging() {
    delete weightedGraph;
}

void EVCharging::inputLocations() {
    ifstream infile;
    char fileName[50] = "Locations.txt";

//	cout << "Enter charging location file name: ";
//	cin >> fileName;
//	cout << endl;

    infile.open(fileName);

    if (!infile) {
        cout << "Cannot open input file." << endl;
        return;
    }

    int locationIndex = 0;

    while (!infile.eof()) {
        Location s;
        string charger;
        string price;
        while (!infile.eof()) {
            getline(infile, s.locationName, ',');
            getline(infile, charger, ',');
            getline(infile, price);
            s.numOfChargers = stoi(charger);
            s.chargingPrice = stod(price);
            s.index = locationIndex;
            locations[locationIndex++] = s;
        }
    }

    numberOfLocations = locationIndex;
}

void EVCharging::inputVehicle() {
    ifstream infile;
//    infile.open("ChargingDemands.txt");
//    infile.open("test.txt");
    infile.open("ChargingDemandsTest.txt");
//    infile.open("ChargingDemandMelbourne.txt");

    if (infile.fail()) {
        cout << "File doesn't exist " << endl;
        exit(1);
    } else {
        cout << "Reading file..." << endl;

        while (!infile.eof()) {
            char c;
            infile >> c;
            if (c == '[') {
                int values[5];
                for (int i = 0; i < 5; i++) {
                    infile >> values[i];
                    infile >> c;
                }
                Vehicle *vptr = new Vehicle(values[0], values[1], values[2], values[3], values[4]);
                demands.push_back(vptr);
            }
            if (c != ']') {
                cout << "Problem of reading file" << endl;
                break;
            }

        }
    }
    infile.close();
}

void EVCharging::printVehicles() {

    cout << "Vehicle Id" << setw(20) << "Origin" << setw(20) << "Destination" << setw(20)
         << "Capacity Range" << setw(20) << "Remaining Range" << setw(20)
         << "First recharge" << setw(25) << "Distance To Destination" << endl;


    for (int i = 0; i < demands.size(); i++) {
        int tempOrigin = demands[i]->originCityId;
        vector<double> shortestPathiO = weightedGraph->shortestPath(tempOrigin);
        int tempDestiantion = demands[i]->destinationId;
        vector<double> shortestPathiD = weightedGraph->shortestPath(tempDestiantion);

        double min_result = DBL_MAX;
        double totalDistance = shortestPathiO[tempDestiantion];
        demands[i]->displayVehicleInformation();
        cout << setw(20) << totalDistance << endl;
    }

}

double EVCharging::statistics(vector<Vehicle *> &demandList) {

    for (int i = 0; i < locations.size(); i++) {
        locations[i].resetLength();
    }


    for (int i = 0; i < demandList.size(); i++) {
        if (demandList[i]->firstRecharge != -1) {
            locations[demandList[i]->firstRecharge].addToQueue();
        }
    }
    double totalWaitingTime = 0.0;
    int totalQueueLength = 0;
    for (int i = 0; i < locations.size(); i++) {
        if (locations[i].numOfChargers > 0) {
            totalQueueLength += locations[i].getLengthofQueue();
            totalWaitingTime += locations[i].getLengthofQueue() * locations[i].getWaitingTime();
        }

    }

    cout << totalQueueLength << "queue Length" << endl;
    cout << totalWaitingTime / (double) totalQueueLength << "Waiting Time" << endl;

    printLocations();
    return totalWaitingTime / (double) totalQueueLength;

}

void EVCharging::ChargingDemand() {

    for (int i = 0; i < demands.size(); i++) {
        int tempOrigin = demands[i]->originCityId;
        vector<double> shortestPathiO = weightedGraph->shortestPath(tempOrigin);
        int tempDestiantion = demands[i]->destinationId;
        vector<double> shortestPathiD = weightedGraph->shortestPath(tempDestiantion);

        double min_result = DBL_MAX;
        int selectChargingStation = -1;
        double totalDistance = shortestPathiO[tempDestiantion];
        for (int j = 0; j < locations.size(); j++) {
            if (locations[j].numOfChargers > 0 && shortestPathiO[j] <= demands[i]->remainRange &&
                totalDistance > demands[i]->remainRange) {
                double totalCost = shortestPathiO[j] * 0.1 +
                                   locations[j].chargingPrice * (demands[i]->capacityRange - shortestPathiO[j]) +
                                   shortestPathiD[j] * 0.1;
                if (totalCost < min_result) {
                    min_result = totalCost;
                    selectChargingStation = j;

                }
            }
        }
        if (selectChargingStation != -1) {
            demands[i]->firstRecharge = selectChargingStation;
        }
    }
    statistics(demands);

}

void EVCharging::balancing() {

    for (int simulation = 0; simulation < SIMULATION_TIMES; simulation++) {

        for (int chargingStationIndex = 0; chargingStationIndex < locations.size(); chargingStationIndex++) {

            overallWaitingTime = statistics(demands);
            cout << "before balancing overallWaitingTime: " << overallWaitingTime << endl;
            vector<Vehicle *> bestDemands(demands);
            vector<Vehicle *> tempDemands(demands);
            double currentWT;
            for (int i = 0; i < tempDemands.size(); i++) {
                int tempOrigin = tempDemands[i]->originCityId;
                vector<double> shortestPathiO = weightedGraph->shortestPath(tempOrigin);
                int tempDestiantion = tempDemands[i]->destinationId;
                vector<double> shortestPathiD = weightedGraph->shortestPath(tempDestiantion);

                double min_result = DBL_MAX;
                int selectChargingStation = -1;
                double totalDistance = shortestPathiO[tempDestiantion];
                for (int j = 0; j < locations.size(); j++) {
                    if (locations[j].numOfChargers > 0 && shortestPathiO[j] <= tempDemands[i]->remainRange &&
                        totalDistance > tempDemands[i]->remainRange) {
                        double totalCost = locations[j].getWaitingTime() * 30
                                           + locations[j].chargingPrice *
                                             (tempDemands[i]->capacityRange - shortestPathiO[j])
                                           + shortestPathiO[j] * 0.1 + shortestPathiD[j] * 0.1;

                        if (totalCost < min_result) {
                            min_result = totalCost;
                            selectChargingStation = j;
                        }
                    }
                }
                if (selectChargingStation != -1) {
                    tempDemands[i]->firstRecharge = selectChargingStation;
                }
                currentWT = statistics(tempDemands);
            }

            if (currentWT < overallWaitingTime) {
                demands = tempDemands;
                overallWaitingTime = currentWT;
                bestDemands = tempDemands;
                cout << "Improved overall average waiting time = "
                     << overallWaitingTime << " hours at simulation "
                     << endl;
            }

            //    double max_result = 1;
            //   double utility = locations[chargingStationIndex].chargingPrice*locations[chargingStationIndex].lengthofQueue  - locations[chargingStationIndex].getWaitingTime() * 0.5 ;

            if (locations[chargingStationIndex].getWaitingTime() > overallWaitingTime) {
                locations[chargingStationIndex].pricePlus();
            } else if (locations[chargingStationIndex].getWaitingTime() < overallWaitingTime &&
                       locations[chargingStationIndex].chargingPrice >= 0.02) {
                locations[chargingStationIndex].priceDecrease();
            }


        }

    }

}

void EVCharging::calculateInitialCosts() {
    ofstream initialCostsFile("initial_vehicle_costs.txt");
    initialCostsFile << "VehicleId, TotalCost" << endl;
    double totalCostSum = 0.0;
    int chargingVehicleCount = 0;
    const double waitingTimeCostPerHour = 5.0;

    for (int i = 0; i < demands.size(); i++) {
        int origin = demands[i]->originCityId;
        int destination = demands[i]->destinationId;
        int firstRecharge = demands[i]->firstRecharge;

        if (firstRecharge != -1) {
            double travelCost = weightedGraph->shortestPath(origin)[firstRecharge] * 0.1 +
                                weightedGraph->shortestPath(firstRecharge)[destination] * 0.1;
            double chargingCost = 250 * 0.28 * locations[firstRecharge].chargingPrice;
            double waitingTimeCost = locations[firstRecharge].getWaitingTime() * waitingTimeCostPerHour;
            double totalCost = travelCost + chargingCost *0 + waitingTimeCost ;

            totalCostSum += totalCost;
            chargingVehicleCount++;

            initialCostsFile << demands[i]->vehicleId << ", " << totalCost << endl;
        }
    }


    initialCostsFile.close();

    if (chargingVehicleCount > 0) {
        double averageCost = totalCostSum / chargingVehicleCount;
        cout << "Initial Average Cost for Charging Vehicles: " << averageCost  << endl;
    } else {
        cout << "No vehicles required charging initially." << endl;
    }
}


void EVCharging::balancing2() {
    ofstream outfile("vehicle_costs.txt");
    outfile << "VehicleId, TotalCost" << endl;
    double totalCostSum = 0.0;
    int chargingVehicleCount = 0;
    const double waitingTimeCostPerHour = 5.0;

    for (int simulation = 0; simulation < SIMULATION_TIMES; simulation++) {
        for (int chargingStationIndex = 0; chargingStationIndex < locations.size(); chargingStationIndex++) {
            overallWaitingTime = statistics(demands);
            cout << "before balancing overallWaitingTime: " << overallWaitingTime << endl;
            vector<Vehicle *> bestDemands(demands);
            vector<Vehicle *> tempDemands(demands);
            double currentWT;

            for (int i = 0; i < tempDemands.size(); i++) {
                int tempOrigin = tempDemands[i]->originCityId;
                vector<double> shortestPathiO = weightedGraph->shortestPath(tempOrigin);
                int tempDestination = tempDemands[i]->destinationId;
                vector<double> shortestPathiD = weightedGraph->shortestPath(tempDestination);

                double min_result = DBL_MAX;
                int selectChargingStation = -1;
                double totalDistance = shortestPathiO[tempDestination];
                for (int j = 0; j < locations.size(); j++) {
                    if (locations[j].numOfChargers > 0 && shortestPathiO[j] <= tempDemands[i]->remainRange &&
                        totalDistance > tempDemands[i]->remainRange) {
                        double totalCost = (locations[j].getWaitingTime() * 30) * 0.67
                                           + (locations[j].chargingPrice * 250) * 0
                                             //(tempDemands[i]->capacityRange - shortestPathiO[j])) * 0
                                           + (shortestPathiO[j] * 0.1 + shortestPathiD[j] * 0.1) * 0.33;

                        if (totalCost < min_result) {
                            min_result = totalCost;
                            selectChargingStation = j;
                        }
                    }
                }
                if (selectChargingStation != -1) {
                    tempDemands[i]->firstRecharge = selectChargingStation;
                }
                currentWT = statistics(tempDemands);
            }

            if (currentWT < overallWaitingTime) {
                demands = tempDemands;
                overallWaitingTime = currentWT;
                bestDemands = tempDemands;
                cout << "Improved overall average waiting time = "
                     << overallWaitingTime << " hours at simulation "
                     << endl;
            }

            if (locations[chargingStationIndex].getWaitingTime() > overallWaitingTime) {
                locations[chargingStationIndex].pricePlus();
            } else if (locations[chargingStationIndex].getWaitingTime() < overallWaitingTime &&
                       locations[chargingStationIndex].chargingPrice >= 0.02) {
                locations[chargingStationIndex].priceDecrease();
            }
        }
    }

    cout << "Number of demands: " << demands.size() << endl;
    for (int i = 0; i < demands.size(); i++) {
        int origin = demands[i]->originCityId;
        int destination = demands[i]->destinationId;
        int firstRecharge = demands[i]->firstRecharge;

        if (firstRecharge != -1) {
            double travelCost = weightedGraph->shortestPath(origin)[firstRecharge] * 0.1 +
                                weightedGraph->shortestPath(firstRecharge)[destination] * 0.1;
            double chargingCost = 250 * 0.28 * locations[firstRecharge].chargingPrice;
                   // (demands[i]->capacityRange - demands[i]->remainRange) *0.28

            double waitingTimeCost = locations[firstRecharge].getWaitingTime() * waitingTimeCostPerHour;
            double totalCost = travelCost + chargingCost * 0 + waitingTimeCost ;

            totalCostSum += totalCost;
            chargingVehicleCount++;

            outfile << demands[i]->vehicleId << ", " << totalCost << endl;
//            cout << "VehicleId: " << demands[i]->vehicleId << ", TotalCost: " << totalCost << endl;
        } else {
//            cout << "VehicleId: " << demands[i]->vehicleId << " has no assigned charging station." << endl;
        }
    }

    outfile.close();

    if (chargingVehicleCount > 0) {
        double averageCost = totalCostSum / chargingVehicleCount;
        cout << "Average Cost for Charging Vehicles: " << averageCost<< endl;
    } else {
        cout << "No vehicles required charging." << endl;
    }
}






//
//void EVCharging::StackelbergVersion2() {
//    cout <<"Sorted according to the queueing length"<<endl;
//
//    //using vector to find the longest queuing in the road network;
//    vector<Location> locationsQueulength;
//
//    for (map<int, Location>::iterator it = locations.begin();
//         it != locations.end(); it++) {
//        if (it->second.numOfChargers > 0)
//            locationsQueulength.push_back(it->second);
//    }
//
//    sort(locationsQueulength.begin(), locationsQueulength.end());
//
//    for(int iteration = 0; iteration <10; iteration++) {
//
//        overallWaitingTime = statistics(demands);
//        cout << "before balancing overallWaitingTime: " << overallWaitingTime << endl;
//        vector<Vehicle *> bestDemands(demands);
//        vector<Vehicle *> tempDemands(demands);
//
//        // All of charging stations are leaders.
//        for (int i = 0; i < locationsQueulength.size(); i++) {
//            //locationsQueulength[i].printLocation();
//            if (locationsQueulength[i].getWaitingTime() < overallWaitingTime) {
//                locationsQueulength[i].priceDecrease();
//            }else{
//                locationsQueulength[i].pricePlus();
//            }
//        }
//
//        // All of EV are followers.
//        for(int j = 0; j< demands.size(); j++){
//
//        }
//
//    }
//

//using priority queue;
//    priority_queue<Location> sortedChargingStation;
//    for(int i = 0; i <locations.size(); i++){
//        sortedChargingStation.push(locations[i]);
//    }
//
//    while(!sortedChargingStation.empty()){
//        Location CS = sortedChargingStation.top();
//        sortedChargingStation.pop();
//        CS.printLocation();
//
//    }
//}

void EVCharging::balancing3() {
    ofstream outfile("vehicle_costs.txt");
    outfile << "VehicleId, TotalCost" << endl;
    double totalCostSum = 0.0;
    int chargingVehicleCount = 0;
    const double waitingTimeCostPerHour = 300.0;
    const double costPrice = 0.2;

    double maxOverallProfit = -DBL_MAX;
    vector<Vehicle *> bestDemands(demands);
    map<int, double> bestPrices;

    for (int simulation = 0; simulation < SIMULATION_TIMES; simulation++) {
        overallWaitingTime = statistics(demands);
        cout << "balancing overallWaitingTime: " << overallWaitingTime << endl;

        vector<Vehicle *> tempDemands(demands);

        for (int i = 0; i < tempDemands.size(); i++) {
            int tempOrigin = tempDemands[i]->originCityId;
            vector<double> shortestPathiO = weightedGraph->shortestPath(tempOrigin);
            int tempDestination = tempDemands[i]->destinationId;
            vector<double> shortestPathiD = weightedGraph->shortestPath(tempDestination);

            double min_result = DBL_MAX;
            int selectChargingStation = -1;
            double totalDistance = shortestPathiO[tempDestination];
            for (int j = 0; j < locations.size(); j++) {
                if (locations[j].numOfChargers > 0 && shortestPathiO[j] <= tempDemands[i]->remainRange &&
                    totalDistance > tempDemands[i]->remainRange) {
                    double totalCost = (locations[j].getWaitingTime() * 30) * 0.33
                                       + (locations[j].chargingPrice * 250) * 0.33
                                       + (shortestPathiO[j] * 0.1 + shortestPathiD[j] * 0.1) * 0.33;

                    if (totalCost < min_result) {
                        min_result = totalCost;
                        selectChargingStation = j;
                    }
                }
            }
            if (selectChargingStation != -1) {
                tempDemands[i]->firstRecharge = selectChargingStation;
            }
        }


        double totalProfit = 0.0;
        for (int chargingStationIndex = 0; chargingStationIndex < locations.size(); chargingStationIndex++) {
            double profit = (locations[chargingStationIndex].chargingPrice - costPrice) * locations[chargingStationIndex].numOfChargers
                            - locations[chargingStationIndex].getWaitingTime() * waitingTimeCostPerHour;
            totalProfit += profit;
        }

        if (totalProfit > maxOverallProfit) {
            maxOverallProfit = totalProfit;
            bestDemands = tempDemands;
            for (int chargingStationIndex = 0; chargingStationIndex < locations.size(); chargingStationIndex++) {
                bestPrices[chargingStationIndex] = locations[chargingStationIndex].chargingPrice;
            }
            cout << "Improved overall profit to " << maxOverallProfit << " at simulation " << simulation << endl;
        }


        for (int chargingStationIndex = 0; chargingStationIndex < locations.size(); chargingStationIndex++) {
            double originalPrice = locations[chargingStationIndex].chargingPrice;
            double bestPrice = originalPrice;
            double maxProfit = -DBL_MAX;
            bool increasePrice = locations[chargingStationIndex].getWaitingTime() > overallWaitingTime;

            for (double price = 0.01; price <= 0.7; price += 0.01) {
                locations[chargingStationIndex].chargingPrice = price;
                double profit = (locations[chargingStationIndex].chargingPrice - costPrice) * locations[chargingStationIndex].numOfChargers
                                - locations[chargingStationIndex].getWaitingTime() * waitingTimeCostPerHour;

                if (increasePrice) {
                    if (profit > maxProfit && price > originalPrice) {
                        maxProfit = profit;
                        bestPrice = price;
                    }
                } else {
                    if (profit > maxProfit && price < originalPrice) {
                        maxProfit = profit;
                        bestPrice = price;
                    }
                }
            }

            locations[chargingStationIndex].chargingPrice = bestPrice;

            cout << "Adjusted price for station " << chargingStationIndex << " to " << locations[chargingStationIndex].chargingPrice << endl;
        }
    }

    demands = bestDemands;
    for (int chargingStationIndex = 0; chargingStationIndex < locations.size(); chargingStationIndex++) {
        locations[chargingStationIndex].chargingPrice = bestPrices[chargingStationIndex];
    }

    cout << "Number of demands: " << demands.size() << endl;
    for (int i = 0; i < demands.size(); i++) {
        int origin = demands[i]->originCityId;
        int destination = demands[i]->destinationId;
        int firstRecharge = demands[i]->firstRecharge;

        if (firstRecharge != -1) {
            double travelCost = weightedGraph->shortestPath(origin)[firstRecharge] * 0.1 +
                                weightedGraph->shortestPath(firstRecharge)[destination] * 0.1;
            double chargingCost = 250 * 0.28 * locations[firstRecharge].chargingPrice;

            double waitingTimeCost = locations[firstRecharge].getWaitingTime() * waitingTimeCostPerHour;
            double totalCost = travelCost + chargingCost + waitingTimeCost;

            totalCostSum += totalCost;
            chargingVehicleCount++;

            outfile << demands[i]->vehicleId << ", " << totalCost << endl;
        }
    }

    outfile.close();

    if (chargingVehicleCount > 0) {
        double averageCost = totalCostSum / chargingVehicleCount;
        cout << "Average Cost for Charging Vehicles: " << averageCost / 250 << endl;
    } else {
        cout << "No vehicles required charging." << endl;
    }
}









void EVCharging::printLocations() {
    cout << setw(8) << "Index" << setw(20) << "Location name" << setw(20)
         << "Charging station" << setw(20) << "Charging price" << setw(20) << "Waiting Time" << setw(20)
         << "Queuing Length" << endl;

    map<int, Location>::iterator it = locations.begin();

    for (; it != locations.end(); it++) {
        it->second.printLocation();
    }

}

void EVCharging::printAdjacencyMatrix() {
    cout
            << "Adjacency matrix (0 means no direct connection, non-zero value represents the distance of adjacent locations)\n"
            << endl;
    cout << setw(13) << " ";
    for (int i = 0; i < numberOfLocations; i++) {
        cout << setw(13) << locations[i].locationName;
    }
    cout << endl;
    for (int i = 0; i < numberOfLocations; i++) {
        cout << setw(13) << locations[i].locationName;
        for (int j = 0; j < numberOfLocations; j++) {
            cout << setw(13)
                 << (weightedGraph->getWeight(i, j) == DBL_MAX ?
                     0.0 : weightedGraph->getWeight(i, j));
        }
        cout << endl;
    }
}

//Task 3
void EVCharging::listPriceofChargingStations() {
    cout
            << "\nTask 3: list all the locations with charging stations in ascending order of charging price:"
            << endl;
    vector<Location> locationsinPrice;

    for (map<int, Location>::iterator it = locations.begin();
         it != locations.end(); it++) {
        if (it->second.numOfChargers > 0)
            locationsinPrice.push_back(it->second);
    }

    sort(locationsinPrice.begin(), locationsinPrice.end());

    for (int i = 0; i < locationsinPrice.size(); i++)
        locationsinPrice[i].printLocation();
}

//Task 4
void EVCharging::listAdjacentChargingStations() {
    cout << "\nTask 4: List adjacent charging stations. " << endl;
    //cin.ignore();
    int index = getInputofLocation();
    if (index == -1)
        return;

    bool found = false;
    for (int i : weightedGraph->getAdjancencyList(index)) {
        if (locations[i].numOfChargers > 0) {
            cout << locations[i].locationName << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No charging station adjacent to "
             << locations[index].locationName << "!" << endl;
}

//Task 5
void EVCharging::cheapestAdjacentStations() {
    cout
            << "\nTask 5: Find the adjacent charging stations with lowest charging cost. "
            << endl;

    int index = getInputofLocation();
    if (index == -1)
        return;

    int chargingAmount = rand() % 41 + 10;
    cout << "Charging amount: " << chargingAmount << "kWh" << endl;

    double lowestCost = DBL_MAX;
    int lowestId = -1;

    for (int i : weightedGraph->getAdjancencyList(index)) {
        double cost = weightedGraph->getWeight(index, i) * 2 * 0.1
                      + chargingAmount * locations[i].chargingPrice;
        if (locations[i].numOfChargers > 0 && cost < lowestCost) {
            if (locations[i].chargingPrice > 0 || chargingAmount <= 25) {
                lowestCost = cost;
                lowestId = i;
            }
        }
    }

    if (lowestId != -1)
        cout << "The cheapest next charging station is "
             << locations[lowestId].locationName << ". Charging cost= "
             << (double) lowestCost << endl;
    else
        cout << "No charging station adjacent to "
             << locations[index].locationName << "!" << endl;
}

//Task 6
void EVCharging::nearestChargingStation() {
    cout << "\nTask 6: Find the nearest charging station." << endl;
    int index = getInputofLocation();
    if (index == -1)
        return;

    vector<double> shortestPath = weightedGraph->shortestPath(index);

    double nearest = DBL_MAX;
    int nearestSI = 0;
    for (int i = 0; i < shortestPath.size(); i++) {
        if (i != index && locations[i].numOfChargers > 0
            && shortestPath[i] < nearest) {
            nearest = shortestPath[i];
            nearestSI = i;
        }
    }

    cout << locations[nearestSI].locationName << " in the distance of "
         << nearest << endl;
}

//Task 7
void EVCharging::cheapestOtherStations() {
    cout.precision(4);
    cout
            << "\nTask 7: Find the cheapest charging station other than the current location (including return travel cost). "
            << endl;

    int index = getInputofLocation();
    if (index == -1)
        return;

    int chargingAmount = rand() % 41 + 10;
    cout << "Charging amount: " << chargingAmount << "kWh" << endl;

    double travelCost, chargingCost;
    int lowestId = cheapestChargingStation(index, index, index, chargingAmount,
                                           travelCost, chargingCost);

    if (lowestId != -1) {
        cout << "The cheapest other charging station is "
             << locations[lowestId].locationName << "." << endl;
        cout << "Charging cost= " << chargingCost << endl;
        cout << "Travel cost= " << travelCost << endl;

        cout << "Total cost = " << travelCost + chargingCost << endl;

    } else
        cout << "No available charging station!" << endl;
}

//Task 8
void EVCharging::cheapestChargingPathSingle() {

    cout
            << "\nTask 8: Find the cheapest charging station between origin and destination. "
            << endl;

    int v1 = getInputofLocation();
    if (v1 == -1)
        return;

    int v2 = getInputofLocation();
    if (v2 == -1)
        return;

    int chargingAmount = rand() % 41 + 10;
    cout << "Charging amount: " << chargingAmount << "kWh" << endl;

    double travelCost, chargingCost;
    int lowestId = cheapestChargingStation(v1, v2, -1, chargingAmount,
                                           travelCost, chargingCost);

    cout.precision(4);

    if (lowestId != -1) {
        cout << "The cheapest other charging station is "
             << locations[lowestId].locationName << "." << endl;
        cout << "Charging cost= " << chargingCost << endl;
        cout << "Travel cost= " << travelCost << endl;

        cout << "Total cost = " << travelCost + chargingCost << endl;

    } else
        cout << "No available charging station!" << endl;

    cout << "Travel path: ";

    if (lowestId != v1) {
        stack<int> path1 = weightedGraph->shortestPath(v1, lowestId);

        while (path1.size() > 1) {
            cout << locations[path1.top()].locationName << ", ";
            path1.pop();
        }
    }

    stack<int> path2 = weightedGraph->shortestPath(lowestId, v2);
    while (!path2.empty()) {
        cout << locations[path2.top()].locationName << ", ";
        path2.pop();
    }

    cout << endl;
}

//Task 9
void EVCharging::cheapestChargingPathMultiple() {

    cout << "\nTask 9: Find the best way of charging (multiple charging). "
         << endl;
    cout.precision(4);
    int v1 = getInputofLocation();
    if (v1 == -1)
        return;

    int v2 = getInputofLocation();
    if (v2 == -1)
        return;

    int chargingAmount = rand() % 41 + 10;
    cout << "Charging amount: " << chargingAmount << "kWh" << endl;

    double travelCost, chargingCost;
    int freeCharging = cheapestChargingStation(v1, v2, -1, 25, travelCost,
                                               chargingCost);

    int lowestId = cheapestChargingStation(v1, v2, -1, chargingAmount,
                                           travelCost, chargingCost);

    if (chargingAmount <= 25 || locations[freeCharging].chargingPrice > 0) {
        if (lowestId != -1) {
            cout << "The best way of charging is to charge at "
                 << locations[lowestId].locationName << "." << endl;
            cout << "Charging cost = " << chargingCost << endl;
            cout << "Travel cost = " << travelCost << endl;

            cout << "Total cost = " << travelCost + chargingCost << endl;

        } else
            cout << "No available charging station!" << endl;

        cout << "Travel path: ";

        if (lowestId != v1) {
            stack<int> path1 = weightedGraph->shortestPath(v1, lowestId);

            while (path1.size() > 1) {
                cout << locations[path1.top()].locationName << ", ";
                path1.pop();
            }
        }

        stack<int> path2 = weightedGraph->shortestPath(lowestId, v2);
        while (!path2.empty()) {
            cout << locations[path2.top()].locationName << ", ";
            path2.pop();
        }

    } else {
        double travelCost1, chargingCost1, travelCost2, chargingCost2;
        int lowestIdl = v1, lowestIdr = v2;

//		int freeCharging = cheapestChargingStation(v1, v2, -1, 25, travelCost1,
//				chargingCost1);

//		if (locations[freeCharging].chargingPrice > 0) {
//			cout << "Something wrong" << endl;
//			return;
//		}

        if (freeCharging != v1) {
            lowestIdl = cheapestChargingStation(v1, freeCharging, freeCharging,
                                                chargingAmount - 25, travelCost1, chargingCost1);
        } else {
            travelCost1 = 0;
            chargingCost1 = DBL_MAX;
        }

        vector<double> shortestPath = weightedGraph->shortestPath(freeCharging);
        travelCost1 = travelCost1 + shortestPath[v2] * 0.1;

        if (freeCharging != v2) {
            lowestIdr = cheapestChargingStation(freeCharging, v2, freeCharging,
                                                chargingAmount - 25, travelCost2, chargingCost2);
        } else {
            travelCost2 = 0;
            chargingCost2 = DBL_MAX;
        }

        shortestPath = weightedGraph->shortestPath(v1);
        travelCost2 = travelCost2 + shortestPath[freeCharging] * 0.1;

        cout << "The best way of charging is " << endl;
        if (travelCost1 + chargingCost1 <= travelCost2 + chargingCost2) {
            cout << "Charging " << chargingAmount - 25 << "kWh at "
                 << locations[lowestIdl].locationName << endl;
            cout << "Charging 25kWh at " << locations[freeCharging].locationName
                 << endl;

            cout << "Total charging costs = " << chargingCost1 << endl;
            cout << "Total travel costs = " << travelCost1 << endl;

            cout << "Total costs = " << travelCost1 + chargingCost1 << endl;

            cout << "Travel path: ";
            if (lowestIdl != v1) {
                stack<int> path1 = weightedGraph->shortestPath(v1, lowestIdl);

                while (path1.size() > 1) {
                    cout << locations[path1.top()].locationName << ", ";
                    path1.pop();
                }
            } else {
                cout << locations[v1].locationName << ", "; //Check
            }

            stack<int> path2 = weightedGraph->shortestPath(lowestIdl,
                                                           freeCharging);
            while (!path2.empty()) {
                cout << locations[path2.top()].locationName << ", ";
                path2.pop();
            }

            if (freeCharging != v2) {
                stack<int> path3 = weightedGraph->shortestPath(freeCharging,
                                                               v2);
                while (!path3.empty()) {
                    cout << locations[path3.top()].locationName << ", ";
                    path3.pop();
                }
            }

        } else {
            cout << "Charging 25kWh at " << locations[freeCharging].locationName
                 << endl;
            cout << "Charging " << chargingAmount - 25 << "kWh at "
                 << locations[lowestIdr].locationName << endl;
            cout << "Total charging costs = " << chargingCost2 << endl;
            cout << "Total travel costs = " << travelCost2 << endl;
            cout << "Total costs = " << travelCost2 + chargingCost2 << endl;

            cout << "Travel path: ";
            if (freeCharging != v1) {
                stack<int> path1 = weightedGraph->shortestPath(v1,
                                                               freeCharging);

                while (path1.size() > 1) {
                    cout << locations[path1.top()].locationName << ", ";
                    path1.pop();
                }
            }

            stack<int> path2 = weightedGraph->shortestPath(freeCharging,
                                                           lowestIdr);
            while (path2.size() > 1) {
                cout << locations[path2.top()].locationName << ", ";
                path2.pop();
            }

            if (lowestIdr != v2) {
                stack<int> path3 = weightedGraph->shortestPath(lowestIdr, v2);
                while (!path3.empty()) {
                    cout << locations[path3.top()].locationName << ", ";
                    path3.pop();
                }
            }
        }
        cout << endl;
    }
}

#endif /* EVCHARGING_H_ */
