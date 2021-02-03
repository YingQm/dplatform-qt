#include "privacyui.h"
#include "ui_privacyui.h"
#include "basefuntion.h"
#include "platformstyle.h"

PrivacyUi::PrivacyUi(QWidget *parent, const PlatformStyle *platformStyle) :
    JsonConnectorWidget(parent),
    m_platformStyle(platformStyle),
    ui(new Ui::PrivacyUi)
{
    ui->setupUi(this);
    initUI();

    return;

    // 开启隐私功能
    {
        QJsonObject jsonParms;
        jsonParms.insert("addrs", "addr1,addr2");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_EnablePrivacy, params);
    }

    // 获取隐私地址
    {
        QJsonObject jsonParms;
        jsonParms.insert("data", "addr1");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_ShowPrivacyKey, params);
    }

    // 获取privacy合约地址
    {
        QJsonObject jsonParms;
        jsonParms.insert("execname", "privacy");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_ConvertExectoAddr, params);
    }

    // 转账到隐私合约
    {
        QJsonObject jsonParms;
        jsonParms.insert("from", "strAddr");
        jsonParms.insert("to", "1BXvgjmBw1aBgmGn1hjfGyRkmN3krWpFP4");
        jsonParms.insert("amount", 0);
        jsonParms.insert("note", "test");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_SendToAddress, params);
    }

    // 构建交易 type:1 公对私转账, 2 私对私转账, 3 私到公转账
    {
        QJsonObject jsonParms;
        jsonParms.insert("type", "1");
        jsonParms.insert("from", "1BXvgjmBw1aBgmGn1hjfGyRkmN3krWpFP4");
        jsonParms.insert("amount", 0);
        jsonParms.insert("pubkeypair", "test");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_SendToAddress, params);
    }

    // 构建交易 3 私到公转账
    {
        QJsonObject jsonParms;
        jsonParms.insert("type", "1");
        jsonParms.insert("from", "1BXvgjmBw1aBgmGn1hjfGyRkmN3krWpFP4");
        jsonParms.insert("to", "1BXvgjmBw1aBgmGn1hjfGyRkmN3krWpFP4");
        jsonParms.insert("amount", 0);
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_SendToAddress, params);
    }

    // 查询utxo余额信息
    {
        QJsonObject jsonParms;
        jsonParms.insert("addr", "1BXvgjmBw1aBgmGn1hjfGyRkmN3krWpFP4");
        jsonParms.insert("token", "bty");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_ShowPrivacyAccountInfo, params);
    }

    // 查询钱包中指定地址的隐私交易
    {
        QJsonObject jsonParms;
        jsonParms.insert("address", "1BXvgjmBw1aBgmGn1hjfGyRkmN3krWpFP4");
        jsonParms.insert("sendRecvFlag", 0);    // send or recv flag (0: send, 1: recv)
        jsonParms.insert("direction", 0);       // query direction (0: pre page, 1: next page), valid with seedtxhash param
        jsonParms.insert("count", 10);          // number of transactions
        jsonParms.insert("seedtxhash", "xxxxx");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_GetPrivacyTxByAddr, params);
    }

}

PrivacyUi::~PrivacyUi()
{
    delete ui;
}

void PrivacyUi::initUI()
{
   // ui->sendWidget->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_main());

    // wallet
    ui->verticalWidget0->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child());
    ui->verticalWidget1->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child());
    ui->verticalWidget0->setStyleSheet("border-radius:4px;");
    ui->verticalWidget1->setStyleSheet("border-radius:4px;");
    if (CStyleConfig::GetInstance().GetStyleType() == QSS_BLUE) {
        this->setStyleSheet("QWidget {background-color:#FFFFFF;border:none;}" + CStyleConfig::GetInstance().GetStylesheet());
        ui->sendwidget1_3->setStyleSheet("QWidget {background-color:#F5F4F9;border-radius: 4px;}");
    }
    else {
        ui->sendwidget1_3->setStyleSheet("QWidget {background-color:#2c2c2c;border-radius: 4px;}");
    }

  //  ui->copyBtn->setFocusPolicy(Qt::NoFocus);

#ifndef MAC_OSX
    ui->labelBalance->setStyleSheet("QLabel { font: " + QString::number(GetBaseFontSize() + 2) + "pt;}");
    ui->labelContract->setStyleSheet("QLabel { font: " + QString::number(GetBaseFontSize() + 2) + "pt;}");
    ui->labelPrivacy->setStyleSheet("QLabel { font: " + QString::number(GetBaseFontSize() + 4) + "pt;}");
    ui->label_Balance_text->setStyleSheet("QLabel { color: #969BA6; font: " + QString::number(GetBaseFontSize()) + "pt;}");
    ui->label_Contract_text->setStyleSheet("QLabel { color: #969BA6; font: " + QString::number(GetBaseFontSize()) + "pt;}");
    ui->label_Privacy_text->setStyleSheet("QLabel { color: #969BA6; font: " + QString::number(GetBaseFontSize()) + "pt;}");
#endif

    // send
    ui->addressFromButton->setIcon(m_platformStyle->SingleColorIcon(":/address_book"));
    ui->addressBookButton->setIcon(m_platformStyle->SingleColorIcon(":/address_book"));
    ui->selectBtn->setIcon(m_platformStyle->SingleColorIcon(":/address_book"));


    // tx list



}

void PrivacyUi::requestFinished(const QVariant &result, const QString &error)
{
   /* QMap<QString, QVariant> resultMap = result.toMap();

    if (ID_ShowPrivacyAccountInfo == m_nID) {
        double totalAmount = 0.0;
        QList utxosList = resultMap["utxos"];
        for(int i=0; i<utxosList.length(); ++i){
            QMap<QString, QVariant> utxosMap = utxosList[i].toMap;
            totalAmount += utxosMap["amount"];
        }
    }
*/
}


