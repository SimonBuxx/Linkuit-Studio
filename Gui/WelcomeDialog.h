#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include "QtAwesome.h"

#include <QDialog>
#include <QFileInfo>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WelcomeDialog(QWidget *pParent = nullptr);

    ~WelcomeDialog(void);

    void SetShowOnStartup(bool pShowOnStartup);
    void SetRecentFilePaths(const std::vector<QFileInfo>& pRecentFilePaths);

signals:
    void NewCircuitClickedSignal(void);
    void OpenCircuitClickedSignal(void);
    void OpenGithubClickedSignal(void);
    void OpenTwitterClickedSignal(void);
    void CheckForUpdateClickedSignal(void);
    void OpenRecentClickedSignal(const QFileInfo& pPath);
    void ShowOnStartupToggledSignal(bool pChecked);

protected:
    Ui::WelcomeDialog *mUi;

    QtAwesome *mAwesome; // Contains Fontawesome logos

    QVariantMap mStandardIconVariant;
    QVariantMap mWhiteIconVariant;

    std::vector<QFileInfo> mRecentFilePaths;
};

#endif // WELCOMEDIALOG_H
