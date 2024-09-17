/*
 * Station.h
 *
 *  Created on: 29 Apr 2023
 *      Author: 2002345
 */

#ifndef LOCATION_H_
#define LOCATION_H_

class Location {
public:
	int index;//Key
	string locationName;
//	bool chargerInstalled;
	int numOfChargers;
    int lengthofQueue = 0;
	double chargingPrice;
    void addToQueue(){
        lengthofQueue++;
    }

    int getLengthofQueue() {
        return lengthofQueue;
    }

    void resetLength() {
        lengthofQueue = 0;
    }

    double resetPrice(){
        chargingPrice = chargingStationPricing[index];
    }

    double getWaitingTime() {
        return 0.5 * (double)  lengthofQueue / numOfChargers;;
    }

    double pricePlus(){
        chargingPrice += 0.01;
    }

    double priceDecrease(){
        chargingPrice -= 0.01;
    }




	void printLocation() {
			cout << setw (6) << index << setw (20) << locationName << setw (15);

			if(numOfChargers > 0)
				cout << numOfChargers;
			else
				cout << 0;

			if(numOfChargers > 0 && chargingPrice == 0){
			    cout << setw (28) << "free of charge";
                cout << setw (23) <<getWaitingTime();
			}else if (numOfChargers == 0){
                cout << setw (17) << "N/A";
                cout << setw (28) <<0;
			}

			else{
                cout << setw(15) << "$" << setprecision(2) << chargingPrice << "/kWh";
                cout << setw (23) <<getWaitingTime();
			}


            cout << setw (18)<<getLengthofQueue()<<endl ;


    }

//	bool operator<(const Location& l)  const {//For task 3
//		return chargingPrice <l.chargingPrice;
//	}

    bool operator<(const Location& l)  const{
        return lengthofQueue< l.lengthofQueue;
    }


};



#endif /* LOCATION_H_ */
