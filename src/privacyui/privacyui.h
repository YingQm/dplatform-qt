#ifndef PRIVACYUI_H
#define PRIVACYUI_H

#include <QWidget>
#include "basejsonconnector.h"

class PlatformStyle;

namespace Ui {
class PrivacyUi;
}

class PrivacyUi : public JsonConnectorWidget
{
    Q_OBJECT

public:
    explicit PrivacyUi(QWidget *parent, const PlatformStyle *platformStyle);
    ~PrivacyUi();

private:
    void initUI();

protected:
    virtual void requestFinished(const QVariant &result, const QString &error);

private:
    void PostMsgOpenPrivacy(const QString &addr);
    void PostMsgShowPrivacyKey(const QString &addr);
    void PostMsgGetContractBalance(const QString &addr);
    void PostMsgGetPrivacyBalance(const QString &addr);


private slots:
    // wallet
    void on_selectBtn_clicked();
    void on_rollOutBtn_clicked();
    void on_copyBtn_clicked();
    void on_rollInBtn_clicked();

    // send
    void on_addressFromButton_clicked();
    void on_addressBookButton_clicked();
    void on_clearButton_clicked();
    void on_sendButton_clicked();

    // txs
    void on_typeWidget_activated(int index);
    void on_firstPageBtn_clicked();
    void on_prevPageBtn_clicked();
    void on_nextPageBtn_clicked();

private:
    Ui::PrivacyUi *ui;
    const PlatformStyle *m_platformStyle;

    // 当前选中的地址
    QString m_strSelectedAddr;
};

#endif // PRIVACYUI_H
