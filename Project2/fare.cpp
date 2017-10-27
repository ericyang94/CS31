	// Code for Project 2
	// Tell the fare for bus riders

	#include <iostream>
	#include <string>
	using namespace std;

	int main()
	{
		int age;
		cout << "Age of rider: ";
		cin >> age;
		cin.ignore(10000, '\n');

		if (age < 0)	//check if age is negative
		{
			cout << "---" << endl;
	        cout << "The age must not be negative" << endl;
	        return 1;
		}

		string student;
		cout << "Student? (y/n): ";
		getline(cin, student);

		if (student != "y" && student != "n")	//check if y or n is entered
		{
			cout << "---" << endl;
	        cout << "You must enter y or n" << endl;
	        return 1;
		}

		string destination;
		cout << "Destination: ";
		getline(cin, destination);

		if (destination == "")		//check if destination is empty string
		{
			cout << "---" << endl;
			cout << "You must enter a destination" << endl;
			return 1;
		}

		int boundaries;
		cout << "Number of zone boundaries crossed: ";
		cin >> boundaries;

		if (boundaries < 0)		//check if boundaries is negative
		{
			cout << "---" << endl;
	        cout << "The number of zone boundaries crossed must not be negative" << endl;
	        return 1;
		}

		double fare;

		if (age < 18 && boundaries < 2)								//check if under 18 and 0 or 1 boundaries
			fare = .65;
		else if (age >= 65 && boundaries == 0)						//check if 65 and over with 0 boundaries
			fare = .35;
		else if (age >= 18 && student == "y" && boundaries < 2)		//check if student 18 and over with 0 or 1 boundaries
			fare = .65;
		else if (age >= 65)											//check if 65 and over
			fare = .55 + .25 * boundaries;
		else
			fare = 1.55 + .50 * boundaries;

		cout.setf(ios::fixed);
		cout.precision(2);
		cout << "---" << endl;
		cout << "The fare to " << destination << " is $" << fare << endl;
	}
