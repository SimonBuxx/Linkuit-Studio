#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include "QtAwesome.h"

#include <QDialog>
#include <QFileInfo>

// forward declarations
namespace Ui {
class WelcomeDialog;
}

///
/// \brief The WelcomeDialog class represents the welcome dialog pop-up
///
class WelcomeDialog : public QDialog
{
    Q_OBJECT
public:
    /// \brief Constructor for the welcome dialog
    /// \param pAwesome: Reference to the QtAwesome object
    /// \param pParent: Pointer to the parent widget
    explicit WelcomeDialog(QtAwesome &pAwesome, QWidget *pParent = nullptr);

    /// \brief Destructor for the welcome dialog
    ~WelcomeDialog(void) override;

    /// \brief Sets the "show on startup" checkbox on the dialog
    /// \param pShowOnStartup: Whether the checbox should be checked or not
    void SetShowOnStartup(bool pShowOnStartup);

    /// \brief Sets the recently used files
    /// \param pRecentFilePaths: The file infos to display as recent files
    void SetRecentFilePaths(const std::vector<QFileInfo>& pRecentFilePaths);

signals:
    /// \brief Emitted when the user clicks the new circuit button
    void NewCircuitClickedSignal(void);

    /// \brief Emitted when the user clicks the open circuit button
    void OpenCircuitClickedSignal(void);

    /// \brief Emitted when the user clicks the GitHub button
    void OpenGithubClickedSignal(void);

    /// \brief Emitted when the user clicks the Twitter button
    void OpenTwitterClickedSignal(void);

    /// \brief Emitted when the user clicks the update button
    void CheckForUpdateClickedSignal(void);

    /// \brief Emitted when the user clicks on a recently used file
    /// \param pPath: The file info of the recently used file
    void OpenRecentClickedSignal(const QFileInfo& pPath);

    /// \brief Emitted when the user toggles the "show on startup" checkbox
    /// \param pChecked: The new checkbox state
    void ShowOnStartupToggledSignal(bool pChecked);

protected:
    Ui::WelcomeDialog *mUi;

    QtAwesome &mAwesome; // Contains Fontawesome logos

    QVariantMap mStandardIconVariant;
    QVariantMap mWhiteIconVariant;

    std::vector<QFileInfo> mRecentFilePaths;
};

#endif // WELCOMEDIALOG_H
