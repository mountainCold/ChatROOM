# V1
#   1. 建立能够接收客户端连接的服务端
#   2. 收发客户端的数据
#   2.1 制定一个客户端和服务端数据传输的标准
# V2
#   3. 实现服务端群发客户端消息
#       3.1 其中一个客户端发送消息, 其他在线的客户端都能接收到该消息.
#   4. 实现服务端给客户端转发消息
#       4.1 客户端上线时必须起一个名字. 不起名字就没有这个功能
#       4.2 客户端在发送消息时, 消息如果以 `@xxx 消息内容` 的格式发送.
# #           服务端就只将消息的内容发给具有指定名字的客户端
# V3
#   5. 实现注册,登陆功能
#   5.1 数据库用户表的设计
#   5.2 插入语句
#   5.3 查询语句
# V4
#   6. 实现好友功能
# V5
#   7. 实现群功能
import base64
import hashlib
import datetime
import random
import socket
import struct
import threading
from enum import Enum
import pymysql


class Database:
    def __init__(self):
        self.connecter = pymysql.connect("127.0.0.1","root","root","chat")
        self.cursor = self.connecter.cursor(cursor=pymysql.cursors.DictCursor)

# 构造一个全局的数据库对象
_db = Database()
#添加好友临时列表
_dictFriend ={}
#在线成员
_clientDict = {}

class Request(Enum):
    # 登陆请求
    # 客户端 -> 服务端, 参数: 账号\n密码
    login       = 1

    # 注册请求
    # 客户端 -> 服务端, 参数: 账号\n密码
    register    = 2

    # 发消息请求
    # 客户端 -> 服务端, 参数: 消息
    sendMultiMsg= 3
    #添加好友
    # 客户端 -> 服务端, 参数: 目标用户名
    addFriend =4
    #添加群聊
    # 客户端 -> 服务端, 参数: 目标房间名
    addRoom =5
    #通知回复
    # 客户端 -> 服务端, 参数: 回复类型，回复码，回复对象
    returnCode= 6
    # 更新数据
    # 客户端 -> 服务端, 参数: 0 friend 1 room
    UpdateData = 7
    # 聊天消息
    # 客户端 -> 服务端,
    ChatMSG =8
    # 历史消息
    # 客户端 -> 服务端,
    history =9
    # 创建群
    # 客户端 -> 服务端, 参数: 群的名称
    createRoom = 10

    # 加入群
    # 客户端 -> 服务端, 参数: 群的名称
    joinRoom = 11
    #搜索群
    # 客户端 -> 服务端, 参数: 群的名称
    selectRoom =12
    # 获取群聊成员
    # 客户端 -> 服务端, 参数: 群的名称
    getroommember=13



class ResultCode(Enum):
    notify    = -1
    success   = 0
    bad_pack  = 1
    user_exist = 2
    user_non_exist = 3
    password_wrong = 4
    same_name =5
    friend_exist = 6
    refuse =7
    room_exist=8
    room_non_exist=9

def md5( s ):
    hashObj = hashlib.md5()
    hashObj.update( s.encode('gb2312') )
    return hashObj.hexdigest()

class Handler():
    def __init__(self):
        self.args = None
        self.socket=None

    def register(self):
        print('注册 参数: ' , self.args)
        # 注册的流程
        # 1. 检查账号有没有注册过.
        sql = "select id from user_info where username='%s';"%(self.args[0])
        # 执行sql语句
        _db.cursor.execute(sql)
        # 获取结果集
        result = _db.cursor.fetchone()
        if result != None:
            sendToclientSocket(self.socket,
                               Request.register.value,
                               ResultCode.user_exist.value,
                               "用户已存在")
            return
        # 2. 保存账号到数据库
        passwd = md5(self.args[1])
        sql = "insert into user_info (username, password,phone) VALUE('%s','%s','%s');" \
              %(self.args[0],passwd,self.args[2])
        _db.cursor.execute(sql)
        _db.cursor.execute('commit;')# 提交到数据库
        sendToclientSocket(self.socket,
                           Request.register.value,
                           ResultCode.success.value,
                           "注册成功")

    def login(self):
        print('登陆 参数: ' , self.args)
        # 1. 查表,判断用户是否存在
        sql = "select * from user_info where username='%s';"%(self.args[0])
        _db.cursor.execute(sql)
        userInfo =_db.cursor.fetchone()
        if userInfo == None:
            sendToclientSocket(self.socket,
                               Request.login.value,
                               ResultCode.user_non_exist.value,
                               "用户名不存在")
            return
        # 2. 比较密码是否正确
        if userInfo['password'] != md5( self.args[1] ):
            sendToclientSocket(self.socket,
                               Request.login.value,
                               ResultCode.password_wrong.value,
                               "密码错误")
            return
        if self.args[0]  in _clientDict.keys():
            sendToclientSocket(self.socket,
                               Request.login.value,
                               ResultCode.user_exist.value,
                               "用户已经登录")
            return
        sendToclientSocket(self.socket,
                           Request.login.value,
                           ResultCode.success.value,
                           "登陆成功")

        # 将已登陆的用户信息保存到字典
        # self.user = userInfo
        self.__dict__.update(userInfo)
        _clientDict[self.username] = self


    def sendMultiMsg(self):
        msg = self.args[0]
        for k,v in _clientDict.items():
            if( k == self.username):
                continue
            sendToclientSocket( v.socket,
                                Request.sendMultiMsg.value,
                                ResultCode.notify.value,
                                msg)

    def addFriend(self):
        name =self.args[0]
        sql = "select id from user_info where username='%s';" % name
        _db.cursor.execute(sql)
        user1= _db.cursor.fetchone()
        userid1=user1['id']
        sql = "select id from user_info where username='%s';" % self.username
        _db.cursor.execute(sql)
        user2= _db.cursor.fetchone()
        userid2=user2['id']
        sql = "select id from friend_info where user1=%d and user2=%d;" % (userid2,userid1)
        _db.cursor.execute(sql)
        userINfo= _db.cursor.fetchone()
        sql = "select id from friend_info where user1=%d and user2=%d;" % (userid1,userid2)
        _db.cursor.execute(sql)
        userINfo2= _db.cursor.fetchone()
        if None == userid1:
            sendToclientSocket(_clientDict[name].socket,
                               Request.addFriend.value,
                               ResultCode.user_non_exist.value,
                               "用户名不存在")
            return
        if userid1==userid2:
            sendToclientSocket(_clientDict[name].socket,
                               Request.addFriend.value,
                               ResultCode.same_name.value,
                               "不能添加自己")
            return
        if None!=userINfo or  None!=userINfo2:
            sendToclientSocket(_clientDict[name].socket,
                               Request.addFriend.value,
                               ResultCode.friend_exist.value,
                               "对方已经是你的好友了")
            return
        if name in _clientDict.keys():
            _dictFriend[name]=[userid1,userid2]
            sendToclientSocket(_clientDict[name].socket,
                               Request.addFriend.value,
                               ResultCode.notify.value,
                               self.username)
            return
        else:
            sql = "insert into unRead (user1, user2,type,data) VALUE('%s','%s',%d,'%s');" \
                  % (self.username,name,Request.addFriend.value,self.username)
            _db.cursor.execute(sql)
            _db.cursor.execute('commit;')  # 提交到数据库
            sendToclientSocket(_clientDict[name].socket,
                               Request.addFriend.value,
                               ResultCode.user_non_exist.value,
                               "用户未上线")
            return

    def ChatMSG(self):
        print(self.args)
        type1 =eval(self.args[0])
        name=self.args[1]
        data=self.args[2]
        #私聊消息 0表示
        if type1 == 0 :
            try:
                if name in _clientDict.keys():
                    sendToclientSocket(_clientDict[name].socket,
                                       Request.ChatMSG.value,
                                       ResultCode.success.value,
                                       data)
                else:
                    print("待续")
                    #sql = "insert into unRead (user1, user2,type,data) VALUE('%s','%s',%d,'%s');" \
                     #     % (self.username, name, Request.ChatMSG.value, data)
                    #_db.cursor.execute(sql)
                now_time = datetime.datetime.now()
                time1_str = datetime.datetime.strftime(now_time, '%Y-%m-%d %H:%M:%S')
                data = self.username + ":" + time1_str + "\x10" + data + "\x10"
                sql = "insert into pastRecords (user1, user2,data) VALUE('%s','%s','%s');" \
                      % (self.username, name, data)
                _db.cursor.execute(sql)
                _db.cursor.execute('commit;')  # 提交到数据库

            except Exception as e:
                print(e)
                _db.cursor.execute('rollback;')
        # 群聊消息 1表示
        if type1 == 1:
            try:
                now_time = datetime.datetime.now()
                time1_str = datetime.datetime.strftime(now_time, '%Y-%m-%d %H:%M:%S')
                data = self.username + ":" + time1_str + "\x10" + data + "\x10"
                sql = "insert into pastRoomRecords (user1, room,data) VALUE('%s','%s','%s');" \
                      % (self.username, name, data)
                _db.cursor.execute(sql)
                _db.cursor.execute('commit;')  # 提交到数据库
                sql = "select user from user_room where room ='%s'" %name
                _db.cursor.execute(sql)
                userlist = _db.cursor.fetchall()
                for u in userlist:
                    if u['user'] == self.username:
                        continue
                    if u['user'] in _clientDict.keys():
                        sendToclientSocket(_clientDict[u['user']].socket,
                                           Request.ChatMSG.value,
                                           ResultCode.notify.value,
                                           data)
                    else:
                        print("待续")
            except Exception as e:
                print(e)
                _db.cursor.execute('rollback;')

    def UpdateData(self):
        msg =eval(self.args[0])
        #0 表示更新好友列表
        if 0==msg:
            userlist = []
            namelist=[]
            sql = "select id from user_info where username='%s';" % self.username
            _db.cursor.execute(sql)
            user = _db.cursor.fetchone()
            userid = user['id']
            sql = "select user1 from friend_info where user2='%s';" % userid
            _db.cursor.execute(sql)
            relist1 = _db.cursor.fetchall()
            print(type(relist1),relist1)
            sql = "select user2 from friend_info where user1='%s';" % userid
            _db.cursor.execute(sql)
            relist2 = _db.cursor.fetchall()
            hebing(relist1,relist2,userlist)
            for i in userlist:
                a=i.values()
                for ax in a:
                    sql = "select username from user_info where id =%s" % ax
                    _db.cursor.execute(sql)
                    name = _db.cursor.fetchone()
                    namelist.append(name['username'])

            sax="0\\"
            for i in namelist:
                sax+=i+"\\"
            sendToclientSocket(self.socket,Request.UpdateData.value,ResultCode.success.value,sax)
        if 1==msg:
            roomlist = []
            sql = "select room from user_room where user='%s';" % self.username
            _db.cursor.execute(sql)
            user = _db.cursor.fetchall()
            sax = "1\\"
            if 0 != len(user) :
                for i in user:
                    roomlist.append(i['room'])
                for x in roomlist:
                    sax += x + "\\"
            sendToclientSocket(self.socket,Request.UpdateData.value,ResultCode.success.value,sax)

    def returnCode(self):
        print(self.args)
        if Request.addFriend.value == eval(self.args[0]):
            if '0' == self.args[1]:
                try:
                    sql = "insert into friend_info(user1,user2) values(%d,%d)" % (
                        _dictFriend[self.username][0], _dictFriend[self.username][1])
                    _db.cursor.execute(sql)
                    _db.cursor.execute('commit;')  # 提交到数据库
                    del _dictFriend[self.username]
                    sendToclientSocket(_clientDict[self.args[2]].socket,
                                       Request.addFriend.value,
                                       ResultCode.success.value,
                                       self.username)
                except Exception as e:
                    _db.cursor.execute('rollback')  # 提交到数据库
                    print(e)
            else:
                sendToclientSocket(_clientDict[self.args[2]].socket,
                                   Request.addFriend.value,
                                   ResultCode.refuse.value,
                                   "对方拒绝了你的好友请求")

    def history(self):
        msg = eval(self.args[0])
        name = self.args[1]
        #0表示获取私聊消息
        if 0 == msg:
            sql = "select data from pastRecords where (user1='%s'and user2='%s') or (user1='%s'and user2='%s')" % (self.username,name,name,self.username)
            _db.cursor.execute(sql)
            datas = _db.cursor.fetchall()
            data="0\x11"
            for i in datas:
                data =data+i['data']+"\x11"
            sendToclientSocket(self.socket, Request.history.value, ResultCode.success.value, data)
        if 1 == msg:
            sql = "select data from pastRoomRecords where room='%s'" % name
            _db.cursor.execute(sql)
            datas = _db.cursor.fetchall()
            data="0\x11"
            for i in datas:
                data =data+i['data']+"\x11"
            sendToclientSocket(self.socket, Request.history.value, ResultCode.notify.value, data)

    def selectRoom(self):
        roomName = self.args[0]
        # 1. 检查群是否已经创建
        sql = "select id from room_info where name='%s';" % roomName
        _db.cursor.execute(sql)
        roomid = _db.cursor.fetchone()
        if roomid:  # 群存在了
            sendToclientSocket(self.socket,
                               Request.selectRoom.value,
                               ResultCode.success.value,
                               roomName)
            return
        sendToclientSocket(self.socket,
                           Request.selectRoom.value,
                           ResultCode.notify.value,
                           roomName)
    def createRoom(self):
        roomName=self.args[0]
        # 2. 创建一个新的
        try:
            sql = "insert into room_info (name,creator) VALUES('%s','%s');" % (roomName, self.username)
            _db.cursor.execute(sql)
            _db.cursor.execute('commit;')
        except Exception as e:
            print(e)
            _db.cursor.execute('rollback;')

        # 3. 将自己自动加入到新群中
        # 3.1 获取群在数据表中的id
        sql = "insert into user_room (room,user) VALUES('%s','%s');" % (roomName,self.username)
        _db.cursor.execute(sql)
        _db.cursor.execute('commit;')
        # 4. 返回成功
        sendToclientSocket(self.socket,
                           Request.createRoom.value,
                           ResultCode.success.value,
                           roomName)

    def joinRoom(self):
        roomName=self.args[0]
        # 3.1 是否已经存在
        sql = "select id from user_room where user='%s' and room='%s';" % (self.username,roomName)
        _db.cursor.execute(sql)
        room = _db.cursor.fetchall()
        if 0!=len(room):
            sendToclientSocket(self.socket,
                               Request.selectRoom.value,
                               ResultCode.user_exist.value,
                               "你已经在群聊中")
            return
        try:
            sql = "insert into user_room (room,user) VALUES('%s','%s');" % (roomName, self.username)
            _db.cursor.execute(sql)
            _db.cursor.execute('commit;')
        except Exception as e:
            print(e)
            _db.cursor.execute('rollback;')
        # 4. 返回成功
        sendToclientSocket(self.socket,
                           Request.joinRoom.value,
                           ResultCode.success.value,
                           roomName)

    def getroommember(self):
        roomName = self.args[0]
        sql = "select user from user_room where room='%s';" % roomName
        _db.cursor.execute(sql)
        room = _db.cursor.fetchall()
        if  len(room)==0:
            sendToclientSocket(self.socket,
                               Request.getroommember.value,
                               ResultCode.room_exist.value,
                               "此群不存在")
            return
        friendList = []
        for f in room:
            friendList.append(f['user'])
        naems = '\\'.join(friendList)
        sendToclientSocket(self.socket,
                           Request.getroommember.value,
                           ResultCode.success.value,
                           naems)

#查询是否有未读消息
def selectUnreadMsg(username):
    sql = "select id from unRead where user2='%s';" % username
    _db.cursor.execute(sql)
    userInfos = _db.cursor.fetchall()
    num =len(userInfos)
# 查询历史消息
def selectPastRecords(username):
    sql = "select id from pastRecords where user2='%s';" % username
    _db.cursor.execute(sql)
    userInfos = _db.cursor.fetchall()
#将元组 a和B中的元素一起放入l列表中
def hebing(a,b,l):
    def add(x):
        return l.append(x)
    list(map(add, a))
    list(map(add, b))
def sendToclientSocket(clientSocket,type, status, strData):
    data = strData.encode('gb2312')
    # data=base64.b64encode(data)
    size = len(data)
    print(size)
    strFormat = 'iii%ds'%(size)
    rawData = struct.pack( strFormat ,type, status, size,data)
    clientSocket.send( rawData )

def recvFromclientSocket(clientSocket):
    data = clientSocket.recv(8)
    if len(data) == 0:
        raise Exception('客户端断开连接')
    # 接收8个字节的头部.
    type, size = struct.unpack('ii', data)
    print('type=%d , size%d' % (type, size))

    # 接收后续的内容
    bodyData = clientSocket.recv(size)
    endata=bodyData.decode('gb2312')
    print('bodyData: %s' % endata)
    # 解码之后分割参数
    # return type , bodyData.decode('gb2312').split('\x11')
    return type , endata.split('\x11')


class Server:
    def __init__(self, ip , port):
        # 1. 创建一个套接字
        self.socket = socket.socket()
        # 2. 绑定ip和端口
        self.socket.bind( (ip , port) )
        # 3. 监听
        self.socket.listen( socket.SOMAXCONN )

    def recvclientSocketData(self,handler):
        type,args = None,None

        while  True:
            type , args = None,None
            try:
                # 接收数据
                type , args = recvFromclientSocket(handler.socket)
            except Exception as e:
                print("已经下线"+handler.username)
                del _clientDict[ handler.username ]
                break
            try:
                # 将一个整型值转换成枚举类型
                # 如果这个整型值在枚举类型中没有定义
                # 就会抛出异常
                type = Request(type)
            except:
                sendToclientSocket(handler.socket,
                                   type,
                                   ResultCode.bad_pack.value ,
                                   "无效的请求")
            # 1. 获取函数
            func = getattr(handler , type.name)
            # 2. 设置属性, 将客户端传来的参数设置成类对象的一个成员变量
            handler.args = args
            func()

    def run(self):
        # 4. 循环接收客户端连接
        while True:
            clientSocket , addr = self.socket.accept()
            h = Handler()
            h.socket = clientSocket
            h.addr = addr
            print('接入新客户端: ' , addr)
            threading.Thread(
                target=self.recvclientSocketData,
                args=(h,)
            ).start()


if __name__ == '__main__':
    s = Server('127.0.0.1',10086)
    s.run()