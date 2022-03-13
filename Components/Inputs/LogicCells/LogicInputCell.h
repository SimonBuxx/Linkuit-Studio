#ifndef LOGICINPUTCELL_H
#define LOGICINPUTCELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic cell class for the switch input
///
class LogicInputCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicInputCell
    LogicInputCell(void);

    /// \brief Toggles the input's logic state
    void ToggleState(void);

    /// \brief Getter for the curent output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    LogicState mState;
};

#endif // LOGICINPUTCELL_H
