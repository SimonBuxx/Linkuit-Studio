#ifndef ICONTOOLBUTTON_H
#define ICONTOOLBUTTON_H

#include <QPushButton>

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

    /// \brief Setter for the icon that is displayed when the button is not checkable
    /// \param pImage: Image for the icon to display
    void SetIcon(const QImage &pImage);

protected:
    /// \brief Updates the icon based on the buttons state
    /// \param pChecked
    void UpdateIcon(bool pChecked);

protected:
    QIcon mCheckedIcon;
    QIcon mUncheckedIcon;
};

#endif // ICONTOOLBUTTON_H
