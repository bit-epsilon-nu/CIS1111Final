/* Benjamin_Campbell_Final Project.cpp : Defines the entry point for the console application.

Version:
1.0

Written by:
Benjamin Campbell

Date(y/m/d):
2018 / 09 / 09

Description:
A program to keep track of hikes.  Hikes are logged according to month, trail and distance.
Hiking statistics are seen can be seen on a total for the year and per month basis.
files and hikes can be saved, but not deleted.
Files are in .csv format and can be edited with excel then used in this program.
I attempted to hardcode as little as possible.
I keept the only hardcoded variables of the progarm to the names of the files that read data during program initilization.

Known issues:
no way to delete hikes.
no way to log hikes in more than one year.
no way to change the hiking log filename or location from the program
the program does not check for unsaved data before exit
no date class implimented for more accurate logging
The program should be broken up into seperate files for better organization
*/

#include "stdafx.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
const bool DEBUG = false;	//IS THE PROGRAM IS DEBUG MODE?


class trail {
public:
	trail(string parkname, string trail, double length, bool status);
	bool isopen();
	double distance();
	string name();
	string getpark();
	void setopen(bool x);
	void setdistance(double x);
	void setname(string x);

private:
	string park;
	string trailname;
	double traillength = 0;
	bool open = true;
};

bool trail::isopen() {
	return open;
}
double trail::distance() {
	return traillength;
}
string trail::name() {
	return trailname;
}
string trail::getpark() {
	return park;
}
void trail::setopen(bool x) {
	open = x;
}
void trail::setdistance(double x) {
	traillength = x;
}
void trail::setname(string x) {
	trailname = x;
}

trail::trail(string parkname, string name, double length, bool status) {
	park = parkname;
	trailname = name;
	traillength = length;
	open = status;
}

struct hike {
	int month;
	double distance;
	string trail;
};

void parsdata(ifstream& str, string& temp, bool& endline) {
	temp = "";
	while (true) {
		if (str.eof()) return;
		char test = str.get();
		switch (test) {
		case ',': {
			return;
		}
		case '\r': case '\n': case'/f': case EOF: { //   0x0D carage return   0x0A Line Feed
			endline = true;
			return;
		}

		default: {
			temp += test;
		}

		}
	}
}

void readcsv(string file, vector<string>& data) {
	ifstream getfile;
	string temp;
	bool end = false;
	if (DEBUG)cout << "Opening file: " << file << endl;
	getfile.open(file);
	if (getfile.is_open() && DEBUG) cout << file << " is open" << endl;

	do {
		parsdata(getfile, temp, end);
		data.push_back(temp);
	} while (!getfile.eof());
	//data.push_back(temp);
	getfile.close();
}
void readcsv(string file, vector<hike>& data) {
	ifstream getfile;
	getfile.open(file);
	string temp;
	hike temphike;
	bool endline = false;
	while (!getfile.eof()) {

		parsdata(getfile, temp, endline);
		if (getfile.eof()) return;

		if (temp.length() <= 2 && temp.length() > 0) { temphike.month = stoi(temp); }

		else if (isdigit(temp.front())) {
			temphike.distance = stod(temp);
		}
		else {
			temphike.trail = temp;
		}
		if (endline) {
			data.push_back(temphike);
			endline = false;
		}
	}
	getfile.close();

}

void printvec(vector<string> vec) {
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i] << endl << endl;
	}
}
void printvec(vector<trail>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		cout << setw(15) << vec[i].getpark() << vec[i].name() << "\t" << vec[i].distance() << "\t" << vec[i].isopen();
	}
}
void printvec(vector<hike>& vec) {
	cout << left << setw(7) << "Month" << setw(28) << "Trail" << setw(10) << "Distance" << endl;
	for (int i = 0; i < vec.size(); i++) {
		cout << left << setw(7) << vec[i].month << setw(28) << vec[i].trail << setw(10) << vec[i].distance << endl;
	}
	//int month;
	//double distance;
	//string trail;
}

void readtrails(string park, vector<trail>& trails) {
	ifstream getfile;
	getfile.open(park);
	for (int i = 0; i < 4; i++) {
		park.pop_back();
	}
	bool endline = false;
	string temp;
	string name;
	double dist;
	bool opn;
	while (!getfile.eof()) {
		parsdata(getfile, temp, endline);
		if (getfile.eof()) {
			getfile.close();
			return;
		}
		if (DEBUG) { cout << temp << endl << endl; }
		if (isdigit(temp.front())) {	// is temp a number?
			dist = stod(temp);
			if (DEBUG) cout << dist << endl;
		}
		else if (!temp.compare("open")) {
			opn = true;
			if (DEBUG) cout << temp << "\t" << opn << "\t" << temp.compare("open") << endl;
		}
		else if (!temp.compare("closed")) {
			opn = true;
			if (DEBUG) cout << temp << "\t" << opn << "\t" << temp.compare("open") << endl;
		}
		else {
			name = temp;
			if (DEBUG) cout << name << endl;
		}
		if (endline) {
			trail temptrail(park, name, dist, opn);
			trails.push_back(temptrail);
			endline = false;
		}
	}
}

hike loghike(vector<trail>& trails) {
	hike temp;
	int month = 0;
	double distance = -1;
	int trailnum;
	string trail;
	char choice;
	string prevpark;

	do {		// get the month of the hike
		cout << "Enter 1-12 for the month that you hiked:";
		cin >> month;
	} while (month < 1 || month > 12);


	do {		// get the trail that was hiked
		cout << endl << "Enter the number that corisponds to the trail you hiked";
		for (int i = 0; i < trails.size(); i++) {
			if (trails[i].getpark() != prevpark) cout << endl << trails[i].getpark() << endl;
			cout << i << "\t" << trails[i].name() << endl;
			prevpark = trails[i].getpark();
		}
		cin >> trailnum;
		while (true) {
			cout << endl << "You hiked " << trails[trailnum].name();
			cout << endl << "enter y for correct, n to reselect trail" << endl;
			cin >> choice;
			if (choice == 'N' || choice == 'n' || choice == 'y' || choice == 'Y') { break; }
			else cout << "Plese enter Y or N" << endl;
		}
		if (choice == 'N' || choice == 'n') { continue; }
		if (choice == 'Y' || choice == 'y') {
			trail = trails[trailnum].name();
			break;
		}
	} while (true);

	while (distance <= 0 || distance > trails[trailnum].distance()) {	// get the distance hiked on that trail
		cout << trail << " is " << trails[trailnum].distance() << " miles long." << endl
			<< "How far did you hike on " << trail << "?" << endl;
		cin >> distance;
		if (distance > trails[trailnum].distance()) {
			cout << "The distance you entered is longer than the trail length." << endl
				<< "Please enter a distance shorter than " << trails[trailnum].distance() << endl;
		}
	}
	temp.month = month;
	temp.distance = distance;
	temp.trail = trail;
	return temp;
}


bool logmenu(vector<trail> trails, bool& first) {
	if (first) return true;
	char choice;
	cout << "Press 1 to log another hike" << endl
		<< "Press q to return to main menue" << endl;

	while (true) {
		cin >> choice;
		switch (choice) {
		case '1': {
			return true;
			break;
		}
		case 'q': case 'Q': {
			return false;
		}
		default: {
			cout << "Please enter a valid choice";
			break;
		}
		}
	}
}
void printtotaldistance(vector<hike>& data, double& totaldist) {
	if (data.size() == 0) {
		cout << "WARNING: empty vector" << endl;
		return;
	}
	for (int i = 0; i < data.size(); i++) {
		totaldist += data[i].distance;
	}
	cout << "You have walked a total of " << totaldist << " Miles this year." << endl;
}
void printaverage(double totaldistance) {
	cout << "If you don't hike the rest of the year," << endl
		<< "you will have hiked an average of " << round(totaldistance / 365.25)   //days in a year 
		<< " miles per day" << endl;
}
void printarea(double totaldistance) {
	double area = sqrt(totaldistance);
	double xenia = 13.29; // according to wikipedia
	cout << "Here is another way of look at how far you have hiked." << endl
		<< "You hiked " << setprecision(4) << area << " Miles squared." << endl;
	if (area > 13.29) cout << "That's larger than the booming metrapolis of XENIA" << endl;
}

int monthhikes(int monthseek, vector<hike>& data) {
	int numhikes = 0;
	for (int i = 0; i < data.size(); i++) {
		if (data[i].month == monthseek) numhikes++;		// super innefficient
	}
	return numhikes;
}

double monthtotal(int monthseek, vector<hike>& data) {
	double total = 0;
	for (int i = 0; i < data.size(); i++) {
		if (data[i].month == monthseek) {
			total += data[i].distance;
		}
	}
	return total;
}

void printyearstats(vector<hike>& data) {

	vector<int> hikespermonth;
	vector<vector<double> > distances(12, vector<double>(2));
	for (int i = 0; i < 12; i++) {
		hikespermonth.push_back(monthhikes(i + 1, data));
		distances[i][0] = monthtotal(i + 1, data);
		distances[i][1] = distances[i][0] / hikespermonth[i];
		if (hikespermonth[i] == 0) distances[i][1] = 0;
	}
	cout << setw(6) << "month"
		<< setw(15) << "number of hikes"
		<< setw(14) << "total distance"
		<< setw(20) << "average dist / hike" << endl;

	for (int i = 0; i < hikespermonth.size(); i++) {
		cout << setw(6) << i + 1 //month
			<< setw(15) << hikespermonth[i]
			<< setw(14) << distances[i][0]
			<< setw(20) << distances[i][1] << endl;
	}
}
void savehikes(vector<hike> history, const string& filename) {
	ofstream ofile;
	ofile.open(filename);
	for (int i = 0; i < history.size(); i++) {
		ofile << history[i].month << ',' << history[i].trail << ',' << setprecision(5) << history[i].distance << endl;
	}
	ofile.close();
	cout << "Data saved to " << filename << endl;
}



void menu(vector<hike> history, vector<trail> trails, const string& filename) {
	char choice = '0';
	bool logmore = true;

	while (true) {
		cout << endl << "What would you like to do?" << endl
			<< "1: log hike" << endl
			<< "2: View this year's hikes" << endl
			<< "3: view hiking statistics" << endl
			<< "4: save this year's hikes to file" << endl
			<< "q: exit program" << endl;
		cin >> choice;
		switch (choice) {
		case '1': {
			bool first = true;
			while (logmenu(trails, first)) {
				history.push_back(loghike(trails));
				first = false;
			}
			logmore = true;
			break;
		}
		case '2': {
			printvec(history);
			break;
		}
		case '3': {
			if (history.size() == 0) { break; }
			double dist = 0;
			printtotaldistance(history, dist);
			printaverage(dist);
			printarea(dist);
			printyearstats(history);
			break;
		}
		case '4': {
			savehikes(history, filename);
			break;
		}
		case '5': {
			cout << "5" << endl;
			break;
		}
		case 'q': case 'Q': {
			cout << "exiting program" << endl;
			return;
		}
		default: {
			cout << "Please enter an acceptable value";
			break;
		}


		}
	}



}
//  enum months {january=1, february, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};

int main()
{
	vector<string> parks;
	vector<string> stats;
	vector<hike> history;

	readcsv("parks.csv", parks);
	readcsv("stats.csv", stats);

	vector<trail> trails;

	if (DEBUG) cout << "Parks is size " << parks.size() << endl;

	for (int i = 0; i < parks.size(); i++) {
		readtrails(parks[i], trails);
	}

	if (DEBUG) cout << "stats is size " << stats.size() << endl;
	for (int i = 0; i < stats.size(); i++) {
		if (DEBUG) cout << "reading file " << i << endl;
		readcsv(stats[i], history);
	}

	if (DEBUG) cout << endl << "INITIALIZATION COMPLETE" << endl;
	cout << endl << "Welcome to Hike Logger." << endl;

	menu(history, trails, stats[0]);


	system("PAUSE");
	return 0;
}

