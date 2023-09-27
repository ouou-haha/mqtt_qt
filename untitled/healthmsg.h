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


    void connect();

public slots:
    void updateLogStateChange();
    void on_buttonSubscribe_clicked();

private:
    QString hostname;
    QString port;
    QString topic;
    QString msg;
    QPushButton *sub_button;
    QLabel *label_mbp;
    QLabel *label_mhr;
    QString data;
    QJsonParseError jsonError;
    QJsonDocument jsonData;

    QString mHR;
    QString mBP;

    QMqttClient *client;

    void analysisMsg();

    QString getMHR();
    QString getMBP();
};

#endif // HEALTHMSG_H
