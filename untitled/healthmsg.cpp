#include "healthmsg.h"

healthmsg::healthmsg(QString hostname, QString port, QString topic, QPushButton *button, QLabel *label_mbp, QLabel *label_mhr)
{
    client = new QMqttClient(this);
    this->hostname = hostname;
    this->port = port;
    this->topic = topic;
    this->sub_button = button;
    this->label_mbp = label_mbp;
    this->label_mhr = label_mhr;
}

healthmsg::~healthmsg(){
    delete client;
}


void healthmsg::connect(){
    this->client->setHostname(this->hostname);
    this->client->setPort(this->port.toInt());
    this->client->connectToHost();
    this->client->subscribe(topic);


    QAbstractSocket::connect(this->client, &QMqttClient::stateChanged, this, &healthmsg::updateLogStateChange);

    QAbstractSocket::connect(client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        this->msg = QDateTime::currentDateTime().toString()
                                + QLatin1String(" Received Topic: ")
                                + topic.name()
                                + QLatin1String(" Message: ")
                                + message
                                + QLatin1Char('\n');

        qDebug()<< this->msg;
        this->data = message;
        analysisMsg();
        label_mbp->setText(getMBP());
        label_mhr->setText(getMHR());
        qDebug()<< getMBP();

    });

    QAbstractSocket::connect(client, &QMqttClient::pingResponseReceived, this, [this]() {

        this->msg = QDateTime::currentDateTime().toString()
                                + QLatin1String(" PingResponse")
                                + QLatin1Char('\n');
        qDebug()<< this->msg;
    });

    QAbstractButton::connect(sub_button, &QPushButton::clicked, [this]{
        client->subscribe(this->topic);
    });

    updateLogStateChange();

}


void healthmsg::updateLogStateChange(){
    const QString content = QDateTime::currentDateTime().toString()
                            + QLatin1String(": State Change")
                            + QString::number(client->state())
                            + QLatin1Char('\n');
    qDebug()<< content;
}

void healthmsg::on_buttonSubscribe_clicked()
{
    auto subscription = client->subscribe(topic);
    if (!subscription) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }
}

void  healthmsg::analysisMsg(){
    jsonData = QJsonDocument::fromJson(data.toUtf8(), &jsonError);
    if (!jsonData.isNull()){
        // 检查根数据是否为对象类型
        if (jsonData.isObject()) {
            // 将 QJsonDocument 转换为 QJsonObject
            QJsonObject jsonObj = jsonData.object();
            // 从JSON对象中提取 mhr mbp数据
            mHR = jsonObj.value("mHR").toString();
            mBP = jsonObj.value("mBP").toString();
        }
    }
}


QString healthmsg::getMBP(){
    return this->mBP;
}

QString healthmsg::getMHR(){
    return this->mHR;
}
