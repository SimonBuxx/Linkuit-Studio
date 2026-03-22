#include "OutputColorConfigurator.h"
#include "ui_OutputColorConfigurator.h"

OutputColorConfigurator::OutputColorConfigurator(QWidget *pParent)
    : QWidget(pParent)
    , mUi(new Ui::OutputColorConfigurator)
{
    mUi->setupUi(this);

    QObject::connect(mUi->uOutputColorDefaultButton, &QPushButton::toggled, this, &OutputColorConfigurator::OnOutputColorDefaultButtonToggled);
    QObject::connect(mUi->uOutputColorGreenButton, &QPushButton::toggled, this, &OutputColorConfigurator::OnOutputColorGreenButtonToggled);
    QObject::connect(mUi->uOutputColorRedButton, &QPushButton::toggled, this, &OutputColorConfigurator::OnOutputColorRedButtonToggled);
    QObject::connect(mUi->uOutputColorBlueButton, &QPushButton::toggled, this, &OutputColorConfigurator::OnOutputColorBlueButtonToggled);
    QObject::connect(mUi->uOutputColorYellowButton, &QPushButton::toggled, this, &OutputColorConfigurator::OnOutputColorYellowButtonToggled);

    mUi->uOutputColorDefaultButton->setChecked(true);
}

void OutputColorConfigurator::OnOutputColorDefaultButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        emit OutputColorChangedSignal(OutputColor::DEFAULT);
    }
}

void OutputColorConfigurator::OnOutputColorGreenButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        emit OutputColorChangedSignal(OutputColor::GREEN);
    }
}

void OutputColorConfigurator::OnOutputColorRedButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        emit OutputColorChangedSignal(OutputColor::RED);
    }
}

void OutputColorConfigurator::OnOutputColorBlueButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        emit OutputColorChangedSignal(OutputColor::BLUE);
    }
}

void OutputColorConfigurator::OnOutputColorYellowButtonToggled(bool pChecked)
{
    if (pChecked)
    {
        emit OutputColorChangedSignal(OutputColor::YELLOW);
    }
}

OutputColorConfigurator::~OutputColorConfigurator()
{
    delete mUi;
}
