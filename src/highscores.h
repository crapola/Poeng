/*******************************************************************************
High Scores
*******************************************************************************/
#pragma once
#include <array>
class HighScores
{
public:
	static const size_t kNameSize=16;
	static const size_t kNumEntries=10;
	static constexpr char const* kDefaultName="Poeng";
	static const long kDefaultBest=50000;
	static const long kDefaultDecrement=5000;
	typedef std::pair<long,std::string> Entry;
	HighScores();
	~HighScores();
	Entry Get(size_t rank) const;
	int Insert(Entry);
	bool Load();
	void Populate();
	bool Qualifies(int score) const;
	void Replace(size_t rank,Entry);
	void Save();
private:
	void Sort();
	std::array<Entry,kNumEntries> _entries;
};
