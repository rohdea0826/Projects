# 群聊软件

---

该群聊软件分为两大部分客户端和服务端

### 服务器端
    服务器端维护了一个数据池，其采用生产者与消费者模型。
    
    每当有消息发送过来时，将数据发送给在线的每一个用户。
    
    通过jsoncpp完成数据的序列化与反序列化，以便通过网络传输。
    
    服务器端的用户列表采用map来存储。
    

### 客户端
    客户端主要用udp实现消息的发送与接收，每条消息都是一个拥有用户名、学校名、消息、指令等字符串的data类
    
    然后用data类中的成员函数通过jsoncpp完成数据的序列化及反序列化。
    
    使用vector维护了一个用户列表。
    
    通过信号与槽机制，完成udp数据接收时的响应。



    客户端之前是在linux上用ncurses做的简单的界面，
    
    然而由于一些兼容性问题，界面乱码，
    
    因此我又在window上用QT实现了其功能，
    
    QT配置好后可以用一份代码构建windows桌面和Android的应用，
    
    非常方便, 尽管Android这边界面很不友好😂。
    
    刚开始用QT，踩了很多坑，目前仍比较简陋，如有错误或可改进之处，还望不吝指教。

