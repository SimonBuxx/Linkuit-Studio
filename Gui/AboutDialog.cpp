#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "Configuration.h"

AboutDialog::AboutDialog(QWidget *pParent):
    QDialog(pParent),
    mUi(new Ui::AboutDialog)
{
    const QString BUILD_TIME = QStringLiteral(__DATE__ " " __TIME__);

    mUi->setupUi(this);
    mUi->softwareNameLabel->setText(tr("Linkuit Studio ") + SW_VERSION_STRING);
    mUi->buildInfoLabel->setText(tr("Built with Qt 6.1.0 (MinGW 64-bit)\nBuild time: ") + BUILD_TIME);
}

AboutDialog::~AboutDialog()
{
    delete mUi;
}
