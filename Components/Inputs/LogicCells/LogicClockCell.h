#ifndef LOGICCLOCKCELL_H
#define LOGICCLOCKCELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic cell class for the clock input
///
class LogicClockCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicClockCell
    LogicClockCell(void);

    /// \brief The logic function that determines the output states based on the inputs
    void LogicFunction(void) override;

    /// \brief Getter for the curent output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

#warning missing documentation
    void SetToggleTicks(uint32_t pTicks);
    void SetPulseTicks(uint32_t pTicks);
    void SetClockMode(ClockMode pMode);

    uint32_t GetToggleTicks(void);
    uint32_t GetPulseTicks(void);
    ClockMode GetClockMode(void);

public slots:
    /// \brief Advances the simulation of this cell by one logic tick
    void OnSimulationAdvance(void) override;

    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    LogicState mState;

    uint32_t mToggleTicks;
    uint32_t mPulseTicks;

    uint32_t mTickCountdown;
    uint32_t mPulseCountdown;

    bool mStateChanged;

    ClockMode mMode;
};

#endif // LOGICCLOCKCELL_H
