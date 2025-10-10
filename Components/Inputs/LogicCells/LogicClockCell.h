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

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retrieve
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

    /// \brief Sets the toggle speed of this clock logic cell
    /// \param pTicks: The new toggle speed in ticks
    void SetToggleTicks(uint32_t pTicks);

    /// \brief Sets the pulse duration of this clock logic cell
    /// \param pTicks: The new pulse duration in ticks
    void SetPulseTicks(uint32_t pTicks);

    /// \brief Sets the clock mode of this clock logic cell
    /// \param pMode: The new clock mode (toggle or pulse)
    void SetClockMode(ClockMode pMode);

    /// \brief Getter for the toggle speed of this clock logic cell
    /// \return The toggle speed in ticks
    uint32_t GetToggleTicks(void);

    /// \brief Getter for the pulse duration of this clock logic cell
    /// \return The pulse duration in ticks
    uint32_t GetPulseTicks(void);

    /// \brief Getter for the clock mode of this clock logic cell
    /// \return The clock mode (toggle or pulse)
    ClockMode GetClockMode(void);

public slots:
    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    uint32_t mToggleTicks;
    uint32_t mPulseTicks;

    uint32_t mTickCountdown;
    uint32_t mPulseCountdown;

    ClockMode mMode;
};

#endif // LOGICCLOCKCELL_H
