#include "ItemDirectionConfigurator.h"
#include "ui_ItemDirectionConfigurator.h"

ItemDirectionConfigurator::ItemDirectionConfigurator(QWidget *pParent)
    : QWidget(pParent)
    , mUi(new Ui::ItemDirectionConfigurator)
{
    mUi->setupUi(this);

    QObject::connect(mUi->uItemRightButton, &QPushButton::toggled, this, &ItemDirectionConfigurator::OnItemRightButtonToggled);
    QObject::connect(mUi->uItemDownButton, &QPushButton::toggled, this, &ItemDirectionConfigurator::OnItemDownButtonToggled);
    QObject::connect(mUi->uItemLeftButton, &QPushButton::toggled, this, &ItemDirectionConfigurator::OnItemLeftButtonToggled);
    QObject::connect(mUi->uItemUpButton, &QPushButton::toggled, this, &ItemDirectionConfigurator::OnItemUpButtonToggled);

    mUi->uItemRightButton->setChecked(true);
}

void ItemDirectionConfigurator::SetUncheckedIcons(const QIcon& pRight, const QIcon& pDown, const QIcon& pLeft, const QIcon& pUp)
{

    mUncheckedIcons[static_cast<int32_t>(Direction::RIGHT)] = pRight;
    mUncheckedIcons[static_cast<int32_t>(Direction::DOWN)] = pDown;
    mUncheckedIcons[static_cast<int32_t>(Direction::LEFT)] = pLeft;
    mUncheckedIcons[static_cast<int32_t>(Direction::UP)] = pUp;
}

void ItemDirectionConfigurator::SetCheckedIcons(const QIcon& pRight, const QIcon& pDown, const QIcon& pLeft, const QIcon& pUp)
{
    mCheckedIcons[static_cast<int32_t>(Direction::RIGHT)] = pRight;
    mCheckedIcons[static_cast<int32_t>(Direction::DOWN)] = pDown;
    mCheckedIcons[static_cast<int32_t>(Direction::LEFT)] = pLeft;
    mCheckedIcons[static_cast<int32_t>(Direction::UP)] = pUp;
}

void ItemDirectionConfigurator::InitIcons()
{
    mUi->uItemRightButton->setIcon(mCheckedIcons[static_cast<int32_t>(Direction::RIGHT)]);
    mUi->uItemDownButton->setIcon(mUncheckedIcons[static_cast<int32_t>(Direction::DOWN)]);
    mUi->uItemLeftButton->setIcon(mUncheckedIcons[static_cast<int32_t>(Direction::LEFT)]);
    mUi->uItemUpButton->setIcon(mUncheckedIcons[static_cast<int32_t>(Direction::UP)]);
}

void ItemDirectionConfigurator::OnItemRightButtonToggled(bool pChecked)
{
    mUi->uItemRightButton->setIcon(pChecked ? mCheckedIcons[static_cast<int32_t>(Direction::RIGHT)] : mUncheckedIcons[static_cast<int32_t>(Direction::RIGHT)]);
    if (pChecked)
    {
        emit ComponentDirectionChangedSignal(Direction::RIGHT);
    }
}

void ItemDirectionConfigurator::OnItemDownButtonToggled(bool pChecked)
{
    mUi->uItemDownButton->setIcon(pChecked ? mCheckedIcons[static_cast<int32_t>(Direction::DOWN)] : mUncheckedIcons[static_cast<int32_t>(Direction::DOWN)]);
    if (pChecked)
    {
        emit ComponentDirectionChangedSignal(Direction::DOWN);
    }
}

void ItemDirectionConfigurator::OnItemLeftButtonToggled(bool pChecked)
{
    mUi->uItemLeftButton->setIcon(pChecked ? mCheckedIcons[static_cast<int32_t>(Direction::LEFT)] : mUncheckedIcons[static_cast<int32_t>(Direction::LEFT)]);
    if (pChecked)
    {
        emit ComponentDirectionChangedSignal(Direction::LEFT);
    }
}

void ItemDirectionConfigurator::OnItemUpButtonToggled(bool pChecked)
{
    mUi->uItemUpButton->setIcon(pChecked ? mCheckedIcons[static_cast<int32_t>(Direction::UP)] : mUncheckedIcons[static_cast<int32_t>(Direction::UP)]);
    if (pChecked)
    {
        emit ComponentDirectionChangedSignal(Direction::UP);
    }
}

ItemDirectionConfigurator::~ItemDirectionConfigurator()
{
    delete mUi;
}
