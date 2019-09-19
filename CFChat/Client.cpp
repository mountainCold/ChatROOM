#include "pch.h"
#include "Client.h"
#include <Ws2tcpip.h>
#include <regex>
#include <string.h>
#include <atlstr.h>
bool g_isLogin = false;
bool g_isConnet = true;;
bool g_TFlag = false;
 HWND hWin =NULL;
 HWND hFriend =NULL;
 HWND hChatFriend =NULL;
 HWND hChatRoom =NULL;
 HWND hRoom =NULL;
Client client("127.0.0.1", 10086);
User user;
void Client::initSocket()
{
	//WSADATA wd = { 0 };
	//WSAStartup(MAKEWORD(2, 2), &wd);
}

Client::Client(const char* serverIp, short port)
{
	WSADATA wd = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wd);
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr.S_un.S_addr = inet_addr(serverIp);
	inet_pton(AF_INET, serverIp, &addr.sin_addr);
	int size =connect(m_hSocket, (sockaddr*)& addr, sizeof(addr));
	if (size == SOCKET_ERROR)
	{
		MessageBox(0, L"连接服务器失败", 0, 0);
		g_isConnet = false;
	}
	
}

void Client::send(DataPackType type, const char* data, int size)
{
	if (size ==-1)
	{
		size = strlen(data);
	}
	DataPack dp = { type,size };		
	::send(m_hSocket, (char*)& dp, sizeof(dp), 0);
	::send(m_hSocket, data, size, 0);
}

DataPackResult* Client::recv()
{
	DataPackResult head = { 0 };
	//没有data数据
	if (::recv(m_hSocket, (char*)& head, sizeof(head) - 1, 0) != sizeof(head) - 1) {
		return NULL;
	}
	DataPackResult* pBuff = (DataPackResult*)malloc(sizeof(DataPackResult) + head.size);
	memset(pBuff, 0, head.size + sizeof(DataPackResult));
	memcpy(pBuff, &head, sizeof(head));
	if (::recv(m_hSocket, pBuff->data, pBuff->size, 0) != pBuff->size) {
		free(pBuff);
		return NULL;
	}
	return (DataPackResult*)pBuff;
}

void Client::freeResult(DataPackResult* p)
{
	free(p);
}
