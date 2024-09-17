//
// Created by 王琦 on 24/10/2023.
//

#ifndef ROADNETWORK_VEHICLENETWORK_H
#define ROADNETWORK_VEHICLENETWORK_H

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

    void displayVehicleInformation() const {
//		cout << "Vehicle Id" << setw(20)  << "Current Location" << setw(20)<< "Destination" << setw(20)
//				<< "Capacity Range" << setw(20) << "Remaining Range" << setw(20) << "First charge" << endl;

        cout << setw(5) << vehicleId;
        cout << setw(20) << nameMap[originCityId];
        cout << setw(20) << nameMap[destinationId];
        cout << setw(20) << capacityRange;
        cout << setw(20) << remainRange;

        if (firstRecharge != -1) {
            cout << setw(20) << nameMap[firstRecharge];
        } else
            cout << setw(20) << "----"; // no need to charging

//        if (secondRecharge != -1)
//            cout << setw(20) << nameMap[secondRecharge];
//        else
//            cout << setw(20) << "----"; // no need to charging

//        cout << endl;
    }

};

#endif //ROADNETWORK_VEHICLENETWORK_H
