#ifndef ICONTOOLBUTTON_H
#define ICONTOOLBUTTON_H

#include <QPushButton>

#include "QtAwesome.h"

///
/// \brief The IconToolButton class represents a tool button with associated icons
///
class IconToolButton : public QPushButton
{
    Q_OBJECT
public:
    /// \brief Constructor for IconToolButton
    /// \param pParent: Pointer to the parent widget
    IconToolButton(QWidget *pParent = nullptr);

    /// \brief Setter for the icon that is displayed when the button is checked
    /// \param pIcon: Icon to display
    void SetCheckedIcon(const QIcon &pIcon);

    /// \brief Setter for the icon that is displayed when the button is unchecked
    /// \param pIcon: Icon to display
    void SetUncheckedIcon(const QIcon &pIcon);

    /// \brief Setter for the icon that is displayed when the button is not checkable
    /// \param pIcon: Icon to display
    void SetIcon(const QIcon &pIcon);

protected:
    /// \brief Updates the icon based on the buttons state
    /// \param pChecked
    void UpdateIcon(bool pChecked);

protected:
    QIcon mCheckedIcon;
    QIcon mUncheckedIcon;
};

#endif // ICONTOOLBUTTON_H
