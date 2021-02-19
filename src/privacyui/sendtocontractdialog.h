#ifndef SENDTOCONTRACTDIALOG_H
#define SENDTOCONTRACTDIALOG_H

#include <QDialog>
#include "enumtype.h"

namespace Ui {
class SendToContractDialog;
}

class SendToContractDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendToContractDialog(QWidget *parent = 0, ContractCoinsType type = ContractRollIn);
    ~SendToContractDialog();

private:
    void initUI();

private:
    Ui::SendToContractDialog *ui;
    ContractCoinsType m_type;
};

#endif // SENDTOCONTRACTDIALOG_H
