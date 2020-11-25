#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Movie.h"

#ifndef MOVIEDATABASE_H
#define MOVIEDATABASE_H


using namespace std;

class MovieDatabase
{
public:

	MovieDatabase(vector<Movie> initialMovies)
	{
		for (int i = 0; i < initialMovies.size(); i++)
		{
			movies.push_back(initialMovies[i]);
		}
	}

	MovieDatabase()
	{

	}

	//Currently only works on the Kaggle data set.
	//Takes in a file name of a CSV, loads lines from that file and creates a list of movies based on it.
	//It takes a noticeable amount of time to get through the loading, at least on my machine.
	void LoadCSV(string filename)
	{
		fstream fin;
		string temp, yFilm, yCerem, cat, actorName, fName, winner;
		stringstream ss;
		vector<string> split;
		Movie *tMovie;

		fin.open(filename, ios::in);


		//get rid of category titles.
		getline(fin, temp);

		//Load the file line by line until we reach the end
		while (getline(fin, temp))
		{ 
			//This is for Debug
			lines.push_back(temp);

			ss << temp;
			//With the line in a string stream, use getline with comma as delimiter to get each component of the line separately and push them into a string vector.
			while (ss.good())
			{
				string substr;
				if (ss.peek() == '"')
				{
					getline(ss, substr, '"');
					getline(ss, substr, '"');
				}
				else
				{
					getline(ss, substr, ',');
				}
				split.push_back(substr);
				
			}

			//clear the string stream for the next line
			ss.str("");
			ss.clear();

			//Split out the line into the important parts.  The branch here is based on an extra element that gets inserted when there's a comma in the name/s of people being nominated.
			//There's definitely a more elegant way to do this, but this was quick and works.
			//This is the part that would have to change to make it work on data sets with different formats.
			if (split.size() == 7)
			{
				yFilm = split[0];
				yCerem = split[1];
				cat = split[3];
				actorName = split[4];
				fName = split[5];
				winner = split[6];
			}
			else if (split.size() == 8)
			{
				yFilm = split[0];
				yCerem = split[1];
				cat = split[3];
				actorName = split[4];
				fName = split[6];
				winner = split[7];
			}
			
			

			//clear the string vector for the next line
			split.clear();

			//If the list doesn't have this movie already, then create it
			if (!ContainsMovie(fName))
			{
				//Convert strings and make the movie
				tMovie = new Movie(atoi(yFilm.c_str()), atoi(yCerem.c_str()), fName);

				//Add the new movie, then clear tMovie
				movies.push_back(*tMovie);
				delete tMovie;
			}
			//Add the nomination to the existing movie
			AddNomToMovie(cat, actorName, ((winner == "True") ? true : false), fName);
		}

		fin.close();
	}
	
	Movie GetMovieAt(int num)
	{
		return movies[num];
	}

	string ReturnLineNum(int num)
	{
		return lines[num];
	}

	int MovieCount()
	{
		return movies.size();
	}

	bool ContainsMovie(string fName)
	{
		for (int i = 0; i < movies.size(); i++)
		{
			if (movies[i].GetFilmName() == fName)
			{
				return true;
			}
		}
		return false;
	}

	int GetMovieIndex(string fName)
	{
		for (int i = 0; i < movies.size(); i++)
		{
			if (movies[i].GetFilmName() == fName)
			{
				return i;
			}
		}
		return -1;
	}

	void AddNomToMovie(string cat, string aName, bool win, string fName)
	{
		int x = GetMovieIndex(fName);
		if (x >= 0)
		{
			movies[x].AddNomination(cat, aName, win);
		}
	}

	//This is obsolete.  Leaving it here just in case it's useful later
	string ConvertStringFormat(string targetStr, char changeThis, char toThis)
	{
		for (int i = 0; i < targetStr.length(); i++)
		{
			if (targetStr[i] == changeThis)
			{
				targetStr[i] = toThis;
			}
		}

		return targetStr;
	}


	//This function takes in any number of search terms and returns all movies that fit them.  Each term should be in the format "type:key"
	//For example, to find movies from 1977 you would use a term like this "MovieYear:1977" or to find movies nominated for Best Actress you would use "NominationCategory:ACTRESS"
	//Currently the input must match exactly.  If you put in BEST ACTRESS, but the data just says ACTRESS, it will not match those terms.
	vector<Movie> GetMovies(vector<string> searchTerms)
	{
		int splitLoc;
		string type, key;
		vector<Movie> retMovies;

		//Copy the whole list so that we can remove the parts that don't fit what we're looking for.
		for (int j = 0; j < movies.size();j++)
		{
			retMovies.push_back(movies[j]);
		}

		for (int i = 0; i < searchTerms.size(); i++)
		{
			
			//These three lines split each string into type and key
			splitLoc = searchTerms[i].find(':');
			type = searchTerms[i].substr(0, splitLoc);
			key = searchTerms[i].substr(splitLoc + 1);

			

			//You can't switch by strings in C++???  If statements it is then.

			if (type == "MovieYear") //Release Year
			{
				FilterByYearReleased(retMovies, atoi(key.c_str()));
			}
			else if (type == "CeremonyYear")//Ceremony Year
			{
				FilterByYearCeremony(retMovies, atoi(key.c_str()));
			}
			else if (type == "NominationCategory") //Type of Nomination, like ACTRESS, SOUND DIRECTION, etc
			{
				FilterByNominationCategory(retMovies, key);
			}
			else if (type == "WinnerCategory") //Winner of a specific category
			{
				FilterByWinner(retMovies, key);
			}
			else if (type == "Nominee") //Including the name of a person in the nominations
			{
				FilterByNominee(retMovies, key);
			}
			else if (type == "FilmName") //Finds a movie with the exact name provided
			{
				FilterByFilmName(retMovies, key);
			}
			else if (type == "Winner")  //For finding all movies that won, like from a specific year or for a specific category
			{
				FilterByWinGeneral(retMovies);
			}
		}


		return retMovies;

	}

	//Removes all movies from retMovies that aree not released in the year indicated by yearReleased
	void FilterByYearReleased(vector<Movie> &retMovies, int yearReleased)
	{
		for (int i = 0; i < retMovies.size();i++)
		{
			if (retMovies[i].GetYearM() != yearReleased)
			{
				retMovies.erase(retMovies.begin() + i);
				i--;
			}
		}
	}

	void FilterByYearCeremony(vector<Movie>& retMovies, int yearCerem)
	{
		for (int i = 0; i < retMovies.size();i++)
		{
			if (retMovies[i].GetYearC() != yearCerem)
			{
				retMovies.erase(retMovies.begin() + i);
				i--;
			}
		}
	}

	void FilterByNominationCategory(vector<Movie>& retMovies, string catName)
	{
		for (int i = 0; i < retMovies.size();i++)
		{
			if (!retMovies[i].HasNomCategory(catName))
			{
				retMovies.erase(retMovies.begin() + i);
				i--;
			}
		}
	}

	void FilterByWinner(vector<Movie>& retMovies, string catName)
	{
		for (int i = 0; i < retMovies.size();i++)
		{
			if (!retMovies[i].WonCategory(catName))
			{
				retMovies.erase(retMovies.begin() + i);
				i--;
			}
		}

	}

	void FilterByNominee(vector<Movie>& retMovies, string nomineeName)
	{
		for (int i = 0; i < retMovies.size();i++)
		{
			
			if (!retMovies[i].HasNominee(nomineeName))
			{
				retMovies.erase(retMovies.begin() + i);
				i--;
			}
		}
	}

	void FilterByFilmName(vector<Movie>& retMovies, string fName)
	{
		for (int i = 0; i < retMovies.size();i++)
		{
			if (retMovies[i].GetFilmName() != fName)
			{
				retMovies.erase(retMovies.begin() + i);
				i--;
			}
		}
	}

	void FilterByWinGeneral(vector<Movie>& retMovies)
	{
		for (int i = 0; i < retMovies.size();i++)
		{
			if (!retMovies[i].IsWinner())
			{
				retMovies.erase(retMovies.begin() + i);
				i--;
			}
		}
	}


	void ConvertToJSON(vector<Movie> movieSubset)
	{
		ofstream os("subset.json");
		vector<Nomination> noms;
		int i = 0;
		int escape = 0;

		
		os << "{\n";
		os << "\"Movies\":[\n";
		while (movieSubset.size() > 0)
		{
			i = movieSubset.size() - 1;
			noms = movieSubset[i].GetAllNominations();
			os << "{\n";
			os << "\"FilmName\":\""<<movieSubset[i].GetFilmName()<<"\",\n";
			os << "\"YearReleased\":\"" << movieSubset[i].GetYearM() << "\",\n";
			os << "\"YearNominated\":\"" << movieSubset[i].GetYearC() << "\",\n";
			os << "\"Nominations\": [\n";
			for (int j = 0; j < noms.size(); j++)
			{
				os << "	{\"Category\":\"" << noms[j].category << "\"},\n";
				os << "	{\"NominatedPeople\":\"" << noms[j].actorName << "\"},\n";
				os << "	{\"Winner\":\"" << (noms[j].winner == true ? "TRUE" : "FALSE") << "\"}";
				if (j < noms.size() - 1)
				{
					os << ',';
				}
				os << '\n';
			}
			os << "]\n}";
			if (movieSubset.size() > 1)
			{
				os << ",";
			}
			os << "\n";
			movieSubset.pop_back();
			escape++;
			if (escape >= 10)
				break;
		}
		os << "]\n}";

		os.close();
		
	}

private:
	vector<string> lines;
	vector<Movie> movies;
};


#endif
