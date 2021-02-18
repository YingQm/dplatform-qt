#ifndef BASEJSONCONNECTOR_H
#define BASEJSONCONNECTOR_H

/* json Post
 * 通过参数向底层获取数据类
 */

#include <QDialog>
#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#if QT_VERSION >= 0x050000
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#else
#include <QtScript>
#endif
#include <QMap>
#include <sstream>

enum IdType{
    ID_Lock  = 0,
    ID_UnLock,
    ID_SetPasswd,
    ID_SetLabl,
    ID_NewAccount,
    ID_GetAccounts,
    ID_GetBalance,
    ID_MergeBalance,
    ID_SetTxFee,
    ID_SendToAddress,
    ID_ImportPrivKey,
    ID_WalletTxList,
    ID_GetMemPool,
    ID_SendTransaction,
    ID_QueryTransaction,
    ID_GetTxByAddr,
    ID_GetTxByHashes,
    ID_GetBlocks,
    ID_GetLastHeader,
    ID_GetPeerInfo,
    ID_GetNetInfo,
    ID_GenSeed,
    ID_SaveSeed,
    ID_GetSeed,    
    ID_GetWalletStatus,
    ID_SetAutoMining,
    ID_GetTicketCount,
    ID_GetVersion,
    ID_IsSync,
    ID_IsNtpClockSync,
    ID_Query,
    ID_Query_GetTokens,
    ID_Query_GetTokenSellOrderByStatus,
    ID_CreateRawTradeSellTx,
    ID_CreateRawTradeBuyTx,
    ID_CreateRawTradeRevokeTx,
    ID_SignRawTx,
    ID_SignRawTx_CreateBindMiner,
    ID_SignRawTx_CreateRawTransaction,
    ID_GetAccountTokenAssets_token,
    ID_GetAccountTokenAssets_trade,
    ID_GetFatalFailure,
    ID_CreateBindMiner,
    ID_CreateRawTransaction,
    ID_GetTimeStatus,
    ID_CloseQueue,
    ID_SendToAddress_Token,
    ID_CreateRawTxGroup,
    ID_GetCoinSymbol,
    ID_GetProperFee,
    ID_ConvertExectoAddr,
    ID_EnablePrivacy,
    ID_ShowPrivacyKey,
    ID_PrivacyCreateRawTransaction,
    ID_ShowPrivacyAccountInfo,
    ID_ShowPrivacyAccountSpend,
    ID_GetPrivacyTxByAddr
};

class BaseJsonConnector
{
public:
    BaseJsonConnector();
    ~BaseJsonConnector();

protected:
    void PostJsonMessage(IdType id, const QJsonArray &params);
    void PostJson(const QJsonObject &PostJson);
    void PostJsonMessage(IdType id);
    void PostJsonMessage(IdType id, const char* strParams);
    void finishedNetwork(QNetworkReply *reply);
    virtual void requestFinished(const QVariant &result, const QString &error) = 0;    
    virtual void PostJsonData(const QByteArray &postData) = 0;
    bool bPrintf();
    QNetworkReply* GetNetworkReplyPost(const QByteArray &postData);

protected:
    int                     m_nID;
    QByteArray              m_postData;
};

class JsonConnectorDialog
        : public QDialog
        , public BaseJsonConnector
{
    Q_OBJECT
public:
    explicit JsonConnectorDialog(QWidget *parent = NULL);

protected:
    virtual void requestFinished(const QVariant &result, const QString &error) = 0;
    virtual void PostJsonData(const QByteArray &postData);

public slots:
    void finishedSlot();
};

class JsonConnectorMainWindow
        : public QMainWindow
        , public BaseJsonConnector
{
    Q_OBJECT
public:
    explicit JsonConnectorMainWindow(QWidget *parent = NULL);

protected:
    virtual void requestFinished(const QVariant &result, const QString &error) = 0;
    virtual void PostJsonData(const QByteArray &postData);

public slots:
    void finishedSlot();
};

class JsonConnectorWidget
        : public QWidget
        , public BaseJsonConnector
{
    Q_OBJECT
public:
    explicit JsonConnectorWidget(QWidget *parent = NULL);

protected:
    virtual void requestFinished(const QVariant &result, const QString &error) = 0;
    virtual void PostJsonData(const QByteArray &postData);

public slots:
    void finishedSlot();
};

#endif // BASEJSONCONNECTOR_H
