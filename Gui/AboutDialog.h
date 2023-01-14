#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

///
/// \brief The WelcomeDialog class represents the about dialog
///
class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    /// \brief Constructor for the about dialog
    /// \param pParent: Pointer to the parent widget
    explicit AboutDialog(QWidget *pParent = nullptr);

    /// \brief Destructor for the about dialog
    ~AboutDialog(void) override;

signals:
    /// \brief Emitted when the user clicks the update button
    void CheckForUpdateClickedSignal(void);

protected:
    Ui::AboutDialog *mUi;
};

#endif // ABOUTDIALOG_H
