#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

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

protected:
    Ui::AboutDialog *mUi;
};

#endif // ABOUTDIALOG_H
