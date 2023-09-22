/*
			 _   ___
			|_.'_____`._
		__ .'.-'     `-.`.		   Full name: Zejian Kong
	   \  /,'           `.\		   Andrew ID: zejiank
		\//               \\	   Course: Eng. Computation, 24-780-B
		;;                 ::	   Problem number: Prob Set 4
		||        O        ||	   Due: Tues. Sept. 26, 2023
	   /,:                 ;;
	  /__\\               //	   Implement a Gear class to maintain
		  \`.           ,'/		   properties of gears. Also allow for
		  /_.`-._____.-'.'		   reading/writing files and display
			 `-._____.-'

*/

#include <string.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <iostream>
#include "Gear.h"
#define PI 3.14159
using namespace std;

// Displays menu in console
void showMenu();

// clears window and redirects user to input something in console
void inputRequiredMessage();

// uses the geometry of the gears to set up a "zoom all" view
void setOriginAndScale(vector<Gear>& allGears,
	float& originX, float& originY, float& scale);

Point2D getModelCoords(int screenX, int screenY,
	float originX, float originY, float scale);

int main(void)
{
	const int width = 800, height = 600; //window size
	float originX = 0., originY = 0.;
	float scale = 10.;
	int mouseEvent, leftButton, middleButton, rightButton;
	int screenX, screenY;
	stringstream coordStream; // for displaying coordinates on screen
	Point2D modelCoord;

	bool terminate = false;
	float rotateAngle = 4.f;
	char yesNo;
	int currGearIndex = -1;
	Gear newGear;
	vector<Gear> allGears;
	string datafilename;
	int key;
	ifstream inFile;
	bool canPlaySound = false;

	FsOpenWindow(16, 16, width, height, 1, "Gear Viewer (NG2023)"); // use double buffer

	showMenu();

	//// for UI testing, just load some gears, commment/uncomment as needed
	inFile.open("b1.gear");
	newGear.load(inFile);
	inFile.close();
	allGears.push_back(newGear);
	inFile.open("b2.gear");
	newGear.load(inFile);
	inFile.close();
	allGears.push_back(newGear);
	inFile.open("b3.gear");
	newGear.load(inFile);
	inFile.close();
	allGears.push_back(newGear);
	currGearIndex = 0;
	setOriginAndScale(allGears, originX, originY, scale);

	while (!terminate) {

		//all gears are spinning
		for (int i = 0; i < allGears.size(); i++) {
			allGears[i].rotate(0.1);
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		//get the keyboard event
		FsPollDevice();
		key = FsInkey();
		switch (key) {
		// exit program
		case FSKEY_ESC:
			terminate = true;
			break;
		case FSKEY_X:   
			terminate = true;
			break;
		// prev gear
		case FSKEY_UP: 
			if (currGearIndex == 0) {
				currGearIndex = allGears.size() - 1;
			}
			else {
				currGearIndex -= 1;
			}
			break;
		// next gear
		case FSKEY_DOWN: 
			if (currGearIndex == allGears.size() - 1) {
				currGearIndex =0 ;
			}
			else {
				currGearIndex += 1;
			}
			break;
		// rotate CCW
		case FSKEY_LEFT: 
			allGears[currGearIndex].rotate(1);
			break;
		// rotate CW
		case FSKEY_RIGHT: 
			allGears[currGearIndex].rotate(-1);
			break;

		// list all data and redisplay menu
		case FSKEY_L: 
			for (int i = 0; i < allGears.size(); i++) {
				allGears[i].print();
			}
			showMenu();
			break;
		case FSKEY_A: // add a gear
			// redirect to console
			inputRequiredMessage();

			//ask for filename
			cout << "\n\nPlease enter the name of the file to read > ";
			cin >> datafilename;
			if (datafilename.find(".gear") == string::npos)
				datafilename += ".gear";     // allows user to omit extension
			inFile.open(datafilename);

			if (inFile.is_open()) { // may not have found file
				newGear.load(inFile);
				inFile.close();
				allGears.push_back(newGear);
				currGearIndex = allGears.size() - 1;
				setOriginAndScale(allGears, originX, originY, scale);
			}
			else
				cout << "\nError reading file " << datafilename << ".Please check data and try again." << endl;

			// redisplay console menu
			showMenu();
			break;
		// delete a gear, if there are any in the collection
		case FSKEY_D: 
			if (currGearIndex >= 0) {
				// redirect to console
				inputRequiredMessage();

				// ask for verification
				cout << "\n\nAre you sure you want to delete this gear ("
					<< allGears.at(currGearIndex).getID() << ")? (Y/N) >> ";
				cin >> yesNo;

				// delete/erase the current gear and make the previous curve current
				if (yesNo == 'Y' || yesNo == 'y') {
					allGears.erase(allGears.begin() + currGearIndex);
					currGearIndex--;
					if (currGearIndex < 0)
						currGearIndex = allGears.size() - 1;
					setOriginAndScale(allGears, originX, originY, scale);
				}

				while (getchar() != '\n'); // this will clear the newline associated with last input
				showMenu();
			}
			break;
		}

		//adjust the graphics plane
		glTranslated(0, height, 0); // make bottom-left the origin
		glScaled(scale, -scale, 1.);  // scale everything, with positive y going up
		glTranslated(-originX, -originY, 0);

		//highlight current Gear by drawing thick red line
		glColor3ub(255, 0, 0);
		glLineWidth(3);
		if (currGearIndex >= 0 && currGearIndex < allGears.size()) {// extra protection
			allGears.at(currGearIndex).draw();
		}

		// draw the gears
		glColor3ub(0, 0, 0);
		glLineWidth(1);
		for (Gear& currGear : allGears) {
			currGear.draw();
		}
		// reset transformation matrix completely (don.t want transformation on top of transformation
		glLoadIdentity();

		// handling mouse inputs
		mouseEvent = FsGetMouseEvent(leftButton, middleButton,
			rightButton, screenX, screenY);

		if (leftButton) {
			// write coords on screen if left button is held down
			modelCoord = getModelCoords(screenX, screenY,
				originX, originY, scale);
			coordStream.str("");  // reset stream
			coordStream.precision(4);
			coordStream << modelCoord.x << ", " << modelCoord.y;
			glColor3ub(60, 230, 60);
			glRasterPos2i(screenX, screenY);  // set position
			YsGlDrawFontBitmap7x10(coordStream.str().c_str());
		}

		FsSwapBuffers();
		FsSleep(20);
	}

}

void setOriginAndScale(vector<Gear>& allGears, float& originX, float& originY, float& scale)
{
	if (allGears.size() > 0) {
		Point2D minBnd, maxBnd, currMin, currMax;

		minBnd = allGears[0].minBound();
		maxBnd = allGears[0].maxBound();
		for (Gear& currGear : allGears) {
			currMin = currGear.minBound();
			currMax = currGear.maxBound();
			if (minBnd.x > currMin.x)
				minBnd.x = currMin.x;
			if (minBnd.y > currMin.y)
				minBnd.y = currMin.y;
			if (maxBnd.x < currMax.x)
				maxBnd.x = currMax.x;
			if (maxBnd.y < currMax.y)
				maxBnd.y = currMax.y;
		}
		originX = minBnd.x; originY = minBnd.y; //find the minimum bound and the maximum bound

		int hei, wid;
		FsGetWindowSize(wid, hei);

		float scaleX, scaleY;
		int extraBorder = 10; // allows for some space around all the gears
		scaleX = (wid - extraBorder) / (maxBnd.x - minBnd.x);
		scaleY = (hei - extraBorder) / (maxBnd.y - minBnd.y);
		scale = min(scaleX, scaleY);
		if (scale == scaleX) // adjust originY so as to center vertically
			originY -= 0.5 * (hei / scale - (maxBnd.y - minBnd.y));
		else // adjust originX so as to center horizontally
			originX -= 0.5 * (wid / scale - (maxBnd.x - minBnd.x));
		
		// adjust for half extraborder
		originX -= extraBorder / 2. / scale;
		originY -= extraBorder / 2. / scale;
	}
	else {
		originX = originY = 0.;
		scale = 1.0;
	}

}

void showMenu()
{
	cout << "\n\n";
	cout << "Use these keys on the graphics window:" << endl << endl;
	cout << "\tA : Add a gear from input file" << endl;
	cout << "\tD : Delete the currently selected gear from the collection" << endl;
	cout << "\tL : List all the data for the currently selected gear on the console window" << endl;
	cout << endl;
	cout << "\tUp/Down arrows: cycle through gears in the collection" << endl;
	cout << "\tLeft/Right arrows: rotate the currently selected gear" << endl;
	cout << endl;
	cout << "\tLeft click and hold anywhere to see coordinates" << endl;
	cout << endl;
	cout << "\tX/esc : eXit the program" << endl;
}

void inputRequiredMessage()
{
	// display message on graphics window directing user to check console

	string message = "Input required on console . . .";
	// black color
	glColor3f(0, 0, 0);

	//center horizontally and vertically for 20-pixel-wide by 28-pixel-height letters
	int wid, hei;
	FsGetWindowSize(wid, hei);
	//get the start coordinates of message
	glRasterPos2d(wid / 2 - message.length() * 20 / 2, hei / 2 + 28 / 2);
	//display the message
	YsGlDrawFontBitmap20x28(message.c_str()); // function requires a C-language string
	FsSwapBuffers();

}

Point2D getModelCoords(int screenX, int screenY, float originX, float originY, float scale) {
	int wei, hei;
	FsGetWindowSize(wei, hei); // puts window parameters into the two given variables
	return { screenX / scale + originX, (hei - screenY) / scale + originY };
}