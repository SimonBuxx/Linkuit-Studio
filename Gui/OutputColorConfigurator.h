#ifndef OUTPUTCOLORCONFIGURATOR_H
#define OUTPUTCOLORCONFIGURATOR_H

#include <QWidget>

#include "HelperStructures.h"

namespace Ui {
class OutputColorConfigurator;
}

class OutputColorConfigurator : public QWidget
{
    Q_OBJECT

public:
    explicit OutputColorConfigurator(QWidget *pParent = nullptr);
    ~OutputColorConfigurator(void);

signals:
    void OutputColorChangedSignal(OutputColor pColor);

protected slots:
    /// \brief Sets the output color for new outputs to default if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnOutputColorDefaultButtonToggled(bool pChecked);

    /// \brief Sets the output color for new outputs to green if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnOutputColorGreenButtonToggled(bool pChecked);

    /// \brief Sets the output color for new outputs to red if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnOutputColorRedButtonToggled(bool pChecked);

    /// \brief Sets the output color for new outputs to blue if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnOutputColorBlueButtonToggled(bool pChecked);

    /// \brief Sets the output color for new outputs to yellow if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnOutputColorYellowButtonToggled(bool pChecked);

protected:
    Ui::OutputColorConfigurator *mUi;
};

#endif // OUTPUTCOLORCONFIGURATOR_H
