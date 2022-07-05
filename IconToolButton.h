#ifndef ICONTOOLBUTTON_H
#define ICONTOOLBUTTON_H

#include <QPushButton>

#include "QtAwesome.h"

class IconToolButton : public QPushButton
{
    Q_OBJECT
public:
    IconToolButton(QWidget *pParent = nullptr);

    void SetCheckedIcon(const QIcon &pIcon);
    void SetUncheckedIcon(const QIcon &pIcon);
    void SetIcon(const QIcon &pIcon);

protected:
    void UpdateIcon(bool pChecked);

protected:
    QIcon mCheckedIcon;
    QIcon mUncheckedIcon;
};

#endif // ICONTOOLBUTTON_H
