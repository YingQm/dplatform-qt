#include "sendtocontractdialog.h"
#include "ui_sendtocontractdialog.h"
#include "basefuntion.h"

SendToContractDialog::SendToContractDialog(QString strAddr, double amount, ContractCoinsType type, QWidget *parent) :
    JsonConnectorDialog(parent),
    ui(new Ui::SendToContractDialog),
    m_type(type),
    m_strConvertExectoAddr(""),
    m_strAddr(strAddr)
{
    ui->setupUi(this);
    if (amount >= CStyleConfig::GetInstance().GetMinFee()) {
        m_amount = amount - CStyleConfig::GetInstance().GetMinFee();
    }
    initUI();
    PostMsgGetConvertAddr();
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
    jsonParms.insert("amount", amount*le8);
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
    //QMap<QString, QVariant> resultMap = result.toMap();
    if (ID_ConvertExectoAddr == m_nID) {
        m_strConvertExectoAddr = result.toString();
    } else if (ID_SendToAddress == m_nID) {
        if (!error.isEmpty()) {
            ui->errorLabel->setText(error);
            return;
        } else {
            QMessageBox::information(this, tr("提示"), tr("转账成功，等区块链确认后，手动点击刷新按钮!"));
            close();
            // ui->errorLabel->setText("转账成功，等区块链确认后，等待列表中刷新!");
        }
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
        PostMsgSendPrivacyConvert(m_strAddr, m_strConvertExectoAddr, ui->toAmountEdit->text().toDouble());
        break;
    case ContractRollOut:
        PostMsgSendPrivacyConvert(m_strAddr, m_strConvertExectoAddr, (0.0 - ui->toAmountEdit->text().toDouble()));
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
