// Code for Project 3
// Check for spam in emails

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

string getFirstWord(string text);	//declaring functions
string getLastWord(string text);
string extractWord(string& text);
bool isUppercase(string text);
string makeUppercase(string text);
bool hasMultipleExclamations(string text);
bool isGibberishWord(string text);

int main()
{
	int spamMessages = 0;
	int legitMessages = 0;

	while (true)
	{
		cout << "Enter the subject line of the email: ";	//get subject
		string subject;
		getline(cin, subject);

		cout << "Enter the body of the email.  Press Enter on an empty line to finish." << endl;
		string body;
		string body2;
		while (true)
		{
			getline(cin, body2);	//get body of email and put into one string
			if (body2 == "")
				break;
			body += body2;
			body += " ";
		}
	
		int spamScore = 0;

		if (isGibberishWord(getLastWord(subject)))	//check if last word has more than 3 consecutive consonants
			spamScore += 40;

		if (hasMultipleExclamations(subject))	//check if subject has 3 or more consecutive exclamation marks
			spamScore += 20;

		double uppercase = 0;	//check if subject has more than 90% all uppercase words
		double totalWords = 0;
		while (getFirstWord(subject) != "")
		{
			if (isUppercase(extractWord(subject)))
				uppercase += 1.00;
			totalWords += 1.00;
		}

		if (totalWords > 0)
		{
			if ((uppercase / totalWords) > .90)
				spamScore += 30;
		}

		string bodyCopy = makeUppercase(body);		//check for special words
		for (string s; getFirstWord(bodyCopy) != ""; extractWord(bodyCopy))
		{
			s = getFirstWord(bodyCopy);
			if (s == "BUY" || s == "CHEAP" || s == "CLICK" || s == "DIPLOMA" || s == "ENLARGE" || s == "FREE" || 
				s == "LONELY" || s == "MONEY" || s == "NOW" || s == "OFFER" || s == "ONLY" || s == "PILLS" || s == "SEX")
				spamScore += 5;
		}

		uppercase = 0;	//check if body has more than 50% all uppercase words
		totalWords = 0;
		while (getFirstWord(body) != "")
		{
			if (isUppercase(extractWord(body)))
				uppercase += 1.00;
			totalWords += 1.00;
		}

		if (totalWords > 0)
		{
			if ((uppercase / totalWords) > .50)
				spamScore += 40;
		}

		if (spamScore > 100)	//check if spam score is over 100
		{
			cout << "This email is classified as spam, because its spam score is " << spamScore << "." << endl;
			spamMessages += 1;
		}
		else
		{
			cout << "This email is classified as legitimate, because its spam score is " << spamScore << "." << endl;
			legitMessages += 1;
		}
	
		string more;	//ask if user wants to check another email
		cout << "Would you like to classify another email (y or n)? ";
		getline(cin,more);
		while (more != "y" && more != "n")	//check if y or n is entered
		{
			cout << "Please enter y or n." << endl;
			cout << "Would you like to classify another email (y or n)? ";
			getline(cin,more);
		}

		if (more == "n")	//break from loop if n, else stay in loop
			break;
	}

	cout << endl;	//display results
	cout << "Number of spam messages: " << spamMessages << endl;
	cout << "Number of legitimate messages: " << legitMessages << endl;
}

string getFirstWord(string text)
{
	string t;
	int x = 0;
	int y = 0;
	int k = 0;
	for(; k != text.size(); k++)
	{
		if (isalpha(text[k]))
		{
			x = k;
			break;
		}
	}
	for(; k != text.size(); k++)
	{
		if (!isalpha(text[k]))
		{
			y = k;
			break;
		}
		y = k + 1;
	}
	t = text.substr(x,y-x);
	return t;
}

string getLastWord(string text)
{
	string t;
	int x = 0;
	int y = 0;
	int k = text.size();
	for(; k >= 0; k--)
	{
		if (isalpha(text[k]))
		{
			y = k + 1;
			break;
		}
	}
	for(; k >= 0; k--)
	{
		if (!isalpha(text[k]))
		{
			x = k + 1;
			break;
		}
		x = k;
	}
	t = text.substr(x,y-x);
	return t;
}

string extractWord(string& text)
{
	string t;
	int x = 0;
	int y = 0;
	int k = 0;
	for(; k != text.size(); k++)
	{
		if (isalpha(text[k]))
		{
			x = k;
			break;
		}
	}
	for(; k != text.size(); k++)
	{
		if (!isalpha(text[k]))
		{
			y = k;
			break;
		}
		else y = k + 1;
	}
	t = text.substr(x,y-x);
	text = text.substr(y,text.size() + 1 - y);
	if (t == "")
		text = "";
	return t;
}

bool isUppercase(string text)
{
	int lower = 0;
	for(int k = 0; k != text.size(); k++)
	{
		if (islower(text[k]))
			return false;
	}
	return true;
}

string makeUppercase(string text)
{
	string t;
	for(int k = 0; k != text.size(); k++)
		t += toupper(text[k]);
	return t;
}

bool hasMultipleExclamations(string text)
{
	int exclamation = 0;
	for(int k = 0; k != text.size(); k++)
	{
		if (text[k] == '!' && text[k+1] == '!' && text[k+2] == '!')
			return true;
	}
	return false;
}

bool isGibberishWord(string text)
{
	string t;
	int count = 0;
	for(int k = 0; k != text.size(); k++)
		t += tolower(text[k]);
	for(int k =0; k != t.size(); k++)
	{
		if (t[k] == 'a' || t[k] == 'e' || t[k] == 'i' || t[k] == 'o' || t[k] == 'u' || !isalpha(t[k]))
			count = 0;
		else count ++;
		if (count == 4)	
			return true;
	}
	return false;
}
