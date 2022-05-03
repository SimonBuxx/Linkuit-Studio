#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "Configuration.h"

AboutDialog::AboutDialog(QWidget *pParent):
    QDialog(pParent),
    mUi(new Ui::AboutDialog)
{
    mUi->setupUi(this);
    mUi->softwareNameLabel->setText(tr("Linkuit Studio ") + SW_VERSION_STRING);
}

AboutDialog::~AboutDialog()
{
    delete mUi;
}
