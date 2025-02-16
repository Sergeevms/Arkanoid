#pragma once
#include <iostream>
#include <fstream>

namespace Arkanoid
{
	class ISave
	{
	public:
		virtual void SaveToFile(std::ofstream& ostream) const = 0 ;
		virtual void LoadFromFile(std::ifstream& ifstream) = 0;
	};

	class ISaveable
	{
	public:
		~ISaveable() = default;
		virtual std::shared_ptr<ISave> SaveState() const = 0;
		virtual void SaveState(std::shared_ptr<ISave> save) const = 0;
		virtual void LoadState(const std::shared_ptr<ISave> save) = 0;
	};
}

