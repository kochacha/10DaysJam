#include "CSVReader.h"
#include <windows.h>
#include <cassert>

using namespace std;

KochaEngine::CSVReader* KochaEngine::CSVReader::instance = nullptr;

KochaEngine::CSVReader::CSVReader()
{
	Clear();
}

KochaEngine::CSVReader::~CSVReader()
{
	Clear();
}

KochaEngine::CSVReader* KochaEngine::CSVReader::GetInstance()
{
	if (!instance)
	{
		instance = new CSVReader();
	}
	return instance;
}

void KochaEngine::CSVReader::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

bool KochaEngine::CSVReader::LoadCSV(const std::string& fileName, const std::string& registName, const bool arg_isOverWrite)
{
	auto str = registName;
	if (registName == "")
	{
		str = fileName;
	}

	//引数の文字列で登録されているか
	auto itr = mapList.find(str);
	if (itr != mapList.end())
	{
		//上書きするので消去
		if (arg_isOverWrite)
		{
			mapList.erase(itr);
		}
		//上書きしないのでfalse
		else
		{
			return false;
		}
	}

	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		OutputDebugStringA("CSVファイルを開けませんでした");
		return false;
	}

	std::vector<std::vector<int>> m_map;
	std::vector<int> row;
	std::string line;

	while (getline(ifs, line))
	{
		std::string work;
		char token = ',';
		std::istringstream stream(line);

		while (getline(stream, work, token))
		{
			row.push_back(std::stoi(work));
		}
		m_map.push_back(row);
		{
			std::vector<int> e;
			e.swap(row);
			row.resize(0);
		}
	}

	mapList.emplace(str, m_map);
	return true;
}

std::vector<std::vector<int>> KochaEngine::CSVReader::GetMapData(const std::string& registName)
{
	auto itr = mapList.find(registName);
	if (itr == mapList.end())
	{
		assert(0);
	}

	return itr->second;
}

void KochaEngine::CSVReader::Clear()
{
	if(!mapList.empty())
	{
		std::unordered_map<std::string, std::vector<std::vector<int>>> e;
		e.swap(mapList);
	}
}
