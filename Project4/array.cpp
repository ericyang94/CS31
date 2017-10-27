//code for Project 4
//produce a library of functions for arrays of strings

#include <iostream>
#include <string>
using namespace std;

int appendToAll(string a[], int n, string value);	//state functions
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int rotateRight(string a[], int n, int pos);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int split(string a[], int n, string splitter);

int main()
{
	string cast[6] = { "sansa", "robb", "daenerys", "tyrion", "jon", "arya" };	//test appendToAll function
	int j = appendToAll(cast, 6, "!!!");  // returns 6
	cout << j << endl; 

	string casts[6] = { "sansa", "robb", "daenerys", "tyrion", "jon", "arya" };	//test positionOfMax function
	int k = positionOfMax(casts, 6);
	cout << k << endl; 

	string character[5] = { "joffrey", "bran", "catelyn", "sansa", "cersei" };	//test rotateLeft function
	int m = rotateLeft(character, 5, 1);  // returns 1
	cout << m << endl; 

	string characters[5] = { "joffrey", "bran", "catelyn", "sansa", "cersei" };	//test rotateRight function
	int p = rotateRight(characters, 5, 2);  // returns 2
	cout << p << endl;

	string role[6] = { "sandor", "theon", "", "arya", "daenerys", "jaime" };	//test flip function
	int q = flip(role, 4);  // returns 4
	cout << q << endl;

	string roles[6] = { "sandor", "theon", "", "arya", "daenerys", "jaime" };	//test differ function
	string group[5] = { "sandor", "theon", "jaime", "", "jon" };
	int r = differ(roles, 6, group, 5);  //  returns 2
	int s = differ(roles, 2, group, 1);  //  returns 1
	cout << r << endl;
	cout << s << endl;

	string names[10] = { "catelyn", "tyrion", "jorah", "sansa", "bran", "cersei" };	//test subsequence function
	string names1[10] = { "tyrion", "jorah", "sansa" };
	int t = subsequence(names, 6, names1, 3);  // returns 1
	string names2[10] = { "catelyn", "sansa" };
	int u = subsequence(names, 5, names2, 2);  // returns -1
	cout << t << endl;
	cout << u << endl;

	string name[10] = { "catelyn", "tyrion", "jorah", "sansa", "bran", "cersei" };	//test lookupAny function
	string set1[10] = { "jon", "bran", "sansa", "tyrion" };
	int v = lookupAny(name, 6, set1, 4);  // returns 1 (a1 has "tyrion" there)
	string set2[10] = { "arya", "daenerys" };
	int w = lookupAny(name, 6, set2, 2);  // returns -1 (a1 has none)
	cout << v << endl;
	cout << w << endl;

	string cast3[6] = { "sansa", "robb", "daenerys", "tyrion", "jon", "arya" };	//test split function
	int x = split(cast3, 6, "jorah");  //  returns 3
	string cast2[4] = { "daenerys", "tyrion", "jon", "arya" };
	int y = split(cast2, 4, "daenerys");  //  returns 1
	cout << x << endl;
	cout << y << endl;
}

int appendToAll(string a[], int n, string value)
{
	if (n < 0)	//check for bad argument
		return -1;
	else
	{
		for(int k = 0; k != n; k++) //add value to each string
		{
			a[k] += value;
		}
		return n;
	}
}

int lookup(const string a[], int n, string target)
{
	if (n < 0)	//check for bad argument
		return -1;
	else
	{
		for(int k = 0; k != n; k++)	//check each string for target
		{
			if (a[k] == target)
				return k;
		}
		return -1;
	}
}

int positionOfMax(const string a[], int n)
{
	if (n < 0)	//check for bad argument
		return -1;
	else
	{
		string b;
		int x = -1;
		for(int k = 0; k != n; k++)	// test each string against each other
		{
			if (a[k] > b)
			{
				b = a[k];
				x = k;
			}
		}
		return x;
	}
}

int rotateLeft(string a[], int n, int pos)
{
	if (n < 0 || pos < 0 || pos >= n)	//check for bad argument
		return -1;
	else
	{
		string array[10000];
		for(int k = 0; k != pos; k++)	//assign first half to new array
			array[k] = a[k];
		for(int k = pos; k != n-1; k++)	//assign second half to new array
			array[k] = a[k+1];
		array[n-1] = a[pos];	//assign string at pos to end of new array
		for (int k = 0; k != n; k++)	//assign back to original array
			a[k] = array[k];
		return pos;
	}
}

int rotateRight(string a[], int n, int pos)
{
	if (n < 0 || pos < 0 || pos >= n)	//check for bad argument
		return -1;
	else
	{
		string array[10000];
		for(int k = n-1; k != pos; k--)	//assign second half to new array
			array[k] = a[k];
		for(int k = pos; k != 0; k--)		//assign first half to new array
			array[k] = a[k-1];
		array[0] = a[pos];	//assign string at pos to beginning of new array
		for (int k = 0; k != n; k++)	//assign back to original array
			a[k] = array[k];
		return pos;
	}
}

int flip(string a[], int n)
{
	if (n < 0)	//check for bad argument
		return -1;
	else
	{
		string array[10000];
		int x = 0;
		for(int k = n-1; k >= 0; k--)	//assign reversed array to new array
		{
			array[x] = a[k];
			x++;
		}
		for(int k = 0; k != n; k++)	//assign back to original array
			a[k] = array[k];
		return n;
	}
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0)	//check for bad argument
		return -1;
	else
	{
		int x;
		if (n1 >= n2)	//check shorter array
			x = n2;
		else x = n1;
		for(int k = 0; k != x; k++)	//compare strings
		{
			if (a1[k] != a2[k])
				return k;
		}
		return x;
	}
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0)	//check for bad argument
		return -1;
	else if (n2 == 0)	//check for 0 elements
		return 0;
	else
	{
		int z;
		for(int k = 0; k != n1; k++)
		{
			if (a1[k] == a2[0])	//check for matching subsequence if string at k matches first element of a2 array
			{
				z = k;
				int x = 0;
				for(; x != n2; x++)
				{
					if (a1[z] != a2[x])	//break if string doesn't match
						break;
					if (z != n1)	//break if over number of interesting items
						z++;
					else break;
				}
				if (x == n2)
					return k;
			}
		}
		return -1;
	}
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0)	//check for bad argument
		return -1;
	else
	{
		for(int k = 0; k != n1; k++)
		{
			for(int x = 0; x != n2; x++)
			{
				if (a1[k] == a2[x])	//check each string combination for matches
					return k;
			}
		}
		return -1;
	}
}

int split(string a[], int n, string splitter)
{
	if (n < 0)	//check for bad argument
		return -1;
	else
	{
		string array[10000];
		int x = 0;
		int position = 0;
		for(int k = 0; k != n; k++)
		{
			if (a[k] < splitter)	//assign anything < splitter to new array first
			{
				array[x] = a[k];
				x++;
				position = x;
			}
		}
		for(int k = 0; k != n; k++)
		{
			if (a[k] == splitter)	//assign anything == splitter to new array
			{
				array[x] = a[k];
				x++;
			}
		}
		for(int k = 0; k != n; k++)
		{
			if (a[k] > splitter)	//assign anything > splitter to new array last
			{
				array[x] = a[k];
				x++;
			}
		}
		for (int k = 0; k != n; k++)	//assign back to original array
			a[k] = array[k];
		return position;
	}
}