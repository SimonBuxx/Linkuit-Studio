#include "ClockConfigurator.h"
#include "ui_ClockConfigurator.h"

ClockConfigurator::ClockConfigurator(QWidget *pParent)
    : QWidget(pParent)
    , mUi(new Ui::ClockConfigurator)
{
    mUi->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true); // Force background styling

    QObject::connect(mUi->uButtonToggle, &QPushButton::toggled, this, &ClockConfigurator::OnToggleButtonToggled);
    QObject::connect(mUi->uToggleSpinBox, &QSpinBox::editingFinished, this, [this](){
        mUi->uPulseSpinBox->setMaximum(mUi->uToggleSpinBox->value());
        emit ToggleValueChangedSignal(mUi->uToggleSpinBox->value());
        emit PulseValueChangedSignal(mUi->uPulseSpinBox->value());
    });
    QObject::connect(mUi->uPulseSpinBox, &QSpinBox::editingFinished, this, [this](){
        emit PulseValueChangedSignal(mUi->uPulseSpinBox->value());
    });
}

void ClockConfigurator::OnToggleButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        // Set to toggle
        mUi->uPulseFrame->hide();
        emit ClockModeChangedSignal(ClockMode::TOGGLE);
    }
    else
    {
        // Set to pulse
        mUi->uPulseFrame->show();
        emit ClockModeChangedSignal(ClockMode::PULSE);
    }
}

void ClockConfigurator::Configure(ClockMode pMode, uint32_t pToggle, uint32_t pPulse)
{
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

ClockConfigurator::~ClockConfigurator()
{
    delete mUi;
}
