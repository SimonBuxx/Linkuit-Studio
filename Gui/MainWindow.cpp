#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow),
    mView(mCoreLogic),
    mCoreLogic(mView)
{
    mUi->setupUi(this);

    mAwesome = new QtAwesome(this);
    mAwesome->initFontAwesome();

    mScene.setSceneRect(canvas::DIMENSIONS);
    mView.SetScene(mScene);

    mUi->uViewLayout->addWidget(&mView, 0, 0, 4, 6);

    mView.stackUnder(mUi->uToolboxContainer);

    QObject::connect(&mCoreLogic, &CoreLogic::ControlModeChangedSignal, this, &MainWindow::OnControlModeChanged);
    QObject::connect(&mCoreLogic, &CoreLogic::SimulationModeChangedSignal, this, &MainWindow::OnSimulationModeChanged);

    ConnectGuiSignalsAndSlots();

    InitializeToolboxTree();
    InitializeGuiIcons();
    InitializeGlobalShortcuts();
    InitializeMessageBoxes();

    mUi->uItemRightButton->setChecked(true); // Button for component direction RIGHT

    mUi->uItemConfigurator->hide();
    mUi->uClockConfigurator->hide();

    mFadeOutOnCtrlTimer.setSingleShot(true);

    mAboutDialog.setAttribute(Qt::WA_QuitOnClose, false);   // Make about dialog close when main window closes
    mWelcomeDialog.setAttribute(Qt::WA_QuitOnClose, false); // Make welcome dialog close when main window closes

    ConfigureWelcomeDialog(mCoreLogic.GetRuntimeConfigParser().IsWelcomeDialogEnabledOnStartup(), mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
    SetRecentFileMenuActions(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::InitializeMessageBoxes()
{
    mSaveChangesBox.setIcon(QMessageBox::Icon::Question);
    mSaveChangesBox.setWindowTitle("Linkuit Studio");
    mSaveChangesBox.setWindowIcon(QIcon(":/images/icons/icon_default.png"));
    mSaveChangesBox.setText(tr("There are unsaved changes to this ciruit."));
    mSaveChangesBox.setInformativeText(tr("Would you like to save these changes?"));
    mSaveChangesBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    mSaveChangesBox.setDefaultButton(QMessageBox::Save);

    mErrorOpenFileBox.setIcon(QMessageBox::Icon::Critical);
    mErrorOpenFileBox.setWindowTitle("Linkuit Studio");
    mErrorOpenFileBox.setWindowIcon(QIcon(":/images/icons/icon_default.png"));
    mErrorOpenFileBox.setText(tr("The file could not be opened."));
    mErrorOpenFileBox.setInformativeText("Please make sure that the file exists.");
    mErrorOpenFileBox.setStandardButtons(QMessageBox::Ok);
    mErrorOpenFileBox.setDefaultButton(QMessageBox::Ok);

    mErrorSaveFileBox.setIcon(QMessageBox::Icon::Critical);
    mErrorSaveFileBox.setWindowTitle("Linkuit Studio");
    mErrorSaveFileBox.setWindowIcon(QIcon(":/images/icons/icon_default.png"));
    mErrorSaveFileBox.setText(tr("The circuit could not be saved."));
    mErrorSaveFileBox.setStandardButtons(QMessageBox::Ok);
    mErrorSaveFileBox.setDefaultButton(QMessageBox::Ok);
}

void MainWindow::ConnectGuiSignalsAndSlots()
{
    QObject::connect(&mView, &View::ZoomLevelChangedSignal, this, &MainWindow::UpdateZoomLabelAndSlider);

    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::LoadCircuitFileSuccessSignal, this, &MainWindow::OnCircuitFileOpenedSuccessfully);
    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::LoadCircuitFileFailedSignal, this, &MainWindow::OnCircuitFileOpeningFailed);

    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::SaveCircuitFileSuccessSignal, this, &MainWindow::OnCircuitFileSavedSuccessfully);
    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::SaveCircuitFileFailedSignal, this, &MainWindow::OnCircuitFileSavingFailed);

    // Connect to core logic signals

    QObject::connect(&mCoreLogic, &CoreLogic::ShowClockConfiguratorSignal, this, &MainWindow::ShowClockConfigurator);
    QObject::connect(&mCoreLogic, &CoreLogic::HideClockConfiguratorSignal, this, [&]()
    {
        FadeOutWidget(mUi->uClockConfigurator);
    });
    QObject::connect(&mCoreLogic, &CoreLogic::ControlModeChangedSignal, this, [&](ControlMode pNewMode)
    {
        mUi->uLabelStatus->setText(tr("Ready."));

        if (pNewMode != ControlMode::ADD)
        {
            FadeOutWidget(mUi->uItemConfigurator);
            mIsItemConfiguratorVisible = false;
        }
        if (pNewMode == ControlMode::SIMULATION)
        {
            mIsToolboxVisible = false;
            mIsClockConfiguratorVisible = false;
            mUi->uLabelStatus->setText(tr("Loading..."));
        }
    });

    QObject::connect(&mCoreLogic, &CoreLogic::ComponentTypeChangedSignal, this, [&](ComponentType pNewType)
    {
        ShowItemConfigurator(GetConfiguratorModeForComponentType(pNewType));
    });

    QObject::connect(&mCoreLogic, &CoreLogic::UpdateUndoRedoEnabledSignal, this, [this]()
    {
        UpdateUndoRedoEnabled(true);
    });

    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::CircuitModifiedSignal, this, [&]()
    {
        if (mCoreLogic.GetCircuitFileParser().IsFileOpen())
        {
            setWindowTitle(tr("Linkuit Studio - %0%1").arg(mCoreLogic.GetCircuitFileParser().GetFileInfo().value().fileName(), "*"));
        }
        else
        {
            setWindowTitle(tr("Linkuit Studio - Untitled*"));
        }
    });

    QObject::connect(&mCoreLogic, &CoreLogic::ProcessingStartedSignal, this, [&]()
    {
        mUi->menuBar->setEnabled(false);
        mUi->uZoomSlider->setEnabled(false);
        mUi->uLabelStatus->setText("Loading...");
        FadeOutGui();
    });

    QObject::connect(&mCoreLogic, &CoreLogic::ProcessingEndedSignal, this, [&]()
    {
        mUi->menuBar->setEnabled(true);
        mUi->uZoomSlider->setEnabled(true);
        if (mCoreLogic.GetControlMode() == ControlMode::SIMULATION)
        {
            if (mCoreLogic.GetSimulationMode() == SimulationMode::RUNNING)
            {
                mUi->uLabelStatus->setText(tr("Simulation running..."));
            }
            else
            {
                mUi->uLabelStatus->setText(tr("Simulation paused."));
            }
        }
        else
        {
            mUi->uLabelStatus->setText(tr("Ready."));
        }
        FadeInGui();
    });

    QObject::connect(&mFadeOutOnCtrlTimer, &QTimer::timeout, this, &MainWindow::FadeOutGui);

    QObject::connect(mUi->uZoomSlider, &QSlider::valueChanged, &mView, &View::SetZoom);

    QObject::connect(mUi->uEditButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    });

    QObject::connect(mUi->uWiringButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::WIRE);
    });

    // Connect widgets from item configuration GUI

    QObject::connect(mUi->uItemRightButton, &QPushButton::toggled, this, &MainWindow::OnItemRightButtonToggled);
    QObject::connect(mUi->uItemDownButton, &QPushButton::toggled, this, &MainWindow::OnItemDownButtonToggled);
    QObject::connect(mUi->uItemLeftButton, &QPushButton::toggled, this, &MainWindow::OnItemLeftButtonToggled);
    QObject::connect(mUi->uItemUpButton, &QPushButton::toggled, this, &MainWindow::OnItemUpButtonToggled);

    QObject::connect(mUi->uGateInputCountSlider, &QSlider::valueChanged, this, &MainWindow::OnGateInputCountSliderValueChanged);
    QObject::connect(mUi->uEncoderDecoderInputCountSlider, &QSlider::valueChanged, this, &MainWindow::OnEncoderDecoderInputCountSliderValueChanged);
    QObject::connect(mUi->uBitWidthSlider, &QSlider::valueChanged, this, &MainWindow::OnBitWidthSliderValueChanged);

    // Connect widgets from clock configuration GUI

    QObject::connect(mUi->uButtonToggle, &QPushButton::toggled, this, &MainWindow::OnToggleButtonToggled);
    QObject::connect(mUi->uToggleSlider, &QSlider::valueChanged, this, &MainWindow::OnToggleSliderValueChanged);
    QObject::connect(mUi->uPulseSlider, &QSlider::valueChanged, this, &MainWindow::OnPulseSliderValueChanged);

    QObject::connect(mUi->uDeleteButton, &QAbstractButton::clicked, mUi->uActionDelete, &QAction::trigger);
    QObject::connect(mUi->uUndoButton, &QAbstractButton::clicked, mUi->uActionUndo, &QAction::trigger);
    QObject::connect(mUi->uRedoButton, &QAbstractButton::clicked, mUi->uActionRedo, &QAction::trigger);
    QObject::connect(mUi->uStartButton, &QAbstractButton::clicked, mUi->uActionStart, &QAction::trigger);
    QObject::connect(mUi->uRunButton, &QAbstractButton::clicked, mUi->uActionRun, &QAction::trigger);
    QObject::connect(mUi->uStepButton, &QAbstractButton::clicked, mUi->uActionStep, &QAction::trigger);
    QObject::connect(mUi->uResetButton, &QAbstractButton::clicked, mUi->uActionReset, &QAction::trigger);
    QObject::connect(mUi->uPauseButton, &QAbstractButton::clicked, mUi->uActionPause, &QAction::trigger);

    QObject::connect(&mWelcomeDialog, &WelcomeDialog::NewCircuitClickedSignal, this, [&]()
    {
        mUi->uActionNew->trigger();
        mWelcomeDialog.close();
    });

    QObject::connect(&mWelcomeDialog, &WelcomeDialog::OpenCircuitClickedSignal, this, [&]()
    {
        mUi->uActionOpen->trigger();
        mWelcomeDialog.close();
    });

    QObject::connect(&mWelcomeDialog, &WelcomeDialog::OpenRecentClickedSignal, this, &MainWindow::OpenRecentFile);

    QObject::connect(mUi->uActionRecentFile1, &QAction::triggered, this, [&]()
    {
        if (mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths().size() > 0)
        {
            OpenRecentFile(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths()[0]);
        }
    });

    QObject::connect(mUi->uActionRecentFile2, &QAction::triggered, this, [&]()
    {
        if (mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths().size() > 1)
        {
            OpenRecentFile(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths()[1]);
        }
    });

    QObject::connect(mUi->uActionRecentFile3, &QAction::triggered, this, [&]()
    {
        if (mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths().size() > 2)
        {
            OpenRecentFile(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths()[2]);
        }
    });

    QObject::connect(mUi->uActionRecentFile4, &QAction::triggered, this, [&]()
    {
        if (mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths().size() > 3)
        {
            OpenRecentFile(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths()[3]);
        }
    });

    QObject::connect(mUi->uActionRecentFile5, &QAction::triggered, this, [&]()
    {
        if (mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths().size() > 4)
        {
            OpenRecentFile(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths()[4]);
        }
    });

    QObject::connect(&mWelcomeDialog, &WelcomeDialog::OpenTwitterClickedSignal, mUi->uActionOpenTwitter, &QAction::trigger);
    QObject::connect(&mWelcomeDialog, &WelcomeDialog::OpenGithubClickedSignal, mUi->uActionOpenGithub, &QAction::trigger);
    QObject::connect(&mWelcomeDialog, &WelcomeDialog::CheckForUpdateClickedSignal, mUi->uActionCheckUpdate, &QAction::trigger);

    QObject::connect(&mWelcomeDialog, &WelcomeDialog::ShowOnStartupToggledSignal, this, [&](bool pChecked)
    {
        mCoreLogic.SetShowWelcomeDialogOnStartup(pChecked);
    });

    QObject::connect(&mAboutDialog, &AboutDialog::CheckForUpdateClickedSignal, mUi->uActionCheckUpdate, &QAction::trigger);

    QObject::connect(mUi->uActionStart, &QAction::triggered, this, [&]()
    {
        if (mCoreLogic.IsProcessing())
        {
            return;
        }

        if (!mCoreLogic.IsSimulationRunning())
        {
            EnterSimulation();
            mUi->uStartButton->setToolTip(tr("Stop [Alt+Return]"));
            mUi->uStartButton->setChecked(true);
            mUi->uActionStart->setText(tr("Stop"));
        }
        else
        {
            StopSimulation();
            mUi->uStartButton->setToolTip(tr("Start [Alt+Return]"));
            mUi->uStartButton->setChecked(false);
            mUi->uActionStart->setText(tr("Start"));
        }
    });
    QObject::connect(mUi->uActionRun, &QAction::triggered, this, &MainWindow::RunSimulation);
    QObject::connect(mUi->uActionStep, &QAction::triggered, this, &MainWindow::StepSimulation);
    QObject::connect(mUi->uActionReset, &QAction::triggered, this, &MainWindow::ResetSimulation);
    QObject::connect(mUi->uActionPause, &QAction::triggered, this, &MainWindow::PauseSimulation);
    QObject::connect(mUi->uActionAbout, &QAction::triggered, &mAboutDialog, &AboutDialog::show);
    QObject::connect(mUi->uActionClose, &QAction::triggered, this, &MainWindow::close);

    QObject::connect(mUi->uActionNew, &QAction::triggered, this, [&]()
    {
        mFadeOutOnCtrlTimer.stop();

        if (!IsSaveChangesIfModifiedCanceled())
        {
            mCoreLogic.NewCircuit();
            setWindowTitle(tr(gui::DEFAULT_WINDOW_TITLE));

            mIsToolboxVisible = true;
            FadeInGui();
        }
    });

    QObject::connect(mUi->uActionOpen, &QAction::triggered, this, [&]()
    {
        mFadeOutOnCtrlTimer.stop();

        if (!IsSaveChangesIfModifiedCanceled())
        {
            QString path = mCoreLogic.GetCircuitFileParser().IsFileOpen() ? mCoreLogic.GetCircuitFileParser().GetFileInfo().value().absolutePath() : QDir::homePath();
            const auto fileInfo = QFileInfo(QFileDialog::getOpenFileName(this, tr(gui::OPEN_FILE_DIALOG_TITLE), path, tr("Linkuit Studio Circuit Files (*.lsc)")));

            if (!fileInfo.absoluteFilePath().isEmpty())
            {
                mCoreLogic.GetCircuitFileParser().LoadJson(fileInfo);
            }
        }
    });

    QObject::connect(mUi->uActionSave, &QAction::triggered, this, [&]()
    {
        if (mCoreLogic.GetCircuitFileParser().IsFileOpen())
        {
            mCoreLogic.GetCircuitFileParser().SaveJson(mCoreLogic.GetJson());
        }
        else
        {
            mUi->uActionSaveAs->trigger();
        }
    });

    QObject::connect(mUi->uActionSaveAs, &QAction::triggered, this, [&]()
    {
        mFadeOutOnCtrlTimer.stop();
        FadeInGui();

        QString path = mCoreLogic.GetCircuitFileParser().IsFileOpen() ? mCoreLogic.GetCircuitFileParser().GetFileInfo().value().absolutePath() : QDir::homePath();
        const auto fileInfo = QFileInfo(QFileDialog::getSaveFileName(this, tr(gui::SAVE_FILE_DIALOG_TITLE), path, tr("Linkuit Studio Circuit Files (*.lsc)")));

        if (fileInfo.absoluteFilePath() != "")
        {
            mCoreLogic.GetCircuitFileParser().SaveJsonAs(fileInfo, mCoreLogic.GetJson());
        }
    });

    QObject::connect(mUi->uActionUndo, &QAction::triggered, this, &MainWindow::Undo);
    QObject::connect(mUi->uActionRedo, &QAction::triggered, this, &MainWindow::Redo);

    QObject::connect(mUi->uActionCopy, &QAction::triggered, &mCoreLogic, &CoreLogic::CopySelectedComponents);
    QObject::connect(mUi->uActionPaste, &QAction::triggered, &mCoreLogic, &CoreLogic::PasteCopiedComponents);
    QObject::connect(mUi->uActionCut, &QAction::triggered, this, [&]()
    {
        if (!mCoreLogic.IsSimulationRunning() && !(QApplication::mouseButtons() & Qt::MouseButton::LeftButton))
        {
            if (mCoreLogic.GetControlMode() != ControlMode::COPY)
            {
                mCoreLogic.CutSelectedComponents();
            }
            else
            {
                mCoreLogic.AbortPasting();
            }
        }
    });

    QObject::connect(mUi->uActionDelete, &QAction::triggered, this, [&]()
    {
        if (!mCoreLogic.IsSimulationRunning() && !(QApplication::mouseButtons() & Qt::MouseButton::LeftButton))
        {
            if (mCoreLogic.GetControlMode() != ControlMode::COPY)
            {
                mCoreLogic.DeleteSelectedComponents();
            }
            else
            {
                mCoreLogic.AbortPasting();
            }
        }
    });

    QObject::connect(mUi->uActionSelectAll, &QAction::triggered, &mCoreLogic, &CoreLogic::SelectAll);

    QObject::connect(mUi->uActionScreenshot, &QAction::triggered, this, [&]()
    {
        qDebug() << "Not implemented";
    });

    QObject::connect(mUi->uActionStartTutorial, &QAction::triggered, this, [&]()
    {
        qDebug() << "Not implemented";
    });

    QObject::connect(mUi->uActionWelcomePage, &QAction::triggered, this, [&]()
    {
        ConfigureWelcomeDialog(mCoreLogic.GetRuntimeConfigParser().IsWelcomeDialogEnabledOnStartup(), mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
        SetRecentFileMenuActions(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
        mWelcomeDialog.show();
    });

    QObject::connect(mUi->uActionReportBugs, &QAction::triggered, this, [&]()
    {
#warning change to final bug report link
        QDesktopServices::openUrl(QUrl("https://linkuit.com/bugreport/"));
    });

    QObject::connect(mUi->uActionOpenWebsite, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl("https://linkuit.com/"));
    });

    QObject::connect(mUi->uActionOpenTwitter, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl("https://twitter.com/goLinkuit/"));
    });

    QObject::connect(mUi->uActionOpenGithub, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl("https://github.com/SimonBuxx/Linkuit-Studio/"));
    });

    QObject::connect(mUi->uActionCheckUpdate, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl(QString("https://linkuit.com/update/%0").arg(SW_VERSION_STRING)));
    });
}

bool MainWindow::IsSaveChangesIfModifiedCanceled()
{
    if (mCoreLogic.GetCircuitFileParser().IsCircuitModified())
    {
        int ret = mSaveChangesBox.exec();

        switch (ret) {
            case QMessageBox::Save:
            {
                mUi->uActionSave->trigger();
                break;
            }
            case QMessageBox::Discard:
            {
                break;
            }
            case QMessageBox::Cancel:
            {
                return true;
            }
            default:
            {
                break;
            }
        }
    }

    return false;
}

void MainWindow::OnCircuitFileOpenedSuccessfully(const QFileInfo& pFileInfo)
{
    ConfigureWelcomeDialog(mCoreLogic.GetRuntimeConfigParser().IsWelcomeDialogEnabledOnStartup(), mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
    SetRecentFileMenuActions(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
    setWindowTitle(QString("Linkuit Studio - %0").arg(pFileInfo.fileName()));
    mWelcomeDialog.close();
    mIsToolboxVisible = true;
    FadeInGui();
}

void MainWindow::OnCircuitFileOpeningFailed(const QFileInfo& pFileInfo)
{
#warning [ENHANCEMENT] remove file from recent files list when opening failed
    mErrorOpenFileBox.setText(tr("The file %0 could not be opened.").arg(pFileInfo.fileName()));
    mErrorOpenFileBox.exec();
}

void MainWindow::OnCircuitFileSavedSuccessfully(const QFileInfo& pFileInfo)
{
    setWindowTitle(tr("Linkuit Studio - %0").arg(pFileInfo.fileName()));
    ConfigureWelcomeDialog(mCoreLogic.GetRuntimeConfigParser().IsWelcomeDialogEnabledOnStartup(), mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
    SetRecentFileMenuActions(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
}

void MainWindow::OnCircuitFileSavingFailed()
{
    mErrorSaveFileBox.exec();
}

void MainWindow::OpenRecentFile(const QFileInfo& pFileInfo)
{
    if (!IsSaveChangesIfModifiedCanceled())
    {
        mCoreLogic.GetCircuitFileParser().LoadJson(pFileInfo);
    }
}

void MainWindow::ConfigureWelcomeDialog(bool pShowOnStartup, const std::vector<QFileInfo>& pRecentFilePaths)
{
    mWelcomeDialog.SetShowOnStartup(pShowOnStartup);
    mWelcomeDialog.SetRecentFilePaths(pRecentFilePaths);
}

void MainWindow::SetRecentFileMenuActions(const std::vector<QFileInfo>& pRecentFilePaths)
{
    // Workaround because findChild<QAction*>() does not work for some reason
    mUi->uActionRecentFile1->setVisible(false);
    mUi->uActionRecentFile2->setVisible(false);
    mUi->uActionRecentFile3->setVisible(false);
    mUi->uActionRecentFile4->setVisible(false);
    mUi->uActionRecentFile5->setVisible(false);

    if (pRecentFilePaths.size() > 0)
    {
        mUi->uActionRecentFile1->setText(pRecentFilePaths[0].absoluteFilePath());
        mUi->uActionRecentFile1->setVisible(true);
    }
    if (pRecentFilePaths.size() > 1)
    {
        mUi->uActionRecentFile2->setText(pRecentFilePaths[1].absoluteFilePath());
        mUi->uActionRecentFile2->setVisible(true);
    }
    if (pRecentFilePaths.size() > 2)
    {
        mUi->uActionRecentFile3->setText(pRecentFilePaths[2].absoluteFilePath());
        mUi->uActionRecentFile3->setVisible(true);
    }
    if (pRecentFilePaths.size() > 3)
    {
        mUi->uActionRecentFile4->setText(pRecentFilePaths[3].absoluteFilePath());
        mUi->uActionRecentFile4->setVisible(true);
    }
    if (pRecentFilePaths.size() > 4)
    {
        mUi->uActionRecentFile5->setText(pRecentFilePaths[4].absoluteFilePath());
        mUi->uActionRecentFile5->setVisible(true);
    }
}

void MainWindow::ShowWelcomeDialog(std::chrono::milliseconds pDelay)
{
    QTimer::singleShot(pDelay, &mWelcomeDialog, &WelcomeDialog::show);
}

void MainWindow::UpdateZoomLabelAndSlider(uint8_t pPercentage, uint32_t pValue)
{
    mUi->uZoomLabel->setText(QString("%0%").arg(pPercentage));
    mUi->uZoomSlider->setValue(pValue);
}

void MainWindow::OnToggleButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        // Set to toggle
        mUi->uPulseFrame->hide();
        mCoreLogic.OnClockModeChanged(ClockMode::TOGGLE);
    }
    else
    {
        // Set to pulse
        mUi->uPulseFrame->show();
        mCoreLogic.OnClockModeChanged(ClockMode::PULSE);
    }
}

void MainWindow::OnToggleSliderValueChanged(int32_t pValue)
{
    mUi->uLabelToggle->setText(tr(pValue > 1 ? "%0 Ticks / Toggle" : "%0 Tick / Toggle").arg(pValue));

    mUi->uPulseSlider->setMaximum(pValue);

    mCoreLogic.OnToggleValueChanged(pValue);
    mCoreLogic.OnPulseValueChanged(mUi->uPulseSlider->value());
}

void MainWindow::OnPulseSliderValueChanged(int32_t pValue)
{
    mUi->uLabelPulse->setText(tr(pValue > 1 ? "%0 Ticks / Pulse" : "%0 Tick / Pulse").arg(pValue));

    mCoreLogic.OnPulseValueChanged(pValue);
}

void MainWindow::ShowClockConfigurator(ClockMode pMode, uint32_t pToggle, uint32_t pPulse)
{
    if (!mIsGuiHidden)
    {
        FadeInWidget(mUi->uClockConfigurator);
    }
    else
    {
        mIsClockConfiguratorVisible = true;
    }

    if (pMode == ClockMode::TOGGLE)
    {
        mUi->uPulseFrame->hide();
        mUi->uButtonToggle->setChecked(true);
    }
    else
    {
        mUi->uPulseFrame->show();
        mUi->uButtonPulse->setChecked(true);
    }

    mUi->uToggleSlider->setValue(pToggle);
    mUi->uPulseSlider->setValue(pPulse);
}

void MainWindow::ShowItemConfigurator(ConfiguratorMode pMode)
{
    switch (pMode)
    {
        case ConfiguratorMode::NO_CONFIGURATION:
        {
            FadeOutWidget(mUi->uItemConfigurator);
            return;
        }
        case ConfiguratorMode::DIRECTION_ONLY:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uBitWidthFrame->hide();
            break;
        }
        case ConfiguratorMode::DIRECTION_AND_INPUT_COUNT:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->show();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uBitWidthFrame->hide();
            break;
        }
        case ConfiguratorMode::MULTIPLEXER_BITS:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uBitWidthFrame->show();
            break;
        }
        case ConfiguratorMode::ENCODER_DECODER:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->show();
            mUi->uBitWidthFrame->hide();
            SetEncoderDecoderInputCountIfAllowed(mUi->uEncoderDecoderInputCountSlider->value());
            break;
        }
        default:
        {
            throw std::logic_error("ConfiguratorMode unimplemented or invalid");
        }
    }

    if (!mIsGuiHidden)
    {
        FadeInWidget(mUi->uItemConfigurator);
    }
    else
    {
        mIsItemConfiguratorVisible = true;
    }

    /*auto mousePos = QWidget::mapFromGlobal(QCursor::pos());
    mUi->uItemConfigContainer->move(mUi->uItemConfigContainer->x(), mousePos.y() - mUi->menuBar->height() - mUi->uItemConfigContainer->height() / 2);*/
}

void MainWindow::OnItemRightButtonToggled(bool pChecked)
{
    mUi->uItemRightButton->setIcon(mAwesome->icon(fa::arrowright, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::RIGHT);
    }
}

void MainWindow::OnItemDownButtonToggled(bool pChecked)
{
    mUi->uItemDownButton->setIcon(mAwesome->icon(fa::arrowdown, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::DOWN);
    }
}

void MainWindow::OnItemLeftButtonToggled(bool pChecked)
{
    mUi->uItemLeftButton->setIcon(mAwesome->icon(fa::arrowleft, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::LEFT);
    }
}

void MainWindow::OnItemUpButtonToggled(bool pChecked)
{
    mUi->uItemUpButton->setIcon(mAwesome->icon(fa::arrowup, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::UP);
    }
}

void MainWindow::OnGateInputCountSliderValueChanged(int32_t pValue)
{
    SetGateInputCountIfAllowed(pValue);
}

void MainWindow::OnEncoderDecoderInputCountSliderValueChanged(int32_t pValue)
{
    SetEncoderDecoderInputCountIfAllowed(pValue);
}

void MainWindow::OnBitWidthSliderValueChanged(int32_t pValue)
{
    SetMultiplexerBitWidthIfAllowed(pValue);
}

void MainWindow::EnterSimulation()
{
    if (!mCoreLogic.IsSimulationRunning())
    {
        mCoreLogic.EnterControlMode(ControlMode::SIMULATION);
        FadeOutWidget(mUi->uToolboxContainer);
    }
}

void MainWindow::RunSimulation()
{
    mCoreLogic.RunSimulation();
}

void MainWindow::StepSimulation()
{
    mCoreLogic.StepSimulation();
}

void MainWindow::ResetSimulation()
{
    mCoreLogic.ResetSimulation();
}

void MainWindow::PauseSimulation()
{
    mCoreLogic.PauseSimulation();
}

void MainWindow::StopSimulation()
{
    if (mCoreLogic.IsSimulationRunning())
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
        if (!mIsGuiHidden)
        {
            FadeInWidget(mUi->uToolboxContainer);
        }
        else
        {
            mIsToolboxVisible = true;
        }
    }
}

void MainWindow::UpdateUndoRedoEnabled(bool pEnable)
{
    if (pEnable)
    {
        mUi->uActionUndo->setEnabled(!mCoreLogic.IsUndoQueueEmpty());
        mUi->uActionRedo->setEnabled(!mCoreLogic.IsRedoQueueEmpty());
        mUi->uUndoButton->setEnabled(!mCoreLogic.IsUndoQueueEmpty());
        mUi->uRedoButton->setEnabled(!mCoreLogic.IsRedoQueueEmpty());
    }
    else
    {
        mUi->uActionUndo->setEnabled(false);
        mUi->uActionRedo->setEnabled(false);
        mUi->uUndoButton->setEnabled(false);
        mUi->uRedoButton->setEnabled(false);
    }
}

void MainWindow::Undo()
{
    if (mCoreLogic.GetControlMode() == ControlMode::COPY)
    {
        mCoreLogic.AbortPasting();
    }

    mCoreLogic.Undo();
    UpdateUndoRedoEnabled(true);
}

void MainWindow::Redo()
{
    if (mCoreLogic.GetControlMode() == ControlMode::COPY)
    {
        mCoreLogic.AbortPasting();
    }

    mCoreLogic.Redo();
    UpdateUndoRedoEnabled(true);
}

void MainWindow::OnControlModeChanged(ControlMode pNewMode)
{
    switch (pNewMode)
    {
        case ControlMode::EDIT:
        case ControlMode::COPY:
        {
            mUi->uToolboxTree->clearSelection();

            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uDeleteButton->setEnabled(true);
            mUi->uStartButton->setEnabled(true);
            mUi->uRunButton->setEnabled(false);
            mUi->uStepButton->setEnabled(false);
            mUi->uResetButton->setEnabled(false);
            mUi->uPauseButton->setEnabled(false);

            UpdateUndoRedoEnabled(true);

            mUi->uActionNew->setEnabled(true);
            mUi->uActionOpen->setEnabled(true);

            mUi->uActionCut->setEnabled(true);
            mUi->uActionCopy->setEnabled(true);
            mUi->uActionPaste->setEnabled(true);
            mUi->uActionDelete->setEnabled(true);
            mUi->uActionSelectAll->setEnabled(true);

            mUi->uActionStart->setEnabled(true);
            mUi->uActionRun->setEnabled(false);
            mUi->uActionReset->setEnabled(false);
            mUi->uActionStep->setEnabled(false);
            mUi->uActionPause->setEnabled(false);

            mUi->uEditButton->setChecked(true);
            ForceUncheck(mUi->uRunButton);
            ForceUncheck(mUi->uWiringButton);
            ForceUncheck(mUi->uPauseButton);
            mUi->uStartButton->setChecked(false);
            break;
        }
        case ControlMode::WIRE:
        {
            mUi->uToolboxTree->clearSelection();

            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uDeleteButton->setEnabled(true);
            mUi->uStartButton->setEnabled(true);
            mUi->uRunButton->setEnabled(false);
            mUi->uStepButton->setEnabled(false);
            mUi->uResetButton->setEnabled(false);
            mUi->uPauseButton->setEnabled(false);

            UpdateUndoRedoEnabled(true);

            mUi->uActionNew->setEnabled(true);
            mUi->uActionOpen->setEnabled(true);

            mUi->uActionCut->setEnabled(true);
            mUi->uActionCopy->setEnabled(true);
            mUi->uActionPaste->setEnabled(true);
            mUi->uActionDelete->setEnabled(true);
            mUi->uActionSelectAll->setEnabled(true);

            mUi->uActionStart->setEnabled(true);
            mUi->uActionRun->setEnabled(false);
            mUi->uActionReset->setEnabled(false);
            mUi->uActionStep->setEnabled(false);
            mUi->uActionPause->setEnabled(false);

            ForceUncheck(mUi->uEditButton);
            mUi->uWiringButton->setChecked(true);
            ForceUncheck(mUi->uRunButton);
            ForceUncheck(mUi->uPauseButton);
            mUi->uStartButton->setChecked(false);
            break;
        }
        case ControlMode::ADD:
        {
            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uDeleteButton->setEnabled(true);
            mUi->uStartButton->setEnabled(true);
            mUi->uRunButton->setEnabled(false);
            mUi->uStepButton->setEnabled(false);
            mUi->uResetButton->setEnabled(false);
            mUi->uPauseButton->setEnabled(false);

            UpdateUndoRedoEnabled(true);

            mUi->uActionNew->setEnabled(true);
            mUi->uActionOpen->setEnabled(true);

            mUi->uActionCut->setEnabled(true);
            mUi->uActionCopy->setEnabled(true);
            mUi->uActionPaste->setEnabled(true);
            mUi->uActionDelete->setEnabled(true);
            mUi->uActionSelectAll->setEnabled(true);

            mUi->uActionStart->setEnabled(true);
            mUi->uActionRun->setEnabled(false);
            mUi->uActionReset->setEnabled(false);
            mUi->uActionStep->setEnabled(false);
            mUi->uActionPause->setEnabled(false);

            ForceUncheck(mUi->uEditButton);
            ForceUncheck(mUi->uWiringButton);
            ForceUncheck(mUi->uRunButton);
            ForceUncheck(mUi->uPauseButton);
            mUi->uStartButton->setChecked(false);
            break;
        }
        case ControlMode::SIMULATION:
        {
            // Note: Toolbox is never disabled, only hidden
            mUi->uToolboxTree->clearSelection();

            mUi->uEditButton->setEnabled(false);
            mUi->uWiringButton->setEnabled(false);
            mUi->uDeleteButton->setEnabled(false);
            mUi->uStartButton->setEnabled(true);
            mUi->uRunButton->setEnabled(true);
            mUi->uStepButton->setEnabled(true);
            mUi->uResetButton->setEnabled(true);
            mUi->uPauseButton->setEnabled(true);

            UpdateUndoRedoEnabled(false);

            mUi->uActionNew->setEnabled(true);
            mUi->uActionOpen->setEnabled(true);

            mUi->uActionCut->setEnabled(false);
            mUi->uActionCopy->setEnabled(false);
            mUi->uActionPaste->setEnabled(false);
            mUi->uActionDelete->setEnabled(false);
            mUi->uActionSelectAll->setEnabled(false);

            mUi->uActionStart->setEnabled(true);
            mUi->uActionRun->setEnabled(true);
            mUi->uActionReset->setEnabled(true);
            mUi->uActionStep->setEnabled(true);
            mUi->uActionPause->setEnabled(false);

            ForceUncheck(mUi->uEditButton);
            ForceUncheck(mUi->uWiringButton);
            ForceUncheck(mUi->uRunButton);
            mUi->uPauseButton->setChecked(true);
            mUi->uStartButton->setChecked(true);
            break;
        }
        default:
        {
            break;
        }
    }

    mScene.clearSelection();
}

void MainWindow::OnSimulationModeChanged(SimulationMode pNewMode)
{
    switch (pNewMode)
    {
        case SimulationMode::STOPPED:
        {
            mUi->uPauseButton->setChecked(true);
            mUi->uStepButton->setEnabled(true);

            mUi->uActionRun->setEnabled(true);
            mUi->uActionPause->setEnabled(false);
            mUi->uActionStep->setEnabled(true);

            mUi->uLabelStatus->setText(tr("Simulation paused."));
            break;
        }
        case SimulationMode::RUNNING:
        {
            mUi->uRunButton->setChecked(true);
            mUi->uStepButton->setEnabled(false);

            mUi->uActionRun->setEnabled(false);
            mUi->uActionPause->setEnabled(true);
            mUi->uActionStep->setEnabled(false);
            mUi->uLabelStatus->setText(tr("Simulation running..."));
            break;
        }
        default:
        {
            break;
        }
    }
}

void MainWindow::FadeOutGui()
{
    mIsToolboxVisible = mUi->uToolboxContainer->isVisible();
    mIsClockConfiguratorVisible = mUi->uClockConfigurator->isVisible();
    mIsItemConfiguratorVisible = mUi->uItemConfigurator->isVisible();

    FadeOutWidget(mUi->uTopBar);
    FadeOutWidget(mUi->uToolboxContainer);
    FadeOutWidget(mUi->uClockConfigurator);
    FadeOutWidget(mUi->uItemConfigurator);
    mIsGuiHidden = true;
}

void MainWindow::FadeInGui()
{
    if (mCoreLogic.IsProcessing())
    {
        return;
    }

    FadeInWidget(mUi->uTopBar);
    if (mIsToolboxVisible && mCoreLogic.GetControlMode() != ControlMode::SIMULATION) // Keep GUI hidden if simulation started
    {
        FadeInWidget(mUi->uToolboxContainer);
    }
    if (mIsClockConfiguratorVisible && mCoreLogic.GetControlMode() != ControlMode::SIMULATION)
    {
        FadeInWidget(mUi->uClockConfigurator);
    }
    if (mIsItemConfiguratorVisible && mCoreLogic.GetControlMode() != ControlMode::SIMULATION)
    {
        FadeInWidget(mUi->uItemConfigurator);
    }

    mIsGuiHidden = false;
}

void MainWindow::ForceUncheck(IconToolButton *pButton)
{
    if (nullptr != pButton->group() && pButton->group()->exclusive())
    {
        pButton->group()->setExclusive(false);
        pButton->setChecked(false);
        pButton->group()->setExclusive(true);
    }
    else
    {
        pButton->setChecked(false);
    }
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    if (mCoreLogic.GetCircuitFileParser().IsCircuitModified())
    {
        int ret = mSaveChangesBox.exec();

        switch (ret) {
            case QMessageBox::Save:
            {
                mUi->uActionSave->trigger();
                pEvent->accept();
                break;
            }
            case QMessageBox::Discard:
            {
                pEvent->accept();
                break;
            }
            case QMessageBox::Cancel:
            {
                pEvent->ignore();
                break;
            }
            default:
            {
                pEvent->accept();
                break;
            }
        }
    }
    else
    {
        pEvent->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *pEvent)
{
    if (pEvent->key() == Qt::Key_Control && gui::FADE_OUT_GUI_ON_CTRL)
    {
        mFadeOutOnCtrlTimer.start(gui::FADE_OUT_GUI_TIMEOUT);
    }

    QMainWindow::keyPressEvent(pEvent);
}

void MainWindow::keyReleaseEvent(QKeyEvent *pEvent)
{
    if (pEvent->key() == Qt::Key_Control && gui::FADE_OUT_GUI_ON_CTRL)
    {
        mFadeOutOnCtrlTimer.stop();

        FadeInGui();
    }

    QMainWindow::keyReleaseEvent(pEvent);
}

void MainWindow::InitializeToolboxTree()
{
    mChevronIconVariant.insert("color", QColor(0, 45, 50));
    mChevronIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mChevronIconVariant.insert("color-active", QColor(0, 45, 50));
    mChevronIconVariant.insert("color-selected", QColor(0, 45, 50));

    QObject::connect(mUi->uToolboxTree, &QTreeView::pressed, this, &MainWindow::OnToolboxTreeClicked);

    // Tracks the currently selected item when it is changed by dragging
    QObject::connect(mUi->uToolboxTree, &QTreeView::entered, this, [&](const QModelIndex &pIndex)
    {
        if ((QGuiApplication::mouseButtons() == Qt::LeftButton) && (mUi->uToolboxTree->currentIndex().row() >= 0))
        {
            if (!mToolboxTreeModel.itemFromIndex(pIndex)->isSelectable())
            {
                mUi->uToolboxTree->clearSelection();
            }
            OnToolboxTreeClicked(pIndex);
        }
    });

    // Expand/collapse on single click
    QObject::connect(mUi->uToolboxTree, &QTreeView::clicked, [this]()
    {
        if (mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->hasChildren())
        {
            if (mUi->uToolboxTree->isExpanded(mUi->uToolboxTree->currentIndex()))
            {
                mUi->uToolboxTree->collapse(mUi->uToolboxTree->currentIndex());
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(mAwesome->icon(fa::chevrondown, mChevronIconVariant));
            }
            else
            {
                mUi->uToolboxTree->expand(mUi->uToolboxTree->currentIndex());
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(mAwesome->icon(fa::chevronup, mChevronIconVariant));
            }
        }
    });

    QIcon gateIcon(":images/icons/gate.png");
    gateIcon.addPixmap(QPixmap(":images/icons/gate.png"), QIcon::Mode::Selected);

    QIcon inputIcon(":images/icons/input_icon.png");
    inputIcon.addPixmap(QPixmap(":images/icons/input_icon.png"), QIcon::Mode::Selected);

    QIcon buttonIcon(":images/icons/button_icon.png");
    buttonIcon.addPixmap(QPixmap(":images/icons/button_icon.png"), QIcon::Mode::Selected);

    QIcon clockIcon(":images/icons/clock_icon.png");
    clockIcon.addPixmap(QPixmap(":images/icons/clock_icon.png"), QIcon::Mode::Selected);

    QIcon outputIcon(":images/icons/output_icon.png");
    outputIcon.addPixmap(QPixmap(":images/icons/output_icon.png"), QIcon::Mode::Selected);

    QIcon flipflopIcon(":images/icons/flipflop_icon.png");
    flipflopIcon.addPixmap(QPixmap(":images/icons/flipflop_icon.png"), QIcon::Mode::Selected);

    QIcon fulladderIcon(":images/icons/full_adder_icon.png");
    fulladderIcon.addPixmap(QPixmap(":images/icons/full_adder_icon.png"), QIcon::Mode::Selected);

    QIcon demultiplexerIcon(":images/icons/demultiplexer.png");
    demultiplexerIcon.addPixmap(QPixmap(":images/icons/demultiplexer.png"), QIcon::Mode::Selected);

    QIcon encoderIcon(":images/icons/encoder.png");
    encoderIcon.addPixmap(QPixmap(":images/icons/encoder.png"), QIcon::Mode::Selected);

    QIcon decoderIcon(":images/icons/decoder.png");
    decoderIcon.addPixmap(QPixmap(":images/icons/decoder.png"), QIcon::Mode::Selected);

    QIcon labelIcon(":images/icons/label_icon.png");
    labelIcon.addPixmap(QPixmap(":images/icons/label_icon.png"), QIcon::Mode::Selected);

    // Create category and root level items
    mCategoryGatesItem = new QStandardItem(mAwesome->icon(fa::chevronup, mChevronIconVariant), "Gates");
    mCategoryGatesItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryGatesItem);

    mCategoryInputsItem = new QStandardItem(mAwesome->icon(fa::chevronup, mChevronIconVariant), "Inputs");
    mCategoryInputsItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryInputsItem);

    auto outputItem = new QStandardItem(outputIcon, "Output");
    mToolboxTreeModel.appendRow(outputItem);

    mCategoryAddersItem = new QStandardItem(mAwesome->icon(fa::chevrondown, mChevronIconVariant), "Adders");
    mCategoryAddersItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryAddersItem);

    mCategoryMemoryItem = new QStandardItem(mAwesome->icon(fa::chevrondown, mChevronIconVariant), "Memory");
    mCategoryMemoryItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryMemoryItem);

    mCategoryConvertersItem = new QStandardItem(mAwesome->icon(fa::chevrondown, mChevronIconVariant), "Converters");
    mCategoryConvertersItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryConvertersItem);

    auto textLabelItem = new QStandardItem(labelIcon, "Text label");
    mToolboxTreeModel.appendRow(textLabelItem);

    // Create component items
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "AND gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "OR gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "XOR gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "NOT gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "Buffer gate"));

    mCategoryInputsItem->appendRow(new QStandardItem(inputIcon, "Switch"));
    mCategoryInputsItem->appendRow(new QStandardItem(buttonIcon, "Button"));
    mCategoryInputsItem->appendRow(new QStandardItem(clockIcon, "Clock"));

    mCategoryAddersItem->appendRow(new QStandardItem(flipflopIcon, "Half adder"));
    mCategoryAddersItem->appendRow(new QStandardItem(fulladderIcon, "Full adder"));

    mCategoryMemoryItem->appendRow(new QStandardItem(flipflopIcon, "RS flip-flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(flipflopIcon, "D flip-flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(flipflopIcon, "T flip-flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(fulladderIcon, "JK flip-flop"));

    mCategoryConvertersItem->appendRow(new QStandardItem(gateIcon, "Multiplexer"));
    mCategoryConvertersItem->appendRow(new QStandardItem(demultiplexerIcon, "Demultiplexer"));
    mCategoryConvertersItem->appendRow(new QStandardItem(decoderIcon, "Decoder"));
    mCategoryConvertersItem->appendRow(new QStandardItem(encoderIcon, "Encoder"));

    mUi->uToolboxTree->setModel(&mToolboxTreeModel);
    mUi->uToolboxTree->setExpanded(mCategoryGatesItem->index(), true);
    mUi->uToolboxTree->setExpanded(mCategoryInputsItem->index(), true);
    mUi->uToolboxTree->setExpanded(mCategoryAddersItem->index(), true);
}

void MainWindow::InitializeGuiIcons()
{
    // Initialize icon color variants
    mMenuBarIconVariant.insert("color", QColor(0, 39, 43));
    mMenuBarIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mMenuBarIconVariant.insert("color-active", QColor(0, 39, 43));
    mMenuBarIconVariant.insert("color-selected", QColor(0, 39, 43));

    mUncheckedButtonVariant.insert("color", QColor(0, 45, 50));
    mUncheckedButtonVariant.insert("color-disabled", QColor(200, 200, 200));
    mUncheckedButtonVariant.insert("color-active", QColor(0, 45, 50));
    mUncheckedButtonVariant.insert("color-selected", QColor(0, 45, 50));

    mCheckedButtonVariant.insert("color", QColor(255, 255, 255));
    mCheckedButtonVariant.insert("color-disabled", QColor(200, 200, 200));
    mCheckedButtonVariant.insert("color-active", QColor(255, 255, 255));
    mCheckedButtonVariant.insert("color-selected", QColor(255, 255, 255));

    mStatusBarIconVariant.insert("color", QColor(0, 204, 143));
    mStatusBarIconVariant.insert("color-disabled", QColor(180, 180, 180));
    mStatusBarIconVariant.insert("color-active", QColor(0, 204, 143));
    mStatusBarIconVariant.insert("color-selected", QColor(0, 204, 143));

    mPlusMinusIconVariant.insert("color", QColor(0, 45, 50));
    mPlusMinusIconVariant.insert("color-disabled", QColor(180, 180, 180));
    mPlusMinusIconVariant.insert("color-active", QColor(0, 45, 50));
    mPlusMinusIconVariant.insert("color-selected", QColor(0, 45, 50));

    mConfigButtonIconVariant.insert("color", QColor(180, 180, 180));
    mConfigButtonIconVariant.insert("color-disabled", QColor(180, 180, 180));
    mConfigButtonIconVariant.insert("color-active", QColor(180, 180, 180));
    mConfigButtonIconVariant.insert("color-selected", QColor(180, 180, 180));

    mWhiteIconVariant.insert("color", QColor(255, 255, 255));
    mWhiteIconVariant.insert("color-disabled", QColor(255, 255, 255));
    mWhiteIconVariant.insert("color-active", QColor(255, 255, 255));
    mWhiteIconVariant.insert("color-selected", QColor(255, 255, 255));

    // Icons for GUI buttons
    mUi->uEditButton->SetCheckedIcon(mAwesome->icon(fa::mousepointer, mCheckedButtonVariant));
    mUi->uEditButton->SetUncheckedIcon(mAwesome->icon(fa::mousepointer, mUncheckedButtonVariant));

    mUi->uWiringButton->SetCheckedIcon(QIcon(":/images/icons/wiring_checked.png"));
    mUi->uWiringButton->SetUncheckedIcon(QIcon(":/images/icons/wiring.png"));

    mUi->uDeleteButton->SetIcon(QIcon(":/images/icons/delete.png"));
    mUi->uUndoButton->SetIcon(mAwesome->icon(fa::undo, mUncheckedButtonVariant));
    mUi->uRedoButton->SetIcon(mAwesome->icon(fa::repeat, mUncheckedButtonVariant));

    mUi->uStartButton->SetUncheckedIcon(mAwesome->icon(fa::poweroff, mUncheckedButtonVariant));
    mUi->uStartButton->SetCheckedIcon(mAwesome->icon(fa::poweroff, mCheckedButtonVariant));
    mUi->uRunButton->SetUncheckedIcon(mAwesome->icon(fa::play, mUncheckedButtonVariant));
    mUi->uRunButton->SetCheckedIcon(mAwesome->icon(fa::play, mCheckedButtonVariant));
    mUi->uPauseButton->SetUncheckedIcon(mAwesome->icon(fa::pause, mUncheckedButtonVariant));
    mUi->uPauseButton->SetCheckedIcon(mAwesome->icon(fa::pause, mCheckedButtonVariant));
    mUi->uStepButton->SetIcon(mAwesome->icon(fa::stepforward, mUncheckedButtonVariant));
    mUi->uResetButton->SetIcon(mAwesome->icon(fa::refresh, mUncheckedButtonVariant));

    // Icons for configuration elements
    mUi->uLabelToggleIcon->setPixmap(mAwesome->icon(fa::tachometer, mStatusBarIconVariant).pixmap(20, 20));
    mUi->uLabelTogglePlus->setPixmap(mAwesome->icon(fa::plus, mPlusMinusIconVariant).pixmap(8, 8));
    mUi->uLabelToggleMinus->setPixmap(mAwesome->icon(fa::minus, mPlusMinusIconVariant).pixmap(8, 8));

    mUi->uLabelPulseIcon->setPixmap(mAwesome->icon(fa::hourglasso, mStatusBarIconVariant).pixmap(20, 20));
    mUi->uLabelPulsePlus->setPixmap(mAwesome->icon(fa::plus, mPlusMinusIconVariant).pixmap(8, 8));
    mUi->uLabelPulseMinus->setPixmap(mAwesome->icon(fa::minus, mPlusMinusIconVariant).pixmap(8, 8));

    mUi->uItemRightButton->setIcon(mAwesome->icon(fa::arrowright, mConfigButtonIconVariant));
    mUi->uItemDownButton->setIcon(mAwesome->icon(fa::arrowdown, mConfigButtonIconVariant));
    mUi->uItemLeftButton->setIcon(mAwesome->icon(fa::arrowleft, mConfigButtonIconVariant));
    mUi->uItemUpButton->setIcon(mAwesome->icon(fa::arrowup, mConfigButtonIconVariant));

    mUi->uLabelGateInputCountIcon->setPixmap(mAwesome->icon(fa::sortamountasc, mStatusBarIconVariant).pixmap(20, 20));
    mUi->uLabelGateInputCountPlus->setPixmap(mAwesome->icon(fa::plus, mPlusMinusIconVariant).pixmap(8, 8));
    mUi->uLabelGateInputCountMinus->setPixmap(mAwesome->icon(fa::minus, mPlusMinusIconVariant).pixmap(8, 8));

    mUi->uLabelEncoderDecoderInputCountIcon->setPixmap(mAwesome->icon(fa::sortamountasc, mStatusBarIconVariant).pixmap(20, 20));
    mUi->uLabelEncoderDecoderInputCountPlus->setPixmap(mAwesome->icon(fa::plus, mPlusMinusIconVariant).pixmap(8, 8));
    mUi->uLabelEncoderDecoderInputCountMinus->setPixmap(mAwesome->icon(fa::minus, mPlusMinusIconVariant).pixmap(8, 8));

    mUi->uLabelBitWidthIcon->setPixmap(mAwesome->icon(fa::sortamountasc, mStatusBarIconVariant).pixmap(20, 20));
    mUi->uLabelBitWidthPlus->setPixmap(mAwesome->icon(fa::plus, mPlusMinusIconVariant).pixmap(8, 8));
    mUi->uLabelBitWidthMinus->setPixmap(mAwesome->icon(fa::minus, mPlusMinusIconVariant).pixmap(8, 8));

    // Icons for status bar elements
    mUi->uLabelZoomIcon->setPixmap(mAwesome->icon(fa::search, mStatusBarIconVariant).pixmap(20, 20));
    mUi->uLabelPlus->setPixmap(mAwesome->icon(fa::plus, mPlusMinusIconVariant).pixmap(8, 8));
    mUi->uLabelMinus->setPixmap(mAwesome->icon(fa::minus, mPlusMinusIconVariant).pixmap(8, 8));

    // Icons for menu bar elements
    mUi->uActionNew->setIcon(mAwesome->icon(fa::fileo, mMenuBarIconVariant));
    mUi->uActionOpen->setIcon(mAwesome->icon(fa::folderopeno, mMenuBarIconVariant));
    mUi->uActionSave->setIcon(mAwesome->icon(fa::floppyo, mMenuBarIconVariant));

    mUi->uActionUndo->setIcon(mAwesome->icon(fa::undo, mMenuBarIconVariant));
    mUi->uActionRedo->setIcon(mAwesome->icon(fa::repeat, mMenuBarIconVariant));
    mUi->uActionCut->setIcon(mAwesome->icon(fa::scissors, mMenuBarIconVariant));
    mUi->uActionCopy->setIcon(mAwesome->icon(fa::copy, mMenuBarIconVariant));
    mUi->uActionPaste->setIcon(mAwesome->icon(fa::clipboard, mMenuBarIconVariant));
    mUi->uActionDelete->setIcon(mAwesome->icon(fa::trasho, mMenuBarIconVariant));

    mUi->uActionStart->setIcon(mAwesome->icon(fa::poweroff, mMenuBarIconVariant));
    mUi->uActionRun->setIcon(mAwesome->icon(fa::play, mMenuBarIconVariant));
    mUi->uActionStep->setIcon(mAwesome->icon(fa::stepforward, mMenuBarIconVariant));
    mUi->uActionReset->setIcon(mAwesome->icon(fa::refresh, mMenuBarIconVariant));
    mUi->uActionPause->setIcon(mAwesome->icon(fa::pause, mMenuBarIconVariant));

    mUi->uActionScreenshot->setIcon(mAwesome->icon(fa::camera, mMenuBarIconVariant));

    mUi->uActionStartTutorial->setIcon(mAwesome->icon(fa::graduationcap, mMenuBarIconVariant));
    mUi->uActionReportBugs->setIcon(mAwesome->icon(fa::bug, mMenuBarIconVariant));
    mUi->uActionOpenWebsite->setIcon(mAwesome->icon(fa::externallink, mMenuBarIconVariant));
    mUi->uActionOpenTwitter->setIcon(mAwesome->icon(fa::twitter, mMenuBarIconVariant));
    mUi->uActionOpenGithub->setIcon(mAwesome->icon(fa::github, mMenuBarIconVariant));
    mUi->uActionAbout->setIcon(mAwesome->icon(fa::info, mMenuBarIconVariant));
}

void MainWindow::InitializeGlobalShortcuts()
{
    // Shortcuts to change component input count
    mOneGateInputShortcut    = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_1), this);
    mTwoGateInputsShortcut   = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_2), this);
    mThreeGateInputsShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_3), this);
    mFourGateInputsShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_4), this);
    mFiveGateInputsShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_5), this);
    mSixGateInputsShortcut   = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_6), this);
    mSevenGateInputsShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_7), this);
    mEightGateInputsShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_8), this);
    mNineGateInputsShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_9), this);

    mOneGateInputShortcut->setAutoRepeat(false);
    mTwoGateInputsShortcut->setAutoRepeat(false);
    mThreeGateInputsShortcut->setAutoRepeat(false);
    mFourGateInputsShortcut->setAutoRepeat(false);
    mFiveGateInputsShortcut->setAutoRepeat(false);
    mSixGateInputsShortcut->setAutoRepeat(false);
    mSevenGateInputsShortcut->setAutoRepeat(false);
    mEightGateInputsShortcut->setAutoRepeat(false);
    mNineGateInputsShortcut->setAutoRepeat(false);

    QObject::connect(mOneGateInputShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(1);
       SetEncoderDecoderInputCountIfAllowed(1);
       SetMultiplexerBitWidthIfAllowed(1);
    });
    QObject::connect(mTwoGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(2);
       SetEncoderDecoderInputCountIfAllowed(2);
       SetMultiplexerBitWidthIfAllowed(2);
    });
    QObject::connect(mThreeGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(3);
       SetEncoderDecoderInputCountIfAllowed(3);
       SetMultiplexerBitWidthIfAllowed(3);
    });
    QObject::connect(mFourGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(4);
       SetEncoderDecoderInputCountIfAllowed(4);
       SetMultiplexerBitWidthIfAllowed(4);
    });
    QObject::connect(mFiveGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(5);
       SetEncoderDecoderInputCountIfAllowed(5);
       SetMultiplexerBitWidthIfAllowed(5);
    });
    QObject::connect(mSixGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(6);
       SetEncoderDecoderInputCountIfAllowed(6);
       SetMultiplexerBitWidthIfAllowed(6);
    });
    QObject::connect(mSevenGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(7);
       SetEncoderDecoderInputCountIfAllowed(7);
       SetMultiplexerBitWidthIfAllowed(7);
    });
    QObject::connect(mEightGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(8);
       SetEncoderDecoderInputCountIfAllowed(8);
       SetMultiplexerBitWidthIfAllowed(8);
    });
    QObject::connect(mNineGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(9);
       SetEncoderDecoderInputCountIfAllowed(9);
       SetMultiplexerBitWidthIfAllowed(9);
    });

    mEscapeShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);

    mEscapeShortcut->setAutoRepeat(false);

    QObject::connect(mEscapeShortcut, &QShortcut::activated, this, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
        mScene.clearSelection();
        mUi->uToolboxTree->clearSelection();
        if (!mIsGuiHidden)
        {
            FadeInWidget(mUi->uToolboxContainer);
        }
        else
        {
            mIsToolboxVisible = true;
        }
    });
}

void MainWindow::SetGateInputCountIfAllowed(uint8_t pCount)
{
    if (pCount >= components::gates::MIN_INPUT_COUNT && pCount <= components::gates::MAX_INPUT_COUNT)
    {
        if (mCoreLogic.GetControlMode() == ControlMode::ADD && mCoreLogic.GetSelectedComponentType() <= ComponentType::XOR_GATE)
        {
            mCoreLogic.SetGateInputCount(pCount);
            mUi->uLabelGateInputCount->setText(tr(pCount > 1 ? "%0 Inputs" : "%0 Input").arg(pCount));
            mUi->uGateInputCountSlider->setValue(pCount);
        }
    }
}

void MainWindow::SetEncoderDecoderInputCountIfAllowed(uint8_t pCount)
{
    if (pCount >= components::encoder_decoder::MIN_INPUT_COUNT && pCount <= components::encoder_decoder::MAX_INPUT_COUNT)
    {
        if (mCoreLogic.GetControlMode() == ControlMode::ADD &&
                (mCoreLogic.GetSelectedComponentType() == ComponentType::DECODER || mCoreLogic.GetSelectedComponentType() == ComponentType::ENCODER))
        {
            mCoreLogic.SetEncoderDecoderInputCount(pCount);
            if (mCoreLogic.GetSelectedComponentType() == ComponentType::ENCODER)
            {
                mUi->uLabelEncoderDecoderInputCount->setText(tr(pCount > 1 ? "%0 Outputs" : "%0 Output").arg(pCount));
            }
            else
            {
                mUi->uLabelEncoderDecoderInputCount->setText(tr(pCount > 1 ? "%0 Inputs" : "%0 Input").arg(pCount));
            }
            mUi->uEncoderDecoderInputCountSlider->setValue(pCount);
        }
    }
}

void MainWindow::SetMultiplexerBitWidthIfAllowed(uint8_t pBitWidth)
{
    if (pBitWidth > 0 && pBitWidth <= components::multiplexer::MAX_BIT_WIDTH)
    {
        if (mCoreLogic.GetControlMode() == ControlMode::ADD && (mCoreLogic.GetSelectedComponentType() == ComponentType::MULTIPLEXER
                                                                || mCoreLogic.GetSelectedComponentType() == ComponentType::DEMULTIPLEXER))
        {
            mCoreLogic.SetMultiplexerBitWidth(pBitWidth);
            mUi->uLabelBitWidth->setText(tr(pBitWidth > 1 ? "%0 Bits" : "%0 Bit").arg(pBitWidth));
            mUi->uBitWidthSlider->setValue(pBitWidth);
        }
    }
}

void MainWindow::SetComponentDirectionIfInAddMode(Direction pDirection)
{
    if (mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.SetComponentDirection(pDirection);
    }
}

View& MainWindow::GetView()
{
    return mView;
}

CoreLogic& MainWindow::GetCoreLogic()
{
    return mCoreLogic;
}

void MainWindow::OnToolboxTreeClicked(const QModelIndex &pIndex)
{
    if (pIndex.row() == -1)
    {
        throw std::logic_error("Model index invalid");
    }
    else if (pIndex.parent().row() == -1)
    {
        // Item is on root level
        switch(pIndex.row())
        {
            case 2: // Output
            {
                mCoreLogic.EnterAddControlMode(ComponentType::OUTPUT);
                break;
            }
            case 6: // Text label
            {
                mCoreLogic.EnterAddControlMode(ComponentType::TEXT_LABEL);
                break;
            }
            default:
            {
                mCoreLogic.EnterControlMode(ControlMode::EDIT);
                mScene.clearSelection();
                break;
            }
        }
    }
    else if (pIndex.parent().parent().row() == -1)
    {
        // Item is on second level
        switch (pIndex.parent().row())
        {
            case 0: // Gates
            {
                switch(pIndex.row())
                {
                    case 0: // AND gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::AND_GATE);
                        break;
                    }
                    case 1: // OR gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::OR_GATE);
                        break;
                    }
                    case 2: // XOR gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::XOR_GATE);
                        break;
                    }
                    case 3: // NOT gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::NOT_GATE);
                        break;
                    }
                    case 4: // Buffer gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::BUFFER_GATE);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown gate";
                        break;
                    }
                }

                break;
            }
            case 1: // Inputs
            {
                switch(pIndex.row())
                {
                    case 0: // Switch
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::INPUT);
                        break;
                    }
                    case 1: // Button
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::BUTTON);
                        break;
                    }
                    case 2: // Clock
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::CLOCK);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown input";
                        break;
                    }
                }
                break;
            }
            case 3: // Adders
            {
                switch(pIndex.row())
                {
                    case 0: // Half adder
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::HALF_ADDER);
                        break;
                    }
                    case 1: // Full adder
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::FULL_ADDER);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown adder";
                        break;
                    }
                }
                break;
            }
            case 4: // Memory
            {
                switch(pIndex.row())
                {
                    case 0: // RS flip flop
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::RS_FLIPFLOP);
                        break;
                    }
                    case 1: // D flip flop
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::D_FLIPFLOP);
                        break;
                    }
                    case 2: // T flip flop
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::T_FLIPFLOP);
                        break;
                    }
                    case 3: // JK flip flop
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::JK_FLIPFLOP);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown memory";
                        break;
                    }
                }
                break;
            }
            case 5: // Converters
            {
                switch(pIndex.row())
                {
                    case 0: // Multiplexer
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::MULTIPLEXER);
                        break;
                    }
                    case 1: // Demultiplexer
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::DEMULTIPLEXER);
                        break;
                    }
                    case 2: // Decoder
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::DECODER);
                        break;
                    }
                    case 3: // Encoder
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::ENCODER);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown converter";
                        break;
                    }
                }
                break;
            }
        }
    }
    else
    {
        qDebug() << "Unknown higher level item";
    }
}

template <typename T>
void MainWindow::FadeInWidget(T& pWidget)
{
    bool stoppedCurrentAnimation = false;
    if (pWidget->graphicsEffect() != nullptr)
    {
        delete pWidget->graphicsEffect();
        stoppedCurrentAnimation = true;
    }

    if (stoppedCurrentAnimation || !pWidget->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        pWidget->setGraphicsEffect(effect);
        pWidget->show();

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(gui::FADE_ANIMATION_DURATION);
        anim->setStartValue(0.0f);
        anim->setEndValue(1.0f);
        anim->setEasingCurve(QEasingCurve::OutCirc);

        QObject::connect(anim, &QPropertyAnimation::finished, [&]()
        {
            delete pWidget->graphicsEffect();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

template <typename T>
void MainWindow::FadeOutWidget(T& pWidget)
{
    bool stoppedCurrentAnimation = false;
    if (pWidget->graphicsEffect() != nullptr)
    {
        delete pWidget->graphicsEffect();
        stoppedCurrentAnimation = true;
    }

    if (stoppedCurrentAnimation || pWidget->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        pWidget->setGraphicsEffect(effect);

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(gui::FADE_ANIMATION_DURATION);
        anim->setStartValue(1.0f);
        anim->setEndValue(0.0f);
        anim->setEasingCurve(QEasingCurve::OutCirc);

        QObject::connect(anim, &QPropertyAnimation::finished, [&]()
        {
            delete pWidget->graphicsEffect();
            pWidget->hide();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

