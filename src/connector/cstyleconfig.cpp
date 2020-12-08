#include "cstyleconfig.h"
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QCoreApplication>
#include <stdio.h>
#include <QDebug>

CStyleConfig* CStyleConfig::s_lpStyleConfig = nullptr;

CStyleConfig::CStyleConfig()
: m_strDplatformName ("dplatform")
, m_strAppName ("dplatform-qt")
, m_strAppName_en ("dplatform-qt")
, m_strAppName_zh ("dplatform-qt")
, m_strUnitName ("")
, m_dMinFee (0.0)
, m_strNetworkUrl ("http://localhost:8801/")
, m_stylesheet_type ("yellow")
{
    readConfigFile();
    setDplatformNamePath();
}

void CStyleConfig::readConfigFile()
{
    QString strPath = QCoreApplication::applicationDirPath() + "/StyleConfig.ini";
    QFileInfo fileInfo(strPath);

    qDebug() << (strPath.toStdString().c_str());

    if(fileInfo.exists()) {
        QSettings *lpconfigIni = new QSettings(strPath, QSettings::IniFormat);
        lpconfigIni->setIniCodec(QTextCodec::codecForName("UTF-8"));

        readValue(lpconfigIni, "Config/DplatformName", m_strDplatformName);
        readValue(lpconfigIni, "Config/AppName", m_strAppName);
        readValue(lpconfigIni, "Config/AppName_zh", m_strAppName_zh);
        readValue(lpconfigIni, "Config/AppName_en", m_strAppName_en);
        readValue(lpconfigIni, "Config/StyleType", m_stylesheet_type);
        readValue(lpconfigIni, "Config/NetworkUrl", m_strNetworkUrl);
        delete lpconfigIni;
    }

    QString strQssName = ":/qss_yellow";
    if (m_stylesheet_type == "blue")
        strQssName = ":/qss_blue";

    QFile file(strQssName);
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    m_stylesheet = filetext.readAll();
    file.close();

    m_stylesheet_main = "QWidget {background-color:#2c2c2c;border:none;}" + m_stylesheet;
    m_stylesheet_child = "QWidget {background-color:#3d3d3d;border:none;}" + m_stylesheet;

    if (m_stylesheet_type == "blue") {
        m_stylesheet_main = m_stylesheet;
        m_stylesheet_child = m_stylesheet;
//        m_stylesheet_main = "QWidget {background-color:#E7EDF1;border:none;}" + m_stylesheet;
//        m_stylesheet_child = "QWidget {background-color:#E7EDF1;border:none;}" + m_stylesheet;
    }

    QString lang_territory = QString::fromStdString(QLocale::system().name().toStdString());
    if(lang_territory == "en") {
        m_strAppName = m_strAppName_en;
    } else {
        m_strAppName = m_strAppName_zh;
    }
}

void CStyleConfig::readValue(QSettings *lpconfig, const QString &key, QString &ret)
{
    if (lpconfig) {
        QString strConfig = lpconfig->value(key).toString();
        if(!strConfig.isEmpty())
            ret = strConfig;
    }
}

void CStyleConfig::setDplatformNamePath()
{
    m_strDplatformExe = m_strDplatformName;
    m_strDplatformcliExe = m_strDplatformName + "-cli";

#ifdef WIN32
    if(!isWow64()) {
        m_strDplatformExe += "-x86";
        m_strDplatformcliExe += "-x86";
    }
    m_strDplatformExe += ".exe";
    m_strDplatformcliExe += ".exe";
#endif

    m_strDplatformPath = QCoreApplication::applicationDirPath() + "/" + m_strDplatformExe;
    m_strDplatformcliPath = QCoreApplication::applicationDirPath() + "/" + m_strDplatformcliExe;

    qDebug() << "m_strDplatformName:" << m_strDplatformExe << "m_strDplatformcliName:" << m_strDplatformcliExe;
    qDebug() << "m_strDplatformPath:" << m_strDplatformPath << "m_strDplatformcliPath:" << m_strDplatformcliPath;
}

#ifdef WIN32
bool CStyleConfig::isWow64()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		return true;
	else
        return false;
}
#endif

CStyleConfig &CStyleConfig::GetInstance()
{
    if(s_lpStyleConfig == nullptr) {
          s_lpStyleConfig = new CStyleConfig;
    }
    return *s_lpStyleConfig;
}

STYLE_QSS CStyleConfig::GetStyleType() const
{
    if (m_stylesheet_type == "blue")
        return QSS_BLUE;

    return QSS_YELLOW;
}

COINS_TYPE CStyleConfig::GetCoinsType() const
{
    if (m_strAppName_en == "ycc")
        return TOKEN_YCC;

    return TOKEN_BTY;
}
