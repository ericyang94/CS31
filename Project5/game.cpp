//code for Project 5
//play word deduction game for set number of rounds

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <algorithm>
using namespace std;

const int MINWORDLENGTH = 4;
const int MAXWORDLENGTH = 6;
const int MAXWORDS = 9000;
const char WORDFILENAME[] = "C:/words.txt";

int loadWords(char words[][MAXWORDLENGTH+1], int maxWords);
int manageOneRound(const char words[][MAXWORDLENGTH+1], int num, int wordnum);
int randInt(int lowest, int highest);

int main()
{
	char wordList[MAXWORDS][MAXWORDLENGTH+1];	//load words into array first
	int nWords = loadWords(wordList, MAXWORDS);

	if (nWords < 1)	//check if there were no words loaded
	{
		cout << "No words were loaded, so I can't play the game" << endl;
		return 1;
	}

	cout << "How many rounds do you want to play? ";
	int rounds;
	cin >> rounds;
	cin.ignore(10000, '\n');

	if (rounds < 0)	// check if rounds is negative
	{
		cout << "The number of rounds must be positive" << endl;
		return 1;
	}

	double totalScore = 0;
	double average = 0;
	int minimum = 10000;
	int maximum = 0;

	for(int k = 0; k != rounds; k++)	// repeat for number of rounds entered 
	{
		int currentRound = k+1;
		int random = randInt(0, nWords - 1);
		char hiddenWord[MAXWORDLENGTH+1];
		strcpy(hiddenWord, wordList[random]);
		int hiddenWordLength = strlen(hiddenWord);
		
		cout << endl;
		cout << "Round " << currentRound << endl;
		cout << "The hidden word is "<< hiddenWordLength << " letters long" << endl;
		
		int score = manageOneRound(wordList, nWords, random);	//play the round
		
		if (score == 1) cout << "You got it in 1 try" << endl;
		else cout << "You got it in " << score << " tries" << endl;

		cout.setf(ios::fixed);	//set double precision to 2 decimal places
		cout.precision(2);
		totalScore += score;
		average = totalScore / currentRound;	//calculate average for all rounds up to current point
		if (score > maximum) maximum = score;
		if (score < minimum) minimum = score;
		cout << "Average: " << average << ", minimum: " << minimum << ", maximum: " << maximum << endl;
	}
}

int loadWords(char words[][MAXWORDLENGTH+1], int maxWords)
{
    ifstream wordfile(WORDFILENAME);
    if ( ! wordfile)
    {
        cout << "Cannot open " << WORDFILENAME << endl;
        return -1;
    }
    const int LINELIMIT = 10000;
    char line[LINELIMIT];
    int numWords = 0;
    while (wordfile.getline(line, LINELIMIT))
    {
        if (numWords == maxWords)
        {
            cout << "Using only the first " << numWords
                 << " words from " << WORDFILENAME << endl;
            break;
        }
        int k;
        for (k = 0; islower(line[k]); k++)
            ;

          // The following two lines fix things for non-Windows users and
          // are harmless for Windows users.
        if (line[k] == '\r')
                line[k] = '\0';

        if (line[k] == '\0'  &&  k >= MINWORDLENGTH  &&  k <= MAXWORDLENGTH)
        {
            strcpy(words[numWords], line);
            numWords++;
        }
    }
    return numWords;
}

int manageOneRound(const char words[][MAXWORDLENGTH+1], int num, int wordnum)
{
	if (num <= 0 || wordnum < 0 || wordnum >= num)	//check if num or wordnum are negative and for unidentified behavior
		return -1;

	char hiddenWord[MAXWORDLENGTH+1];
	strcpy (hiddenWord, words[wordnum]);
	char trial[100];
	int tries = 0;

	while(true)	//loop until hidden word is guessed
	{
		cout << "Trial word: ";
		cin.getline(trial, 100);
		int restartLoop = 0;

		if (strlen(trial) < 4 || strlen(trial) > 6)	//check if word is between 4 and 6 letters
		{
			cout << "Your trial word must be a word of 4 to 6 lower case letters" << endl;
			tries ++;
			restartLoop++;
		}

		else
		{
			for(int k = 0; k != strlen(trial); k++)
			{
				if (!isalpha(trial[k]) || !islower(trial[k]))	//check if all characters are lower case letters
				{
					cout << "Your trial word must be a word of 4 to 6 lower case letters" << endl;
					tries ++;
					restartLoop++;
					break;
				}
			}

		}

		if (restartLoop == 0)	//skip if input was invalid
		{
			for(int k = 0; k != num; k++)
			{
				if (strcmp(trial, words[k]) == 0)	//check if word is known
				{
					if (strcmp(hiddenWord, trial) == 0) //check if trial word matches hidden word
					{
						tries++;
						return tries;
					}
					else
					{
						int counter = 0;
						for(int i = 0; i != strlen(trial); i++)
						{
							for(int z = 0; z != strlen(hiddenWord); z++)
							{
								if (trial[i] == hiddenWord[z])	//compare each letter from trial and hidden word
								{
									counter ++;
									hiddenWord[z] = '0';	//replace letter with 0 if matches so counter won't be repeated
									break;
								}
							}
						}
						cout << counter << endl;
						strcpy (hiddenWord, words[wordnum]);	//restore hidden word
						tries++;
						restartLoop++;
					}
					break;
				}
			}
			if (restartLoop == 0)	//word is unknown
			{
				cout << "I don't know that word" << endl;
				tries ++;
			}
		}
	}
}

int randInt(int lowest, int highest)
{
    if (highest < lowest)
        std::swap(highest, lowest);
    return lowest + (std::rand() % (highest - lowest + 1));
}