#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"

#include "MainWindow.h"
#include "View/View.h"
#include "Configuration.h"

#include <QHBoxLayout>
#include <QVector>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>

MainWindow::MainWindow(QGraphicsScene &pScene):
    mScene(pScene),
    mView(mCoreLogic),
    mCoreLogic(mView)
{
    mView.Scene(mScene);

    auto *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 10, 0, 0);

    setWindowTitle(tr(gui::WINDOW_TITLE));
    setWindowIcon(QIcon(":/images/linkuit_icon3.png"));

    mTitleBarLabel = new QLabel();
    mTitleBarLabel->setObjectName("titleBarLabel");

    mLogoImage = new QImage();
    mLogoImage->load(":/images/linkuit_logo.png");
    mTitleBarLabel->setPixmap(QPixmap::fromImage(*mLogoImage));
    mTitleBarLabel->setStyleSheet("margin-left: 10px");

    layout->addWidget(mTitleBarLabel);

    layout->addWidget(&mView);
    setLayout(layout);

    /*setObjectName("mainWindow");
    setMinimumSize(800, 600);
    setWindowIcon(QIcon(":/images/linkuit_icon3.png"));

    mView.Scene(mScene);

    mCentralWidget = new QWidget();
    mCentralWidget->setObjectName("widgetCentral");

    mTitleBarWidget = new QWidget();
    mTitleBarWidget->setObjectName("titleBar");

    mTitleBarLabel = new QLabel();
    mTitleBarLabel->setObjectName("labelTitle");
    mTitleBarLabel->setText(tr(gui::WINDOW_TITLE));

    mCloseButton = new QPushButton();
    mCloseButton->setObjectName("closeButton");
    mCloseButton->setFixedSize(30, 30);
    mCloseButton->setStyleSheet("border: none; image: url(:/images/icon_window_close.png);");

    mMinimizeButton = new QPushButton();
    mMinimizeButton->setObjectName("minimizeButton");
    mMinimizeButton->setFixedSize(30, 30);
    mMinimizeButton->setStyleSheet("border: none; image: url(:/images/icon_window_minimize.png);");

    mLogoImage = new QImage();
    mLogoImage->load(":/images/linkuit_logo.png");
    mTitleBarLabel->setPixmap(QPixmap::fromImage(*mLogoImage));

    QHBoxLayout* titleBarLayout = new QHBoxLayout();
    titleBarLayout->setContentsMargins(10, 0, 10, 0);
    titleBarLayout->setSpacing(0);
    titleBarLayout->addWidget(mTitleBarLabel);
    titleBarLayout->addStretch(2);
    titleBarLayout->addWidget(mMinimizeButton);
    titleBarLayout->addWidget(mCloseButton);

    mTitleBarWidget->setLayout(titleBarLayout);
    mTitleBarWidget->setFixedHeight(30);

    QGridLayout* gridBody = new QGridLayout();
    gridBody->setSpacing(0);
    gridBody->setContentsMargins(0, 0, 0, 0);
    gridBody->addWidget(&mView);

    QVBoxLayout* centralLayout = new QVBoxLayout();
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->addWidget(mTitleBarWidget);
    centralLayout->addLayout(gridBody);
    mCentralWidget->setLayout(centralLayout);

    setCentralWidget(mCentralWidget);

    setTitlebar({mTitleBarWidget, mTitleBarLabel});
    setResizeable(false);

    connect(mCloseButton, &QPushButton::clicked, this, [&]() {
        close();
    });

    connect(mMinimizeButton, &QPushButton::clicked, this, [&]() {
        showMinimized();
    });

    setStyleSheet("#mainWindow { background-color: rgb(0, 39, 43);} #titleBar { background-color: rgb(255, 255, 255);}");*/
}

View& MainWindow::GetView()
{
    return mView;
}

CoreLogic& MainWindow::GetCoreLogic()
{
    return mCoreLogic;
}
