#include "sendtocontractdialog.h"
#include "ui_sendtocontractdialog.h"
#include "basefuntion.h"

SendToContractDialog::SendToContractDialog(QWidget *parent, ContractCoinsType type) :
    QDialog(parent),
    ui(new Ui::SendToContractDialog),
    m_type(type)
{
    ui->setupUi(this);
    initUI();
}

SendToContractDialog::~SendToContractDialog()
{
    delete ui;
}

void SendToContractDialog::initUI()
{

}
