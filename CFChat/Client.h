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
	login = 1, //登录
	registe = 2,//注册
	sendMultiMsg = 3, //通知消息
	addFriend = 4, //添加好友
	addRoom =5,//添加好友
	ReturnCode =6, //通知回复 0 成功 else失败
	updateData =7, //跟新数据 0 friend 1 room
	ChatMSG =8, //发送聊天消息 0 friend 1 room
	history =9,  // 历史消息 0 friend 1 room
	createRoom=10, //创建群聊
	joinRoom =11,  //加入群聊
	selectRoom=12,  //搜索群聊
	getroommember=13 //获取群聊成员
};
#pragma pack(push,1) //一字节对齐
struct DataPack
{
	DataPackType type;
	uint32_t	 size;
};
struct DataPackResult
{
	uint32_t type;		//回复类型
	int status; //状态码
	uint32_t size;	 //大小
	char data[1]; //只给一个字节进行越界操作
};
#pragma pack(pop) //恢复默认对齐方式
class Client {
	SOCKET m_hSocket;
	
public:

	/*初始化套接字环境
	*/
	static void initSocket();
	/*创建连接服务端
	*/
	Client(const char* serverIp, short port);
	/*发送消息
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