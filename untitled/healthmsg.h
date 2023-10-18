#ifndef HEALTHMSG_H
#define HEALTHMSG_H

#include <QMainWindow>
#include <QMqttClient>
#include <QMessageBox>
#include <QtCore/QDateTime>
#include <QAbstractButton>
#include <QPushButton>
#include <QJsonParseError>
#include <QJsonObject>
#include <QLabel>

class healthmsg : public QMainWindow
{
    Q_OBJECT
public:
    healthmsg(QString hostname, QString port, QString topic, QPushButton *button, QLabel *label_mbp, QLabel *label_mhr);
    ~healthmsg();

    // 连接代理服务器
    void connect();

public slots:
    void updateLogStateChange();
    void on_buttonSubscribe_clicked();

private:
    // 需要监听的ip地址
    QString hostname;

    // 需要监听的端口
    QString port;

    // 需要监听的主题（了解mqtt的主题概念）
    QString topic;

    // 接收到的信息
    QString msg;

    // 订阅主题按键
    QPushButton *sub_button;

    // 显示数据的label 血氧 血压
    QLabel *label_mbp;
    QLabel *label_mhr;

    // 数据解析
    QString data;
    QJsonParseError jsonError;
    QJsonDocument jsonData;

    // 血氧 血压数据
    QString mHR;
    QString mBP;


    // mqtt对象
    QMqttClient *client;

  //  *****************内部调用函数****************************

    // 数据解析函数 解析msg
    void analysisMsg();

    // 获取血氧 血压数据
    QString getMHR();
    QString getMBP();
};

#endif // HEALTHMSG_H
