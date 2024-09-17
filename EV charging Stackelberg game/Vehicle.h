//
// Created by 王琦 on 18/10/2023.
//

#ifndef ROADNETWORK_VEHICLE_H
#define ROADNETWORK_VEHICLE_H
#define Max 503

class Vehicle {

public:
    int firstRecharge;
    int secondRecharge;
    int vehicleId;
    int currentCityId; //0-11
    int originCityId;
    int destinationId; // 1-11
    int capacityRange; // in kilometers
    int remainRange; //in kilometers

    Vehicle(int vi, int oi, int di, int cr, int rr) {
        currentCityId = 0;
        originCityId = oi;
        vehicleId = vi;
        destinationId = di;
        capacityRange = cr;
        remainRange = rr;
        firstRecharge = -1;
        secondRecharge = -1;
    }

    int furthestChargeStation(int ocid, int ccid, int range) const {
        if (ccid == -1)
            return -1;

        int total = 0;
        if (ocid < destinationId ) {
            for (int i = ccid + 1; i <= destinationId; i++) {
                total += distanceMap[i];
                if (total > range) {
                    return i - 1;
                }
            }
        }
        if (ocid > destinationId) {
            for (int i = ccid; i > destinationId; i--) {
                total += distanceMap[i];
                if (total > range) {
                    return i + 1;
                }
            }
        }

        return -1;
    }

//可选择的最早的充电桩
    int earliestChargeStation(int ocid, int range) const {

        int total = 0;
        if (ocid < destinationId) {
            for (int i = destinationId; i > ocid; i--) {
                total += distanceMap[i];
                if (total > range) {
                    return i + 1;
                }
            }
            return 0;
        }
        if (ocid > destinationId) {
            for (int i = destinationId + 1; i <= ocid; i++) {
                total += distanceMap[i];
                if (total > range) {
                    return i - 1 ;
                }
            }
            return 11;

        }

    }

    int DistanceToOrigin(int ocId)const{
        int distance = 0;
        if(ocId <= destinationId){
            for(int i = ocId+1; i <= destinationId; i++ ){
                distance += distanceMap[i];
            }
        }else{
            for (int i = ocId; i > destinationId; i--) {
                distance += distanceMap[i];
            }
        }
        return distance;
    }

    int getDistanceToDestination(int ccId) const{
        int distance = 0;
        if(ccId <= destinationId){
            for(int i = ccId+1; i <= destinationId; i++ ){
                distance += distanceMap[i];
            }
        }else{
            for (int i = ccId; i > destinationId; i--) {
                distance += distanceMap[i];
            }
        }
        return capacityRange- distance;
    }


    int getFirstRecharge() {
        return firstRecharge;
    }

    int getSecondRecharge() {
        return secondRecharge;
    }

    void setLatestCharging() {
        firstRecharge = furthestChargeStation(originCityId, originCityId, remainRange);

        if (firstRecharge != -1)
            secondRecharge = furthestChargeStation(originCityId, firstRecharge,
                                                   capacityRange);
        else
            secondRecharge = -1;
    }


    void displayVehicleInformation(int type = 1) const {
//		cout << "Vehicle Id" << setw(20)  << "Current Location" << setw(20)<< "Destination" << setw(20)
//				<< "Capacity Range" << setw(20) << "Remaining Range" << setw(20) << "First charge" << endl;

        cout << setw(5) << vehicleId;
        cout << setw(20) << nameMap[originCityId];
        if (type == 1)
            cout << setw(20) << nameMap[currentCityId];

        cout << setw(20) << nameMap[destinationId];
        cout << setw(20) << capacityRange;
        cout << setw(20) << remainRange;

        if (type == 2) {
            if (firstRecharge != -1) {
                cout << setw(20) << nameMap[firstRecharge];
            } else
                cout << setw(20) << "----"; // no need to charging

            if (secondRecharge != -1)
                cout << setw(20) << nameMap[secondRecharge];
            else
                cout << setw(20) << "----"; // no need to charging
        }
        cout << setw(20) << DistanceToOrigin(originCityId);

        cout << endl;
    }
};

#endif //ROADNETWORK_VEHICLE_H
