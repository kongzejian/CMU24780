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
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

// useful for managing coordinates
struct Point2D {
	float x, y;
};

class Gear {
private:
	std::string partID; // used for identification
	float pitch;        // given in teeth per inch
	int numbTeeth;      //given the number of teeth
	Point2D loc;        // current location of gear {x, y}
	float angle;        // current rotation of gear in degrees

public:
	// reads data from a file over-riding any existing data
	void load(std::ifstream& inFile);

	// outputs all the data for a gear in format matching attached example
	void print(std::ostream& output = std::cout);

	// calculates and return the pitch diameter
	float pitchDiam() { return numbTeeth / pitch; };

	// draws the gear on graphics window
	void draw(bool fill = false);

	//get the ID of the gear
	std::string getID() { return partID; }

	//void rotate(float rotAngle) { angle += rotAngle; }
	void rotate(float rotAngle) { angle = std::fmod(angle+rotAngle, 360.); }

	//set the ID of the gear
	void setID(const std::string& newID) { partID = newID; }

	// returns bottom left corner of gear bounding box
	Point2D minBound();
	// returns top right corner of gear bounding box
	Point2D maxBound();

	// returns true if given point is inside pitch diameter of gear
	bool pointIsInGear(Point2D aPoint) {
		return ((aPoint.x - loc.x) * (aPoint.x - loc.x)
			+ (aPoint.y - loc.y) * (aPoint.y - loc.y))
			< (pitchDiam() * pitchDiam())/4.;
	}
};