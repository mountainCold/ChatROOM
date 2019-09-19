#pragma once
#include <cstdint>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <vector>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
extern bool g_isLogin;
extern bool g_isConnet;
extern bool g_TFlag;
extern HWND hWin;
extern HWND hFriend;
extern HWND hChatFriend;
extern HWND hRoom;
extern HWND hChatRoom;
enum DataPackType {
	login = 1, //��¼
	registe = 2,//ע��
	sendMultiMsg = 3, //֪ͨ��Ϣ
	addFriend = 4, //��Ӻ���
	addRoom =5,//��Ӻ���
	ReturnCode =6, //֪ͨ�ظ� 0 �ɹ� elseʧ��
	updateData =7, //�������� 0 friend 1 room
	ChatMSG =8, //����������Ϣ 0 friend 1 room
	history =9,  // ��ʷ��Ϣ 0 friend 1 room
	createRoom=10, //����Ⱥ��
	joinRoom =11,  //����Ⱥ��
	selectRoom=12,  //����Ⱥ��
	getroommember=13 //��ȡȺ�ĳ�Ա
};
#pragma pack(push,1) //һ�ֽڶ���
struct DataPack
{
	DataPackType type;
	uint32_t	 size;
};
struct DataPackResult
{
	uint32_t type;		//�ظ�����
	int status; //״̬��
	uint32_t size;	 //��С
	char data[1]; //ֻ��һ���ֽڽ���Խ�����
};
#pragma pack(pop) //�ָ�Ĭ�϶��뷽ʽ
class Client {
	SOCKET m_hSocket;
	
public:

	/*��ʼ���׽��ֻ���
	*/
	static void initSocket();
	/*�������ӷ����
	*/
	Client(const char* serverIp, short port);
	/*������Ϣ
	*/
	void send(DataPackType type, const char* data, int size = -1);
	DataPackResult* recv();
	void freeResult(DataPackResult* p);
};
class User {
public:
	CStringA username;
	vector<CStringA> friendList;
	vector<CStringA> roomdList;
};
extern Client client;
extern User user;