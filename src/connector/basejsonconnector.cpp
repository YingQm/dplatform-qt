#include "basejsonconnector.h"
#include "manageui.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include "mainui.h"
#include "statusbarui.h"
#include "basefuntion.h"
#include "cstyleconfig.h"

QMap<IdType, QString>   g_methodMap;
extern MainUI*              g_lpMainUI;

void InitMethodMap()
{
    g_methodMap.insert(ID_Lock, "DplatformOS.Lock");
    g_methodMap.insert(ID_UnLock, "DplatformOS.UnLock");
    g_methodMap.insert(ID_SetPasswd, "DplatformOS.SetPasswd");
    g_methodMap.insert(ID_SetLabl, "DplatformOS.SetLabl");
    g_methodMap.insert(ID_NewAccount, "DplatformOS.NewAccount");
    g_methodMap.insert(ID_GetAccounts, "DplatformOS.GetAccounts");
    g_methodMap.insert(ID_GetBalance, "DplatformOS.GetBalance");
    g_methodMap.insert(ID_GetBalance_coins, "DplatformOS.GetBalance");
    g_methodMap.insert(ID_GetBalance_privacy, "DplatformOS.GetBalance");
    g_methodMap.insert(ID_MergeBalance, "DplatformOS.MergeBalance");
    g_methodMap.insert(ID_SetTxFee, "DplatformOS.SetTxFee");
    g_methodMap.insert(ID_SendToAddress, "DplatformOS.SendToAddress");
    g_methodMap.insert(ID_ImportPrivKey, "DplatformOS.ImportPrivkey");
    g_methodMap.insert(ID_WalletTxList, "DplatformOS.WalletTxList");
    g_methodMap.insert(ID_GetMemPool, "DplatformOS.GetMempool");
    g_methodMap.insert(ID_SendTransaction, "DplatformOS.SendTransaction");
    g_methodMap.insert(ID_QueryTransaction, "DplatformOS.QueryTransaction");
    g_methodMap.insert(ID_GetTxByAddr, "DplatformOS.GetTxByAddr");
    g_methodMap.insert(ID_GetTxByHashes, "DplatformOS.GetTxByHashes");
    g_methodMap.insert(ID_GetBlocks, "DplatformOS.GetBlocks");
    g_methodMap.insert(ID_GetLastHeader, "DplatformOS.GetLastHeader");
    g_methodMap.insert(ID_GetPeerInfo, "DplatformOS.GetPeerInfo");
    g_methodMap.insert(ID_GetNetInfo, "DplatformOS.GetNetInfo");
    g_methodMap.insert(ID_GenSeed, "DplatformOS.GenSeed");
    g_methodMap.insert(ID_SaveSeed, "DplatformOS.SaveSeed");
    g_methodMap.insert(ID_GetSeed, "DplatformOS.GetSeed");
    g_methodMap.insert(ID_GetWalletStatus, "DplatformOS.GetWalletStatus");
    g_methodMap.insert(ID_SetAutoMining, "ticket.SetAutoMining");
    g_methodMap.insert(ID_GetTicketCount, "ticket.GetTicketCount");
    g_methodMap.insert(ID_GetVersion, "DplatformOS.Version");
    g_methodMap.insert(ID_IsSync, "DplatformOS.IsSync");
    g_methodMap.insert(ID_IsNtpClockSync, "DplatformOS.IsNtpClockSync");
    g_methodMap.insert(ID_Query, "DplatformOS.Query");
    g_methodMap.insert(ID_Query_GetTokens, "DplatformOS.Query");
    g_methodMap.insert(ID_Query_GetTokenSellOrderByStatus, "DplatformOS.Query");
    g_methodMap.insert(ID_CreateRawTradeSellTx, "trade.CreateRawTradeSellTx");
    g_methodMap.insert(ID_CreateRawTradeBuyTx, "trade.CreateRawTradeBuyTx");
    g_methodMap.insert(ID_CreateRawTradeRevokeTx, "trade.CreateRawTradeRevokeTx");
    g_methodMap.insert(ID_SignRawTx, "DplatformOS.SignRawTx");
    g_methodMap.insert(ID_SignRawTx_CreateBindMiner, "DplatformOS.SignRawTx");
    g_methodMap.insert(ID_SignRawTx_CreateRawTransaction, "DplatformOS.SignRawTx");
    g_methodMap.insert(ID_GetAccountTokenAssets_token, "DplatformOS.Query");
    g_methodMap.insert(ID_GetAccountTokenAssets_trade, "DplatformOS.Query");
    g_methodMap.insert(ID_GetFatalFailure, "DplatformOS.GetFatalFailure");
    g_methodMap.insert(ID_CreateBindMiner, "ticket.CreateBindMiner");
    g_methodMap.insert(ID_CreateRawTransaction, "DplatformOS.CreateRawTransaction");
    g_methodMap.insert(ID_GetTimeStatus, "DplatformOS.GetTimeStatus");
    g_methodMap.insert(ID_CloseQueue, "DplatformOS.CloseQueue");
    g_methodMap.insert(ID_SendToAddress_Token, "DplatformOS.SendToAddress");
    g_methodMap.insert(ID_CreateRawTxGroup, "DplatformOS.CreateRawTxGroup");
    g_methodMap.insert(ID_GetCoinSymbol, "DplatformOS.GetCoinSymbol");
    g_methodMap.insert(ID_GetProperFee, "DplatformOS.GetProperFee");
    g_methodMap.insert(ID_ConvertExectoAddr, "DplatformOS.ConvertExectoAddr");
    g_methodMap.insert(ID_EnablePrivacy, "privacy.EnablePrivacy");
    g_methodMap.insert(ID_ShowPrivacyKey, "privacy.ShowPrivacyKey");
    g_methodMap.insert(ID_PrivacyCreateRawTransaction, "privacy.CreateRawTransaction");
    g_methodMap.insert(ID_ShowPrivacyAccountInfo, "privacy.ShowPrivacyAccountInfo");
    g_methodMap.insert(ID_ShowPrivacyAccountSpend, "privacy.ShowPrivacyAccountSpend");
    g_methodMap.insert(ID_ShowPrivacyAccountSpend, "privacy.GetPrivacyTxByAddr");
}

BaseJsonConnector::BaseJsonConnector()
    : m_nID (-1)
{
    if(g_methodMap.empty())
    {
        InitMethodMap();
    }
}

BaseJsonConnector::~BaseJsonConnector()
{
}

void BaseJsonConnector::PostJsonMessage(IdType id, const QJsonArray &params)
{
    m_nID = id;
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("id", m_nID);
    json.insert("method", g_methodMap[id]);
    json.insert("params", params);

    PostJson(json);
}

void BaseJsonConnector::PostJson(const QJsonObject &PostJson)
{
    QJsonDocument document;
    document.setObject(PostJson);
    QByteArray postData = document.toJson(QJsonDocument::Compact);
    PostJsonData(postData);
}

void BaseJsonConnector::PostJsonMessage(IdType id)
{
    m_nID = id;
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("id", m_nID);
    json.insert("method", g_methodMap[id]);
    QJsonArray params;
    json.insert("params", params);
    PostJson(json);
}

void BaseJsonConnector::PostJsonMessage(IdType id, const char* strParams)
{
    m_nID = id;
    QByteArray dataArray;
    std::stringstream ostr;
    ostr << "{\"id\":" << id << ",\"jsonrpc\":\"2.0\",\"method\":\"" << g_methodMap[id].toStdString().c_str() << "\",\"params\":[" << strParams << "]}";
    dataArray.append(ostr.str().c_str());
    PostJsonData(dataArray);
}

bool BaseJsonConnector::bPrintf()
{
    if(m_nID == ID_SendToAddress
            || m_nID == ID_CreateRawTradeSellTx
            || m_nID == ID_CreateRawTradeBuyTx
            || m_nID == ID_CreateRawTradeRevokeTx
            || m_nID == ID_SignRawTx_CreateBindMiner
            || m_nID == ID_SignRawTx_CreateRawTransaction
            || m_nID == ID_CreateRawTxGroup
            || m_nID == ID_SendTransaction
            || m_nID == ID_CreateBindMiner
            || m_nID == ID_CreateRawTransaction
            || m_nID == ID_SetAutoMining
            || m_nID == ID_CloseQueue)
    {
        return true;
    }

    return false;
}

QNetworkReply *BaseJsonConnector::GetNetworkReplyPost(const QByteArray &postData)
{
    m_postData = postData;

    if(bPrintf())
        qDebug() << ("Post 内容: ") << postData;

    QNetworkRequest request;
    request.setUrl(QUrl(CStyleConfig::GetInstance().GetNetworkUrl()));

    return g_lpaccessManager->post(request, postData);
}

void BaseJsonConnector::finishedNetwork(QNetworkReply *reply)
{
    if(reply ==  NULL)
        return;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  // bytes

        if(bPrintf())
            qDebug() << ("Post 结果: ") << bytes;

        int id = -1;
        QVariant result;
        QString error;
        #if QT_VERSION >= 0x050000
        QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes);
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("id"))
            {
                id = obj.take("id").toVariant().toInt();                
            }

            if(obj.contains("result"))
            {
                result = obj.take("result").toVariant();                
            }

            if(obj.contains("error"))
            {
                error = obj.take("error").toVariant().toString();
            }
        }
#else
        QScriptEngine engine;

        QScriptValue sc = engine.evaluate("value=" + QString(bytes));
        id = sc.property("id").toInt32();
        result = sc.property("result").toVariant();
        error = sc.property("error").toString();
#endif

        m_nID = id;

        if(!error.isEmpty() && !g_mapErrorCode[error].isEmpty())
        {
            error = g_mapErrorCode[error];
        }
        requestFinished(result, error);
    }
    else
    {
        if (g_lpMainUI && g_lpMainUI->m_lpStatusBarUI)
            g_lpMainUI->m_lpStatusBarUI->setNumConnections(0);
    }

    reply->abort();
    reply->close();
    reply->deleteLater();
    g_lpapp->processEvents();
}

JsonConnectorDialog::JsonConnectorDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint); // 没有帮助按钮
}

void JsonConnectorDialog::PostJsonData(const QByteArray &postData)
{
   QObject::connect(GetNetworkReplyPost(postData), SIGNAL(finished()), this, SLOT(finishedSlot()));
}

void JsonConnectorDialog::finishedSlot()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    finishedNetwork(reply);
}

JsonConnectorMainWindow::JsonConnectorMainWindow(QWidget *parent) : QMainWindow(parent)
{
}

void JsonConnectorMainWindow::PostJsonData(const QByteArray &postData)
{
    QObject::connect(GetNetworkReplyPost(postData), SIGNAL(finished()), this, SLOT(finishedSlot()));
}

void JsonConnectorMainWindow::finishedSlot()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    finishedNetwork(reply);
}

JsonConnectorWidget::JsonConnectorWidget(QWidget *parent) : QWidget(parent)
{
}

void JsonConnectorWidget::PostJsonData(const QByteArray &postData)
{
    QObject::connect(GetNetworkReplyPost(postData), SIGNAL(finished()), this, SLOT(finishedSlot()));
}

void JsonConnectorWidget::finishedSlot()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    finishedNetwork(reply);
}
