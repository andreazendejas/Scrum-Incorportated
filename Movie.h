#pragma once
#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>

using namespace std;

class Nomination
{
public:
	bool winner;
	string category;
	string actorName;
};

class Movie
{
public:

	Movie(int yearM, int yearC, string fName)
	{
		yearMovie = yearM;
		yearCeremony = yearC;
		filmName = fName;
	}
	Movie()
	{
		yearMovie = -1;
		yearCeremony = -1;
		filmName = "";
	}

	void AddNomination(string cat, string aName, bool win)
	{
		Nomination tNom;
		tNom.category = cat;
		tNom.actorName = aName;
		tNom.winner = win;

		noms.push_back(tNom);

	}

	int GetYearM()
	{
		return yearMovie;
	}

	int GetYearC()
	{
		return yearCeremony;
	}

	int GetNumberOfNominations()
	{
		return noms.size();
	}

	string GetNomCategory(int checkNom)
	{
		return noms[checkNom].category;
	}

	string GetNomActor(int checkNom)
	{
		return noms[checkNom].actorName;
	}

	bool GetNomWinner(int checkNom)
	{
		return noms[checkNom].winner;
	}

	string GetFilmName()
	{
		return filmName;
	}

	bool IsWinner()
	{
		for (int i = 0; i < noms.size(); i++)
		{
			if (noms[i].winner)
			{
				return true;
			}
		}

		return false;
	}

	bool WonCategory(string catName)
	{
		for (int i = 0; i < noms.size(); i++)
		{
			if (noms[i].category == catName && noms[i].winner)
			{
				return true;
			}
		}

		return false;
	}

	bool HasNomCategory(string catName)
	{
		for (int i = 0; i < noms.size(); i++)
		{
			if (noms[i].category == catName)
			{
				return true;
			}
		}

		return false;
	}

	bool HasNominee(string nomineeName)
	{
		for (int i = 0; i < noms.size(); i++)
		{
			if (noms[i].actorName == nomineeName)
			{
				return true;
			}
		}

		return false;
	}

	vector<Nomination> GetAllNominations()
	{
		return noms;
	}


private:
	int yearMovie, yearCeremony;
	string filmName;
	vector<Nomination> noms;
};



#endif