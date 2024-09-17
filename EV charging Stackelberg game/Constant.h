//
//

#ifndef ROADNETWORK_CONSTANT_H
#define ROADNETWORK_CONSTANT_H

const int distanceMap[] = { 0, 57, 60, 83, 86, 99, 115, 62, 74, 87, 106, 62};

const int chargersMap[] = { 10, 4, 3, 4, 3, 3, 3, 4, 3, 3, 5, 8};

const double chargingStationPricing[] = {0.0, 0.75, 0.36, 0.5, 0.7, 0.59, 0.5, 0.40, 0.25, 0.3, 0.5, 0.0};

const string nameMap[] = {"Sydney", "Campbelltown", "Mittagong", "Goulburn",
                          "Yass", "Gundagai", "Holbrook",
                          "Albury", "Wangaratta", "Euroa", "Wallan", "Melbourne" };

const int NUM_CITIES = 21;

const int MIN_DEMANDS = 15;

const int MAX_DEMANDS = 200;

const int MIN_CAPACITY = 150;

const int MAX_CAPACITY = 300;

const int MIN_REMAIN_RANGE = 20;

const int SIMULATION_TIMES = 5000;

#endif //ROADNETWORK_CONSTANT_H
