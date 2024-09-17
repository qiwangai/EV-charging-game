//
// Created by 王琦 on 24/10/2023.
//

#ifndef ROADNETWORK_CONSTANTNEW_H
#define ROADNETWORK_CONSTANTNEW_H
const double chargingStationPricing[] = {0.1, -1, 0, -1, -1, 0.25, 0.25, 0.4, 0.46, 0.45, 0.75, 0, 0.55, 0.33, -1, -1, 0.3, 0.55, 0.3, -1, 0.55};
const string nameMap[] = {"Penrith", "St Marys", "Box Hill", "Kellyville ","Castle Hill", "Blacktown", "Parramatta",
                          "Olympic Park", "Burwood", "Mascot", "Bondi Juction", "Manly","Chatswood","Macquarie Pk", "Hornsby",
                          "Central","Hurstville","Bankstown","Liverpool","Leppington","Campbelltown"};

const int NUM_CITIES = 21;

const int MIN_DEMANDS = 15;

const int MAX_DEMANDS = 200;

const int MIN_CAPACITY = 150;

const int MAX_CAPACITY = 300;

const int MAX_REMAIN_RANGE = 30;

const int MIN_REMAIN_RANGE = 10;

const int SIMULATION_TIMES = 12;
#endif //ROADNETWORK_CONSTANTNEW_H
