#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>

#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

#include <QImage>//图片
#include <QPainter>//二维图形引擎
#include <QKeyEvent>
#include <QMessageBox>//消息提示框
#include <QPixmap> 
#include <QJsonDocument> // json 解析相关函数
#include <QJsonObject>



QT_BEGIN_NAMESPACE
namespace Ui { class client; }

// 默认服务器端IP地址
#define SERVER_IP "127.0.0.1"

// 服务器端口号
#define SERVER_PORT 8888

QT_END_NAMESPACE

class client : public QDialog
{
    Q_OBJECT

public:
    client(QWidget *parent = nullptr);
    ~client();

protected:
    //键盘事件的处理
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::client *ui;
    void paintEvent(QPaintEvent *);

    QTcpSocket tcpSocket;//tcp套接字
    QHostAddress serverIp;//服务器地址
    quint16 serverPort;//服务器端口
    
    QString snake_name; // 蛇的名字
    bool first_messige; // 判断是否为初始化消息

    QList<QVariant> data_list;  // 获取窗体重绘消息

    int center_index_x; // 头部x 坐标
    int center_index_y; // 头部y 坐标

    QVariantMap client_config_map;
private slots:
    //和服务器连接成功时执行的槽函数
    void onConnected();
    //接收聊天消息的槽函数
    void onReayRead();
    //网络异常时执行的槽函数
    void onError();

};


#endif // CLIENT_H
