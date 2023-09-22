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
#include "Gear.h"
#include <iostream>
#include "StringPlus.h"
#include "fssimplewindow.h"
#define PI 3.14159
using namespace std;
void Gear::load(std::ifstream& inFile)
{
	string wholeLine;
	int colonLocation;
	bool continueReading = true;
	while (continueReading && inFile) {
		getline(inFile, wholeLine);

		if (wholeLine.find("Gear End") != string::npos) {
			//done
			continueReading = false;
		}
		//the line includes ":"
		else if ((colonLocation = wholeLine.find(":")) != string::npos) {

			if (wholeLine.find("pitch") != string::npos) {
				pitch= stof(StringPlus::trim(wholeLine.substr(colonLocation + 1)));
			}
			else if (wholeLine.find("partID") != string::npos) {
				partID = StringPlus::trim(wholeLine.substr(colonLocation + 1));
			}
			else if (wholeLine.find("numbTeeth") != string::npos) {
				numbTeeth = stoi(StringPlus::trim(wholeLine.substr(colonLocation + 1)));
			}
			else if (wholeLine.find("startAngle") != string::npos) {
				angle= stof(StringPlus::trim(wholeLine.substr(colonLocation + 1)));
			}
			else if (wholeLine.find("startX") != string::npos) {
				loc.x= stof(StringPlus::trim(wholeLine.substr(colonLocation + 1)));
			}
			else if (wholeLine.find("startY") != string::npos) {
				loc.y= stof(StringPlus::trim(wholeLine.substr(colonLocation + 1)));
			}
		}
	}
}

//the printing function
void Gear::print(std::ostream& output)
{
	output << "Gear:" << endl;
	output << "      partID: " << partID <<endl;
	output << "      pitch: " << pitch << endl;
	output << "      numbTeeth: " << numbTeeth << endl;
	output << "      startX: " << loc.x << endl;
	output << "      startY: " << loc.y << endl;
	output << "      startAngle: " << angle << endl;
	output << "Gear End:"<< endl;
}

void Gear::draw(bool fill)
{	
	//calculate some parameters about lengths and angles
	float module = 1 / pitch;
	float dedendum = 1.25 * module;
	float addendum = module;
	float widthToothBottom = 1.8 * module;
	float widthToothTop = module;
	float diameter = pitchDiam();
	float radius = diameter / 2;
	float R2 = sqrt(pow(widthToothBottom / 2, 2)+pow(radius - dedendum, 2));
	float theta = 2 * PI / numbTeeth - atanf(widthToothBottom/2/(radius-dedendum));
	float gama = atanf((widthToothTop/2) / (radius+addendum));
	float beta= acosf((radius-dedendum) / (R2)); 


	for (int i = 0; i < numbTeeth; i++) {
		float relativeAngle = i * 2 * PI / numbTeeth+angle*PI/180; 
		Point2D toothBottom1;
		Point2D toothBottom2;
		Point2D toothTop1;
		Point2D toothTop2;

		//calculate the coordinates of four points
		toothBottom1.x = loc.x + R2 * cos(-beta + relativeAngle);
		toothBottom1.y = loc.y + R2 * sin(-beta + relativeAngle);

		toothBottom2.x = loc.x + R2 * cos(beta + relativeAngle);
		toothBottom2.y = loc.y + R2 * sin(beta + relativeAngle);

		toothTop1.x = loc.x + (radius + addendum) * cos(-gama + relativeAngle);
		toothTop1.y = loc.y + (radius + addendum) * sin(-gama + relativeAngle);

		toothTop2.x = loc.x + (radius + addendum) * cos(gama + relativeAngle);
		toothTop2.y = loc.y + (radius + addendum) * sin(gama + relativeAngle);

		//draw four lines
		glBegin(GL_LINES);
		glVertex2f(toothBottom1.x, toothBottom1.y);
		glVertex2f(toothTop1.x, toothTop1.y);

		glVertex2f(toothTop1.x, toothTop1.y);
		glVertex2f(toothTop2.x, toothTop2.y);

		glVertex2f(toothTop2.x, toothTop2.y);
		glVertex2f(toothBottom2.x, toothBottom2.y);

		glEnd();

		//draw the arc
		glBegin(GL_LINE_STRIP);

		for (int j = 0; j < 100; j++) {
			glVertex2f(loc.x+ R2 * cos(relativeAngle + beta + (theta - beta) * j / 100), loc.y + R2 * sin(relativeAngle + beta + (theta - beta) * j / 100));
		}
		glEnd();

	}
}

Point2D Gear::minBound()
{
	Point2D bottomLeftCorner;
	float diameter = pitchDiam();
	bottomLeftCorner.x = loc.x-diameter/2;
	bottomLeftCorner.y = loc.y-diameter / 2;
	return bottomLeftCorner;
}

Point2D Gear::maxBound()
{
	Point2D topRightCorner;
	float diameter = pitchDiam();
	topRightCorner.x = loc.x + diameter / 2;
	topRightCorner.y = loc.y + diameter / 2;
	return topRightCorner;
}
