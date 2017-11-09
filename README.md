# tcp-server
tcp-server简单实现

# 功能实现
1. 支持多客户端同时连接
2. 只有一个用户是controller，其他用户都是readers
3. controller可以执行"write"命令，其他用户只能执行"read"

# 客户端请求处理逻辑：
1.　"read"请求：所有用户都响应Success
2.　"write"请求：只有controller用户响应Success，其他用户响应Failure
3. "controller"请求：表示请求获取controller权限.
		当前没有controller用户响应Success，client为controller用户响应Success;
		否则响应Failure;
4. 其他请求：除上述三种请求外都响应Failure

# 代码逻辑
1. 每个client连接对应server的一个线程
2. 以线程号作为区分client身份的标志
3. 

