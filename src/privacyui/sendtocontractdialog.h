#ifndef SENDTOCONTRACTDIALOG_H
#define SENDTOCONTRACTDIALOG_H

/*
 * 隐私合约转入转出
 */

#include "enumtype.h"
#include "basejsonconnector.h"

namespace Ui {
class SendToContractDialog;
}

class SendToContractDialog : public JsonConnectorDialog
{
    Q_OBJECT

public:
    explicit SendToContractDialog(QString strAddr, double amount, ContractCoinsType type, QWidget *parent = 0);
    ~SendToContractDialog();

private:
    void initUI();

    void PostMsgSendPrivacyConvert(const QString &fromAddr, const QString &toAddr, double amount);

    void PostMsgGetConvertAddr();

protected:
    virtual void requestFinished(const QVariant &result, const QString &error);

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

    void on_toolButton_clicked();

private:
    Ui::SendToContractDialog *ui;
    ContractCoinsType m_type;
    QString m_strAddr;          // 操作的地址
    QString m_strAddrPrivkey;   // 操作的地址私钥
    double m_amount;
};

#endif // SENDTOCONTRACTDIALOG_H
