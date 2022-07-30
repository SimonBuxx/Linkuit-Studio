#include "WelcomeDialog.h"
#include "ui_WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *pParent):
    QDialog(pParent),
    mUi(new Ui::WelcomeDialog)
{
    mUi->setupUi(this);

    mAwesome = new QtAwesome(this);
    mAwesome->initFontAwesome();

    mStandardIconVariant.insert("color", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mStandardIconVariant.insert("color-active", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-selected", QColor(0, 45, 50));

    mUi->uNewCircuitButton->setIcon(mAwesome->icon(fa::fileo, mStandardIconVariant));
    mUi->uOpenCircuitButton->setIcon(mAwesome->icon(fa::folderopeno, mStandardIconVariant));
    mUi->uStartTutorialButton->setIcon(mAwesome->icon(fa::graduationcap, mStandardIconVariant));
    mUi->uWebsiteButton->setIcon(mAwesome->icon(fa::externallink, mStandardIconVariant));
    mUi->uGithubButton->setIcon(mAwesome->icon(fa::github, mStandardIconVariant));
    mUi->uCheckForUpdateButton->setIcon(mAwesome->icon(fa::refresh, mStandardIconVariant));

    QObject::connect(mUi->uNewCircuitButton, &QPushButton::clicked, this, &WelcomeDialog::NewCircuitClickedSignal);
    QObject::connect(mUi->uOpenCircuitButton, &QPushButton::clicked, this, &WelcomeDialog::OpenCircuitClickedSignal);
    QObject::connect(mUi->uStartTutorialButton, &QPushButton::clicked, this, &WelcomeDialog::StartTutorialClickedSignal);
    QObject::connect(mUi->uWebsiteButton, &QPushButton::clicked, this, &WelcomeDialog::OpenWebsiteClickedSignal);
    QObject::connect(mUi->uGithubButton, &QPushButton::clicked, this, &WelcomeDialog::OpenGithubClickedSignal);
    QObject::connect(mUi->uCheckForUpdateButton, &QPushButton::clicked, this, &WelcomeDialog::CheckForUpdateClickedSignal);
}

WelcomeDialog::~WelcomeDialog()
{
    delete mUi;
}
