#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

namespace KochaEngine
{
	class CSVReader
	{
	public:
		static CSVReader* GetInstance();
		static void DeleteInstance();

		std::unordered_map<std::string, std::vector<std::vector<int>>> mapList;

		bool LoadCSV(const std::string& fileName, const std::string& registName = "", const bool arg_isOverWrite = true);
		std::vector<std::vector<int>> GetMapData(const std::string& registName);
		void Clear();

	protected:
		static CSVReader* instance;
		CSVReader();
		~CSVReader();
		CSVReader(const CSVReader& another) = delete;
		CSVReader& operator=(const CSVReader& another) = delete;
	};
}
