#ifndef CSTYLECONFIG_H
#define CSTYLECONFIG_H

/*
 * 样式
 */

#include <QString>
#include <QSettings>

#ifdef WIN32
#include <windows.h>
#endif

enum STYLE_QSS
{
    QSS_YELLOW,
    QSS_BLUE
};

enum COINS_TYPE
{
    TOKEN_BTY,
    TOKEN_YCC
};

class CStyleConfig
{
public:
    static CStyleConfig &GetInstance();

    STYLE_QSS GetStyleType() const;
    COINS_TYPE GetCoinsType() const;
    const QString &GetDplatformName() const { return m_strDplatformName; }
    const QString &GetAppName() const { return m_strAppName; }
    const QString &GetAppName_en() const { return m_strAppName_en; }
    const QString &GetUnitName() const { return m_strUnitName; }
    void SetUnitName(const QString &strUnitName) { m_strUnitName = strUnitName.toUpper();}
    const double &GetMinFee() const { return m_dMinFee; }
    void SetMinFee(const double &dMinFee) { m_dMinFee = dMinFee;}
    const QString &GetStylesheet() const { return m_stylesheet; }
    const QString &GetStylesheet_main() const { return m_stylesheet_main; }
    const QString &GetStylesheet_child() const { return m_stylesheet_child; }
    const QString &GetDplatformPath() const { return m_strDplatformPath; }
    const QString &GetDplatformcliPath() const { return m_strDplatformcliPath; }
    const QString &GetDplatformExe() const { return m_strDplatformExe; }
    const QString &GetNetworkUrl() const {return m_strNetworkUrl; }
private:
    CStyleConfig();
    void readConfigFile();
    void readValue(QSettings *lpconfig, const QString &key, QString &ret);
    void setDplatformNamePath();
#ifdef WIN32
    bool isWow64();
#endif

private:
    static CStyleConfig* s_lpStyleConfig;

    QString m_strDplatformName;       // 主链名称 默认 dplatform
    QString m_strAppName;           // APP名称 默认 dplatform-qt
    QString m_strAppName_en;        // APP英文名称 默认 dplatform-qt
    QString m_strAppName_zh;        // APP中文名称 默认 dplatform-qt
    QString m_strUnitName;          // 代币单位
    double m_dMinFee;               // 最小手续费
    QString m_strNetworkUrl;        // 连接节点的url 默认 http://localhost:8801/

    QString m_stylesheet_type;      // qss
    QString m_stylesheet;
    QString m_stylesheet_main;      // 颜色主样式
    QString m_stylesheet_child;

    QString m_strDplatformPath;
    QString m_strDplatformcliPath;

    QString m_strDplatformExe;
    QString m_strDplatformcliExe;
};

#endif // CSTYLECONFIG_H
