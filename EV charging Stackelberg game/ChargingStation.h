//
// Created by 王琦 on 18/10/2023.
//

#ifndef ROADNETWORK_CHARGINGSTATION_H
#define ROADNETWORK_CHARGINGSTATION_H

class ChargingStation {
private:
    int locationId;
    string locationName;
    int distanceToLast;
    int numberOfChargers;
    int lengthofQueue;
    double  chargingPricing;
public:
    ChargingStation(int lid) {
        locationId = lid;
        locationName = nameMap[lid];
        distanceToLast = distanceMap[lid];
        numberOfChargers = chargersMap[lid];
        chargingPricing = chargingStationPricing[lid];
        lengthofQueue = 0;
    }

    void resetLength() {
        lengthofQueue = 0;
    }

    string getLocationName() {
        return nameMap[locationId];
    }

    double getWaitingTime() {
        return 0.5 * (double) lengthofQueue / numberOfChargers;;
    }

    int getDistanceToOrigin(int ocId){
        int distance = 0;
        if(locationId > ocId){
            for(int i = ocId+1; i <= locationId; i++ ){
                distance += distanceMap[i];
            }
        }else if (locationId < ocId){
            for (int i = ocId; i > locationId; i--) {
                distance += distanceMap[i];
            }
        }
        return distance;
    }

    int getDistanceToFirstChargingStation(int Fid){
        int total = 0;
        for (int i = Fid+1; i <= locationId; i++)
            total += distanceMap[i];

        return total;
    }

    int getDistanceToFirstChargingStationForMel(int Oid){
        int total = 0;
        for (int i = Oid; i > locationId; i--)
            total += distanceMap[i];

        return total;
    }

    void addToQueue() {
        lengthofQueue++;
    }

    int getLengthofQueue() {
        return lengthofQueue;
    }
    double getChargingPricing(){
        return chargingPricing;
    }

    void ChargingPricingPlus(){
        chargingPricing +=0.01;
    }
    void ChargingPricingDecrease(){
        chargingPricing -=0.01;
    }

//    double utilityChargingStation(){
//        return chargingPricing*lengthofQueue*100;
//    }

    void displayStation(int type = 1) {
        cout << setw(5) << locationId;
        cout << setw(20) << nameMap[locationId];
//        cout << setw(20) << getDistanceToOrigin(locationId,);
        cout << setw(20) << numberOfChargers;

        if (type == 2) {
            cout << setw(20) << lengthofQueue;
            cout << setw(20) << chargingPricing;
//            cout << setw(20)<<utilityChargingStation();
            cout << setw(20) << setprecision(2) << fixed << getWaitingTime()
                 << " hours";
        }
        cout << endl;
    }
};

#endif //ROADNETWORK_CHARGINGSTATION_H
