#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "Configuration.h"

AboutDialog::AboutDialog(QWidget *pParent):
    QDialog(pParent),
    mUi(new Ui::AboutDialog)
{
    const QString BUILD_TIME = QStringLiteral(__DATE__ " " __TIME__);

    mUi->setupUi(this);

    mAwesome = new QtAwesome(this);
    mAwesome->initFontAwesome();

    mStandardIconVariant.insert("color", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mStandardIconVariant.insert("color-active", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-selected", QColor(0, 45, 50));

    mUi->uCheckForUpdateButton->setIcon(mAwesome->icon(fa::refresh, mStandardIconVariant));

    mUi->uSoftwareNameLabel->setText(tr("Linkuit Studio v%0").arg(SW_VERSION_STRING));
    mUi->uBuildInfoLabel->setText(tr("Built with Qt 6.1.0 (MinGW 64-bit)\nBuild time: %0").arg(BUILD_TIME));

    QObject::connect(mUi->uCheckForUpdateButton, &QPushButton::clicked, this, &AboutDialog::CheckForUpdateClickedSignal);
}

AboutDialog::~AboutDialog()
{
    delete mUi;
}
