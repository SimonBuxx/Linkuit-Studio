#ifndef LOGICCONSTANTCELL_H
#define LOGICCONSTANTCELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic cell class for the HIGH/LOW constant
///
class LogicConstantCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicConstantCell
    LogicConstantCell(LogicState pConstantState);

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retrieve
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

    /// \brief Getter for this cell's constant state
    /// \return the constant logic state of this logic cell
    LogicState GetConstantState(void) const;

public slots:
    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    LogicState mConstantState;
};

#endif // LOGICCONSTANTCELL_H
