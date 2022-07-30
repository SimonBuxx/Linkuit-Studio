#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include "QtAwesome.h"

#include <QDialog>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WelcomeDialog(QWidget *pParent = nullptr);
    ~WelcomeDialog(void);

signals:
    void NewCircuitClickedSignal(void);
    void OpenCircuitClickedSignal(void);
    void StartTutorialClickedSignal(void);
    void OpenWebsiteClickedSignal(void);
    void OpenGithubClickedSignal(void);
    void CheckForUpdateClickedSignal(void);
    void OpenRecentClickedSignal(QString pPath);

protected:
    Ui::WelcomeDialog *mUi;

    QtAwesome *mAwesome; // Contains Fontawesome logos

    QVariantMap mStandardIconVariant;
};

#endif // WELCOMEDIALOG_H
