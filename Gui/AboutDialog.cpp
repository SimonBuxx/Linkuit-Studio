#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QtGlobal>

AboutDialog::AboutDialog(QtAwesome &pAwesome, QWidget *pParent):
    QDialog(pParent),
    mUi(new Ui::AboutDialog),
    mAwesome(pAwesome)
{
    const QString BUILD_TIME = QStringLiteral(__DATE__ " " __TIME__);

    mUi->setupUi(this);

    mStandardIconVariant.insert("color", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mStandardIconVariant.insert("color-active", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-selected", QColor(0, 45, 50));

    mUi->uCheckForUpdateButton->setIcon(mAwesome.icon(fa::refresh, mStandardIconVariant));

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
