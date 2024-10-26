#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

namespace Arkanoid
{
	struct Settings;

	class RecordTable
	{
	public:
		RecordTable();
		void AddRecord(std::wstring name, int score);
		std::vector<std::wstring> GetRecords(int count) const;
		bool Serialize() const;
		void Deserialize();
	private:
		std::unordered_multimap<std::wstring, int> recordTable;
		std::multimap<int, std::wstring> GetOrderedTable() const;
	};
}

