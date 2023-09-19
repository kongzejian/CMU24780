//Full name: Zejian Kong
//Andrew ID:zejiank
//Date: 17th Sep, 2023
//Course info: 24780 Engineering Computation
//The program has functions of creating graphics of circle or rectangle,
//drawing graphics, read graphic information from a file, save a file about
//graphic information, and a user-interface.


#include "fssimplewindow.h"
#include<iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#define PI 3.1415926
using namespace std;

//enum for shape
enum Shape { CIRCLE, REC };

//definition of GraphicalItem struct
typedef struct GraphicalItem
{
	Shape shape;
	int xCoordinate;
	int yCoordinate;
	int height;
	int width;
	float red;
	float green;
	float blue;
}GraphicalItem;


//Function of creating graphic
GraphicalItem createItem(const string& input) {
	GraphicalItem graphic;

	//get the shape from the input
	int colonIndex = input.find(":");//the index of colon
	string shape= input.substr(0,colonIndex);
	if (shape == "CIRCLE") {
		graphic.shape = CIRCLE;
	}
	else {
		graphic.shape = REC;
	}


	//Get part of the string after the colon;parse is delimited by spaces
	string inputWithoutColon = input.substr(colonIndex + 1, input.size() - colonIndex);
	stringstream ss(inputWithoutColon);
	string word;
	string stringArray[7];
	int count = 0;
	while (ss >> word) {
		stringArray[count]=word;//create a array to store strings
		count++;
		if (count == 7) {
			break;
		}
	}

	//assign values to the struct graphic
	graphic.xCoordinate = stoi(stringArray[0]);
	graphic.yCoordinate = stoi(stringArray[1]);
	graphic.height = stoi(stringArray[2]);
	graphic.width = stoi(stringArray[3]);
	graphic.red = stof(stringArray[4]);
	graphic.green = stof(stringArray[5]);
	graphic.blue = stof(stringArray[6]);
	return graphic;
}



//function of printing values of the struct GraphicalItem
void printItem(const GraphicalItem& anItem, ostream& output = cout) {
	output << "Shape: " << (anItem.shape == CIRCLE ? "circle" : "rectangle") << endl;
	output << "the x-coord of center: " << anItem.xCoordinate<< endl;
	output << "the y-coord of center: " << anItem.yCoordinate << endl;
	output << "the height: " << anItem.height << endl;
	output << "the width: " << anItem.width << endl;
	output << "the red colur value: " << anItem.red << endl;
	output << "the green colur value: " << anItem.green << endl;
	output << "the blue colur value: " << anItem.blue << endl;
}


//function of drawing graphic
void drawItem(const GraphicalItem& anItem) {

	//get the values
	int xCoordinate = anItem.xCoordinate;
	int yCoordinate = 600-anItem.yCoordinate;// origin (0,0) as the lower left corner
	int height = anItem.height;
	int width = anItem.width;
	float red = anItem.red;
	float green = anItem.green;
	float blue = anItem.blue;
	
	//set the color
	glColor3f(red,green,blue);

	//the rectangle shape
	if (anItem.shape == REC) {
		glBegin(GL_QUADS);
		glVertex2i(xCoordinate-width/2,yCoordinate+height/2);
		glVertex2i(xCoordinate + width / 2, yCoordinate + height / 2);
		glVertex2i(xCoordinate + width / 2, yCoordinate - height / 2);
		glVertex2i(xCoordinate - width / 2, yCoordinate - height / 2);
	}

	//the circle or oval
	else {
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			float radian = i * PI/180;
			glVertex2f(xCoordinate+cos(radian) * width/2,
				yCoordinate+sin(radian) * height/2);
		}
	}
	glEnd();
}



//read graphic information from a file
void readItems(vector<GraphicalItem>& theItems, ifstream& inFile) {
	string longInput;
	while (!inFile.eof()) {
		getline(inFile, longInput);
		if (longInput.find(":") != string::npos) { //detect whether the string has colon
			theItems.push_back(createItem(longInput));
		}
	}
}


//write graphic information to a file
void writeItems(const vector<GraphicalItem>& theItems, ostream& output) {
	int length = theItems.size();
	for (int i = 0; i < length; i++) {
		string longOutput;
		GraphicalItem graphic = theItems[i];
		if (graphic.shape == CIRCLE) {
			longOutput = "CIRCLE: ";
		}
		else {
			longOutput = "RECT: ";
		}

		longOutput += to_string(graphic.xCoordinate)+" ";
		longOutput += to_string(graphic.yCoordinate) + " ";
		longOutput += to_string(graphic.height) + " ";
		longOutput += to_string(graphic.width) + " ";
		longOutput += to_string(graphic.red) + " ";
		longOutput += to_string(graphic.green) + " ";
		longOutput += to_string(graphic.blue);

		output << longOutput << endl;
	}
}



bool getuserinput(vector<GraphicalItem>& theitems)
{ // guides user through data input and makes changes to
// the collection of graphical items
	char userchoice;
	int inputint;
	string longinput, shapedescription;
	bool validresponse = false, stayinprogram = true;
	while (!validresponse) {
		cout << endl;
		cout << "make a selection: add (c)ircle, add (r)ectangle, " << endl;
		cout << " (d)elete item, clear (a)ll, " << endl;
		cout << " (l)oad file, (s)ave file, e(x)it >> ";
		cin >> userchoice;

		cin.clear(); cin.ignore(); // to clear up the input buffer
		if (userchoice == 'c' || userchoice == 'c'
			|| userchoice == 'r' || userchoice == 'r') { 
			
			// add a new shape
			// initialize description string
			if (userchoice == 'c' || userchoice == 'c')
				shapedescription = "circle: ";
			else
				shapedescription = "rect: ";
			// ask for parameters
			cout << " center of shape (x y) >> ";
			getline(cin, longinput);
			shapedescription += longinput + " ";
			cout << " size of shape (height width) >> ";
			getline(cin, longinput);
			shapedescription += longinput + " ";
			cout << " color of shape (red green blue) >> ";
			getline(cin, longinput);
			shapedescription += longinput;
			// create and add the item to collection
			theitems.push_back(createItem(shapedescription));
			validresponse = true;
		}


		else if (userchoice == 'd' || userchoice == 'd') { // delete single item
			if (theitems.size() < 1)
				cout << endl << " no items to delete" << endl;
			else {
				cout << " select index of item to delete (1 to "
					<< theitems.size() << ") >> ";
				cin >> inputint;
				if (0 < inputint && inputint <= theitems.size()) {
					// confirm first
					cout << "are you sure you want to delete the item:" << endl;
						cout << " ";
						printItem(theitems.at(inputint - 1));
					cout << " (y/n) >> ";
					cin >> userchoice;
					if (userchoice == 'y' || userchoice == 'y') {
						theitems.erase(theitems.begin() + inputint - 1);
						validresponse = true;
					}
				}
			}
		}


		else if (userchoice == 'a' || userchoice == 'a') { // clear all items
			if (theitems.size() < 1)
				cout << endl << " no items to delete" << endl;
			else {
				// confirm first
				cout << "are you sure you want to clear all items? (y/n) >> ";
				cin >> userchoice;
				if (userchoice == 'y' || userchoice == 'y') {
					theitems.clear();
					validresponse = true;
				}
				else
					validresponse = false;
			}
		}
		else if (userchoice == 'x' || userchoice == 'x') {
			stayinprogram = false;
			validresponse = true;
		}
		else if (userchoice == 'l' || userchoice == 'l') {
			// ask user for file name
			string inputFileName;
			cout << "Please enter the file name:" << endl;
			cin >> inputFileName;
			// create and open an ifstream, checking if it was able to open
			ifstream inputFile;
			inputFile.open(inputFileName, ios::in);
			if (inputFile.is_open()) {
				// call the readitems() function
				readItems(theitems, inputFile);
			}
			validresponse = true;
		}
		else if (userchoice == 's' || userchoice == 's') {
			// ask user for file name
			string outputFileName;
			cout << "Please enter the file name:" << endl;
			cin >> outputFileName;
			// create and open an ofstream, checking if it was able to open
			ofstream outputFile;
			outputFile.open(outputFileName, ios::out);
			if (outputFile.is_open()) {
				// call the writeitems() function
				writeItems(theitems, outputFile);
			}
			

			validresponse = true;
		}
		if (!validresponse)
			cout << endl << "no changes were recorded " << endl;
	}
	// return true if program should continue
	return stayinprogram;
}

int main() {
	vector<GraphicalItem> theitems;
	while(getuserinput(theitems));
	int length = theitems.size();

	if (length > 0) {
		//draw all graphics in the vector in one time
		FsOpenWindow(300, 50, 800, 600, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < length; i++) {
			drawItem(theitems[i]);
		}
		FsSwapBuffers();
		//show 5s
		FsSleep(5000);
	}

	return 0;
}