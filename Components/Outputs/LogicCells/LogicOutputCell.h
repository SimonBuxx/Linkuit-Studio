#ifndef LOGICOUTPUTCELL_H
#define LOGICOUTPUTCELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic cell class for the logic output
///
class LogicOutputCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicOutputCell
    LogicOutputCell(void);

    /// \brief The logic function that determines the output states based on the inputs
    void LogicFunction(void) override;

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retrieve
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

#endif // LOGICOUTPUTCELL_H
