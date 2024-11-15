#include "RecordTable.h"
#include <fstream>
#include <sstream>

#include "GameWorld.h"
#include "Application.h"

namespace Arkanoid
{
	RecordTable::RecordTable()
	{
	}

	void RecordTable::AddRecord(std::wstring name, int score)
	{
		recordTable.insert({ name, score });
	}

	std::vector<std::wstring> RecordTable::GetRecords(int count) const
	{
		std::vector<std::wstring> records;
		records.reserve(count);
		const std::multimap<int, std::wstring> orderedTable = GetOrderedTable();
		int index = 1;
		std::multimap<int, std::wstring>::const_reverse_iterator current = orderedTable.rbegin();
		GameWorld* world = GameWorld::GetWorld();
		while (current != orderedTable.rend() && index <= count)
		{
			std::wostringstream currentString;
			currentString << index << L". " << current->second << L" " << current->first;
			++index;
			++current;
			records.push_back(currentString.str());
		}
		return records;
	}

	bool RecordTable::Serialize() const
	{
		GameWorld* world = GameWorld::GetWorld();
		std::wofstream output(world->recordsFileName);
		if (output.is_open())
		{
			const std::multimap<int, std::wstring> orderedTable = GetOrderedTable();
			int index = 0;
			std::multimap<int, std::wstring>::const_reverse_iterator current = orderedTable.rbegin();
			while (current != orderedTable.rend() && index < world->bigRecordsSize)
			{
				output << current->first << L" " << current->second << std::endl;
				++index;
				++current;
			}
			output.close();
			return true;
		}
		else
		{
			return false;
		}
	}

	void RecordTable::Deserialize()
	{
		GameWorld* world = GameWorld::GetWorld();
		recordTable.clear();
		recordTable.reserve(world->bigRecordsSize);
		std::wifstream input(world->recordsFileName);
		if (input.is_open())
		{	
			std::wstring name;
			int score = 0;
			while (input >> score)
			{
				std::getline(input, name);
				name = name.substr(1, name.size() - 1);
				recordTable.insert({ name, score });
			}
			input.close();
		}
		else
		{
			for (int i = 1; i <= world->bigRecordsSize; ++i)
			{
				recordTable.insert({ world->defaultPlayerName, i * 10});
			}
		}
	}

	std::multimap<int, std::wstring> RecordTable::GetOrderedTable() const
	{
		std::multimap<int, std::wstring> orderedTable;
		for (auto& current : recordTable)
		{
			orderedTable.insert({ current.second, current.first });
		}
		return orderedTable;
	}
}