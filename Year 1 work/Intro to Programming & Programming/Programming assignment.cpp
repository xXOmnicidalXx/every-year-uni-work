// Luke P Gatehouse
// G20853863

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// removes punctuation so the word can be checked
void RemovePuncuationCheck_CaseSensitivity(string& RPCS)
{
	for (int i = 0; i < RPCS.size(); ++i)
	{
		if (!(RPCS[i] >= 65 && RPCS[i] <= 90 ||
			RPCS[i] >= 97 && RPCS[i] <= 122))
		{
			RPCS.erase(RPCS.begin() + i);
		}

		if (RPCS[i] >= 65 && RPCS[i] <= 90)
		{
			RPCS[i] += 32;
		}
	}
}

void CheckFiles(string FileName)
{
	std::vector <string> WordArray; // new file array
	std::vector <string> BannedWordsArray; // holds all banned words
	std::vector <string> CheckingWordArray; // holds selected files

	// creates files for filtered txt files
	ofstream NewFileCreated(FileName + " filtered.txt");

	// opens existing file
	fstream CheckFile;
	CheckFile.open(FileName + ".txt", ios::in);
	if (!CheckFile)
	{
		cout << "file doesn't exist"; // if it doesnt exist, stops running function
	}
	else
	{
		// opens banned file
		fstream BannedFile;
		BannedFile.open("banned.txt", ios::in);

		// temp holds strings
		string BannedWords;
		while (true)
		{
			BannedFile >> BannedWords; // overwrites temp value with next value in banned file
			BannedWordsArray.push_back(BannedWords); // adds temp value to array
			if (BannedFile.eof())
			{
				break;
			}
		}

		// temp holds strings
		string CheckedWords;
		while (true)
		{
			CheckFile >> CheckedWords; // overwrites temp value with the next value in checked file
			RemovePuncuationCheck_CaseSensitivity(CheckedWords);
			CheckingWordArray.push_back(CheckedWords); // adds temp value to array
			if (CheckFile.eof())
			{
				break;
			}

		}

		WordArray = CheckingWordArray;

		int WordCount = 0;
		cout << FileName << endl;
		for (int A = 0; A < BannedWordsArray.size(); A++) // each bannmed word is checked against each word in the check file
		{
			for (int B = 0; B < CheckingWordArray.size(); B++) // itterates through the check file
			{
				int i = CheckingWordArray[B].find(BannedWordsArray[A]);
				if (i != -1)
				{
					WordCount++;
					for (int C = i; C < BannedWordsArray[A].size(); ++C)
					{
						WordArray[B][C] = '*';
					}
				}
			}
			// shows how many banned words in each file
			cout << "'" + BannedWordsArray[A] + "' banned word found " + to_string(WordCount) + " Time/s" << endl;
			WordCount = 0; // resets after each word
		}

		// seperates each file
		cout << endl;
		cout << endl;

		// writes each word from new word array to word array
		for (size_t i = 0; i < WordArray.size(); i++)
		{
			NewFileCreated << WordArray[i] + " "; // adds space after each word
		}

		// closes files
		NewFileCreated.close();
		BannedFile.close();
		CheckFile.close();
	}

}

// calls all functions
int main()
{
	CheckFiles("text1");
	CheckFiles("text2");
	CheckFiles("text3");
	CheckFiles("text4");
}