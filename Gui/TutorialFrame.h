#ifndef TUTORIALFRAME_H
#define TUTORIALFRAME_H

#include "QtAwesome.h"

#include <QWidget>

namespace Ui {
class TutorialFrame;
}

class TutorialFrame : public QWidget
{
    Q_OBJECT

public:
    explicit TutorialFrame(QWidget *pParent = nullptr);
    ~TutorialFrame(void) override;

    void Init(QtAwesome &pAwesome);

    void SetTopLeftPosition(QPoint pPos);

    void SetTopRightPosition(QPoint pPos);

    void SetCenterPosition(QPoint pPos);

    void SetCurrentStep(uint8_t pStep);

    void NextStep(void);

    void StartTutorial(void);

    void OnAdvanceStepApproved(uint8_t pStep);
    void OnAdvanceStepDeclined(uint8_t pStep);

signals:
    void AdvanceStepRequestSignal(uint8_t pStep);

    void CurrentStepChangedSignal(uint8_t pStep);

protected:
    std::optional<QString> LoadTutorialFile(const QString &pFilename);

protected:
    Ui::TutorialFrame *mUi;

    uint8_t mCurrentStep = 0;
    uint8_t mNumberOfSteps = 3;
};

#endif // TUTORIALFRAME_H
