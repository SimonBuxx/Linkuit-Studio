#ifndef TUTORIALFRAME_H
#define TUTORIALFRAME_H

#include <QWidget>
#include <array>

#include "HelperStructures.h"

namespace Ui {
class TutorialFrame;
}
static constexpr auto NUMBER_OF_STEPS = 5;
static const std::array<bool, NUMBER_OF_STEPS> HAS_AUTO_ADVANCE{false, true, true, true, true};

class TutorialFrame : public QWidget
{
    Q_OBJECT

public:
    /// \brief Constructor for TutorialFrame
    /// \param pParent: Reference to the parent widget
    explicit TutorialFrame(QWidget *pParent = nullptr);

    /// \brief Default destructor for TutorialFrame
    ~TutorialFrame(void) override;

    void SetTopLeftPosition(QPoint pPos);
    void SetTopRightPosition(QPoint pPos);
    void SetCenterPosition(QPoint pPos);

    void StartTutorial(void);

    void OnAdvanceStepApproved(TutorialStep pStep);
    void OnAdvanceStepDeclined(TutorialStep pStep);

    uint8_t GetCurrentStep(void) const;

    void ApproveStepOnCondition(TutorialStep pStep);

signals:
    void AdvanceStepRequestSignal(TutorialStep pStep);

    void CurrentStepChangedSignal(TutorialStep pStep);

protected:
    void SetCurrentStep(TutorialStep pStep);

    void UpdateContinueButton(bool pEnabled);

    std::optional<QString> LoadTutorialFile(const QString &pFilename);

protected:
    Ui::TutorialFrame *mUi;

    TutorialStep mCurrentStep = TutorialStep::NONE;
    uint8_t mNumberOfSteps = NUMBER_OF_STEPS;

    QIcon mWhiteCheckIcon;
};

#endif // TUTORIALFRAME_H
