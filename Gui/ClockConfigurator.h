#ifndef CLOCKCONFIGURATOR_H
#define CLOCKCONFIGURATOR_H

#include "HelperStructures.h"

#include <QWidget>

namespace Ui {
class ClockConfigurator;
}

class ClockConfigurator : public QWidget
{
    Q_OBJECT

public:
    explicit ClockConfigurator(QWidget *pParent = nullptr);

    void Configure(ClockMode pMode, uint32_t pToggle, uint32_t pPulse);

    ~ClockConfigurator(void);

signals:
    void ClockModeChangedSignal(ClockMode pMode);

    void ToggleValueChangedSignal(uint32_t pValue);
    void PulseValueChangedSignal(uint32_t pValue);

protected slots:
    /// \brief Show or hides the pulse value box and forwards the new clock mode
    /// \param pChecked: True, if the toggle button is checked
    void OnToggleButtonToggled(bool pChecked);

protected:
    Ui::ClockConfigurator *mUi;
};

#endif // CLOCKCONFIGURATOR_H
