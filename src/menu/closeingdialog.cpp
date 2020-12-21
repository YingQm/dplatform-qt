#include "closeingdialog.h"
#include "ui_closeingdialog.h"
#include "mainui.h"
#include "basefuntion.h"
#include "manageui.h"

extern ManageUI*   g_lpManageUI;
extern MainUI*              g_lpMainUI;

CloseingThread::CloseingThread()
{

}

void CloseingThread::run()
{
    while (true)
    {
        if(!GetProcessidFromName()) {
            emit DplatformosClosed();
            break;
        } else {
            if(g_lpManageUI)
                g_lpManageUI->CloseDplatformosTemp();
        }
        sleep(10);
    }
}

CloseingDialog::CloseingDialog(QWidget *parent, CloseType type)
    : JsonConnectorDialog(parent)
    , ui(new Ui::CloseingDialog)
    , m_Type (type)
{
    ui->setupUi(this);
    this->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child());
    setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint); // 没有帮助按钮

    m_lpCloseingThread = new CloseingThread();
    connect(m_lpCloseingThread, SIGNAL(DplatformosClosed()), this, SLOT(DplatformosClosed()));

    switch (m_Type) {
    case CloseUI:
        this->setWindowTitle(tr("关闭"));
        m_lpCloseingThread->start();
        break;
    case RestartNewDplatformos:
        this->setWindowTitle(tr("操作"));
        ui->labelText->setText(tr("正在处理，请稍等..."));
        PostJsonMessage(ID_GetWalletStatus);
    default:
        break;
    }
}

CloseingDialog::~CloseingDialog()
{
    delete ui;
}

void CloseingDialog::requestFinished(const QVariant &result, const QString &/*error*/)
{
    QMap<QString, QVariant> resultMap = result.toMap();
    if (m_nID == ID_GetWalletStatus)
    {
        m_isWalletLock = resultMap["isWalletLock"].toBool();
        m_isTicketLock = resultMap["isTicketLock"].toBool();
        m_lpCloseingThread->start();
    }
}

void CloseingDialog::DplatformosClosed()
{
    qDebug() << ("关闭 退出 DplatformosClosed 界面");
    if(m_Type == RestartNewDplatformos) {
        if(g_lpManageUI) {
            g_lpManageUI->StartThread();
            g_lpManageUI->startDplatformos();
            g_lpManageUI->UnlockWallet(m_isWalletLock, m_isTicketLock);
        }

        if(g_lpMainUI) {
            g_lpMainUI->ResumeCommunicateDplatformosThread();
        }
    }

    QDialog::accept();
}
