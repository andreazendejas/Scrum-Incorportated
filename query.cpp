/*  
	Read the id numbers of the movies from a text file
	Take the year the same way
	Then just take the indiviual id numbers and years and plug and chug into a string
	Put the string into a database to take out of for the app
*/

// After finishing this code figure out how to get the app code to use this code

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

/* ofstream write to files
   if stream reads from files  */

int main()
{
	ifstream omdbSearchFile;
	ofstream sendingInfo;
	omdbSearchFile.open("C://Users//natha//Documents//imdbMovieInfo.txt");
	sendingInfo.open("C://Users//natha//Documents//dataSent.txt");

	if (omdbSearchFile.is_open() && sendingInfo.is_open())
	{
		cout << "omdbSearchFile & sendingInfo opened" << endl;
		/* string line;
		while (getline(omdbSearchFile, line))
		cout << line << endl; */

		// Take each line one at a time and put every word into an array where each word is it's own element
		// then before going to the next line loop over the array to make the url formatted and send the url
		// to a file 

		// <name of movie>:<url>

		string url[100];
		url[0] = "www.omdbapi.com/?apikey=cdd3c279&t=";
		url[1] = "&y=";

		int urlIndex = 0;
		string word, line, formattedURL[10000], formattedInfoToSend[10000];
		while (omdbSearchFile >> word)
		{
			getline(omdbSearchFile, line);

			// Used to erase tabs from the name of the movies
			line.erase(remove(line.begin(), line.end(), '\t'), line.end());

			formattedInfoToSend[urlIndex] = line;

			// Used to replace spaces from the names of the movies to +
			for (int index = 0; index < line.length(); index++)
			{
				if (line[index] == ' ')
					line[index] = '+';
			}

			//cout << "Line: " << line << endl;
			//cout << "Year: " << word << endl;
			formattedURL[urlIndex] = url[0] + line + url[1] + word;
			formattedInfoToSend[urlIndex] = formattedInfoToSend[urlIndex] + ':' + formattedURL[urlIndex];
			sendingInfo << formattedInfoToSend[urlIndex] << '\n';
			++urlIndex;
		}
		//cout << "URL: " << formattedInfoToSend[0] << endl;
		//cout << "URL: " << formattedInfoToSend[1] << endl;
		//cout << "URL: " << formattedInfoToSend[2] << endl;
	}
	else
		cout << "omdbSearchFile & sendingInfo opening failed" << endl;

	sendingInfo.close();
	omdbSearchFile.close();

	return 0;
}