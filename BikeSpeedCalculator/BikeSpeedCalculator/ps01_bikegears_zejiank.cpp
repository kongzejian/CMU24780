//Full name: Zejian Kong
//Date: 29th Aug, 2023
//Course info: 24780 Engineering Computation
//The program calculates the gear ratios of a bike and shows the results
#include <iostream>
#include <iomanip>
using namespace std;
int main(void) {
	int frontGears[] = {0,22,32,44};
	int rearGears[] = {0,34,30,26,23,20,18,16,14,12};

	int sizeFront = sizeof(frontGears) / sizeof(frontGears[0]);
	int sizeRear = sizeof(rearGears) / sizeof(rearGears[0]);

	cout<<"24-780B Engineering Computation Prob Set 1 ¨C Bike Speed Calculator"<<endl;
	cout << "Gear Ratios:"<<endl;

	for (int i = 1; i < sizeFront; i++) {
		for (int j = 1; j < sizeRear; j++) {
			cout <<i<<"-" << j <<":" <<" " << setprecision(6) << (float)frontGears[i] / (float)rearGears[j] << endl;
		}
	}

	cout<<endl;
	cout << "Press the ENTER key to close the program >" << endl;
	system("pause");
	return 0;
}