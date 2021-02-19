#include "privacyui.h"
#include "ui_privacyui.h"
#include "basefuntion.h"
#include "platformstyle.h"
#include "addresslistui.h"
#include "addressui.h"
#include "base58.h"
#include "sendtocontractdialog.h"
#include "mainui.h"

extern MainUI*              g_lpMainUI;

PrivacyUi::PrivacyUi(QWidget *parent, const PlatformStyle *platformStyle) :
    JsonConnectorWidget(parent),
    m_platformStyle(platformStyle),
    ui(new Ui::PrivacyUi),
    m_strSelectedAddr(""),
    m_strConvertExectoAddr("")
{
    ui->setupUi(this);
    initUI();
    PostMsgGetConvertAddr();
}

PrivacyUi::~PrivacyUi()
{
    delete ui;
}

void PrivacyUi::initUI()
{
    // wallet
    ui->sendWidget->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child2());
    if (CStyleConfig::GetInstance().GetStyleType() == QSS_BLUE) {
        this->setStyleSheet("QWidget {background-color:#FFFFFF;border:none;}" + CStyleConfig::GetInstance().GetStylesheet());
        ui->sendwidget1_3->setStyleSheet("QWidget {background-color:#F5F4F9;border-radius: 4px;}");
    }
    else {
        ui->sendwidget1_3->setStyleSheet("QWidget {background-color:#2c2c2c;border-radius: 4px;}");
    }
    ui->addrEdit->setEnabled(false);

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
    ui->typeWidget->addItem("发送方", 0);
    ui->typeWidget->addItem("接收方", 1);
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
    } else if (ID_ConvertExectoAddr == m_nID) {
        m_strConvertExectoAddr = result.toString();
    }
}

// 开启隐私功能
void PrivacyUi::PostMsgOpenPrivacy(const QString &addr)
{
    QJsonObject jsonParms;
    jsonParms.insert("addrs", addr);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_EnablePrivacy, params);
}

// 获取隐私地址
void PrivacyUi::PostMsgShowPrivacyKey(const QString &addr)
{
    QJsonObject jsonParms;
    jsonParms.insert("data", addr);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ShowPrivacyKey, params);
}

// 获取合约中的地址余额
void PrivacyUi::PostMsgGetContractBalance(const QString &addr)
{
    QString jsonCmd = "{\"addresses\":[\"";
    jsonCmd.append(addr);
    jsonCmd.append("\"], \"execer\":\"privacy\"}");
    PostJsonMessage(ID_GetBalance, jsonCmd.toStdString().c_str());
}

// 查询utxo余额信息
void PrivacyUi::PostMsgGetPrivacyBalance(const QString &addr)
{
    QJsonObject jsonParms;
    jsonParms.insert("addr", addr);
    jsonParms.insert("token", CStyleConfig::GetInstance().GetUnitName());
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ShowPrivacyAccountInfo, params);
}

// 查询钱包中指定地址的隐私交易
void PrivacyUi::PostMsgPrivacyListTxs(const QString &addr, int direction, const QString &seedtxhash)
{
    QJsonObject jsonParms;
    jsonParms.insert("address", addr);
    jsonParms.insert("sendRecvFlag", ui->typeWidget->currentData().toInt());     // send or recv flag (0: send, 1: recv)
    jsonParms.insert("direction", direction);       // query direction (0: pre page, 1: next page), valid with seedtxhash param
    jsonParms.insert("count", 15);                  // number of transactions
    jsonParms.insert("seedtxhash", seedtxhash);
    jsonParms.insert("assetExec", "coins");
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_GetPrivacyTxByAddr, params);
}

// 构建交易
void PrivacyUi::PostMsgCreatePrivacyTx(const QString &fromAddr, const QString &toAddr, double amount, const QString &note)
{
    // type:1 公对私转账, 2 私对私转账, 3 私到公转账
    int type = 0;
    if(!IsAddrValid(fromAddr.toStdString().c_str()) && !IsAddrValid(toAddr.toStdString().c_str())) {
        type = 2;
    } else if (!IsAddrValid(fromAddr.toStdString().c_str())) {
        type = 3;
    } else if (!IsAddrValid(toAddr.toStdString().c_str())) {
        type = 1;
    } else {
        QMessageBox::information(this, tr("提示"), tr("地址输入有误, 必须有隐私地址!"));
        return;
    }

    QJsonObject jsonParms;
    jsonParms.insert("type", type);
    jsonParms.insert("from", fromAddr);
    jsonParms.insert("amount", amount);
    if (type == 3) {
        jsonParms.insert("to", toAddr);
    } else {
        jsonParms.insert("pubkeypair", toAddr);
    }
    jsonParms.insert("note", note);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_PrivacyCreateRawTransaction, params);
}

void PrivacyUi::PostMsgSignPrivacyTx(const QString &privkey, const QString &txHex)
{
    // 签名
    QJsonObject jsonParms;
    jsonParms.insert("expire", "300s");
    jsonParms.insert("privkey", privkey);
    jsonParms.insert("txHex", txHex);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_SignRawTx, params);
}

void PrivacyUi::PostMsgSendPrivacyTx(const QString &signHex)
{
    // 发送
    QJsonObject jsonParms;
    jsonParms.insert("data", signHex);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_SendTransaction, params);
}

// 获取隐私合约地址
void PrivacyUi::PostMsgGetConvertAddr()
{
    QJsonObject jsonParms;
    jsonParms.insert("execname", "privacy");
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ConvertExectoAddr, params);
}

// 转账到隐私合约
void PrivacyUi::PostMsgSendPrivacyConvert(const QString &fromAddr, const QString &toAddr, double amount)
{
    QJsonObject jsonParms;
    jsonParms.insert("from", fromAddr);
    jsonParms.insert("to", toAddr);
    jsonParms.insert("amount", amount);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_SendToAddress, params);
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

        on_firstPageBtn_clicked();
    }
}

void PrivacyUi::on_rollOutBtn_clicked()
{
    SendToContractDialog dlg(this);
    dlg.exec();
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
    ui->payTo->clear();
    ui->addAsLabel->clear();
    ui->payAmount->clear();
    ui->payTo->setFocus();
}

void PrivacyUi::on_sendButton_clicked()
{

}

void PrivacyUi::on_typeWidget_currentIndexChanged(int index)
{
    if (!m_strSelectedAddr.isEmpty()) {
        on_firstPageBtn_clicked();
    }
}

void PrivacyUi::on_firstPageBtn_clicked()
{    
    ui->prevPageBtn->setEnabled(false);
    PostMsgPrivacyListTxs(m_strSelectedAddr, 0 ,"");
}

void PrivacyUi::on_prevPageBtn_clicked()
{

}

void PrivacyUi::on_nextPageBtn_clicked()
{
    ui->prevPageBtn->setEnabled(true);

}
