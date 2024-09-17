//
//

#ifndef ROADNETWORK_DEMANDGENERATOR_H
#define ROADNETWORK_DEMANDGENERATOR_H

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<vector>

using namespace std;

#include "Vehicle.h"

class DemandGenerator {
private:
    vector<Vehicle> demandList;
public:
    DemandGenerator() {
        demandList.clear();
    }

    void generateDemands();
    void writeDemands();
};

void DemandGenerator::generateDemands() {
    srand(time(0)); // seed random number generator
    int listSize = MIN_DEMANDS + rand() % (MAX_DEMANDS - MIN_DEMANDS);

    for (int i = 0; i < 50; i++) {

        int vehicleId = i;
        int originCityId = rand() % (NUM_CITIES - 1) + 1;
        int destinationId = rand() % (NUM_CITIES - 1) + 1;
        int capacityRange = rand() % (MAX_CAPACITY - MIN_CAPACITY)
                            + MIN_CAPACITY;
        int remainRange = rand() % (30-10) +10;
        // int remainRange = MIN_REMAIN_RANGE + rand() % (capacityRange- MIN_REMAIN_RANGE);

        Vehicle demand(vehicleId, originCityId, destinationId, capacityRange, remainRange);

        demandList.push_back(demand);
    }
}

void DemandGenerator::writeDemands() {
    ofstream output("ChargingDemandsTest.txt", ios::out);
    if (!output) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    for (int i = 0; i < demandList.size(); i++) {
        output << "[";
        output << demandList[i].vehicleId;
        output << ",";
        output << demandList[i].originCityId;
        output << ",";
        output << demandList[i].destinationId;
        output << ",";
        output << demandList[i].capacityRange;
        output << ",";
        output << demandList[i].remainRange;
        output << "]" << endl;
    }

    cout
            << "A new charging demand list has been generated and output to file chargingDemands.txt"
            << endl;
}



#endif //ROADNETWORK_DEMANDGENERATOR_H
