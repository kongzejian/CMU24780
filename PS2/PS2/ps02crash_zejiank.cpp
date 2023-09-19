//Full name: Zejian Kong
//Andrew ID:zejiank
//Date: 9th Sep, 2023
//Course info: 24780 Engineering Computation
//The program allows users to calculate the speed of a plane and 
// maximum tension of a cable given some conditions, and compare 
// the users' answers with the correct result

#include<iostream>
#include<cmath>

//set parameters
#define PI 3.14159
#define G 9.81

using namespace std;

// round a double number to a specific decimal place
double rounding(double number,int decimalPlace){ 
    return round(number * pow(10, decimalPlace)) / pow(10, decimalPlace);
}



//generate a uniform distributed random number from lower_bound to higher_bound 
//and then round it to a specific decimal place
double generateRandomNumber(int lowerBound, int higherBound, int decimalPlace) { 
    double randNumber = lowerBound + (higherBound - lowerBound)*
    (double)rand() / RAND_MAX;
    return rounding(randNumber,decimalPlace);
}



//convert a degree to a radian
double degreeToRadian(double angle) { 
    return angle * PI / 180;
}



//calculate the final speed
double calculateFinalSpeed(double cableLength, double thetaFinal,double thetaInit ) { 
    return sqrt(2 * G * cableLength * (cos(degreeToRadian(thetaFinal)) - 
        cos(degreeToRadian(thetaInit))));
}



//calculate the maximum tension
double caculateMaxTension(double massPlane,double cableLength,double velFinal,double thetaFinal) { 
    return massPlane * pow(velFinal, 2) / cableLength + massPlane * G * cos(degreeToRadian(thetaFinal));
    

}



int main(void) {
    srand(time(NULL));

    //generate random data
    double massPlane = generateRandomNumber(6, 10, 1);
    double thetaInit = generateRandomNumber(50, 80, 0);
    double thetaFinal = generateRandomNumber(5, 20, 0);
    double cableLength = generateRandomNumber(15, 25, 0);

    //display the information
    cout << "The gantry structure in the photo is used to test the response of an " << endl;
    cout << "airplane during a crash. As shown, the plane, having a massPlane of "<<massPlane<<"Mg,"<< endl;
    cout << "is hoisted back until thetaInit is "<<thetaInit<<" degrees, and then the pull - back cable is " << endl;
    cout << "released when the plane is at rest. Determine the speed velFinal of the " << endl;
    cout << "plane just before crashing into the ground when thetaFinal = "<<thetaFinal<<" degrees." << endl;
    cout << " Also, what is the maximum tension maxTension developed in the supporting cable " << endl;
    cout << "(which has length of "<<cableLength<<" meters) during the motion ? Neglect the effect of " << endl;
    cout << "lift caused by the wings during the motion and the size of the airplane." << endl;


    //calculate the standard answer of final speed
    double velFinal = calculateFinalSpeed(cableLength, thetaFinal, thetaInit);
    velFinal = rounding(velFinal,1);

    //calculate the standard answer of maximum tension
    double maxTension = caculateMaxTension(massPlane, cableLength, velFinal, thetaFinal);
    maxTension = rounding(maxTension, 0);

    double userAnswerVelFinal;
    double userAnswerMaxTension;

    cout << "Please enter yout result of the speed velFinal of the plane"
        << "just before cashing into the ground : " << endl;
    cin >> userAnswerVelFinal;

    cout << "Please enter yout result of the maximum tension maxTension:" << endl;
    cin >> userAnswerMaxTension;

    //calculate the error
    double velocityError = rounding(abs(userAnswerVelFinal - velFinal) / velFinal,3);
    double tensionError=  rounding(abs(userAnswerMaxTension - maxTension) / maxTension, 3);

    cout << "The error of velocity is: " << velocityError << endl;
    cout << "The error of maximum tension is: " << tensionError << endl;

    //If the error is smaller than 1%, 
    // the result is determined to be correct
    if (velocityError<0.01&&tensionError<0.01) {
        cout << "Your answer is correct!" << endl;
    }
    else {
        cout << "Your answer is wrong!" << endl;
    }
	return 0;
}
