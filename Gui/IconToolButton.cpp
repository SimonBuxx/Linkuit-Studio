#include "IconToolButton.h"

#include <QPainter>

IconToolButton::IconToolButton(QWidget *pParent) :
    QPushButton(pParent)
{
    QObject::connect(this, &QAbstractButton::toggled, this, &IconToolButton::UpdateIcon);
}

void IconToolButton::SetIcon(const QImage &pImage)
{
    mUncheckedIcon = QIcon(QPixmap::fromImage(pImage));

    {
        auto image = pImage;
        image.invertPixels();

        mCheckedIcon = QIcon(QPixmap::fromImage(image));
    }

    {
        auto img = pImage;
        img.fill(Qt::transparent);

        QPainter painter(&img);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.setOpacity(0.1);
        painter.drawImage(0, 0, pImage);

        mUncheckedIcon.addPixmap(QPixmap::fromImage(img), QIcon::Disabled);
        mCheckedIcon.addPixmap(QPixmap::fromImage(img), QIcon::Disabled);
    }

    UpdateIcon(isChecked());
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
