#include "TutorialFrame.h"
#include "ui_TutorialFrame.h"

#include <QFile>

#include "HelperFunctions.h"

TutorialFrame::TutorialFrame(QWidget *pParent) :
    QWidget(pParent),
    mUi(new Ui::TutorialFrame)
{
    mUi->setupUi(this);

    mUi->uDeclinedFrame->hide();

    QObject::connect(mUi->uContinueButton, &QPushButton::clicked, this, [&]()
    {
        emit AdvanceStepRequestSignal(mCurrentStep);
    });
    QObject::connect(mUi->uQuitButton, &QPushButton::clicked, this, [this](){
        FadeOutWidget(*this, gui::FADE_ANIMATION_DURATION);
    });

    auto image = QImage(":/images/icons/material_symbols/check_circle_FILL1_wght400_GRAD0_opsz20.svg");
    image.invertPixels();

    mWhiteCheckIcon = QIcon(QPixmap::fromImage(image));

    hide();
}

TutorialFrame::~TutorialFrame()
{
    delete mUi;
}

void TutorialFrame::SetTopLeftPosition(QPoint pPos)
{
    QRect geometry(pPos, size());
    setGeometry(geometry);
}

void TutorialFrame::SetTopRightPosition(QPoint pPos)
{
    QRect geometry(pPos - QPoint(size().width(), 0), size());
    setGeometry(geometry);
}

void TutorialFrame::SetCenterPosition(QPoint pPos)
{
    QRect geometry(pPos - QPoint(size().width() / 2, size().height() / 2), size());
    setGeometry(geometry);
}

std::optional<QString> TutorialFrame::LoadTutorialFile(const QString& pFilename)
{
    QFile file(pFilename);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);
        const auto data = stream.readAll();
        file.close();
        return data;
    }
    return std::nullopt;
}

void TutorialFrame::SetCurrentStep(TutorialStep pStep)
{
    mCurrentStep = pStep;

    if (mCurrentStep == TutorialStep::NONE)
    {
        FadeOutWidget(*this, gui::FADE_ANIMATION_DURATION);
        return;
    }

    mUi->uDeclinedFrame->hide();

    FadeInWidget(*this, gui::FADE_ANIMATION_DURATION);

    Q_ASSERT(HAS_AUTO_ADVANCE.size() >= mCurrentStep);

    UpdateContinueButton(!HAS_AUTO_ADVANCE[mCurrentStep - 1]);

    const auto filename = QString(":/tutorial/step%0.html").arg(mCurrentStep);
    const auto html = LoadTutorialFile(filename);
    mUi->uTutorialText->setHtml(html.value_or(tr("Could not load tutorial file %0").arg(filename)));
    mUi->uTutorialText->setFixedHeight(mUi->uTutorialText->document()->size().height());
    this->setFixedHeight(mUi->uTutorialText->document()->size().height() + 110);

    emit CurrentStepChangedSignal(mCurrentStep);
}

void TutorialFrame::UpdateContinueButton(bool pEnabled)
{
    if (mCurrentStep == mNumberOfSteps)
    {
        mUi->uContinueButton->setText("Finish");
    }
    else
    {
        mUi->uContinueButton->setText("Continue");
    }

    if (pEnabled)
    {
        mUi->uContinueButton->setIcon(mWhiteCheckIcon);
    }
    else
    {
        mUi->uContinueButton->setIcon(QIcon(":/images/icons/material_symbols/cancel_FILL1_wght400_GRAD0_opsz20.svg"));
    }

    mUi->uContinueButton->setEnabled(pEnabled);
}

void TutorialFrame::StartTutorial()
{
    SetCurrentStep(TutorialStep::INTRODUCTION);
}

uint8_t TutorialFrame::GetCurrentStep(void) const
{
    return mCurrentStep;
}

void TutorialFrame::ApproveStepOnCondition(TutorialStep pStep)
{
    Q_ASSERT(pStep == mCurrentStep);
    UpdateContinueButton(true);
}

void TutorialFrame::OnAdvanceStepApproved(TutorialStep pStep)
{
    Q_ASSERT(pStep == mCurrentStep);
    uint8_t step = static_cast<uint8_t>(mCurrentStep);
    SetCurrentStep(step < mNumberOfSteps ? static_cast<TutorialStep>(++step) : TutorialStep::NONE);
}

void TutorialFrame::OnAdvanceStepDeclined(TutorialStep pStep)
{
    Q_ASSERT(pStep == mCurrentStep);
    mUi->uDeclinedFrame->show();
}
