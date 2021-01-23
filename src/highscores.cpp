#include "highscores.h"
#include <algorithm>
#include <fstream>
#include <sstream>
HighScores::HighScores():_entries()
{
}
HighScores::~HighScores()
{
}
HighScores::Entry HighScores::Get(size_t rank) const
{
	return _entries.at(rank);
}
int HighScores::Insert(Entry new_entry)
{
	if (!Qualifies(new_entry.first))
		return -1;
	new_entry.second=new_entry.second.substr(0,kNameSize);
	_entries.back()=new_entry;
	Sort();
	auto it=std::find(_entries.begin(),_entries.end(),new_entry);
	return it-_entries.begin();
}
bool HighScores::Load()
{
	std::ifstream file("scores.txt");
	if (!file)
		return false;
	std::string s;
	for (auto& e: _entries)
	{
		if (!std::getline(file,s,'\0'))
		{
			file.close();
			return false;
		}
		std::stringstream ss(s);
		ss>>e.first;
		std::getline(file,e.second,'\0');
	}
	file.close();
	return true;
}
void HighScores::Populate()
{
	auto i=kDefaultBest;
	for (auto& e: _entries)
	{
		e.first=i;
		e.second=kDefaultName;
		i-=kDefaultDecrement;
	}
}
bool HighScores::Qualifies(int score) const
{
	return (score>_entries.back().first);
}
void HighScores::Replace(size_t p_rank,Entry p_e)
{
	_entries.at(p_rank)=p_e;
	Sort();
}
void HighScores::Save()
{
	std::ofstream file("scores.txt",std::ios::trunc);
	for (auto& e: _entries)
	{
		file<<e.first<<'\0'<<e.second<<'\0';
	}
	file.close();
}
void HighScores::Sort()
{
	std::sort(_entries.begin(),_entries.end(),std::greater<Entry>());
}
