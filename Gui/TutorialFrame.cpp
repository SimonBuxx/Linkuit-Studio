#include "TutorialFrame.h"
#include "ui_TutorialFrame.h"

#include <QFile>

TutorialFrame::TutorialFrame(QWidget *pParent) :
    QWidget(pParent),
    mUi(new Ui::TutorialFrame)
{
    mUi->setupUi(this);

    mUi->uDeclinedFrame->hide();

    QObject::connect(mUi->pushButton, &QPushButton::clicked, this, &TutorialFrame::NextStep);

    hide();
}

TutorialFrame::~TutorialFrame()
{
    delete mUi;
}

void TutorialFrame::Init(QtAwesome &pAwesome)
{
    QVariantMap iconVariant;
    iconVariant.insert("color", QColor(255, 0, 0));
    mUi->uDeclinedIcon->setPixmap(pAwesome.icon(fa::exclamationcircle, iconVariant).pixmap(20, 20));
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

void TutorialFrame::SetCurrentStep(uint8_t pStep)
{
    mCurrentStep = pStep;

    show();

    if (mCurrentStep > mNumberOfSteps)
    {
        mCurrentStep = 0;
        hide();
        return;
    }

    const auto filename = QString(":/tutorial/step%0.html").arg(mCurrentStep);
    const auto html = LoadTutorialFile(filename);
    mUi->uTutorialText->setHtml(html.value_or(tr("Could not load tutorial file %0").arg(filename)));
    mUi->uTutorialText->setFixedHeight(mUi->uTutorialText->document()->size().height());
    this->setFixedHeight(mUi->uTutorialText->document()->size().height() + 110);

    if (mCurrentStep == mNumberOfSteps)
    {
        mUi->pushButton->setText("Finish Tutorial");
    }
    else
    {
        mUi->pushButton->setText("Next Step");
    }

    emit CurrentStepChangedSignal(mCurrentStep);
}

void TutorialFrame::NextStep()
{
    emit AdvanceStepRequestSignal(mCurrentStep);
}

void TutorialFrame::StartTutorial()
{
    SetCurrentStep(1);
}

void TutorialFrame::OnAdvanceStepApproved(uint8_t pStep)
{
    if (pStep == mCurrentStep)
    {
        SetCurrentStep(++mCurrentStep);
        mUi->uDeclinedFrame->hide();
    }
    else
    {
        throw std::logic_error("Tutorial step advancing approved for other than the current tutorial step");
    }

}

void TutorialFrame::OnAdvanceStepDeclined(uint8_t pStep)
{
    if (pStep == mCurrentStep)
    {
        mUi->uDeclinedFrame->show();
    }
    else
    {
        throw std::logic_error("Tutorial step advancing declined for other than the current tutorial step");
    }

}
