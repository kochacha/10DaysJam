#include "CSVReader.h"
#include <windows.h>

using namespace std;

KochaEngine::CSVReader::CSVReader()
{
	Clear();
}

KochaEngine::CSVReader::~CSVReader()
{
}

void KochaEngine::CSVReader::LoadCSV(float num, std::string fileName)
{
	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		OutputDebugStringA("CSVファイルを開けませんでした");
		return;
	}

	std::vector<std::vector<float>> m_map;
	std::vector<float> row;
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
			std::vector<float> e;
			e.swap(row);
			row.resize(0);
		}
	}

	mapList.emplace(num, m_map);
}

std::vector<std::vector<float>> KochaEngine::CSVReader::GetMapData(float num)
{
	return this->mapList[num];
}

void KochaEngine::CSVReader::Clear()
{
	if(!mapList.empty())
	{
		std::map<float, std::vector<std::vector<float>>> e;
		e.swap(mapList);
	}
}
