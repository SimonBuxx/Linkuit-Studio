#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QtGlobal>

AboutDialog::AboutDialog(QWidget *pParent):
    QDialog(pParent),
    mUi(new Ui::AboutDialog)
{
    const QString BUILD_TIME = QStringLiteral(__DATE__ " " __TIME__);

    mUi->setupUi(this);

    mUi->uSoftwareNameLabel->setText(tr("Version %0").arg(QString(FULL_VERSION)));

    QString buildMode;
    #ifdef QT_DEBUG
      buildMode = "Debug";
    #else
      buildMode = "Release";
    #endif

    mUi->uBuildInfoText->setText(tr("Software version: %0<br/>Build time: %1<br/>Build mode: %2<br/>Qt version: %3<br/>Compiler: MinGW 64-bit %4").arg(QString(FULL_VERSION), BUILD_TIME, buildMode, qVersion(), __VERSION__));

    QObject::connect(mUi->uCheckForUpdateButton, &QPushButton::clicked, this, &AboutDialog::CheckForUpdateClickedSignal);
}

AboutDialog::~AboutDialog()
{
    delete mUi;
}
