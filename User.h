#pragma once

#include <string>
#include <vector>

#ifndef USER_H
#define USER_H
/*	Adam Swangler
*	adamswangelr@gmail.com
*	A straightforward class for holding a username, password and list of genrepreferences.
*	You can change name, change password, add a single genre preference, remove a preference or add a list of them at once.
*	You can also check a provided password against the current one and there are methods to return everything except password.
*/

using namespace std;
namespace ScrumAdam
{
	class User
	{
	public:

		void SetName(string incName)
		{
			name = incName;
		}
		void AddPreference(string incPref)
		{
			genrePreferences.push_back(incPref);
		}

		void RemovePreference(string remPref)
		{
			for (int i = 0; i < genrePreferences.size(); i++)
			{
				if (genrePreferences[i] == remPref)
				{
					genrePreferences.erase(genrePreferences.begin()+i);
				}
			}
		}

		void AddListOfPreferences(vector<string> incPrefs)
		{
			for (int i = 0; i < incPrefs.size(); i++)
			{
				genrePreferences.push_back(incPrefs[i]);
			}
			
		}

		void SetPassword(string incPass)
		{
			password = incPass;
		}

		string GetName()
		{
			return name;
		}

		vector<string> GetPreferences()
		{
			return genrePreferences;
		}

		bool CheckPassword(string testPass)
		{
			if (password == testPass)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	private:
		string name, password;
		vector<string> genrePreferences;

	};


}

#endif