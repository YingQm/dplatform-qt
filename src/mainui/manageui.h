﻿#ifndef MANAGEUI_H
#define MANAGEUI_H

/*
 * 正在加载请稍等 界面
 * 管理运行 mainUI
 * 启动 dplatform
 */

#include <QMainWindow>
#include <QThread>
#include <QProcess>
#include "basejsonconnector.h"
#include <mutex>
#include <QMutex>

#ifdef WIN32
#include <windows.h>
#include <tlhelp32.h>
#include "clearthread.h"
#endif

// 判断启动 dplatform 是否成功
class RuningThread : public QThread
{
    Q_OBJECT
public:
    RuningThread();
    ~RuningThread(){}
    void SetFinish();
    void SetQuit(bool bQuit);

signals:
    void PostMsgGetWalletstatus();
    void PostMsgGetCoinSymbol();
    void PostMsgGetProperFee();

    void startDplatform();

protected:
    virtual void run();

private:
    bool        m_bFinish;
    bool        m_bQuit;
    QMutex      m_mutexFinish;
    QMutex      m_mutexQuit;
};



namespace Ui {
class ManageUI;
}

class ManageUI : public JsonConnectorMainWindow
{
    Q_OBJECT

public:
    explicit ManageUI(QWidget *parent = 0, const char* lpstylesheet = NULL);
    ~ManageUI();

    virtual void requestFinished(const QVariant &result, const QString &error);

    void StartThread();

    void QuitThread();

    void CloseQueueDplatform();

    void ShowHide();

    void UnlockWallet(bool isWalletLock, bool isTicketLock);

public slots:
    void PostMsgGetWalletstatus();

    void PostMsgGetCoinSymbol();

    void PostMsgGetProperFee();

    void startDplatform();

    void CloseDplatformTemp();

    void CloseDplatform();

private:
    Ui::ManageUI *ui;
    QString         m_stylesheet;
    RuningThread*   m_lpThread;
    QProcess*       m_lpQProcess;
    bool            m_bFirst;
    int             m_nCloseDplatform;
#ifdef WIN32
    DWORD           m_dwDplatformProcessId;
#else
    qint64          m_dwDplatformProcessId;
#endif
#ifdef WIN32
    ClearThread*    m_clearThread;
#endif
};

#endif // MANAGEUI_H