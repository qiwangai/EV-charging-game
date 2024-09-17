//
// Created by 王琦 on 18/10/2023.
//

#ifndef ROADNETWORK_CHARGINGALLOCATION_H
#define ROADNETWORK_CHARGINGALLOCATION_H
#define Max 503

class ChargingAllocation {
private:
    ChargingStation *stations[NUM_CITIES];

    void writeLine(ofstream &, int, int, int, int, long) const;

    vector<Vehicle> demands;
    double overallWaitingTime;

public:
    ChargingAllocation() {
        for (int i = 0; i < NUM_CITIES; i++)
            stations[i] = new ChargingStation(i);

        overallWaitingTime = 0;
    }

    ~ChargingAllocation() {
        for (int i = 0; i < NUM_CITIES; i++)
            delete stations[i];
    }

    void readDemandList();

    double statistics(vector<Vehicle> &demandList);

//    void balancing();

    void displayDemandList() const;

    void displayVehicleList(int = 1) const;

    void displayChargingStations(int = 1);

    void displayImprovedAllocation();

    void balancingMultiOriginDestination();
};

void ChargingAllocation::readDemandList() {
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
                Vehicle demand(values[0], values[1], values[2], values[3], values[4]);
                demand.setLatestCharging();
                demands.push_back(demand);
            }
            if (c != ']') {
                cout << "Problem of reading file" << endl;
                break;
            }

        }
    }
    infile.close();
}

double ChargingAllocation::statistics(vector<Vehicle> &demandList) {

    for (int i = 0; i < NUM_CITIES; i++)
        stations[i]->resetLength();


    for (int i = 0; i < demandList.size(); i++) {
        if (demandList[i].firstRecharge != -1) {
            stations[demandList[i].firstRecharge]->addToQueue();
        }

        if (demandList[i].secondRecharge != -1)
            stations[demandList[i].secondRecharge]->addToQueue();
    }

    double total = 0;
    int v = 0;
    for (int i = 0; i < NUM_CITIES; i++) {
        v += stations[i]->getLengthofQueue();
        total += stations[i]->getLengthofQueue()
                 * stations[i]->getWaitingTime();
    }
    cout << v << "queue Length" << endl;
    return total / (double) v;
}

void ChargingAllocation::balancingMultiOriginDestination() {
    vector<Vehicle> bestDemands(demands);
//    for (int k = 1; k < SIMULATION_TIMES; k++) {

        vector<Vehicle> tempDemands(demands);
        for (int i = 0; i < tempDemands.size(); i++) {
            double minCost = Max;
            if (tempDemands[i].firstRecharge != -1) {

                if (tempDemands[i].originCityId < tempDemands[i].destinationId) {
                    // Sydney to Melbourne
                    for (int j = 0; j < NUM_CITIES; j++) {
                        int distanceToOrigin = stations[j]->getDistanceToOrigin(tempDemands[i].originCityId);
                        if (tempDemands[i].remainRange >= distanceToOrigin && j < tempDemands[i].destinationId &&
                            j >= tempDemands[i].originCityId) {
                            int newFirstChargingStation = j;
                            int newSecondChargingStation = tempDemands[i].furthestChargeStation(
                                    tempDemands[i].originCityId, j, tempDemands[i].capacityRange);
                            int preSecondChargingStation = tempDemands[i].secondRecharge;
                            if (newSecondChargingStation != -1 && preSecondChargingStation != -1) {
                                if (tempDemands[i].getDistanceToDestination(j) >= 0) {
                                    double firstCost = stations[j]->getWaitingTime() +
                                                       stations[j]->getChargingPricing() *
                                                       (tempDemands[i].capacityRange -
                                                        (tempDemands[i].remainRange -
                                                         distanceToOrigin));
                                    double secondCost = stations[newSecondChargingStation]->getWaitingTime() +
                                                        stations[newSecondChargingStation]->getChargingPricing() *
                                                        (tempDemands[i].capacityRange - (tempDemands[i].capacityRange -
                                                                                         stations[newSecondChargingStation]->getDistanceToFirstChargingStation(
                                                                                                 j)));
                                    double totalCost = firstCost + secondCost;
                                    if (totalCost < minCost) {
                                        minCost = totalCost;
                                        tempDemands[i].firstRecharge = newFirstChargingStation;
                                        tempDemands[i].secondRecharge = newSecondChargingStation;
//                                        stations[j]->ChargingPricingPlus();
                                    } else {
//                                        stations[j]->ChargingPricingDecrease();
                                    }

                                }

                            } else if (newSecondChargingStation == -1 && preSecondChargingStation == -1) {
                                double totalCost = stations[j]->getWaitingTime() +
                                                   stations[j]->getChargingPricing() * (tempDemands[i].capacityRange -
                                                                                        (tempDemands[i].remainRange -
                                                                                         distanceToOrigin));
                                if (totalCost < minCost) {
                                    minCost = totalCost;
                                    tempDemands[i].firstRecharge = newFirstChargingStation;
//                                    stations[j]->ChargingPricingPlus();
                                } else {
//                                    stations[j]->ChargingPricingDecrease();
                                }
                            }
                        }
                    }
                } else {
                    for (int j = NUM_CITIES; j >= 0; j--) {
                        //Melbourne to Sydney
                        int distanceToOrigin = stations[j]->getDistanceToOrigin(tempDemands[i].originCityId);
                        if (tempDemands[i].remainRange >= distanceToOrigin &&
                            j > tempDemands[i].destinationId && j <= tempDemands[i].originCityId) {
                            int newFirstChargingStation = j;
                            int newSecondChargingStation = tempDemands[i].furthestChargeStation(
                                    tempDemands[i].originCityId, j, tempDemands[i].capacityRange);
                            int preSecondChargingStation = tempDemands[i].secondRecharge;

                            if (newSecondChargingStation != -1 && preSecondChargingStation != -1) {

                                if (tempDemands[i].getDistanceToDestination(j) >= 0) {
                                    double firstCost = stations[j]->getWaitingTime() +
                                                       stations[j]->getChargingPricing() *
                                                       (tempDemands[i].capacityRange -
                                                        (tempDemands[i].remainRange - distanceToOrigin));
                                    double secondCost = stations[newSecondChargingStation]->getWaitingTime() +
                                                        stations[newSecondChargingStation]->getChargingPricing() *
                                                        (tempDemands[i].capacityRange - (tempDemands[i].capacityRange -
                                                                                         stations[newSecondChargingStation]->getDistanceToFirstChargingStationForMel(tempDemands[i].originCityId)));
                                    double totalCost = firstCost + secondCost;
                                    if (totalCost < minCost) {
                                        minCost = totalCost;
                                        tempDemands[i].firstRecharge = newFirstChargingStation;
                                        tempDemands[i].secondRecharge = newSecondChargingStation;
//                                        stations[j]->ChargingPricingPlus();
                                    } else {
//                                        stations[j]->ChargingPricingDecrease();
                                    }
                                }

                            } else if (newSecondChargingStation == -1 && preSecondChargingStation == -1) {
                                double totalCost = stations[j]->getWaitingTime() +
                                                   stations[j]->getChargingPricing() * (tempDemands[i].capacityRange -
                                                                                        (tempDemands[i].remainRange -
                                                                                         distanceToOrigin));
                                if (totalCost < minCost) {
                                    minCost = totalCost;
                                    tempDemands[i].firstRecharge = newFirstChargingStation;
//                                    stations[j]->ChargingPricingPlus();
                                } else {
//                                    stations[j]->ChargingPricingDecrease();
                                }
                            }
                        }


                    }


                }
            }

        }
        double currentWT = statistics(tempDemands);
        if (currentWT < overallWaitingTime) {
            demands = tempDemands;
            overallWaitingTime = currentWT;
            bestDemands = tempDemands;
            cout << "Improved overall average waiting time = "
                 << overallWaitingTime << " hours at simulation "
                 << endl;
        }
//    }

}

//void ChargingAllocation::balancing() {
//    cout << "\nBalancing waiting queues with Monte-Carlo simulations. \nNumber of simulations = " << SIMULATION_TIMES
//         << endl;
//
//    vector<Vehicle> bestDemands(demands);
////    for (int k = 1; k < SIMULATION_TIMES; k++) {
//    vector<Vehicle> tempDemands(demands);
//    for (int i = 0; i < tempDemands.size(); i++) {
//        double minCost = Max;
//        if (tempDemands[i].firstRecharge != -1) {
//
//            if (tempDemands[i].originCityId == 0) {
//
//                for (int j = 0; j < NUM_CITIES; j++) {
//                    int distanceToOrigin = stations[j]->getDistanceToSydney();
//                    // origin is sydney
//                    if (tempDemands[i].remainRange > distanceToOrigin &&
//                        j < tempDemands[i].destinationId && tempDemands[i].originCityId == 0) {
//
//                        int newFirstChargingStation = j;
//                        int newSecondChargingStation = tempDemands[i].furthestChargeStation(tempDemands[i].originCityId,
//                                                                                            j,
//                                                                                            tempDemands[i].capacityRange);
//                        int preSecondChargingStation = tempDemands[i].secondRecharge;
//
//                        if (newSecondChargingStation != -1 && preSecondChargingStation != -1) {
//
//                            if (j >= tempDemands[i].earliestChargeStation(tempDemands[i].originCityId,
//                                                                          2 * tempDemands[i].capacityRange) &&
//                                j <= tempDemands[i].firstRecharge) {
//                                double firstCost = stations[j]->getWaitingTime() +
//                                                   stations[j]->getChargingPricing() * (tempDemands[i].capacityRange -
//                                                                                        (tempDemands[i].remainRange -
//                                                                                         distanceToOrigin));
//                                double secondCost = stations[newSecondChargingStation]->getWaitingTime() +
//                                                    stations[newSecondChargingStation]->getChargingPricing() *
//                                                    (tempDemands[i].capacityRange - (tempDemands[i].capacityRange -
//                                                                                     stations[newSecondChargingStation]->getDistanceToFirstChargingStation(
//                                                                                             j)));
//                                double totalCost = firstCost + secondCost;
//                                if (totalCost < minCost) {
//                                    minCost = totalCost;
//                                    tempDemands[i].firstRecharge = newFirstChargingStation;
//                                    tempDemands[i].secondRecharge = newSecondChargingStation;
//                                         stations[j]->ChargingPricingPlus();
//                                } else {
//                                         stations[j]->ChargingPricingDecrease();
//                                }
//                            }
//
//                        } else if (newSecondChargingStation == -1 && preSecondChargingStation == -1) {
//                            double totalCost = stations[j]->getWaitingTime() +
//                                               stations[j]->getChargingPricing() * (tempDemands[i].capacityRange -
//                                                                                    (tempDemands[i].remainRange -
//                                                                                     distanceToOrigin));
//                            if (totalCost < minCost) {
//                                minCost = totalCost;
//                                tempDemands[i].firstRecharge = newFirstChargingStation;
//                                   stations[j]->ChargingPricingPlus();
//                            } else {
//                                stations[j]->ChargingPricingDecrease();
//                            }
//
//                        }
//
//
//                    }
//                }
//            }
//                // origin is Melbourne
//                if (tempDemands[i].originCityId == 11) {
//
//                    for (int j = NUM_CITIES; j >= 0; j--) {
//                        int distanceToOrigin = stations[j]->getDistanceToMelbourne();
//
//                        if (tempDemands[i].remainRange > distanceToOrigin &&
//                            j > tempDemands[i].destinationId && tempDemands[i].originCityId == 11) {
//
//                            int newFirstChargingStation = j;
//                            int newSecondChargingStation = tempDemands[i].furthestChargeStation(
//                                    tempDemands[i].originCityId, j, tempDemands[i].capacityRange);
//                            int preSecondChargingStation = tempDemands[i].secondRecharge;
//
//                            if (newSecondChargingStation != -1 && preSecondChargingStation != -1) {
//
//                                if (j <= tempDemands[i].earliestChargeStation(tempDemands[i].originCityId,
//                                                                              2 * tempDemands[i].capacityRange) &&
//                                    j >= tempDemands[i].firstRecharge) {
//                                    double firstCost = stations[j]->getWaitingTime() +
//                                                       stations[j]->getChargingPricing() *
//                                                       (tempDemands[i].capacityRange -
//                                                        (tempDemands[i].remainRange - distanceToOrigin));
//                                    double secondCost = stations[newSecondChargingStation]->getWaitingTime() +
//                                                        stations[newSecondChargingStation]->getChargingPricing() *
//                                                        (tempDemands[i].capacityRange - (tempDemands[i].capacityRange -
//                                                                                         stations[newSecondChargingStation]->getDistanceToFirstChargingStationForMel(
//                                                                                                 j)));
//                                    double totalCost = firstCost + secondCost;
//                                    if (totalCost < minCost) {
//                                        minCost = totalCost;
//                                        tempDemands[i].firstRecharge = newFirstChargingStation;
//                                        tempDemands[i].secondRecharge = newSecondChargingStation;
//                                             stations[j]->ChargingPricingPlus();
//                                    } else {
//                                             stations[j]->ChargingPricingDecrease();
//                                    }
//                                }
//
//                            } else if (newSecondChargingStation == -1 && preSecondChargingStation == -1) {
//                                double totalCost = stations[j]->getWaitingTime() +
//                                                   stations[j]->getChargingPricing() * (tempDemands[i].capacityRange -
//                                                                                        (tempDemands[i].remainRange -
//                                                                                         distanceToOrigin));
//                                if (totalCost < minCost) {
//                                    minCost = totalCost;
//                                    tempDemands[i].firstRecharge = newFirstChargingStation;
//                                       stations[j]->ChargingPricingPlus();
//                                } else {
//                                       stations[j]->ChargingPricingDecrease();
//                                }
//                            }
//                        }
//                    }
//                }
//        }
//    }
//    double currentWT = statistics(tempDemands);
//    if (currentWT < overallWaitingTime) {
//        demands = tempDemands;
//        overallWaitingTime = currentWT;
//        bestDemands = tempDemands;
////            cout << "Improved overall average waiting time = "
////                 << overallWaitingTime << " hours at simulation " << k
////                 << endl;
//    }
//
//
//
////    }
//
//
//}

void ChargingAllocation::displayDemandList() const {
    cout << "Total demands received: " << demands.size() << endl;

    cout << "Vehicle Id" << setw(20) << "Destination Id" << setw(20)
         << "Capacity Range" << setw(20) << "Remaining Range" << endl;

    for (int i = 0; i < demands.size(); i++) {
        cout << setw(5) << demands[i].vehicleId;
        cout << setw(20) << demands[i].destinationId;
        cout << setw(20) << demands[i].capacityRange;
        cout << setw(20) << demands[i].remainRange;
        cout << endl;
    }
}

void ChargingAllocation::displayVehicleList(int type) const {
    if (type == 1) {
        cout << "Vehicle Id" << setw(15) << "Origin" << setw(20)
             << "Destination" << setw(20) << "Capacity Range" << setw(20)
             << "Remaining Range" << endl;

        for (int i = 0; i < demands.size(); i++) {
            demands[i].displayVehicleInformation();
        }
    } else {
        cout << "Vehicle Id" << setw(15) << "Origin" << setw(20) << "Destination" << setw(20)
             << "Capacity Range" << setw(20) << "Remaining Range" << setw(20)
             << "First recharge" << setw(20) << "Second recharge" << setw(25) << "Distance to Origin" << endl;

        for (int i = 0; i < demands.size(); i++) {
            demands[i].displayVehicleInformation(2);
        }

    }
}

void ChargingAllocation::displayChargingStations(int type) {
    if (type == 1) {
        cout << "Location Id" << setw(20) << "Location name" << setw(20) << "no of Chargers"
             << setw(20) << endl;

        for (int i = 0; i < NUM_CITIES; i++) {
            stations[i]->displayStation();
        }
    } else {
        overallWaitingTime = statistics(demands);
        cout << "Location Id" << setw(20) << "Location name" << setw(20)
             << "no of Chargers"
             << setw(20) << "Queue length" << setw(20) << "Charging Pricing"
             << setw(20) << "Waiting hours"
             << endl;

        for (int i = 0; i < NUM_CITIES; i++) {
            stations[i]->displayStation(2);
        }
        cout << "Overall average waiting time per vehicle = "
             << overallWaitingTime << " hours" << endl;
    }
}

#endif //ROADNETWORK_CHARGINGALLOCATION_H
