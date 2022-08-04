#ifndef LOGICBUTTONCELL_H
#define LOGICBUTTONCELL_H

#include "Components/LogicBaseCell.h"

#include <QTimer>

///
/// \brief Logic cell class for the button input
///
class LogicButtonCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicButtonCell
    LogicButtonCell(void);

    /// \brief Sets the button state to HIGH and starts the timer
    void ButtonClick(void);

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    /// \brief Sets the button state to LOW
    void ButtonTimeout(void);

protected:
    LogicState mState;
    bool mStateChanged;

    QTimer mButtonTimer;
};

#endif // LOGICBUTTONCELL_H
