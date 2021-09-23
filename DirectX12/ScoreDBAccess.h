#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
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
		char rcv[1000];
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
		bool isCompleteInitialize;
	public:
		ScoreDBAccess();
		~ScoreDBAccess();

		void Initialize();
		void StartConnection();

		void GetRankingByAddScoreDB(char name[],int score);

		void LoadDBRanking();

		void Disconnect();

		const bool IsOnline() { return online; }

		std::vector<std::string> GetRankingName();
		std::vector<int> GetRankingScore();
	};
}
