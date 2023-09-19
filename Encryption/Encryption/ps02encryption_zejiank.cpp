//Full name: Zejian Kong
//Andrew ID:zejiank
//Date: 9th Sep, 2023
//Course info: 24780 Engineering Computation
//The program includes the encryption and decryption function which 
// apply on text files

#include<iostream>
#include<fstream>
using namespace std;


void encrypt() { //function for encryption
	printItem;
	ofstream encrypted;
	ofstream code;

	string messageFileName;
	string encryptedFileName;
	string codeFileName;

	cout << "Please enter the file name for encryption:" << endl;
	cin >> messageFileName;
	cout << "Please enter the encrypted output file name:" << endl;
	cin >> encryptedFileName;
	cout << "Please enter the code file name:" << endl;
	cin >> codeFileName;


	//open files
	message.open(messageFileName, ios::in | ios::binary);
	encrypted.open(encryptedFileName, ios::out | ios::binary);
	code.open(codeFileName, ios::out | ios::binary);

	srand(time(NULL));


	if (message.is_open()&&encrypted.is_open()&&code.is_open()) {
		unsigned char data;
		int randomNumber;
		unsigned char unsignedCharNumber;
		while (message.read((char*)&data, sizeof(data))) {
			//generate 0-255 random numbers
			randomNumber =rand()%256;
			//encryption
			data += randomNumber;     
			//write encrypted message to the file
			encrypted.write((const char *) &data, sizeof(data));
			unsignedCharNumber = randomNumber;
			//write code to the file
			code.write((const char*)&unsignedCharNumber, sizeof(unsignedCharNumber));
		}
	}
	else {
		cout << "Sorry, the files are not opened successfully" << endl;
	}


	//close files
	message.close();
	encrypted.close();
	code.close();
}



//function for decryption
void decrypt() {   
	ifstream encrypted;
	ofstream decrypted;
	ifstream code;

	string encryptedFileName;
	string decryptedFileName;
	string codeFileName;

	cout << "Please enter the encrypted file name:" << endl;
	cin >> encryptedFileName;
	cout << "Please enter the decrypted output file name:" << endl;
	cin >> decryptedFileName;
	cout << "Please enter the code file name:" << endl;
	cin >> codeFileName;


	//open files
	encrypted.open(encryptedFileName, ios::binary | ios::in);
	decrypted.open(decryptedFileName, ios::out | ios::binary);
	code.open(codeFileName, ios::in | ios::binary);


	if (decrypted.is_open() && encrypted.is_open() && code.is_open()) {
		unsigned char data;
		unsigned char number;
		while (encrypted.read((char*)&data, sizeof(data))) {
			code.read((char*)&number, sizeof(number));
			//decryption
			data-=number; 
			decrypted.write((const char*)&data, sizeof(data));
			}
		}
	else {
		cout << "Sorry, the files are not opened successfully" << endl;
	}


	//close files
	decrypted.close();
	encrypted.close();
	code.close();
}


int main() {

	while (1) {
		cout << "Encrypt: E" << endl;
		cout << "Decrypt: D" << endl;
		cout << "Exit: X" << endl;
		cout << "Please choose a mode and type a signle letter: " << endl;
		string choice;
		cin >> choice;
		if (choice == "E") {
			encrypt();
		}
		else if (choice == "D") {
			decrypt();
		}
		else {
			cout << "Goodbye!" << endl;
			break;
		}
	}

	return 0;
}