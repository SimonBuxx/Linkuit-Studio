#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "QtAwesome.h"

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
    /// \param pAwesome: Reference to the QtAwesome object
    /// \param pParent: Pointer to the parent widget
    explicit AboutDialog(QtAwesome &pAwesome, QWidget *pParent = nullptr);

    /// \brief Destructor for the about dialog
    ~AboutDialog(void) override;

signals:
    /// \brief Emitted when the user clicks the update button
    void CheckForUpdateClickedSignal(void);

protected:
    Ui::AboutDialog *mUi;

    QtAwesome &mAwesome; // Contains Fontawesome logos

    QVariantMap mStandardIconVariant;
};

#endif // ABOUTDIALOG_H
