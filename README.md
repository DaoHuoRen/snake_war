# snake_war
贪吃蛇大战
本项目为c++ 测试项目,功能包括：生成地图，初始化食物，贪吃蛇的 创建 移动 和 销毁
## 1. 结构和环境
	项目是 c/s 架构  分为 服务端：snake_server 和 客户端snake_client 两部分
	1. snake_server  环境: C++ 11
		使用 epoll 循环处理客户端连接
		将食物和 蛇的状态信息拼接出 json 同步给每个客户端
		接收客户端的 方向消息，更改蛇的移动方向
	2. snake_client  环境: C++ 11 和  Qt 5.13
		使用qt 的库函数  解析json 并将 蛇和食物的状态 画在窗体上
		根据键盘事件 发送相应的消息到服务器
		蛇死掉时 断开和服务器的链接结束游戏
以上环境请在 ubuntu 18.4.3 下自行安装 
## 2. 导入指南
	1.可将两个项目中都有 pro 文件 导入 qt  即可编辑开发
	2.其中 snake_client 中使用到图片文件，请关闭影子构建模式
		项目>	构建设置 中取消勾选 shadow build
	3. snake_client 的配置在 client.h 中 ， snake_server 的配置在 config.h  
		请进行必要配置(服务器ip 端口号等)后方可运行
	4. snake_server 并不依赖qt 环境，可以使用项目中的 Makefile 文件 通过命令
		make  // 进行编译
		make clean  // 进行清理
		./snake_server  // 运行
## 3. 已知bug 
	1. 有时蛇的尾部一节会断掉并留在原地
	2. 蛇经过自己的身体时会留下空白区域  （感觉像把自己的身体压到了地下）
## 4. 参考
	1. [linux下聊天demo支持私聊和群发](https://www.jianshu.com/p/0862ff14d273)
	2. [qt 聊天室服务器端](https://github.com/Dgrefic/ServerDialog "qt 聊天室服务器端")
	3. [qt 聊天室客户端](https://github.com/Dgrefic/ClientDialog "qt 聊天室客户端")
	4. [c++11实现异步定时器](https://www.liangzl.com/get-article-detail-1788.html "c++11实现异步定时器")
