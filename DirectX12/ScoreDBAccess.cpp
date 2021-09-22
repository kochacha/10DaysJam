//#include "ScoreDBAccess.h"
//
//#pragma comment( lib, "ws2_32.lib" )
//
//KochaEngine::ScoreDBAccess::ScoreDBAccess()
//{
//}
//
//KochaEngine::ScoreDBAccess::~ScoreDBAccess()
//{
//}
//
//void KochaEngine::ScoreDBAccess::Initialize()
//{
//	result = WSAStartup(MAKEWORD(1, 1), &wsa_data);
//
//	if (result != 0)
//	{
//		printf("WinSockの初期化エラー\n");
//	}
//	else
//	{
//		online = true;
//	}
//
//	port_no = (unsigned short)atoi(portNumber);
//
//	sock = socket(PF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET)
//	{
//		printf("ソケットオープンエラー\n");	
//	}
//
//	lp_host = gethostbyname(server);
//	if (lp_host == NULL)
//	{
//		addr = inet_addr(server);
//		lp_host = gethostbyaddr((char*)&addr, 4, AF_INET);
//
//		if (lp_host == NULL)
//		{
//			printf("%sが見つかりません\n", server);
//		}
//	}
//
//	memset(&sock_add, 0, sizeof(sock_add));
//	sock_add.sin_family = AF_INET;
//	sock_add.sin_port = htons(port_no);
//	sock_add.sin_addr = *((LPIN_ADDR)*lp_host->h_addr_list);
//
//}
//
//void KochaEngine::ScoreDBAccess::StartConnection()
//{
//	if (connect(sock, (PSOCKADDR)&sock_add, sizeof(sock_add)) != 0)
//	{
//		printf("ソケットに接続失敗\n");
//		closesocket(sock);
//		online = false;
//		return;
//	}
//	online = true;
//}
//
//void KochaEngine::ScoreDBAccess::GetRankingByAddScoreDB(char name[],int score)
//{
//	sprintf_s(str, "GET http://%s%s?name=%s&score=%d HTTP/1.0\r\n\r\n”", server, uploadURL, name, score, 1000);
//	//sprintf_s(str, "GET http://%s%s?name=%s&score=%d HTTP/1.0\r\n\r\n”", server, uploadURL, yourName, yourScore, 1000);
//	result = send(sock, str, (int)strlen(str), 0);
//
//	memset(rcv, 0, sizeof(rcv));
//
//	result = recv(sock, rcv, (int)sizeof(rcv) - 1, 0);
//
//	int i = 0;
//	int a = 0;
//	for (i = 0; rcv[i] != '\0'; i++)
//	{
//		if (rcv[i] == 'a')
//		{
//			a++;
//		}
//		else
//		{
//			a = 0;
//		}
//		if (a >= 15)
//		{
//			save = true;
//			i++;
//			break;
//		}
//	}
//	//名前
//	if (save)
//	{
//		rankName.clear();
//		rankScore.clear();
//		for (; rcv[i] != '\0';)
//		{
//			memset(tmpName, 0, sizeof(tmpName));
//			memset(tmpScore, 0, sizeof(tmpScore));
//			for (int j = 0; rcv[i] != ','; j++)
//			{
//				tmpName[j] = rcv[i];
//				i++;
//			}
//			std::string tmpN(tmpName, (int)strlen(tmpName));
//			rankName.push_back(tmpN);
//
//			i++;
//			for (int t = 0; rcv[i] != ','; t++)
//			{
//				tmpScore[t] = rcv[i];
//				i++;
//			}
//			int tmpS = atoi(tmpScore);
//			rankScore.push_back(tmpS);
//			i++;
//		}
//		save = false;
//	}
//
//	if (result == 0)
//	{
//	}
//	else if (result == SOCKET_ERROR) {
//		printf("recvエラー\n");
//	}
//	
//}
//
//void KochaEngine::ScoreDBAccess::LoadDBRanking()
//{
//	sprintf_s(str, "GET http://%s%s HTTP/1.0\r\n\r\n”", server, loadOnlyURL, 1000);
//	result = send(sock, str, (int)strlen(str), 0);
//
//	int i = 0;
//	int a = 0;
//	for (i = 0; rcv[i] != '\0'; i++)
//	{
//		if (rcv[i] == 'a')
//		{
//			a++;
//		}
//		else
//		{
//			a = 0;
//		}
//		if (a >= 15)
//		{
//			save = true;
//			i++;
//			break;
//		}
//	}
//	//名前
//	if (save)
//	{
//		rankName.clear();
//		rankScore.clear();
//		for (; rcv[i] != '\0';)
//		{
//			memset(tmpName, 0, sizeof(tmpName));
//			memset(tmpScore, 0, sizeof(tmpScore));
//			for (int j = 0; rcv[i] != ','; j++)
//			{
//				tmpName[j] = rcv[i];
//				i++;
//			}
//			std::string tmpN(tmpName, (int)strlen(tmpName));
//			rankName.push_back(tmpN);
//
//			i++;
//			for (int t = 0; rcv[i] != ','; t++)
//			{
//				tmpScore[t] = rcv[i];
//				i++;
//			}
//			int tmpS = atoi(tmpScore);
//			rankScore.push_back(tmpS);
//			i++;
//		}
//		save = false;
//	}
//
//	if (result == 0)
//	{
//	}
//	else if (result == SOCKET_ERROR) {
//		printf("recvエラー\n");
//	}
//}
//
//void KochaEngine::ScoreDBAccess::Disconnect()
//{
//	// ⑨．シャットダウン
//	if (shutdown(sock, SD_BOTH) != 0)
//	{
//		printf("シャットダウンに失敗しました\n");
//	}
//	// ⑩．ソケットを閉じる
//	closesocket(sock);
//	online = false;
//}
//
//std::vector<std::string> KochaEngine::ScoreDBAccess::GetRankingName()
//{
//	return rankName;
//}
//
//std::vector<int> KochaEngine::ScoreDBAccess::GetRankingScore()
//{
//	return rankScore;
//}
