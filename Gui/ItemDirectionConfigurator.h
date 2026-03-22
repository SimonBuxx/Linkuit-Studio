#ifndef ITEMDIRECTIONCONFIGURATOR_H
#define ITEMDIRECTIONCONFIGURATOR_H

#include <QWidget>

#include "HelperStructures.h"

namespace Ui {
class ItemDirectionConfigurator;
}

class ItemDirectionConfigurator : public QWidget
{
    Q_OBJECT

public:
    explicit ItemDirectionConfigurator(QWidget *pParent = nullptr);

    void SetUncheckedIcons(const QIcon& pRight, const QIcon& pDown, const QIcon& pLeft, const QIcon& pUp);
    void SetCheckedIcons(const QIcon& pRight, const QIcon& pDown, const QIcon& pLeft, const QIcon& pUp);
    void InitIcons(void);

    ~ItemDirectionConfigurator(void);

signals:
    void ComponentDirectionChangedSignal(Direction pDirection);

protected slots:
    /// \brief Sets the direction for new items to right if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemRightButtonToggled(bool pChecked);

    /// \brief Sets the direction for new items to down if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemDownButtonToggled(bool pChecked);

    /// \brief Sets the direction for new items to left if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemLeftButtonToggled(bool pChecked);

    /// \brief Sets the direction for new items to up if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemUpButtonToggled(bool pChecked);

protected:
    Ui::ItemDirectionConfigurator *mUi;

    std::array<QIcon, 4> mUncheckedIcons;
    std::array<QIcon, 4> mCheckedIcons;
};

#endif // ITEMDIRECTIONCONFIGURATOR_H
