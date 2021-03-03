#ifndef PRIVACYUI_H
#define PRIVACYUI_H

#include <QWidget>
#include "basejsonconnector.h"

class PlatformStyle;
class TransactionsListModel;

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
    void initUnitNameUI();

protected:
    virtual void requestFinished(const QVariant &result, const QString &error);

private:
    void PostMsgOpenPrivacy(const QString &addr);
    void PostMsgShowPrivacyKey(const QString &addr);
    void PostMsgGetCoinsBalance(const QString &addr);
    void PostMsgGetContractBalance(const QString &addr);
    void PostMsgGetPrivacyBalance(const QString &addr, IdType id);
    void PostMsgPrivacyListTxs();
    void PostMsgCreatePrivacyTx(const QString &fromAddr, const QString &toAddr, double amount, const QString &note, int type);
    void PostMsgSignPrivacyTx(const QString &privkey, const QString &txHex);
    void PostMsgSendPrivacyTx(const QString &signHex);

    void refresh();
private slots:
    // wallet
    void on_refreshBtn_clicked();
    void on_selectBtn_clicked();
    void on_copyBtn_clicked();
    void on_rollInBtn_clicked();
    void on_rollOutBtn_clicked();

    // send
    void on_addressFromButton_clicked();
    void on_addressBookButton_clicked();
    void on_clearButton_clicked();
    void on_Public2PrivacyBtn_clicked();
    void on_Privacy2PrivacyBtn_clicked();
    void on_Privacy2PublicBtn_clicked();

    // txs
    void on_typeWidget_currentIndexChanged(int index);
    void on_firstPageBtn_clicked();
    void on_prevPageBtn_clicked();
    void on_nextPageBtn_clicked();
    void on_listTransactions_doubleClicked(const QModelIndex &index);

private:
    Ui::PrivacyUi *ui;
    const PlatformStyle *m_platformStyle;
    TransactionsListModel* model;

    QString m_strSelectedAddr;          // 当前选中的地址
    double m_coinsBalance;
    double m_contractBalance;
    double m_privacyBalance;

    QString m_strTxHex;   // 构造的交易信息

    QString m_strFromTxPrev;
    QString m_strFromTxNext;
    QString m_strFromTx;
    int m_nPage;        // 当前页数 0表示首页 下一页++ 上一页--
    int m_ndirection;
};

#endif // PRIVACYUI_H
