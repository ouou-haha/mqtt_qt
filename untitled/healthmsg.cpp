#include "healthmsg.h"



/***************************************************************
  *  函数名称：healthmsg::healthmsg(构造函数)
  *  函数作用：初始化
  *  输入参数：ip port（端口） topic（主题）  button（订阅主题按钮）  label_mbp label_mhr (显示两个数据的位置)
  *  返 回 值：无
  *  函数的使用方法：在主函数中创建类名并直接调用函数
  *  日    期： 2023.10.18
 **************************************************************/
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


// 析构函数
healthmsg::~healthmsg(){
    delete client;
}

/***************************************************************
  *  函数名称： healthmsg::connect()
  *  函数作用：连接到mqtt代理服务器
  *  输入参数：无
  *  返 回 值：无
  *  函数的使用方法：在主函数中创建类名并直接调用函数
  *  日    期： 2023.10.18
 **************************************************************/
void healthmsg::connect(){

    // 对client进行配置
    this->client->setHostname(this->hostname);
    this->client->setPort(this->port.toInt());
    this->client->connectToHost();
    this->client->subscribe(topic);

    // 判断当前连接的状态（槽函数）连接不上打印1 连接成功打印2
    QAbstractSocket::connect(this->client, &QMqttClient::stateChanged, this, &healthmsg::updateLogStateChange);

    // 监听消息接收显示（槽函数）
    QAbstractSocket::connect(client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        this->msg = QDateTime::currentDateTime().toString()
                                + QLatin1String(" Received Topic: ")
                                + topic.name()
                                + QLatin1String(" Message: ")
                                + message
                                + QLatin1Char('\n');
        // 打印消息
        qDebug()<< this->msg;
        this->data = message;

        // 解析数据
        analysisMsg();
        label_mbp->setText(getMBP());
        label_mhr->setText(getMHR());
        // 获取数据
        qDebug()<< getMBP();

    });

    QAbstractSocket::connect(client, &QMqttClient::pingResponseReceived, this, [this]() {

        this->msg = QDateTime::currentDateTime().toString()
                                + QLatin1String(" PingResponse")
                                + QLatin1Char('\n');
        qDebug()<< this->msg;
    });

    // 订阅主题的按键
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



/***************************************************************
  *  函数名称： healthmsg::analysisMsg()
  *  函数作用： 数据解析函数 将string转成json再获取对应键值
  *  输入参数： 无
  *  返 回 值：无
  *  函数的使用方法：内部调用
  *  日    期： 2023.10.18
 **************************************************************/
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
