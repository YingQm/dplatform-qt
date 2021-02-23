#include "privacyui.h"
#include "ui_privacyui.h"
#include "basefuntion.h"
#include "platformstyle.h"
#include "addresslistui.h"
#include "addressui.h"
#include "base58.h"
#include "sendtocontractdialog.h"
#include "transactionslistmodel.h"
#include "mainui.h"

extern MainUI*              g_lpMainUI;

PrivacyUi::PrivacyUi(QWidget *parent, const PlatformStyle *platformStyle) :
    JsonConnectorWidget(parent),
    m_platformStyle(platformStyle),
    ui(new Ui::PrivacyUi),
    m_strSelectedAddr(""),
    m_coinsBalance(0.0),
    m_contractBalance(0.0),
    m_privacyBalance(0.0),
    m_nPage(0),
    m_ndirection(0)
{
    ui->setupUi(this);
    initUI();
}

PrivacyUi::~PrivacyUi()
{
    delete ui;
}

void PrivacyUi::initUI()
{
    // wallet
    ui->sendWidget->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child2());
    if (CStyleConfig::GetInstance().GetStyleType() == QSS_BLUE) {
        this->setStyleSheet("QWidget {background-color:#FFFFFF;border:none;}" + CStyleConfig::GetInstance().GetStylesheet());
        ui->sendwidget1_3->setStyleSheet("QWidget {background-color:#F5F4F9;border-radius: 4px;}");
    }
    else {
        ui->sendwidget1_3->setStyleSheet("QWidget {background-color:#2c2c2c;border-radius: 4px;}");
    }
    ui->addrEdit->setEnabled(false);
    ui->refreshBtn->setStyleSheet("QToolButton {background-color: transparent; }");
    ui->copyBtn->setStyleSheet("QToolButton {background-color: transparent; }");

    ui->label_Balance_text->setText(tr("公开地址余额 (%1)").arg(CStyleConfig::GetInstance().GetUnitName()));
    ui->label_Contract_text->setText(tr("合约地址余额 (%1)").arg(CStyleConfig::GetInstance().GetUnitName()));
    ui->label_Privacy_text->setText(tr("隐私地址余额 (%1)").arg(CStyleConfig::GetInstance().GetUnitName()));
#ifndef MAC_OSX
    ui->labelBalance->setStyleSheet("QLabel { font: " + QString::number(GetBaseFontSize() + 2) + "pt;}");
    ui->labelContract->setStyleSheet("QLabel { font: " + QString::number(GetBaseFontSize() + 2) + "pt;}");
    ui->labelPrivacy->setStyleSheet("QLabel { font: " + QString::number(GetBaseFontSize() + 4) + "pt;}");
    ui->label_Balance_text->setStyleSheet("QLabel { color: #969BA6; font: " + QString::number(GetBaseFontSize()) + "pt;}");
    ui->label_Contract_text->setStyleSheet("QLabel { color: #969BA6; font: " + QString::number(GetBaseFontSize()) + "pt;}");
    ui->label_Privacy_text->setStyleSheet("QLabel { color: #969BA6; font: " + QString::number(GetBaseFontSize()) + "pt;}");
#endif

    // send
    ui->addressFromButton->setIcon(m_platformStyle->SingleColorIcon(":/address_book"));
    ui->addressBookButton->setIcon(m_platformStyle->SingleColorIcon(":/address_book"));
    ui->selectBtn->setIcon(m_platformStyle->SingleColorIcon(":/address_book"));
    QString strMinFeeText = tr("公对私交易手续费仅 %1 ").arg(CStyleConfig::GetInstance().GetMinFee());
    strMinFeeText = strMinFeeText + tr("%1, 其他交易手续费需要 1 %1").arg(CStyleConfig::GetInstance().GetUnitName());
    ui->label_fee_text->setText(strMinFeeText);
    ui->payAmount->setValidator(new QDoubleValidator(0.0, 5*le8, 4, this));
    ui->addAsLabel->setPlaceholderText(tr("输入发送交易的备注信息"));
    ui->payTo->setPlaceholderText(tr("输入公开地址或者隐私地址"));
    ui->addAsLabel->setToolTip(tr("输入发送交易的备注信息"));
    ui->payTo->setToolTip(tr("输入公开地址或者隐私地址"));

    // tx list
    ui->privacyTxsWidget->setStyleSheet(CStyleConfig::GetInstance().GetStylesheet_child2());
    ui->searchWidget->setVisible(false);
    ui->prevPageBtn->setEnabled(false);
    ui->typeWidget->addItem("发送方", 0);
    ui->typeWidget->addItem("接收方", 1);

    ui->listTransactions->setSelectionMode(QAbstractItemView::SingleSelection);
    TxViewItemDelegate *txdelegate = new TxViewItemDelegate();
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->horizontalHeader()->setVisible(false);
    ui->listTransactions->verticalHeader()->setVisible(false);
    ui->listTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选中整行
    ui->listTransactions->setShowGrid(false);
    ui->listTransactions->setFrameShape(QFrame::NoFrame);
    ui->listTransactions->horizontalHeader()->setStretchLastSection(true);
    ui->listTransactions->verticalHeader()->setDefaultSectionSize(51);
    ui->listTransactions->setEditTriggers(QAbstractItemView::SelectedClicked);

    model = new TransactionsListModel(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->listTransactions->setModel(proxyModel);
    ui->listTransactions->sortByColumn(0, Qt::DescendingOrder);
}

void PrivacyUi::requestFinished(const QVariant &result, const QString &error)
{
    QMap<QString, QVariant> resultMap = result.toMap();
    if (ID_EnablePrivacy == m_nID) {
        QList<QVariant> resultsList = resultMap["results"].toList();
        if (resultsList.length() > 0) {
            QMap<QString, QVariant> resultsMap = resultsList[0].toMap();
            if (resultsMap["IsOK"].toBool() == true) {
                // 获取隐私地址
                PostMsgShowPrivacyKey(resultsMap["addr"].toString());
                return;
            }
        }

        QMessageBox::information(this, tr("提示"), tr("获取隐私地址失败, %1").arg(error));
    } else if (ID_ShowPrivacyKey == m_nID) {
        if (!resultMap["pubkeypair"].toString().isEmpty()) {
            ui->PrivacyAddrLabel->setText(resultMap["pubkeypair"].toString());
        } else {
            QMessageBox::information(this, tr("提示"), tr("获取隐私地址失败, %1").arg(error));
        }
    } else if (ID_GetBalance_coins == m_nID) {
        QList<QVariant> resultsList = result.toList();
        if (resultsList.length() > 0) {
            QMap<QString, QVariant> resultsMap = resultsList[0].toMap();
            m_coinsBalance = GetbalanceD(resultsMap["balance"].toDouble());
            ui->labelBalance->setText(QString::number(m_coinsBalance, 'f', 4));
        }
    } else if (ID_GetBalance_privacy == m_nID) {
        QList<QVariant> resultsList = result.toList();
        if (resultsList.length() > 0) {
            QMap<QString, QVariant> resultsMap = resultsList[0].toMap();
            m_contractBalance = GetbalanceD(resultsMap["balance"].toDouble());
            ui->labelContract->setText(QString::number(m_contractBalance, 'f', 4));
        }
    } else if (ID_ShowPrivacyAccountInfo == m_nID) {
        double totalAmount = 0.0;
        QList<QVariant> utxosList = resultMap["utxos"].toList();
        for(int i=0; i<utxosList.length(); ++i){
            QMap<QString, QVariant> utxosMap = utxosList[i].toMap();
            totalAmount += utxosMap["amount"].toDouble();
        }
        m_privacyBalance = GetbalanceD(totalAmount);
        ui->labelPrivacy->setText(QString::number(m_privacyBalance, 'f', 4));
    } else if (ID_GetPrivacyTxByAddr == m_nID) {
        QList<QVariant> txList = (resultMap["txDetails"]).toList();
        model->RemoveALLEntry();
        QString strFromFirst;
        QString strFromEnd;
        for (int i = 0; i<txList.size(); ++i) {
            QMap<QString, QVariant> txMap = txList[i].toMap();
            double dAmount = txMap["amount"].toDouble();
            QString strToAddr = txMap["tx"].toMap()["to"].toString();
            QString strFromAddr = txMap["fromAddr"].toString();
            QString strTxHash = txMap["txHash"].toString();
            uint nTime = txMap["blockTime"].toUInt();
            int nFee = txMap["tx"].toMap()["fee"].toUInt();
            QString strExecer = txMap["tx"].toMap()["execer"].toString();
            QString strActionname = txMap["actionName"].toString();
            int nTy = txMap["receipt"].toMap()["ty"].toInt();
            QString strNote = txMap["tx"].toMap()["payload"].toMap()["Value"].toMap()["Transfer"].toMap()["note"].toString();

            QString strError;
            if(nTy == 1) {
                QList<QVariant> MapError = txMap["receipt"].toMap()["logs"].toList();
                for(int j=0; j<MapError.size(); ++j) {
                    if(MapError[j].toMap()["ty"] == 1) {
                        strError = MapError[j].toMap()["log"].toString();
                        break;
                    }
                }
            }

            model->AdddateEntry(TransactionsListEntry(nTime, strToAddr, strFromAddr, strTxHash, dAmount, nFee, strExecer, strActionname, nTy, strNote, strError));

            if(i == 0)
                strFromFirst = QString().sprintf("%013d", txMap["height"].toInt()) + QString().sprintf("%05d", txMap["index"].toInt());
            if(i == txList.size() -1)
                strFromEnd = QString().sprintf("%013d", txMap["height"].toInt()) + QString().sprintf("%05d", txMap["index"].toInt());
        }

        if(m_ndirection == 0){
            m_strFromTxPrev = strFromFirst;
            m_strFromTxNext = strFromEnd;
        } else {
            m_strFromTxNext = strFromFirst;
            m_strFromTxPrev = strFromEnd;
        }

        if(txList.size() < COUNT_NUM && m_ndirection == 0) {
            ui->nextPageBtn->setEnabled(false);
        } else {
            ui->nextPageBtn->setEnabled(true);
        }
    }
}

// 开启隐私功能
void PrivacyUi::PostMsgOpenPrivacy(const QString &addr)
{
    QJsonObject jsonParms;
    jsonParms.insert("addrs", QJsonArray() << addr);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_EnablePrivacy, params);
}

// 获取隐私地址
void PrivacyUi::PostMsgShowPrivacyKey(const QString &addr)
{
    QJsonObject jsonParms;
    jsonParms.insert("data", addr);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ShowPrivacyKey, params);
}

// 获取coins中的地址余额
void PrivacyUi::PostMsgGetCoinsBalance(const QString &addr)
{
    QString jsonCmd = "{\"addresses\":[\"";
    jsonCmd.append(addr);
    jsonCmd.append("\"], \"execer\":\"coins\"}");
    PostJsonMessage(ID_GetBalance_coins, jsonCmd.toStdString().c_str());
}

// 获取合约中的地址余额
void PrivacyUi::PostMsgGetContractBalance(const QString &addr)
{
    QString jsonCmd = "{\"addresses\":[\"";
    jsonCmd.append(addr);
    jsonCmd.append("\"], \"execer\":\"privacy\"}");
    PostJsonMessage(ID_GetBalance_privacy, jsonCmd.toStdString().c_str());
}

// 查询utxo余额信息
void PrivacyUi::PostMsgGetPrivacyBalance(const QString &addr)
{
    QJsonObject jsonParms;
    jsonParms.insert("addr", addr);
    jsonParms.insert("token", CStyleConfig::GetInstance().GetUnitName());
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_ShowPrivacyAccountInfo, params);
}

// 查询钱包中指定地址的隐私交易
void PrivacyUi::PostMsgPrivacyListTxs()
{
    QJsonObject jsonParms;
    jsonParms.insert("address", m_strSelectedAddr);
    jsonParms.insert("sendRecvFlag", ui->typeWidget->currentData().toInt());     // send or recv flag (0: send, 1: recv)
    jsonParms.insert("direction", m_ndirection);       // query direction (0: pre page, 1: next page), valid with seedtxhash param
    jsonParms.insert("count", COUNT_NUM);                  // number of transactions
    jsonParms.insert("seedtxhash", m_strFromTx);
    jsonParms.insert("assetExec", "coins");
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_GetPrivacyTxByAddr, params);
}

// 构建交易
void PrivacyUi::PostMsgCreatePrivacyTx(const QString &fromAddr, const QString &toAddr, double amount, const QString &note)
{
    // type:1 公对私转账, 2 私对私转账, 3 私到公转账
    int type = 0;
    if(!IsAddrValid(fromAddr.toStdString().c_str()) && !IsAddrValid(toAddr.toStdString().c_str())) {
        type = 2;
    } else if (!IsAddrValid(fromAddr.toStdString().c_str())) {
        type = 3;
    } else if (!IsAddrValid(toAddr.toStdString().c_str())) {
        type = 1;
    } else {
        QMessageBox::information(this, tr("提示"), tr("地址输入有误, 必须有隐私地址!"));
        return;
    }

    QJsonObject jsonParms;
    jsonParms.insert("type", type);
    jsonParms.insert("from", fromAddr);
    jsonParms.insert("amount", amount*le8);
    if (type == 3) {
        jsonParms.insert("to", toAddr);
    } else {
        jsonParms.insert("pubkeypair", toAddr);
    }
    jsonParms.insert("note", note);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_PrivacyCreateRawTransaction, params);
}

void PrivacyUi::PostMsgSignPrivacyTx(const QString &privkey, const QString &txHex)
{
    // 签名
    QJsonObject jsonParms;
    jsonParms.insert("expire", "300s");
    jsonParms.insert("privkey", privkey);
    jsonParms.insert("txHex", txHex);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_SignRawTx, params);
}

void PrivacyUi::PostMsgSendPrivacyTx(const QString &signHex)
{
    // 发送
    QJsonObject jsonParms;
    jsonParms.insert("data", signHex);
    QJsonArray params;
    params.insert(0, jsonParms);
    PostJsonMessage(ID_SendTransaction, params);
}

void PrivacyUi::on_refreshBtn_clicked()
{
    if(m_strSelectedAddr.isEmpty()){
        QMessageBox::information(this, tr("提示"), tr("请先选择公开地址!"));
        return;
    }

    PostMsgGetCoinsBalance(m_strSelectedAddr);
    PostMsgGetContractBalance(m_strSelectedAddr);
    PostMsgGetPrivacyBalance(m_strSelectedAddr);
    on_firstPageBtn_clicked();
}

void PrivacyUi::on_selectBtn_clicked()
{
    AddressListUI dlg(ForSending, TabsReceiving, this, m_platformStyle);
    if (dlg.exec()) {
        m_strSelectedAddr = dlg.getReturnAddr();
        ui->addrEdit->setText(m_strSelectedAddr);
        m_coinsBalance = dlg.getBalance();
        ui->labelBalance->setText(QString::number(m_coinsBalance, 'f', 4));
        PostMsgOpenPrivacy(m_strSelectedAddr);
        PostMsgGetContractBalance(m_strSelectedAddr);
        PostMsgGetPrivacyBalance(m_strSelectedAddr);
        on_firstPageBtn_clicked();
    }
}

void PrivacyUi::on_copyBtn_clicked()
{
    GUIUtil::setClipboard(ui->PrivacyAddrLabel->text());
}

void PrivacyUi::on_rollInBtn_clicked()
{
    SendToContractDialog dlg(m_strSelectedAddr, m_coinsBalance, ContractRollIn, this);
    dlg.exec();
}

void PrivacyUi::on_rollOutBtn_clicked()
{
    SendToContractDialog dlg(m_strSelectedAddr, m_contractBalance, ContractRollOut, this);
    dlg.exec();
}

void PrivacyUi::on_addressFromButton_clicked()
{
    AddressListUI dlg(ForSending, TabsReceiving, this, m_platformStyle);
    if (dlg.exec()) {
        ui->fromAddr->setText(dlg.getReturnAddr());
        // ui->payAmount->setAmountRange(dlg.getBalance());
    }
}

void PrivacyUi::on_addressBookButton_clicked()
{
    AddressListUI dlg(ForSending, TabsSending, this, m_platformStyle);
    if (dlg.exec()) {
        ui->payTo->setText(dlg.getReturnAddr());
        ui->payAmount->setFocus();
    }
    g_lpMainUI->m_lpAddressUI->m_lpFriendsAddressList->UpdateAddrList();
}

void PrivacyUi::on_clearButton_clicked()
{
    ui->payTo->clear();
    ui->addAsLabel->clear();
    ui->payAmount->clear();
    ui->payTo->setFocus();
}

void PrivacyUi::on_sendButton_clicked()
{
    PostMsgCreatePrivacyTx(ui->fromAddr->text(), ui->payTo->text(), ui->payAmount->text().toDouble(), ui->addAsLabel->text());
}

void PrivacyUi::on_typeWidget_currentIndexChanged(int index)
{
    if (!m_strSelectedAddr.isEmpty()) {
        on_firstPageBtn_clicked();
    }
}

void PrivacyUi::on_firstPageBtn_clicked()
{
    m_nPage = 0;
    m_ndirection = 0;
    m_strFromTx = "";
    PostMsgPrivacyListTxs();

    ui->prevPageBtn->setEnabled(false);
}

void PrivacyUi::on_prevPageBtn_clicked()
{
    --m_nPage;
    if(m_nPage == 0) {
        on_firstPageBtn_clicked();
    } else {
        m_ndirection = 1;
        m_strFromTx = m_strFromTxPrev;
        PostMsgPrivacyListTxs();
    }
}

void PrivacyUi::on_nextPageBtn_clicked()
{
    ++m_nPage;

    m_ndirection = 0;
    m_strFromTx = m_strFromTxNext;
    PostMsgPrivacyListTxs();

    ui->prevPageBtn->setEnabled(true);

}
