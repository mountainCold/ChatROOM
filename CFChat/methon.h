#include "Client.h"
#pragma once
void Login(Client* client,
	const char* pAccount,
	const char* password);
void Register(Client* client,
	const char* pAccount,
	const char* password,
	const char* phone);
void SendMsg(Client* pClient, const char* pMsg);
void Return(Client* pClient, DataPackType type, int status, const char* data);
void AddFriend(Client* pClient, const char* name);
void UpdateData(Client* pClient, int status);
void GetHistory(Client* pClient, const char* name, int status);
void CreateRoom(Client* pClient, const char* roomName);
void JoinRoom(Client* pClient, const char* roomName);
void SelectRoom(Client* pClient, const char* roomName);
void GetRoomMembers(Client* pClient, const char* roomName);
int recvProc(LPVOID arg);
CString CATOCS(CStringA str);
CStringA CWTOCA(CStringW wstr);
void SendToWindow(HWND hFind, const char* data,int status=-1);
void SendChatMSG(Client* pClient, const char* name, const char* data, int status);
DWORD CALLBACK recvProc1(LPVOID arg);
void encode(char* v);
void decode(char* v);