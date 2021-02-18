#include "privacyui.h"
#include "ui_privacyui.h"
#include "basefuntion.h"
#include "platformstyle.h"
#include "addresslistui.h"
#include "addressui.h"
#include "mainui.h"

extern MainUI*              g_lpMainUI;

PrivacyUi::PrivacyUi(QWidget *parent, const PlatformStyle *platformStyle) :
    JsonConnectorWidget(parent),
    m_platformStyle(platformStyle),
    ui(new Ui::PrivacyUi),
    m_strSelectedAddr("")
{
    ui->setupUi(this);
    initUI();

    return;



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
    // wallet
    ui->sendWidget->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child2());
    //ui->verticalWidget0->setAttribute(Qt::WA_StyledBackground);
    //ui->verticalWidget0->setStyleSheet("QWidget{background-color:#6A33AA}");
    //ui->verticalWidget0->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child());
    //ui->verticalWidget1->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child());
   // ui->verticalWidget0->setStyleSheet("border-radius:4px;");
   //ui->verticalWidget1->setStyleSheet("border-radius:4px;");
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
    ui->privacyTxsWidget->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child2());
    ui->searchWidget->setVisible(false);
    ui->prevPageBtn->setEnabled(false);
}

void PrivacyUi::requestFinished(const QVariant &result, const QString &error)
{
    QMap<QString, QVariant> resultMap = result.toMap();
    if (ID_EnablePrivacy == m_nID) {
        QList<QVariant> resultsList = resultMap["results"].toList();
        if (resultsList.length() > 0) {
            QMap<QString, QVariant> resultsMap = resultsList[0].toMap();
            if (resultMap["IsOK"].toBool() == true) {
                // 获取隐私地址
                PostMsgShowPrivacyKey(resultsMap["addr"].toString());
                return;
            }
        }

        QMessageBox::information(this, tr("提示"), tr("获取隐私地址失败, %1").arg(error));
    } else if (ID_ShowPrivacyKey == m_nID) {
        if (!resultMap["pubkeypair"].toString().isEmpty()) {
            ui->PrivacyAddrLabel->setText(resultMap["pubkeypair"].toString());
        } else {
            QMessageBox::information(this, tr("提示"), tr("获取隐私地址失败, %1").arg(error));
        }
    } else if (ID_GetBalance == m_nID) {
        ui->labelContract->setText(QString::number(GetbalanceD(resultMap["balance"].toDouble()), 'f', 4));
    } else if (ID_ShowPrivacyAccountInfo == m_nID) {
        double totalAmount = 0.0;
        QList<QVariant> utxosList = resultMap["utxos"].toList();
        for(int i=0; i<utxosList.length(); ++i){
            QMap<QString, QVariant> utxosMap = utxosList[i].toMap();
            totalAmount += utxosMap["amount"].toDouble();
        }
        ui->labelPrivacy->setText(QString::number(GetbalanceD(totalAmount), 'f', 4));
    }
}

void PrivacyUi::PostMsgOpenPrivacy(const QString &addr)
{
    // 开启隐私功能
    QJsonObject jsonParms;
    jsonParms.insert("addrs", addr);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_EnablePrivacy, params);
}

void PrivacyUi::PostMsgShowPrivacyKey(const QString &addr)
{
    // 获取隐私地址
    QJsonObject jsonParms;
    jsonParms.insert("data", addr);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ShowPrivacyKey, params);
}

void PrivacyUi::PostMsgGetContractBalance(const QString &addr)
{
    // 获取合约中的地址余额
    QString jsonCmd = "{\"addresses\":[\"";
    jsonCmd.append(addr);
    jsonCmd.append("\"], \"execer\":\"privacy\"}");
    PostJsonMessage(ID_GetBalance, jsonCmd.toStdString().c_str());
}

void PrivacyUi::PostMsgGetPrivacyBalance(const QString &addr)
{
    // 查询utxo余额信息
    QJsonObject jsonParms;
    jsonParms.insert("addr", addr);
    jsonParms.insert("token", CStyleConfig::GetInstance().GetUnitName());
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ShowPrivacyAccountInfo, params);
}



void PrivacyUi::on_selectBtn_clicked()
{
    AddressListUI dlg(ForSending, TabsReceiving, this, m_platformStyle);
    if (dlg.exec()) {
        m_strSelectedAddr = dlg.getReturnAddr();
        ui->addrEdit->setText(m_strSelectedAddr);
        ui->labelBalance->setText(QString::number(dlg.getBalance(), 'f', 4));
        PostMsgOpenPrivacy(m_strSelectedAddr);
        PostMsgGetContractBalance(m_strSelectedAddr);
        PostMsgGetPrivacyBalance(m_strSelectedAddr);
    }
}

void PrivacyUi::on_rollOutBtn_clicked()
{

}

void PrivacyUi::on_copyBtn_clicked()
{
    GUIUtil::setClipboard(ui->PrivacyAddrLabel->text());
}

void PrivacyUi::on_rollInBtn_clicked()
{

}

void PrivacyUi::on_addressFromButton_clicked()
{
    AddressListUI dlg(ForSending, TabsReceiving, this, m_platformStyle);
    if (dlg.exec()) {
        ui->fromAddr->setText(dlg.getReturnAddr());
        ui->payAmount->setAmountRange(dlg.getBalance());
    }
}

void PrivacyUi::on_addressBookButton_clicked()
{
    AddressListUI dlg(ForSending, TabsSending, this, m_platformStyle);
    if (dlg.exec()) {
        ui->payTo->setText(dlg.getReturnAddr());
        ui->payAmount->setFocus();
    }
    g_lpMainUI->m_lpAddressUI->m_lpFriendsAddressList->UpdateAddrList();
}

void PrivacyUi::on_clearButton_clicked()
{

}

void PrivacyUi::on_sendButton_clicked()
{

}

void PrivacyUi::on_typeWidget_activated(int index)
{

}

void PrivacyUi::on_firstPageBtn_clicked()
{

}

void PrivacyUi::on_prevPageBtn_clicked()
{

}

void PrivacyUi::on_nextPageBtn_clicked()
{

}
