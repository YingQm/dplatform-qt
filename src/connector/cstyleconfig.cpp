#include "cstyleconfig.h"
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QCoreApplication>
#include <stdio.h>
#include <QDebug>

CStyleConfig* CStyleConfig::s_lpStyleConfig = nullptr;

CStyleConfig::CStyleConfig()
: m_strDplatformosName ("dplatformos")
, m_strAppName ("dplatformos-qt")
, m_strAppName_en ("dplatformos-qt")
, m_strAppName_zh ("dplatformos-qt")
, m_strUnitName ("")
, m_dMinFee (0.0)
, m_strNetworkUrl ("http://localhost:28803/")
, m_stylesheet_type ("yellow")
{
    readConfigFile();
    setDplatformosNamePath();
}

void CStyleConfig::readConfigFile()
{
    QString strPath = QCoreApplication::applicationDirPath() + "/StyleConfig.ini";
    QFileInfo fileInfo(strPath);

    qDebug() << (strPath.toStdString().c_str());

    if(fileInfo.exists()) {
        QSettings *lpconfigIni = new QSettings(strPath, QSettings::IniFormat);
        lpconfigIni->setIniCodec(QTextCodec::codecForName("UTF-8"));

        readValue(lpconfigIni, "Config/DplatformosName", m_strDplatformosName);
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

void CStyleConfig::setDplatformosNamePath()
{
    m_strDplatformosExe = m_strDplatformosName;
    m_strDplatformoscliExe = m_strDplatformosName + "-cli";

#ifdef WIN32
    if(!isWow64()) {
        m_strDplatformosExe += "-x86";
        m_strDplatformoscliExe += "-x86";
    }
#endif

    m_strDplatformosPath = QCoreApplication::applicationDirPath() + "/" + m_strDplatformosExe;
    m_strDplatformoscliPath = QCoreApplication::applicationDirPath() + "/" + m_strDplatformoscliExe;

    qDebug() << "m_strDplatformosName:" << m_strDplatformosExe << "m_strDplatformoscliName:" << m_strDplatformoscliExe;
    qDebug() << "m_strDplatformosPath:" << m_strDplatformosPath << "m_strDplatformoscliPath:" << m_strDplatformoscliPath;
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
