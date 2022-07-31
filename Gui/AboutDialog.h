#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "QtAwesome.h"

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *pParent = nullptr);
    ~AboutDialog(void);

signals:
    void CheckForUpdateClickedSignal(void);

protected:
    Ui::AboutDialog *mUi;

    QtAwesome *mAwesome; // Contains Fontawesome logos

    QVariantMap mStandardIconVariant;
};

#endif // ABOUTDIALOG_H
