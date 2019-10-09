#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);
    first_messige = true;
    this->resize(301*client_config_map["CLIENT_STEP"].toInt(), 151*client_config_map["CLIENT_STEP"].toInt());

    serverIp.setAddress(SERVER_IP);
    //向服务器发送连接请求:
    //如果连接成功,发送信号:connected
    //如果连接失败,发送信号:error
    tcpSocket.connectToHost(serverIp,SERVER_PORT);

    //和服务器连接成功时,发送信号:connected
    connect(&tcpSocket,SIGNAL(connected()),
            this,SLOT(onConnected()));
    //收到聊天消息,发送信号:readyRead
    connect(&tcpSocket,SIGNAL(readyRead()),
            this,SLOT(onReayRead()));
    //网络异常时,发送信号:error
    connect(&tcpSocket,SIGNAL(error(
                QAbstractSocket::SocketError)),
            this,SLOT(onError()));

}

client::~client()
{
    tcpSocket.disconnectFromHost();
    delete ui;
}

//绘图事件处理函数
void client::paintEvent(QPaintEvent *){
    //创建"画家"对象
    QPainter painter(this);
    int head_lef =31 / 2 * client_config_map["CLIENT_STEP"].toInt();
    int head_top =15 / 2 * client_config_map["CLIENT_STEP"].toInt();
    //构造要绘制图像(从资源文件中获取)
    QImage image_map("images/" + client_config_map["map"].toString());
    // 获取当前窗体的坐标
    QRect rect = this->rect();
    QRect target_map(rect.left()+ center_index_x * -client_config_map["CLIENT_STEP"].toInt() + head_lef,rect.top()+ center_index_y * -client_config_map["CLIENT_STEP"].toInt() + head_top, client_config_map["MAP_WIDTH"].toInt()*client_config_map["CLIENT_STEP"].toInt(), client_config_map["MAP_HEIGHT"].toInt()*client_config_map["CLIENT_STEP"].toInt());
    //使用painter将image图片画到rect矩形区域中
    painter.drawImage(target_map,image_map);


    // 根据服务器端返回的数据 绘制窗体
    foreach(QVariant item, data_list)
    {
        QVariantMap map = item.toMap();
        QString type = map["t"].toString();
        int x = map["x"].toInt();
        int y = map["y"].toInt();
        QImage *image;
        if(type == "0"){
            image = nullptr;
        }else{
            image = new QImage("images/"+ client_config_map[type].toString());
        }
        int lef = head_lef + (x - center_index_x) * client_config_map["CLIENT_STEP"].toInt();
        int top = head_top + (y - center_index_y) * client_config_map["CLIENT_STEP"].toInt();
        QRect target(lef,top,client_config_map["CLIENT_STEP"].toInt(),client_config_map["CLIENT_STEP"].toInt());
        //使用painter将image图片画到rect矩形区域中
        if(image){
            painter.drawImage(target,*image);
        }
    }
    painter.drawText(head_lef-2, head_top -2,snake_name);

}
//只要有键盘输入，就会执行这个函数。
void client::keyPressEvent(QKeyEvent *event)
{
    //event->key();就是我们输入的按键
    switch(event->key()){
    case Qt::Key_Up :
        //向上移动
        tcpSocket.write(QString("key_up").toUtf8());
        update();//触发绘图事件
        break;
    case Qt::Key_Down:
        //向下移动
        tcpSocket.write(QString("Key_Down").toUtf8());
        update();//触发绘图事件
        break;
    case Qt::Key_Left:
        //向左移动
        tcpSocket.write(QString("Key_Left").toUtf8());
        update();//触发绘图事件
        break;
    case Qt::Key_Right:
        //向右移动
        tcpSocket.write(QString("Key_Right").toUtf8());
        update();//触发绘图事件
        break;
    default:
        event->ignore();
    }
}

//和服务器连接成功时执行的槽函数
void client::onConnected()
{
    qDebug() << "和服务器连接成功";
}

//接收聊天消息的槽函数
void client::onReayRead()
{
    if(tcpSocket.bytesAvailable()){
        //读取聊天消息
        QByteArray buf = tcpSocket.readAll();
        QString json = buf;
// 用于测试 json 解析
//        qDebug() <<  json;
//        QString json("{"
//                "\"encoding1\" : 7,"
//                "\"encoding\" : \"UTF-8\","
//                "\"plug-ins\" : ["
//                "\"python\","
//                "\"c++\","
//                "\"ruby\""
//                "],"
//                "\"indent\" : { \"length\" : 3, \"use_space\" : true }"
//                "}");
//           QString json("["
//                        "{\"encoding\" : \"UTF-8\","
//                        "\"encoding1\" : \"UTF-81\","
//                        "\"encoding2\" : \"UTF-82\"}"
//                        "]");
//        first_messige = false;


        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(json.toUtf8(), &error);
        if(QJsonParseError::NoError == error.error)
        {
            if(first_messige){
                //map
                client_config_map = document.toVariant().toMap();
                snake_name = client_config_map["snake_name"].toString();
                int c_width = client_config_map["CLIENT_WIDTH"].toInt() * client_config_map["CLIENT_STEP"].toInt();
                int c_height = client_config_map["CLIENT_HEIGHT"].toInt() * client_config_map["CLIENT_STEP"].toInt();
                qDebug() << snake_name <<  c_width << "|" <<c_height;
                this->resize(c_width,c_height);
            }else{
                //数组
                data_list = document.toVariant().toList();
                foreach(QVariant item, data_list)
                {
                    QVariantMap map = item.toMap();
                    if(map["m"].toInt() == 1){
                        center_index_x = map["x"].toInt();
                        center_index_y = map["y"].toInt();
                    }
                }
            }
        }
        first_messige = false;
        update();//触发绘图事件
    }
}

//网络异常时执行的槽函数
void client::onError(){
    //errorString:获取网络异常原因的字符串
    QMessageBox::critical(this,"网络异常",tcpSocket.errorString());
}
