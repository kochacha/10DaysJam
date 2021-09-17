#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace KochaEngine
{
	class CSVReader
	{
	public:
		CSVReader();
		~CSVReader();

		std::map<float, std::vector<std::vector<float>>> mapList;

		void LoadCSV(float num, std::string fileName);
		std::vector<std::vector<float>> GetMapData(float num);
		void Clear();
	};
}
