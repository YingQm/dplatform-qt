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
    Ui::PrivacyUi *ui;
    const PlatformStyle *m_platformStyle;
};

#endif // PRIVACYUI_H
