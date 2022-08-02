#include "WelcomeDialog.h"
#include "ui_WelcomeDialog.h"
#include "Configuration.h"

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
    QObject::connect(mUi->uShowOnStartupCheckbox, &QCheckBox::toggled, this, &WelcomeDialog::ShowOnStartupToggledSignal);

    QObject::connect(mUi->uRecentFileButton_1, &QPushButton::clicked, this, [&]()
    {
        if (mRecentFilePaths.size() > 0)
        {
            emit OpenRecentClickedSignal(mRecentFilePaths[0]);
        }
    });

    QObject::connect(mUi->uRecentFileButton_2, &QPushButton::clicked, this, [&]()
    {
        if (mRecentFilePaths.size() > 1)
        {
            emit OpenRecentClickedSignal(mRecentFilePaths[1]);
        }
    });

    QObject::connect(mUi->uRecentFileButton_3, &QPushButton::clicked, this, [&]()
    {
        if (mRecentFilePaths.size() > 2)
        {
            emit OpenRecentClickedSignal(mRecentFilePaths[2]);
        }
    });

    QObject::connect(mUi->uRecentFileButton_4, &QPushButton::clicked, this, [&]()
    {
        if (mRecentFilePaths.size() > 3)
        {
            emit OpenRecentClickedSignal(mRecentFilePaths[3]);
        }
    });

    QObject::connect(mUi->uRecentFileButton_5, &QPushButton::clicked, this, [&]()
    {
        if (mRecentFilePaths.size() > 4)
        {
            emit OpenRecentClickedSignal(mRecentFilePaths[4]);
        }
    });

    mUi->uVersionLabel->setText(tr("Version %0").arg(SW_VERSION_STRING));
}

WelcomeDialog::~WelcomeDialog()
{
    delete mUi;
}

void WelcomeDialog::SetShowOnStartup(bool pShowOnStartup)
{
    mUi->uShowOnStartupCheckbox->setChecked(pShowOnStartup);
}

void WelcomeDialog::SetRecentFilePaths(const std::vector<QFileInfo>& pRecentFilePaths)
{
    mRecentFilePaths = pRecentFilePaths;

    for (uint8_t i = 0; i < 5; i++)
    {
        auto pathLabel = mUi->leftTile->findChild<QLabel*>(QString("uRecentFileLabel_%0").arg(i + 1));

        if (nullptr != pathLabel)
        {
            if (mRecentFilePaths.size() > i)
            {
                pathLabel->setText(mRecentFilePaths[i].absolutePath());
                pathLabel->show();
            }
            else
            {
                pathLabel->hide();
            }
        }

        auto fileButton = mUi->leftTile->findChild<QPushButton*>(QString("uRecentFileButton_%0").arg(i + 1));

        if (nullptr != fileButton)
        {
            if (mRecentFilePaths.size() > i)
            {
                fileButton->setText(mRecentFilePaths[i].baseName());
                fileButton->show();
            }
            else
            {
                fileButton->hide();
            }
        }
    }
}
