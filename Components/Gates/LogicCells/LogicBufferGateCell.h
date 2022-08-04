#ifndef LOGICBUFFERGATECELL_H
#define LOGICBUFFERGATECELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic cell class for the buffer gate
///
class LogicBufferGateCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicBufferGateCell
    LogicBufferGateCell(void);

    /// \brief The logic function that determines the output states based on the inputs
    void LogicFunction(void) override;

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    /// \brief Advances the simulation of this cell by one logic tick
    void OnSimulationAdvance(void) override;

    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    LogicState mPreviousState;
    LogicState mCurrentState;
    bool mStateChanged;
};

#endif // LOGICBUFFERGATECELL_H
