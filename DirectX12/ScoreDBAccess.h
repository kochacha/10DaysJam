#pragma once
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <vector>
#include <string>

namespace KochaEngine
{
	class ScoreDBAccess
	{
	private:
		WSADATA wsa_data;
		LPHOSTENT lp_host;
		SOCKET sock;
		int result;
		SOCKADDR_IN sock_add;
		const char server[256] = "cafeserver.php.xdomain.jp";
		char uploadURL[256] = "/upload.php";
		char loadOnlyURL[256] = "/only_load.php";
		char rcv[256];
		const char portNumber[8] = "80";
		char str[1000];
		unsigned short port_no;
		unsigned int addr;

		std::vector<std::string> rankName;
		std::vector<int> rankScore;
		char tmpName[256];
		char tmpScore[256];
		bool save = false;
		char yourName[10];
		int yourScore = 0;
		char strScore[8];

		bool online = false;
	public:
		ScoreDBAccess();
		~ScoreDBAccess();

		void Initialize();
		void StartConnection();

		void GetRankingByAddScoreDB(char name[],int score);

		void LoadDBRanking();

		void Disconnect();

		std::vector<std::string> GetRankingName();
		std::vector<int> GetRankingScore();
	};
}

