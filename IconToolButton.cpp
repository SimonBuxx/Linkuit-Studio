#include "IconToolButton.h"

IconToolButton::IconToolButton(QWidget *pParent) :
    QPushButton(pParent)
{
    QObject::connect(this, &QAbstractButton::toggled, this, &IconToolButton::UpdateIcon);
}

void IconToolButton::SetCheckedIcon(const QIcon &pIcon)
{
    mCheckedIcon = pIcon;
    UpdateIcon(isChecked());
}

void IconToolButton::SetUncheckedIcon(const QIcon &pIcon)
{
    mUncheckedIcon = pIcon;
    UpdateIcon(isChecked());
}

void IconToolButton::SetIcon(const QIcon &pIcon)
{
    mUncheckedIcon = pIcon;
    UpdateIcon(false);
}

void IconToolButton::UpdateIcon(bool pChecked)
{
    if (pChecked && isCheckable())
    {
        this->setIcon(mCheckedIcon);
    }
    else
    {
        this->setIcon(mUncheckedIcon);
    }
}
