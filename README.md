# tcp-server
tcp-server简单实现

# 功能实现
1. 支持多客户端同时连接
2. 只有一个用户是controller，其他用户都是readers
3. controller可以执行"write"命令，其他用户只能执行"read"

# 代码逻辑
1. 每个client连接对应server的一个线程
2. 以线程号作为区分client身份的标志
3. 客户端请求处理逻辑：

    a.　"read"请求：所有用户都响应Success

    b.　"write"请求：只有controller用户响应Success，其他用户响应Failure

    c. "controller"请求：表示请求获取controller权限.

            当前没有controller用户响应Success，client为controller用户响应Success;
            
            否则响应Failure;

    d. 其他请求：除上述三种请求外都响应Failure

# 文件描述
1. client.c //客户端实现
2. server.c //服务器实现
3. handle_require.c //服务器处理客户端请求实现

# 代码运行与测试
## 运行
首先在src文件夹下运行make命令，生成执行文件server和client

```shell
#服务器运行
./server [ip] [port]

#客户端运行
./client [server-ip] [server-port]
```
**ps.**
1. 为了保证服务器能处理客户端发送的信息，请保证`[port]`与`[server-port]`填写的是同一端口号
2. 服务器与客户端应保证在同一网段
3. 结束服务器进程之后，被监听的端口并不会立即回收，系统会在一段时间之后进行自动回收。所以两次连续启动服务器并绑定同一端口号可能会出现不能绑定。请尝试新的端口号。

# TODO
1. 由于只有一个用户可以进行写操作，所以没有添加读写锁保护机制
2. 未添加controller用户下线机制。
    主动下线：controller用户主动发送命令下线；
    被动下线：服务器向客户端发送心跳信息，若在规定时间段内没有收到客户端的ack，就认为controller用户已经下线；



