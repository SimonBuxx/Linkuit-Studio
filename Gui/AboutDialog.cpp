#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "Configuration.h"
#include <QtGlobal>

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

    mUi->uSoftwareNameLabel->setText(tr("Version %0").arg(QString::fromStdString(SW_VERSION_STRING)));
    mUi->uBuildInfoText->setText(tr("Software version: %0<br/>Build time: %1<br/>Qt version: %2<br/>Compiler: MinGW 64-bit %3").arg(QString::fromStdString(SW_VERSION_STRING), BUILD_TIME, qVersion(), __VERSION__));

    QObject::connect(mUi->uCheckForUpdateButton, &QPushButton::clicked, this, &AboutDialog::CheckForUpdateClickedSignal);
}

AboutDialog::~AboutDialog()
{
    delete mUi;
}
