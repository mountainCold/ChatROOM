#include "pch.h"
#include "methon.h"

#include <atlstr.h>
#include "resource.h"
#include <string>
void Login(Client* client,
	const char* pAccount,
	const char* password)
{
	CStringA buffer;
	buffer.Format("%s\x11%s", pAccount, password);
	client->send(login,
		buffer,
		buffer.GetLength());
}
void Register(Client* client,
	const char* pAccount,
	const char* password,
	const char* phone)
{
	CStringA buffer;
	buffer.Format("%s\x11%s\x11%s", pAccount, password, phone);
	client->send(registe,
		buffer,
		buffer.GetLength());
}
void Return(Client* pClient, DataPackType type, int status, const char* data)
{
	CStringA buffer;
	buffer.Format("%d\x11%d\x11%s", type, status, data);
	pClient->send(ReturnCode, buffer, buffer.GetLength());
}
void SendMsg(Client* pClient, const char* pMsg)
{
	pClient->send(sendMultiMsg, pMsg);
}
void AddFriend(Client* pClient, const char* name)
{
	pClient->send(addFriend, name);
}
void UpdateData(Client* pClient, int status)
{
	CStringA buffer;
	buffer.Format("%d", status);
	pClient->send(updateData, buffer);
}


void SendChatMSG(Client* pClient, const char* name, const char* data, int status)
{
	CStringA buffer;
	char* buff = new char[strlen(data)+1];
	strcpy_s(buff,strlen(data)+1 ,data);
	encode(buff);
	buffer.Format("%d\x11%s\x11%s", status,name,buff);
	pClient->send(ChatMSG, buffer); 
	delete[] buff;
}
void GetHistory(Client* pClient, const char* name,int status)
{
	CStringA buffer;
	buffer.Format("%d\x11%s", status,name);
	pClient->send(history, buffer);
}
void CreateRoom(Client* pClient, const char* roomName)
{
	pClient->send(createRoom, roomName);
}

void JoinRoom(Client* pClient, const char* roomName)
{
	pClient->send(joinRoom, roomName);
}
void SelectRoom(Client* pClient, const char* roomName)
{
	pClient->send(selectRoom, roomName);
}
void GetRoomMembers(Client* pClient, const char* roomName)
{
	pClient->send(getroommember, roomName);
}

DWORD CALLBACK recvProc1(LPVOID arg)
{
	Client* pClient = (Client*)arg;
	DataPackResult* pResult = NULL;
	while (1)
	{
		pResult = pClient->recv();
		if (pResult == NULL) {
			continue;
		}

		if (pResult->status > 0) {
			char buffer[200] = { 0 };
			sprintf_s(buffer, 200, "[%d] �Ĵ���ʧ��:%s\n", pResult->type, pResult->data);
			MessageBoxA(0, buffer, 0, 0);
			continue;
		}

		switch (pResult->type)
		{
		case login:
			printf("��½�ɹ�\n");
			g_isLogin = true;
			break;
		case registe:
			printf("ע��ɹ�\n");
			g_TFlag = true;
			break;
		case sendMultiMsg:
		{
			if (pResult->status == -1) { // ���Է����֪ͨ
				printf("���µ���Ϣ: %s\n", pResult->data);
			}
			else { // ����˵Ļظ�
				printf("\t��Ϣ���ͳɹ�\n");
			}
		}break;
		case addFriend:
		{
			CStringA buff;
			if (pResult->status == -1) { // ���Է����֪ͨ
				buff.Format("%s���������Ϊ����", pResult->data);
				if (IDYES == MessageBoxA(0, buff, "��������", MB_YESNO))
				{
					CStringA  buffer;
					Return(pClient, addFriend, 0, pResult->data);
					buffer.Format("%d\\%d\\%s\\", addFriend, 0, pResult->data);
					SendToWindow(hFriend, buffer);
				}
				else
				{
					Return(pClient, addFriend, 1, pResult->data);
				}
			}
			else if (pResult->status == 0)
			{
				CStringA  buffer;
				buffer.Format("%d\\%d\\%s\\", addFriend, pResult->status, pResult->data);
				SendToWindow(hFriend, buffer);
			}
		}break;
		case updateData:
		{
			char* Nextbuffer = NULL;
			char* buffa = NULL;
			buffa = strtok_s(pResult->data, "\\", &Nextbuffer);
			if (strcmp(buffa, "0") == 0)
			{
				buffa = strtok_s(NULL, "\\", &Nextbuffer);
				while (buffa != NULL)
				{
					user.friendList.push_back(buffa);
					buffa = strtok_s(NULL, "\\", &Nextbuffer);

				}
			}
			else if(strcmp(buffa, "1") == 0)
			{
				buffa = strtok_s(NULL, "\\", &Nextbuffer);
				while (buffa != NULL)
				{
					user.roomdList.push_back(buffa);
					buffa = strtok_s(NULL, "\\", &Nextbuffer);

				}
			}


		}break;
		case ChatMSG:
		{
			CStringA buff;
			if (pResult->status == -1) { // ����Ⱥ����Ϣ
				CStringA  buffer;
				buffer.Format("%d\\%s\\", ChatMSG, pResult->data);
				SendToWindow(hChatRoom, buffer);
			}
			else if (pResult->status == 0)// ���Ժ�����Ϣ
			{
				CStringA  buffer;
				decode(pResult->data);
				buffer.Format("%d\\%s\\", ChatMSG, pResult->data);
				SendToWindow(hChatFriend, buffer);
			}
		}break;
		case history:
		{
			if(pResult->status == 0)
			{
				CStringA  buffer;
				buffer.Format("%d\\%s\\", history, pResult->data);
				SendToWindow(hChatFriend, buffer);
			}
			if(pResult->status == -1)
			{
				CStringA  buffer;
				buffer.Format("%d\\%s\\", history, pResult->data);
				SendToWindow(hChatRoom, buffer);
			}
		}break;
		case createRoom:
		{
			CStringA  buffer;
			buffer.Format("%d\\%s\\", createRoom,pResult->data);
			SendToWindow(hRoom, buffer);
				
		}break;
		case joinRoom:
		{
			CStringA  buffer;
			buffer.Format("%d\\%s\\", joinRoom, pResult->data);
			SendToWindow(hRoom, buffer);
		}break;
		case selectRoom:
		{
			CStringA buff;
			if (pResult->status==0) //Ⱥ����
			{
				buff.Format("%s Ⱥ�Ĵ����Ƿ���룿", pResult->data);
				if (IDYES == MessageBoxA(0, buff, "���Ⱥ��", MB_YESNO))
				{
					JoinRoom(pClient, pResult->data);
				}
			}
			else if (pResult->status == -1) //Ⱥ������
			{
				buff.Format("%s Ⱥ�Ĳ������Ƿ񴴽���", pResult->data);
				if (IDYES == MessageBoxA(0, buff, "����Ⱥ��", MB_YESNO))
				{
					CreateRoom(pClient, pResult->data);
				}
			}
		}break;
		case getroommember:
		{
			CStringA buffer;
			buffer.Format("%d\\%s\\", getroommember, pResult->data);
			SendToWindow(hChatRoom, buffer);
		}break;
		}
	}
	return 0;
}
CString CATOCS(CStringA str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	memset(wstr, 0, len * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, len);
	return wstr;
}
CStringA CWTOCA(CStringW wstr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, LPCTSTR(wstr), -1, NULL, 0, NULL, NULL);
	char* str = new char[len];
	memset(str, 0, len * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, LPCWSTR(wstr), -1, str, len, NULL, NULL);
	return str;
}
void SendToWindow(HWND hFind, const char* data, int status)
{
	//����copydata��Ϣ
	COPYDATASTRUCT stcData = {};
	stcData.dwData = 0x100;						//�ĸ��ֽ�����ֵ
	stcData.lpData = (LPVOID)data;		//������
	stcData.cbData = strlen(data);						//��������С
	//2. ����WM_COPYDATA����
	SendMessage(hFind, WM_COPYDATA, NULL, (LPARAM)& stcData);
}
int recvProc(LPVOID arg)
{
	Client* pClient = (Client*)arg;
	DataPackResult* pResult = NULL;
	pResult = pClient->recv();
	if(pResult==NULL)return 0;
	if (pResult->status > 0) {
		char buffer[200] = { 0 };
		sprintf_s(buffer, 200, "[%d] �Ĵ���ʧ��:%s\n", pResult->type, pResult->data);
		MessageBoxA(0, buffer, 0, 0);
		return 0;
	}

	switch (pResult->type)
	{
	case login:
		printf("��½�ɹ�\n");
		g_isLogin = true;
		break;
	case registe:
		printf("ע��ɹ�\n");
		g_TFlag = true;
		break;
	case sendMultiMsg:
	{
		if (pResult->status == -1) { // ���Է����֪ͨ
			printf("���µ���Ϣ: %s\n", pResult->data);
		}
		else { // ����˵Ļظ�
			printf("\t��Ϣ���ͳɹ�\n");
		}
	}break;
	case addFriend:
	{
		CStringA buff;
		if (pResult->status == -1) { // ���Է����֪ͨ
			buff.Format("%s���������Ϊ����", pResult->data);
			if (IDYES == MessageBoxA(0, buff, "��������", MB_YESNO))
			{
				Return(pClient, addFriend, 0, pResult->data);
			}
			else
			{
				Return(pClient, addFriend, 1, pResult->data);
			}
		}
		else
		{
			MessageBoxA(0, pResult->data, "�����ظ�", 0);
		}
	}break;
	case updateData:
	{
		char* Nextbuffer = NULL;
		char* buffa = NULL;
		buffa = strtok_s(pResult->data, "\\", &Nextbuffer);
		if (strcmp(buffa, "0") == 0)
		{
			buffa = strtok_s(NULL, "\\", &Nextbuffer);
			while (buffa != NULL)
			{
				user.friendList.push_back(buffa);
				buffa = strtok_s(NULL, "\\", &Nextbuffer);

			}
		}
		else if (strcmp(buffa, "1") == 0)
		{
			buffa = strtok_s(NULL, "\\", &Nextbuffer);
			while (buffa != NULL)
			{
				user.roomdList.push_back(buffa);
				buffa = strtok_s(NULL, "\\", &Nextbuffer);

			}
		}



	}break;

	}
	return 0;
}
//���ܺ���  
void encode(char* v) {
	int len = strlen(v);
	for (size_t i = 0; i < len; i++)
	{
		v[i] = (v[i] ^ 5) + 5;
	}
}
void decode(char* v) {
	int len = strlen(v);
	for (size_t i = 0; i < len; i++)
	{
		v[i] = (v[i] - 5) ^ 5;
	}
}
