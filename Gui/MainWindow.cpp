#include "MainWindow.h"
#include "HelperFunctions.h"
#include "ui_MainWindow.h"

#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow),
    mView(mAwesome, mCoreLogic),
    mCoreLogic(mView),
    mAboutDialog(this),
    mWelcomeDialog(mAwesome, this)
{
    mUi->setupUi(this);

    mAwesome.initFontAwesome();

    mScene.setSceneRect(canvas::DIMENSIONS);
    mView.SetScene(mScene);

    mUi->uViewLayout->addWidget(&mView, 0, 0, 8, 6);
    mTutorialFrame.setParent(mUi->uCentralwidget);

    mView.stackUnder(mUi->uToolboxContainer);

    QObject::connect(&mCoreLogic, &CoreLogic::ControlModeChangedSignal, this, &MainWindow::OnControlModeChanged);
    QObject::connect(&mCoreLogic, &CoreLogic::SimulationModeChangedSignal, this, &MainWindow::OnSimulationModeChanged);

    ConnectGuiSignalsAndSlots();

    InitializeToolboxTree();
    InitializeGuiIcons();
    InitializeGlobalShortcuts();
    InitializeMessageBoxes();
    InitializeTutorial();

    UpdateUndoRedoEnabled(false);

    mUi->uItemRightButton->setChecked(true); // Button for component direction RIGHT
    mUi->uOutputColorDefaultButton->setChecked(true); // Button for output color DEFAULT

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

    mErrorOpenRecentFileBox.setIcon(QMessageBox::Icon::Critical);
    mErrorOpenRecentFileBox.setWindowTitle("Linkuit Studio");
    mErrorOpenRecentFileBox.setWindowIcon(QIcon(":/images/icons/icon_default.png"));
    mErrorOpenRecentFileBox.setText(tr("The file could not be opened."));
    mErrorOpenRecentFileBox.setInformativeText("Remove it from the list?");
    mErrorOpenRecentFileBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mErrorOpenRecentFileBox.setDefaultButton(QMessageBox::Yes);

    mErrorSaveFileBox.setIcon(QMessageBox::Icon::Critical);
    mErrorSaveFileBox.setWindowTitle("Linkuit Studio");
    mErrorSaveFileBox.setWindowIcon(QIcon(":/images/icons/icon_default.png"));
    mErrorSaveFileBox.setText(tr("The circuit could not be saved."));
    mErrorSaveFileBox.setStandardButtons(QMessageBox::Ok);
    mErrorSaveFileBox.setDefaultButton(QMessageBox::Ok);
}

void MainWindow::InitializeTutorial()
{
    QObject::connect(&mView, &View::ZoomLevelChangedSignal, this, [&]() {
        if (mTutorialFrame.GetCurrentStep() == TutorialStep::NAVIGATION)
        {
            mTutorialFrame.ApproveStepOnCondition(TutorialStep::NAVIGATION);
        }
    });

    QObject::connect(&mCoreLogic, &CoreLogic::ComponentAddedSignal, this, [&](ComponentType pType) {
        switch (mTutorialFrame.GetCurrentStep())
        {
            case TutorialStep::OR_GATE:
            {
                if (pType == ComponentType::OR_GATE)
                {
                    mTutorialFrame.ApproveStepOnCondition(TutorialStep::OR_GATE);
                }
                break;
            }
            case TutorialStep::INPUTS:
            {
                if (pType == ComponentType::INPUT)
                {
                    mTutorialFrame.ApproveStepOnCondition(TutorialStep::INPUTS);
                }
                break;
            }
            case TutorialStep::OUTPUT:
            {
                if (pType == ComponentType::OUTPUT)
                {
                    mTutorialFrame.ApproveStepOnCondition(TutorialStep::OUTPUT);
                }
                break;
            }
            default:
            {
                break;
            }
        }
    });

    QObject::connect(&mCoreLogic, &CoreLogic::WireAddedSignal, this, [&]() {
        if (mTutorialFrame.GetCurrentStep() == TutorialStep::WIRES)
        {
            mTutorialFrame.ApproveStepOnCondition(TutorialStep::WIRES);
        }
    });

    QObject::connect(mUi->uStartButton, &QAbstractButton::clicked, this, [&]() {
        if (mTutorialFrame.GetCurrentStep() == TutorialStep::START_SIMULATION)
        {
            mTutorialFrame.ApproveStepOnCondition(TutorialStep::START_SIMULATION);
        }
        if (mTutorialFrame.GetCurrentStep() == TutorialStep::END_SIMULATION)
        {
            mTutorialFrame.ApproveStepOnCondition(TutorialStep::END_SIMULATION);
        }
    });
}

void MainWindow::ConnectGuiSignalsAndSlots()
{
    QObject::connect(&mView, &View::ZoomLevelChangedSignal, this, &MainWindow::UpdateZoomLabelAndSlider);
    QObject::connect(&mView, &View::UndoFromPieMenuSignal, this, &MainWindow::Undo);
    QObject::connect(&mView, &View::RedoFromPieMenuSignal, this, &MainWindow::Redo);

    QObject::connect(&mCoreLogic, &CoreLogic::OpeningFileSuccessfulSignal, this, &MainWindow::OnCircuitFileOpenedSuccessfully);
    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::LoadCircuitFileFailedSignal, this, &MainWindow::OnCircuitFileOpeningFailed);

    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::SaveCircuitFileSuccessSignal, this, &MainWindow::OnCircuitFileSavedSuccessfully);
    QObject::connect(&mCoreLogic.GetCircuitFileParser(), &CircuitFileParser::SaveCircuitFileFailedSignal, this, &MainWindow::OnCircuitFileSavingFailed);

    QObject::connect(&mCoreLogic, &CoreLogic::OpeningFileSuccessfulSignal, this, &MainWindow::OnCircuitFileOpenedSuccessfully);
    QObject::connect(&mCoreLogic, &CoreLogic::FileHasNewerCompatibleVersionSignal, this, &MainWindow::OnCircuitFileHasNewerCompatibleVersion);
    QObject::connect(&mCoreLogic, &CoreLogic::FileHasNewerIncompatibleVersionSignal, this, &MainWindow::OnCircuitFileHasNewerIncompatibleVersion);

    // Connect to core logic signals

    QObject::connect(&mCoreLogic, &CoreLogic::ShowClockConfiguratorSignal, this, &MainWindow::ShowClockConfigurator);
    QObject::connect(&mCoreLogic, &CoreLogic::HideClockConfiguratorSignal, this, [&]()
    {
        FadeOutWidget(*(mUi->uClockConfigurator), gui::FADE_ANIMATION_DURATION);
    });
    QObject::connect(&mCoreLogic, &CoreLogic::ControlModeChangedSignal, this, [&](ControlMode pNewMode)
    {
        mUi->uLabelStatus->setText(tr("Ready."));

        if (pNewMode != ControlMode::ADD)
        {
            FadeOutWidget(*(mUi->uItemConfigurator), gui::FADE_ANIMATION_DURATION);
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

    QObject::connect(mUi->uEditButton, &QAbstractButton::clicked, this, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    });

    QObject::connect(mUi->uWiringButton, &QAbstractButton::clicked, this, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::WIRE);
    });

    // Connect widgets from item configuration GUI

    QObject::connect(mUi->uItemRightButton, &QPushButton::toggled, this, &MainWindow::OnItemRightButtonToggled);
    QObject::connect(mUi->uItemDownButton, &QPushButton::toggled, this, &MainWindow::OnItemDownButtonToggled);
    QObject::connect(mUi->uItemLeftButton, &QPushButton::toggled, this, &MainWindow::OnItemLeftButtonToggled);
    QObject::connect(mUi->uItemUpButton, &QPushButton::toggled, this, &MainWindow::OnItemUpButtonToggled);

    QObject::connect(mUi->uOutputColorDefaultButton, &QPushButton::toggled, this, &MainWindow::OnOutputColorDefaultButtonToggled);
    QObject::connect(mUi->uOutputColorGreenButton, &QPushButton::toggled, this, &MainWindow::OnOutputColorGreenButtonToggled);
    QObject::connect(mUi->uOutputColorRedButton, &QPushButton::toggled, this, &MainWindow::OnOutputColorRedButtonToggled);
    QObject::connect(mUi->uOutputColorBlueButton, &QPushButton::toggled, this, &MainWindow::OnOutputColorBlueButtonToggled);
    QObject::connect(mUi->uOutputColorYellowButton, &QPushButton::toggled, this, &MainWindow::OnOutputColorYellowButtonToggled);

    QObject::connect(mUi->uGateInputCountSlider, &QSlider::valueChanged, this, &MainWindow::SetGateInputCountIfAllowed);
    QObject::connect(mUi->uEncoderDecoderInputCountSlider, &QSlider::valueChanged, this, &MainWindow::SetEncoderDecoderInputCountIfAllowed);
    QObject::connect(mUi->uMultiplexerBitWidthSlider, &QSlider::valueChanged, this, &MainWindow::SetMultiplexerBitWidthIfAllowed);
    QObject::connect(mUi->uShiftRegisterWidthBox, &QComboBox::currentIndexChanged, this, &MainWindow::SetShiftRegisterBitWidthIfAllowed);
    QObject::connect(mUi->uConstantHighButton, &QPushButton::toggled, this, &MainWindow::OnConstantHighButtonToggled);
    QObject::connect(mUi->uCounterBitWidthSlider, &QSlider::valueChanged, this, &MainWindow::SetCounterBitWidthIfAllowed);

    QObject::connect(mUi->uFlipFlopTypeBox, &QComboBox::currentIndexChanged, this, &MainWindow::OnFlipFlopStyleChanged);

    QObject::connect(mUi->uMasterSlaveButton, &QPushButton::toggled, &mCoreLogic, &CoreLogic::OnMasterSlaveToggled);

    // Connect widgets from clock configuration GUI

    QObject::connect(mUi->uButtonToggle, &QPushButton::toggled, this, &MainWindow::OnToggleButtonToggled);
    QObject::connect(mUi->uToggleSpinBox, &QSpinBox::editingFinished, this, [this](){
        mUi->uPulseSpinBox->setMaximum(mUi->uToggleSpinBox->value());
        mCoreLogic.OnToggleValueChanged(mUi->uToggleSpinBox->value());
        mCoreLogic.OnPulseValueChanged(mUi->uPulseSpinBox->value());
    });
    QObject::connect(mUi->uPulseSpinBox, &QSpinBox::editingFinished, this, [this](){
        mCoreLogic.OnPulseValueChanged(mUi->uPulseSpinBox->value());
    });

    QObject::connect(mUi->uDeleteButton, &QAbstractButton::clicked, mUi->uActionDelete, &QAction::trigger);
    QObject::connect(mUi->uUndoButton, &QAbstractButton::clicked, mUi->uActionUndo, &QAction::trigger);
    QObject::connect(mUi->uRedoButton, &QAbstractButton::clicked, mUi->uActionRedo, &QAction::trigger);
    QObject::connect(mUi->uStartButton, &QAbstractButton::clicked, mUi->uActionStart, &QAction::trigger);
    QObject::connect(mUi->uRunButton, &QAbstractButton::clicked, mUi->uActionRun, &QAction::trigger);
    QObject::connect(mUi->uStepButton, &QAbstractButton::clicked, mUi->uActionStep, &QAction::trigger);
    QObject::connect(mUi->uResetButton, &QAbstractButton::clicked, mUi->uActionReset, &QAction::trigger);
    QObject::connect(mUi->uPauseButton, &QAbstractButton::clicked, mUi->uActionPause, &QAction::trigger);

    // Connect tutorial frame
    QObject::connect(&mTutorialFrame, &TutorialFrame::CurrentStepChangedSignal, this, &MainWindow::OnTutorialStepChanged);
    QObject::connect(&mTutorialFrame, &TutorialFrame::AdvanceStepRequestSignal, this, &MainWindow::OnAdvanceTutorialStepRequest);

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

            mUi->uActionHideGrid->setChecked(false);
            mView.SetHideGrid(false);

            mIsToolboxVisible = true;
            FadeInGui();
        }
    });

    QObject::connect(mUi->uActionOpen, &QAction::triggered, this, [&]()
    {
        mFadeOutOnCtrlTimer.stop();

        if (!IsSaveChangesIfModifiedCanceled())
        {
            QString path = mCoreLogic.GetCircuitFileParser().IsFileOpen() ? mCoreLogic.GetCircuitFileParser().GetFileInfo().value().absolutePath() : mCoreLogic.GetRuntimeConfigParser().GetLastFilePath();
            const auto fileInfo = QFileInfo(QFileDialog::getOpenFileName(this, tr(gui::OPEN_FILE_DIALOG_TITLE), path, tr("Linkuit Studio Circuit Files (*.lsc)")));

            if (!fileInfo.absoluteFilePath().isEmpty())
            {
                mCoreLogic.GetCircuitFileParser().LoadJson(fileInfo);
                mUi->uActionHideGrid->setChecked(false);
                mView.SetHideGrid(false);
            }
        }
    });

    QObject::connect(mUi->uActionSave, &QAction::triggered, this, [&]()
    {
        mCoreLogic.AbortPastingIfInCopy();
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

        mCoreLogic.AbortPastingIfInCopy();
        QString path = mCoreLogic.GetCircuitFileParser().IsFileOpen() ? mCoreLogic.GetCircuitFileParser().GetFileInfo().value().absolutePath() : mCoreLogic.GetRuntimeConfigParser().GetLastFilePath();
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
                mCoreLogic.AbortPastingIfInCopy();
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
                mCoreLogic.AbortPastingIfInCopy();
            }
        }
    });

    QObject::connect(mUi->uActionSelectAll, &QAction::triggered, &mCoreLogic, &CoreLogic::SelectAll);

    QObject::connect(mUi->uActionHideGrid, &QAction::triggered, &mView, &View::SetHideGrid);

    QObject::connect(mUi->uActionScreenshot, &QAction::triggered, this, [&]()
    {
        qDebug() << "Not implemented";
    });

    QObject::connect(mUi->uActionStartTutorial, &QAction::triggered, this, [&]()
    {
        mTutorialFrame.StartTutorial();
    });

    QObject::connect(mUi->uActionWelcomePage, &QAction::triggered, this, [&]()
    {
        ConfigureWelcomeDialog(mCoreLogic.GetRuntimeConfigParser().IsWelcomeDialogEnabledOnStartup(), mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
        SetRecentFileMenuActions(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
        ShowWelcomeDialog(std::chrono::milliseconds(0));
    });

    QObject::connect(mUi->uActionReportBugs, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl("mailto:linkuit@outlook.com"));
    });

    QObject::connect(mUi->uActionOpenWebsite, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl("https://linkuit.com/"));
    });

    QObject::connect(mUi->uActionOpenTwitter, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl("https://x.com/goLinkuit/"));
    });

    QObject::connect(mUi->uActionOpenGithub, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl("https://github.com/SimonBuxx/Linkuit-Studio/"));
    });

    QObject::connect(mUi->uActionCheckUpdate, &QAction::triggered, this, [&]()
    {
        QDesktopServices::openUrl(QUrl(QString("https://linkuit.com/update/%0").arg(QString(FULL_VERSION))));
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

void MainWindow::OnCircuitFileHasNewerCompatibleVersion(const QString& pVersion)
{
    mNewerVersionCompatibleBox.setIcon(QMessageBox::Icon::Information);
    mNewerVersionCompatibleBox.setWindowTitle("Linkuit Studio");
    mNewerVersionCompatibleBox.setWindowIcon(QIcon(":/images/icons/icon_default.png"));
    mNewerVersionCompatibleBox.setText(tr("This file has been created with a newer version of Linkuit Studio."));
    mNewerVersionCompatibleBox.setInformativeText(QString("It seems like this file was last saved using version %0. "
        "It is marked compatible with the current version %1, but please consider updating Linkuit Studio.").arg(pVersion, QString(FULL_VERSION)));
    mNewerVersionCompatibleBox.setStandardButtons(QMessageBox::Ok);
    mNewerVersionCompatibleBox.setDefaultButton(QMessageBox::Ok);
    mNewerVersionCompatibleBox.exec();
}

void MainWindow::OnCircuitFileHasNewerIncompatibleVersion(const QString& pVersion)
{
    mNewerVersionIncompatibleBox.setIcon(QMessageBox::Icon::Critical);
    mNewerVersionIncompatibleBox.setWindowTitle("Linkuit Studio");
    mNewerVersionIncompatibleBox.setWindowIcon(QIcon(":/images/icons/icon_default.png"));
    mNewerVersionIncompatibleBox.setText(tr("This file has been created with a newer version of Linkuit Studio."));
    mNewerVersionIncompatibleBox.setInformativeText(QString("This file is compatible with version %0 or newer. "
        "It is therefore incompatible with the current version %1. Please update Linkuit Studio to open the file.").arg(pVersion, QString(FULL_VERSION)));
    mNewerVersionIncompatibleBox.setStandardButtons(QMessageBox::Ok);
    mNewerVersionIncompatibleBox.setDefaultButton(QMessageBox::Ok);
    mNewerVersionIncompatibleBox.exec();
}

void MainWindow::OnCircuitFileOpeningFailed(const QFileInfo& pFileInfo, bool pIsFromRecents)
{
    if (pIsFromRecents)
    {
        mErrorOpenRecentFileBox.setText(tr("The file %0 could not be opened.").arg(pFileInfo.fileName()));
        if (mErrorOpenRecentFileBox.exec() == QMessageBox::Yes)
        {
            mCoreLogic.GetRuntimeConfigParser().RemoveRecentFilePath(pFileInfo);
            ConfigureWelcomeDialog(mCoreLogic.GetRuntimeConfigParser().IsWelcomeDialogEnabledOnStartup(), mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
            SetRecentFileMenuActions(mCoreLogic.GetRuntimeConfigParser().GetRecentFilePaths());
        }
    }
    else
    {
        mErrorOpenFileBox.setText(tr("The file %0 could not be opened.").arg(pFileInfo.fileName()));
        mErrorOpenFileBox.exec();
    }
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
        mCoreLogic.GetCircuitFileParser().LoadJson(pFileInfo, true);
        mUi->uActionHideGrid->setChecked(false);
        mView.SetHideGrid(false);
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

void MainWindow::OnAdvanceTutorialStepRequest(TutorialStep pStep)
{
    mTutorialFrame.OnAdvanceStepApproved(pStep);
}

void MainWindow::OnTutorialStepChanged(TutorialStep pStep)
{
    switch (pStep)
    {
        case TutorialStep::INTRODUCTION:
        case TutorialStep::NAVIGATION:
        {
            mTutorialFrame.SetCenterPosition(QPoint(width() / 2, height() / 2));
            break;
        }
        case TutorialStep::OR_GATE:
        case TutorialStep::INPUTS:
        case TutorialStep::OUTPUT:
        {
            mTutorialFrame.SetTopLeftPosition(mUi->uToolboxContainer->mapToParent(QPoint(mUi->uToolboxContainer->width() + 20, 0)));
            break;
        }
        case TutorialStep::WIRES:
        {
            mTutorialFrame.SetTopLeftPosition(QPoint(mUi->uTopBar->x(), mUi->uTopBar->y() + mUi->uTopBar->height() + 20));
            break;
        }
        case TutorialStep::START_SIMULATION:
        case TutorialStep::END_SIMULATION:
        {
            mTutorialFrame.SetTopRightPosition(QPoint(mUi->uTopBar->x() + mUi->uTopBar->width(), mUi->uTopBar->y() + mUi->uTopBar->height() + 20));
            break;
        }
        /*case TutorialStep::INPUTS:
        {
            mTutorialFrame.SetTopRightPosition(mUi->uClockConfigurator->mapToParent(QPoint(-20, 0)));
            break;
        }*/
        default:
        {
            break;
        }
    }
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

void MainWindow::ShowClockConfigurator(ClockMode pMode, uint32_t pToggle, uint32_t pPulse)
{
    if (!mIsGuiHidden)
    {
        FadeInWidget(*(mUi->uClockConfigurator), gui::FADE_ANIMATION_DURATION);
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

    mUi->uToggleSpinBox->setValue(pToggle);
    mUi->uPulseSpinBox->setValue(pPulse);
}

void MainWindow::ShowItemConfigurator(ConfiguratorMode pMode)
{
    switch (pMode)
    {
        case ConfiguratorMode::NO_CONFIGURATION:
        {
            FadeOutWidget(*(mUi->uItemConfigurator), gui::FADE_ANIMATION_DURATION);
            return;
        }
        case ConfiguratorMode::DIRECTION_ONLY:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();
            break;
        }
        case ConfiguratorMode::DIRECTION_AND_INPUT_COUNT:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->show();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();
            break;
        }
        case ConfiguratorMode::MULTIPLEXER_BITS:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->show();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();
            break;
        }
        case ConfiguratorMode::ENCODER_DECODER:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->show();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();

            // To update the label to "ouputs" or "inputs"
            SetEncoderDecoderInputCountIfAllowed(mUi->uEncoderDecoderInputCountSlider->value());
            break;
        }
        case ConfiguratorMode::SHIFTREGISTER_BITS:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->show();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();
            break;
        }
        case ConfiguratorMode::CONSTANT_STATE:
        {
            mUi->uItemDirectionButtonsFrame->hide();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->show();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();
            break;
        }
        case ConfiguratorMode::COUNTER_BITS:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->show();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();
            break;
        }
        case ConfiguratorMode::RS_FLIPFLOP_TYPE:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->show();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();
            break;
        }
        case ConfiguratorMode::MASTER_SLAVE:
        {
            mUi->uItemDirectionButtonsFrame->show();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uOutputColorButtonsFrame->hide();

            switch (mCoreLogic.GetSelectedComponentType())
            {
                case ComponentType::D_FLIPFLOP:
                {
                    mUi->uMasterSlaveButton->setChecked(mCoreLogic.IsDFlipFlopMasterSlave());
                    break;
                }
                case ComponentType::JK_FLIPFLOP:
                {
                    mUi->uMasterSlaveButton->setChecked(mCoreLogic.IsJkFlipFlopMasterSlave());
                    break;
                }
                default:
                {
                    throw std::logic_error("Configuration mode MASTER_SLAVE selected for unapplicable component type");
                    break;
                }
            }

            /*if (mCoreLogic.GetSelectedComponentType() == ComponentType::D_FLIPFLOP)
            {
            }
            else if (mCoreLogic.GetSelectedComponentType() == ComponentType::D_FLIPFLOP)
            {

            }
            else
            {

            }*/

            mUi->uMasterSlaveFrame->show();
            break;
        }
        case ConfiguratorMode::OUTPUT_COLOR:
        {
            mUi->uItemDirectionButtonsFrame->hide();
            mUi->uGateInputCountFrame->hide();
            mUi->uEncoderDecoderInputCountFrame->hide();
            mUi->uMultiplexerBitWidthFrame->hide();
            mUi->uShiftRegisterBitWidthFrame->hide();
            mUi->uConstantButtonsFrame->hide();
            mUi->uCounterBitWidthFrame->hide();
            mUi->uFlipFlopTypesFrame->hide();
            mUi->uMasterSlaveFrame->hide();
            mUi->uOutputColorButtonsFrame->show();
            break;
        }
        default:
        {
            throw std::logic_error("ConfiguratorMode unimplemented or invalid");
        }
    }

    if (!mIsGuiHidden)
    {
            FadeInWidget(*(mUi->uItemConfigurator), gui::FADE_ANIMATION_DURATION);
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
    mUi->uItemRightButton->setIcon(mAwesome.icon(fa::arrowright, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::RIGHT);
    }
}

void MainWindow::OnItemDownButtonToggled(bool pChecked)
{
    mUi->uItemDownButton->setIcon(mAwesome.icon(fa::arrowdown, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::DOWN);
    }
}

void MainWindow::OnItemLeftButtonToggled(bool pChecked)
{
    mUi->uItemLeftButton->setIcon(mAwesome.icon(fa::arrowleft, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::LEFT);
    }
}

void MainWindow::OnItemUpButtonToggled(bool pChecked)
{
    mUi->uItemUpButton->setIcon(mAwesome.icon(fa::arrowup, pChecked ? mWhiteIconVariant : mConfigButtonIconVariant));
    if (pChecked)
    {
        SetComponentDirectionIfInAddMode(Direction::UP);
    }
}

void MainWindow::OnOutputColorDefaultButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        SetOutputColorIfInAddMode(OutputColor::DEFAULT);
    }
}

void MainWindow::OnOutputColorGreenButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        SetOutputColorIfInAddMode(OutputColor::GREEN);
    }
}

void MainWindow::OnOutputColorRedButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        SetOutputColorIfInAddMode(OutputColor::RED);
    }
}

void MainWindow::OnOutputColorBlueButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        SetOutputColorIfInAddMode(OutputColor::BLUE);
    }
}

void MainWindow::OnOutputColorYellowButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        SetOutputColorIfInAddMode(OutputColor::YELLOW);
    }
}

void MainWindow::OnConstantHighButtonToggled(bool pChecked)
{
    const auto state = pChecked ? LogicState::HIGH : LogicState::LOW;
    SetConstantStateIfAllowed(state);
}

void MainWindow::OnFlipFlopStyleChanged(int32_t pIndex)
{
    if (pIndex >= static_cast<int32_t>(FlipFlopStyle::LATCH) && pIndex <= static_cast<int32_t>(FlipFlopStyle::MASTER_SLAVE))
    {
        mCoreLogic.SetFlipFlopStyle(static_cast<FlipFlopStyle>(pIndex));
    }
}

void MainWindow::EnterSimulation()
{
    if (!mCoreLogic.IsSimulationRunning())
    {
        mCoreLogic.EnterControlMode(ControlMode::SIMULATION);
        FadeOutWidget(*(mUi->uToolboxContainer), gui::FADE_ANIMATION_DURATION);
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
            FadeInWidget(*(mUi->uToolboxContainer), gui::FADE_ANIMATION_DURATION);
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
        mView.GetPieMenu()->SetButtonEnabled(3, !mCoreLogic.IsUndoQueueEmpty());
        mView.GetPieMenu()->SetButtonEnabled(1, !mCoreLogic.IsRedoQueueEmpty());
    }
    else
    {
        mUi->uActionUndo->setEnabled(false);
        mUi->uActionRedo->setEnabled(false);
        mUi->uUndoButton->setEnabled(false);
        mUi->uRedoButton->setEnabled(false);
        mView.GetPieMenu()->SetButtonEnabled(3, false);
        mView.GetPieMenu()->SetButtonEnabled(1, false);
    }

    if (mView.GetPieMenu()->isVisible())
    {
        mView.GetPieMenu()->update();
    }
}

void MainWindow::Undo()
{
    mCoreLogic.Undo();
    UpdateUndoRedoEnabled(true);
}

void MainWindow::Redo()
{
    mCoreLogic.Redo();
    UpdateUndoRedoEnabled(true);
}

void MainWindow::OnControlModeChanged(ControlMode pNewMode)
{
    switch (pNewMode)
    {
        case ControlMode::EDIT:
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

            mUi->uActionCut->setEnabled(false);
            mUi->uActionCopy->setEnabled(false);
            mUi->uActionPaste->setEnabled(false);
            mUi->uActionDelete->setEnabled(true);
            mUi->uActionSelectAll->setEnabled(false);

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

    FadeOutWidget(*(mUi->uTopBar), gui::FADE_ANIMATION_DURATION);
    FadeOutWidget(*(mUi->uToolboxContainer), gui::FADE_ANIMATION_DURATION);
    FadeOutWidget(*(mUi->uClockConfigurator), gui::FADE_ANIMATION_DURATION);
    FadeOutWidget(*(mUi->uItemConfigurator), gui::FADE_ANIMATION_DURATION);
    mIsGuiHidden = true;
}

void MainWindow::FadeInGui()
{
    if (mCoreLogic.IsProcessing())
    {
        return;
    }

    FadeInWidget(*(mUi->uTopBar), gui::FADE_ANIMATION_DURATION);
    if (mIsToolboxVisible && mCoreLogic.GetControlMode() != ControlMode::SIMULATION) // Keep GUI hidden if simulation started
    {
        FadeInWidget(*(mUi->uToolboxContainer), gui::FADE_ANIMATION_DURATION);
    }
    if (mIsClockConfiguratorVisible && mCoreLogic.GetControlMode() != ControlMode::SIMULATION)
    {
        FadeInWidget(*(mUi->uClockConfigurator), gui::FADE_ANIMATION_DURATION);
    }
    if (mIsItemConfiguratorVisible && mCoreLogic.GetControlMode() != ControlMode::SIMULATION)
    {
        FadeInWidget(*(mUi->uItemConfigurator), gui::FADE_ANIMATION_DURATION);
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
    QObject::connect(mUi->uToolboxTree, &QTreeView::clicked, this, [this]()
    {
        if (mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->hasChildren())
        {
            if (mUi->uToolboxTree->isExpanded(mUi->uToolboxTree->currentIndex()))
            {
                mUi->uToolboxTree->collapse(mUi->uToolboxTree->currentIndex());
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(QIcon(":/images/icons/material_symbols/expand_more_FILL0_wght600_GRAD0_opsz20.svg"));
            }
            else
            {
                mUi->uToolboxTree->expand(mUi->uToolboxTree->currentIndex());
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(QIcon(":/images/icons/material_symbols/expand_less_FILL0_wght600_GRAD0_opsz20.svg"));
            }
        }
    });

    QIcon gateIcon(":images/icons/gate24.png");
    gateIcon.addPixmap(QPixmap(":images/icons/gate24.png"), QIcon::Mode::Selected);

    QIcon inputIcon(":images/icons/input24.png");
    inputIcon.addPixmap(QPixmap(":images/icons/input24.png"), QIcon::Mode::Selected);

    QIcon buttonIcon(":images/icons/button24.png");
    buttonIcon.addPixmap(QPixmap(":images/icons/button24.png"), QIcon::Mode::Selected);

    QIcon clockIcon(":images/icons/clock24.png");
    clockIcon.addPixmap(QPixmap(":images/icons/clock24.png"), QIcon::Mode::Selected);

    QIcon constantIcon(":images/icons/constant24.png");
    constantIcon.addPixmap(QPixmap(":images/icons/constant24.png"), QIcon::Mode::Selected);

    QIcon outputIcon(":images/icons/output24.png");
    outputIcon.addPixmap(QPixmap(":images/icons/output24.png"), QIcon::Mode::Selected);

    QIcon flipflopIcon(":images/icons/flipflop24.png");
    flipflopIcon.addPixmap(QPixmap(":images/icons/flipflop24.png"), QIcon::Mode::Selected);

    QIcon fulladderIcon(":images/icons/full_adder24.png");
    fulladderIcon.addPixmap(QPixmap(":images/icons/full_adder24.png"), QIcon::Mode::Selected);

    QIcon demultiplexerIcon(":images/icons/demux24.png");
    demultiplexerIcon.addPixmap(QPixmap(":images/icons/demux24.png"), QIcon::Mode::Selected);

    QIcon encoderIcon(":images/icons/encoder24.png");
    encoderIcon.addPixmap(QPixmap(":images/icons/encoder24.png"), QIcon::Mode::Selected);

    QIcon decoderIcon(":images/icons/decoder24.png");
    decoderIcon.addPixmap(QPixmap(":images/icons/decoder24.png"), QIcon::Mode::Selected);

    QIcon labelIcon(":images/icons/label_icon.png");
    labelIcon.addPixmap(QPixmap(":images/icons/label_icon.png"), QIcon::Mode::Selected);

    // Create category and root level items
    mCategoryGatesItem = new QStandardItem(QIcon(":/images/icons/material_symbols/expand_less_FILL0_wght600_GRAD0_opsz20.svg"), "Logic Gates");
    mCategoryGatesItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryGatesItem);

    mCategoryInputsItem = new QStandardItem(QIcon(":/images/icons/material_symbols/expand_less_FILL0_wght600_GRAD0_opsz20.svg"), "Inputs");
    mCategoryInputsItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryInputsItem);

    auto outputItem = new QStandardItem(outputIcon, "Output");
    mToolboxTreeModel.appendRow(outputItem);

    mCategoryAddersItem = new QStandardItem(QIcon(":/images/icons/material_symbols/expand_less_FILL0_wght600_GRAD0_opsz20.svg"), "Adders");
    mCategoryAddersItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryAddersItem);

    mCategoryMemoryItem = new QStandardItem(QIcon(":/images/icons/material_symbols/expand_more_FILL0_wght600_GRAD0_opsz20.svg"), "Memory");
    mCategoryMemoryItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryMemoryItem);

    mCategoryConvertersItem = new QStandardItem(QIcon(":/images/icons/material_symbols/expand_more_FILL0_wght600_GRAD0_opsz20.svg"), "Converters");
    mCategoryConvertersItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryConvertersItem);

    mCategoryCustomLogicItem = new QStandardItem(QIcon(":/images/icons/material_symbols/expand_more_FILL0_wght600_GRAD0_opsz20.svg"), "Custom Logic");
    mCategoryCustomLogicItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryCustomLogicItem);

    auto textLabelItem = new QStandardItem(labelIcon, "Text Label");
    mToolboxTreeModel.appendRow(textLabelItem);

    // Create component items
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "AND Gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "OR Gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "XOR Gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "NOT Gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(gateIcon, "Buffer Gate"));

    mCategoryInputsItem->appendRow(new QStandardItem(inputIcon, "Switch"));
    mCategoryInputsItem->appendRow(new QStandardItem(buttonIcon, "Button"));
    mCategoryInputsItem->appendRow(new QStandardItem(clockIcon, "Clock"));
    mCategoryInputsItem->appendRow(new QStandardItem(constantIcon, "Constant"));

    mCategoryAddersItem->appendRow(new QStandardItem(flipflopIcon, "Half Adder"));
    mCategoryAddersItem->appendRow(new QStandardItem(fulladderIcon, "Full Adder"));

    mCategoryMemoryItem->appendRow(new QStandardItem(flipflopIcon, "RS Flip-Flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(flipflopIcon, "D Flip-Flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(flipflopIcon, "T Flip-Flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(fulladderIcon, "JK Flip-Flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(fulladderIcon, "Shift Register"));
    mCategoryMemoryItem->appendRow(new QStandardItem(demultiplexerIcon, "Counter"));

    mCategoryConvertersItem->appendRow(new QStandardItem(gateIcon, "Multiplexer"));
    mCategoryConvertersItem->appendRow(new QStandardItem(demultiplexerIcon, "Demultiplexer"));
    mCategoryConvertersItem->appendRow(new QStandardItem(decoderIcon, "Decoder"));
    mCategoryConvertersItem->appendRow(new QStandardItem(encoderIcon, "Encoder"));

    mCategoryCustomLogicItem->appendRow(new QStandardItem(gateIcon, "Custom Logic"));

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

    mConfigButtonIconVariant.insert("color", QColor(180, 180, 180));
    mConfigButtonIconVariant.insert("color-disabled", QColor(180, 180, 180));
    mConfigButtonIconVariant.insert("color-active", QColor(180, 180, 180));
    mConfigButtonIconVariant.insert("color-selected", QColor(180, 180, 180));

    mWhiteIconVariant.insert("color", QColor(255, 255, 255));
    mWhiteIconVariant.insert("color-disabled", QColor(255, 255, 255));
    mWhiteIconVariant.insert("color-active", QColor(255, 255, 255));
    mWhiteIconVariant.insert("color-selected", QColor(255, 255, 255));

    // Icons for GUI buttons
    mUi->uEditButton->SetIcon(QImage(":/images/icons/material_symbols/arrow_selector_tool_FILL1_wght400_GRAD0_opsz24.svg"));
    mUi->uWiringButton->SetIcon(QImage(":/images/icons/material_symbols/edit_FILL1_wght400_GRAD0_opsz24.svg"));
    mUi->uDeleteButton->SetIcon(QImage(":/images/icons/material_symbols/delete_FILL0_wght400_GRAD0_opsz24.svg"));
    mUi->uUndoButton->SetIcon(QImage(":/images/icons/material_symbols/undo_FILL0_wght400_GRAD0_opsz24.svg"));
    mUi->uRedoButton->SetIcon(QImage(":/images/icons/material_symbols/redo_FILL0_wght400_GRAD0_opsz24.svg"));
    mUi->uStartButton->SetIcon(QImage(":/images/icons/material_symbols/power_settings_new_FILL1_wght400_GRAD0_opsz24.svg"));
    mUi->uRunButton->SetIcon(QImage(":/images/icons/material_symbols/play_arrow_FILL1_wght400_GRAD0_opsz24.svg"));
    mUi->uPauseButton->SetIcon(QImage(":/images/icons/material_symbols/pause_FILL1_wght400_GRAD0_opsz24.svg"));
    mUi->uStepButton->SetIcon(QImage(":/images/icons/material_symbols/skip_next_FILL1_wght400_GRAD0_opsz24.svg"));
    mUi->uResetButton->SetIcon(QImage(":/images/icons/material_symbols/restart_alt_FILL0_wght400_GRAD0_opsz24.svg"));

    // Icons for configuration elements
    mUi->uItemRightButton->setIcon(mAwesome.icon(fa::arrowright, mConfigButtonIconVariant));
    mUi->uItemDownButton->setIcon(mAwesome.icon(fa::arrowdown, mConfigButtonIconVariant));
    mUi->uItemLeftButton->setIcon(mAwesome.icon(fa::arrowleft, mConfigButtonIconVariant));
    mUi->uItemUpButton->setIcon(mAwesome.icon(fa::arrowup, mConfigButtonIconVariant));

    // Icons for menu bar elements
    mUi->uActionNew->setIcon(mAwesome.icon(fa::fileo, mMenuBarIconVariant));
    mUi->uActionOpen->setIcon(mAwesome.icon(fa::folderopeno, mMenuBarIconVariant));
    mUi->uActionSave->setIcon(mAwesome.icon(fa::floppyo, mMenuBarIconVariant));

    mUi->uActionCut->setIcon(mAwesome.icon(fa::scissors, mMenuBarIconVariant));
    mUi->uActionCopy->setIcon(mAwesome.icon(fa::copy, mMenuBarIconVariant));
    mUi->uActionPaste->setIcon(mAwesome.icon(fa::clipboard, mMenuBarIconVariant));
    mUi->uActionDelete->setIcon(mAwesome.icon(fa::trasho, mMenuBarIconVariant));

    mUi->uActionStartTutorial->setIcon(mAwesome.icon(fa::graduationcap, mMenuBarIconVariant));
    mUi->uActionReportBugs->setIcon(mAwesome.icon(fa::bug, mMenuBarIconVariant));
    mUi->uActionOpenWebsite->setIcon(mAwesome.icon(fa::externallink, mMenuBarIconVariant));
    //mUi->uActionOpenTwitter->setIcon(mAwesome.icon(fa::twitter, mMenuBarIconVariant));
    mUi->uActionOpenGithub->setIcon(mAwesome.icon(fa::github, mMenuBarIconVariant));
    mUi->uActionAbout->setIcon(mAwesome.icon(fa::info, mMenuBarIconVariant));
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
       SetShiftRegisterBitWidthIfAllowed(0);
       SetCounterBitWidthIfAllowed(1);
    });
    QObject::connect(mTwoGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(2);
       SetEncoderDecoderInputCountIfAllowed(2);
       SetMultiplexerBitWidthIfAllowed(2);
       SetShiftRegisterBitWidthIfAllowed(1);
       SetCounterBitWidthIfAllowed(2);
    });
    QObject::connect(mThreeGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(3);
       SetEncoderDecoderInputCountIfAllowed(3);
       SetMultiplexerBitWidthIfAllowed(3);
       SetShiftRegisterBitWidthIfAllowed(2);
       SetCounterBitWidthIfAllowed(3);
    });
    QObject::connect(mFourGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(4);
       SetEncoderDecoderInputCountIfAllowed(4);
       SetMultiplexerBitWidthIfAllowed(4);
       SetShiftRegisterBitWidthIfAllowed(3);
       SetCounterBitWidthIfAllowed(4);
    });
    QObject::connect(mFiveGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(5);
       SetEncoderDecoderInputCountIfAllowed(5);
       SetMultiplexerBitWidthIfAllowed(5);
       SetShiftRegisterBitWidthIfAllowed(4);
       SetCounterBitWidthIfAllowed(5);
    });
    QObject::connect(mSixGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(6);
       SetEncoderDecoderInputCountIfAllowed(6);
       SetMultiplexerBitWidthIfAllowed(6);
       SetCounterBitWidthIfAllowed(6);
    });
    QObject::connect(mSevenGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(7);
       SetEncoderDecoderInputCountIfAllowed(7);
       SetMultiplexerBitWidthIfAllowed(7);
       SetCounterBitWidthIfAllowed(7);
    });
    QObject::connect(mEightGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(8);
       SetEncoderDecoderInputCountIfAllowed(8);
       SetMultiplexerBitWidthIfAllowed(8);
       SetCounterBitWidthIfAllowed(8);
    });
    QObject::connect(mNineGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetGateInputCountIfAllowed(9);
       SetEncoderDecoderInputCountIfAllowed(9);
       SetMultiplexerBitWidthIfAllowed(9);
       SetCounterBitWidthIfAllowed(9);
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
            FadeInWidget(*(mUi->uToolboxContainer), gui::FADE_ANIMATION_DURATION);
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
            mUi->uLabelMultiplexerBitWidth->setText(tr(pBitWidth > 1 ? "%0 Bits" : "%0 Bit").arg(pBitWidth));
            mUi->uMultiplexerBitWidthSlider->setValue(pBitWidth);
        }
    }
}

void MainWindow::SetShiftRegisterBitWidthIfAllowed(uint8_t pBitWidthIndex)
{
    static const std::vector<uint8_t> values{2, 4, 8, 16, 32};

    if (pBitWidthIndex < values.size())
    {
        if (mCoreLogic.GetControlMode() == ControlMode::ADD && mCoreLogic.GetSelectedComponentType() == ComponentType::SHIFTREGISTER)
        {
            mCoreLogic.SetShiftRegisterBitWidth(values[pBitWidthIndex]);
            mUi->uShiftRegisterWidthBox->setCurrentText(tr("%0 Bits").arg(values[pBitWidthIndex]));
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

void MainWindow::SetOutputColorIfInAddMode(OutputColor pColor)
{
    if (mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.SetOutputColor(pColor);
    }
}

void MainWindow::SetConstantStateIfAllowed(LogicState pState)
{
    if (mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.SetConstantState(pState);
    }
}

void MainWindow::SetCounterBitWidthIfAllowed(uint8_t pBitWidth)
{
    if (pBitWidth >= components::counter::MIN_BIT_WIDTH && pBitWidth <= components::counter::MAX_BIT_WIDTH)
    {
        if (mCoreLogic.GetControlMode() == ControlMode::ADD && (mCoreLogic.GetSelectedComponentType() == ComponentType::COUNTER))
        {
            mCoreLogic.SetCounterBitWidth(pBitWidth);
            mUi->uLabelCounterBitWidth->setText(tr("%0 Bits").arg(pBitWidth));
            mUi->uCounterBitWidthSlider->setValue(pBitWidth);
        }
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
                    case 3: // Constant
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::CONSTANT);
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
                    case 4: // Shift Register
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::SHIFTREGISTER);
                        break;
                    }
                    case 5: // Synchronous counter
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::COUNTER);
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
            case 6: // Custom logic
            {
                switch(pIndex.row())
                {
                case 0: // Custom logic
                {
                    mCoreLogic.EnterAddControlMode(ComponentType::CUSTOM_LOGIC);
                    break;
                }
                default:
                {
                    qDebug() << "Unknown custom logic";
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
