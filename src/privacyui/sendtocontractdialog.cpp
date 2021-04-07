#include "sendtocontractdialog.h"
#include "ui_sendtocontractdialog.h"
#include "basefuntion.h"
#include <QDoubleValidator>

QString g_strConvertExectoAddr;     // 隐私合约地址

SendToContractDialog::SendToContractDialog(QString strAddr, double amount, ContractCoinsType type, QWidget *parent) :
    JsonConnectorDialog(parent),
    ui(new Ui::SendToContractDialog),
    m_type(type),
    m_strAddr(strAddr),
    m_amount(0.0)
{
    ui->setupUi(this);
    if (amount >= CStyleConfig::GetInstance().GetMinFee()) {
        m_amount = amount;
        if (m_type == ContractRollIn) {
            m_amount -= CStyleConfig::GetInstance().GetMinFee();
        }
    }
    initUI();
    if(g_strConvertExectoAddr.isEmpty()) {
        PostMsgGetConvertAddr();
    }
}

SendToContractDialog::~SendToContractDialog()
{
    delete ui;
}

void SendToContractDialog::initUI()
{
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child());

    switch(m_type)
    {
    case ContractRollIn:
        this->setWindowTitle(tr("合约充值"));
        ui->fromLabel->setText(tr("充值地址"));
        ui->toLabel->setText(tr("充值金额"));
        ui->availableAmountLabel->setText(tr("可充值:"));
        break;
    case ContractRollOut:
        this->setWindowTitle(tr("合约转出"));
        ui->fromLabel->setText(tr("转出地址"));
        ui->toLabel->setText(tr("转出金额"));
        ui->availableAmountLabel->setText(tr("可转出:"));
        break;
    }

    QString strMaxUse = QString::number(m_amount, 'f', 4) + " " + CStyleConfig::GetInstance().GetUnitName();
    ui->amountLabel ->setText(strMaxUse);
    ui->toAmountEdit->setValidator(new QDoubleValidator(0.0, 5*le8, 4, this));
    ui->fromAddrLabel->setText(m_strAddr);
}

// 转账到隐私合约
void SendToContractDialog::PostMsgSendPrivacyConvert(const QString &fromAddr, const QString &toAddr, double amount)
{
    QJsonObject jsonParms;
    jsonParms.insert("from", fromAddr);
    jsonParms.insert("to", toAddr);
    qint64 lamount = (qint64)(amount*le8);
    jsonParms.insert("amount", lamount);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_SendToAddress, params);
}

// 获取隐私合约地址
void SendToContractDialog::PostMsgGetConvertAddr()
{
    QJsonObject jsonParms;
    jsonParms.insert("execname", "privacy");
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ConvertExectoAddr, params);
}

void SendToContractDialog::requestFinished(const QVariant &result, const QString &error)
{
    QMap<QString, QVariant> resultMap = result.toMap();
    if (!error.isEmpty()) {
        ui->errorLabel->setText(error);
        return;
    }
    if (ID_ConvertExectoAddr == m_nID) {
        g_strConvertExectoAddr = result.toString();
    } else if (ID_SendToAddress == m_nID) {
         QMessageBox::information(this, tr("提示"), tr("转账成功，等区块链确认后，手动点击刷新按钮!"));
         close();
    } else if (ID_DumpPrivkey == m_nID) {
        m_strAddrPrivkey = resultMap["data"].toString();
        QJsonObject jsonParms;
        jsonParms.insert("from", m_strAddr);
        qint64 lamount = (qint64)(ui->toAmountEdit->text().toDouble()*le8);
        jsonParms.insert("amount", lamount);
        jsonParms.insert("to", g_strConvertExectoAddr);
        jsonParms.insert("isWithdraw", true);
        jsonParms.insert("execName", "coins");
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_CreateRawTransaction, params);
    } else if (ID_CreateRawTransaction == m_nID) {
        // 签名
        QJsonObject jsonParms;
        jsonParms.insert("expire", "300s");
        jsonParms.insert("privkey", m_strAddrPrivkey);
        jsonParms.insert("txHex", result.toString());
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_SignRawTx, params);
    } else if (ID_SignRawTx == m_nID) {
        // 发送
        QJsonObject jsonParms;
        jsonParms.insert("data", result.toString());
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_SendTransaction, params);
        QMessageBox::information(this, tr("提示"), tr("转账成功，等区块链确认后，手动点击刷新按钮!"));
        close();
    }
}

void SendToContractDialog::on_okBtn_clicked()
{
    if(ui->toAmountEdit->text().isEmpty()) {
        ui->errorLabel->setText("请输入金额");
        return;
    } else {
        ui->errorLabel->setText("");
    }

    switch(m_type)
    {
    case ContractRollIn:
        PostMsgSendPrivacyConvert(m_strAddr, g_strConvertExectoAddr, ui->toAmountEdit->text().toDouble());
        break;
    case ContractRollOut:
        // 获取一下私钥, 签名的时候需要
        QJsonObject jsonParms;
        jsonParms.insert("data", m_strAddr);
        QJsonArray params;
        params.insert(0, jsonParms);
        PostJsonMessage(ID_DumpPrivkey, params);
        break;
    }
}

void SendToContractDialog::on_cancelBtn_clicked()
{
    close();
}

void SendToContractDialog::on_toolButton_clicked()
{
    ui->toAmountEdit->setText(QString::number(m_amount, 'f', 4));
}
