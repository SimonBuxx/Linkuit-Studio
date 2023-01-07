#include "PieMenu.h"
#include <QPainter>
#include <QApplication>

PieMenu::PieMenu(QtAwesome &pAwesome, QWidget *pParent):
    QWidget(pParent),
    mButtonIcons{5, QIcon()},
    mButtonDisabledIcons{5, QIcon()},
    mButtonPaths{4, QPainterPath()},
    mAwesome(pAwesome),
    mButtonsEnabled(5, true)
{
    setMinimumSize(202, 202);
    setMaximumSize(202, 202);
    setMouseTracking(true);

    mStandardIconVariant.insert("color", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mStandardIconVariant.insert("color-active", QColor(0, 45, 50));
    mStandardIconVariant.insert("color-selected", QColor(0, 45, 50));

    mButtonPaths[0].moveTo(101, 101);
    mButtonPaths[0].arcTo(QRectF(1, 1, 200, 200), 135, 90);

    mButtonPaths[1].moveTo(101, 101);
    mButtonPaths[1].arcTo(QRectF(1, 1, 200, 200), 45, 90);

    mButtonPaths[2].moveTo(101, 101);
    mButtonPaths[2].arcTo(QRectF(1, 1, 200, 200), -45, 90);

    mButtonPaths[3].moveTo(101, 101);
    mButtonPaths[3].arcTo(QRectF(1, 1, 200, 200), -135, 90);
}

void PieMenu::SetButtonEnabled(uint8_t pButtonIndex, bool pEnable)
{
    Q_ASSERT(pButtonIndex < 5);

    mButtonsEnabled[pButtonIndex] = pEnable;
}

void PieMenu::SetIcons(uint8_t pButtonIndex, const QIcon& pIcon, const QIcon& pDisabledIcon)
{
    Q_ASSERT(pButtonIndex < 5);

    mButtonIcons[pButtonIndex] = pIcon;
    mButtonDisabledIcons[pButtonIndex] = pDisabledIcon;
}

void PieMenu::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    const auto selected = Selected();

    for (int8_t i = 0; i < 4; i++)
    {
        if (selected == i)
        {
            if (i % 2 == 0)
            {
                painter.fillPath(mButtonPaths[i], QBrush(QApplication::mouseButtons() != Qt::NoButton ? QColor(220, 220, 220) : QColor(230, 230, 230)));
            }
            else
            {
                painter.fillPath(mButtonPaths[i], QBrush(QApplication::mouseButtons() != Qt::NoButton ? QColor(200, 200, 200) : QColor(220, 220, 220)));
            }
        }
        else
        {
            painter.fillPath(mButtonPaths[i], QBrush((i % 2 == 0) ? QColor(250, 250, 250) : QColor(242, 242, 242))); // Default
        }
    }

    for (int8_t i = 0; i < 4; i++)
    {
        painter.strokePath(mButtonPaths[i], QPen(QColor(200, 200, 200), 1));
    }

    painter.setPen(QPen(QColor(200, 200, 200), 1));

    if (selected == 4)
    {
        painter.setBrush(QBrush(QApplication::mouseButtons() != Qt::NoButton ? QColor(220, 220, 220) : QColor(230, 230, 230)));
    }
    else
    {
        painter.setBrush(QBrush(QColor(242, 242, 242)));
    }
    painter.drawEllipse(QRectF(67, 67, 70, 70));

    Q_ASSERT(mButtonIcons.size() >= 5);

    painter.drawPixmap(QRect(20, 87, 30, 30), mButtonsEnabled[0] ? mButtonIcons[0].pixmap(30, 30) : mButtonDisabledIcons[0].pixmap(30, 30)); // left
    painter.drawPixmap(QRect(85, 20, 30, 30), mButtonsEnabled[1] ? mButtonIcons[1].pixmap(30, 30) : mButtonDisabledIcons[1].pixmap(30, 30)); // up
    painter.drawPixmap(QRect(152, 85, 30, 30), mButtonsEnabled[2] ? mButtonIcons[2].pixmap(30, 30) : mButtonDisabledIcons[2].pixmap(30, 30)); // right
    painter.drawPixmap(QRect(87, 152, 30, 30), mButtonsEnabled[3] ? mButtonIcons[3].pixmap(30, 30) : mButtonDisabledIcons[3].pixmap(30, 30)); // down
    painter.drawPixmap(QRect(87, 87, 30, 30), mButtonsEnabled[4] ? mButtonIcons[4].pixmap(30, 30) : mButtonDisabledIcons[4].pixmap(30, 30)); // center
}

int8_t PieMenu::Selected() const
{
    const auto cursor = mapFromGlobal(QCursor::pos());

    if (cursor.x() <= 0 || cursor.x() >= 202 || cursor.y() < 0 || cursor.y() >= 202)
    {
        return -1;
    }

    double dist0 = sqrt(std::pow(cursor.x(), 2) + std::pow(cursor.y() - 100, 2));
    double dist1 = sqrt(std::pow(cursor.x() - 100, 2) + std::pow(cursor.y(), 2));
    double dist2 = sqrt(std::pow(200 - cursor.x(), 2) + std::pow(100 - cursor.y(), 2));
    double dist3 = sqrt(std::pow(100 - cursor.x(), 2) + std::pow(200 - cursor.y(), 2));
    double dist4 = sqrt(std::pow(100 - cursor.x(), 2) + std::pow(100 - cursor.y(), 2));

    if (dist4 <= 38)
    {
        return 4;
    }

    if (dist0 <= dist1 && dist0 <= dist2 && dist0 <= dist3)
    {
        return 0;
    }

    if (dist1 <= dist0 && dist1 <= dist2 && dist1 <= dist3)
    {
        return 1;
    }

    if (dist2 <= dist0 && dist2 <= dist1 && dist2 <= dist3)
    {
        return 2;
    }

    if (dist3 <= dist0 && dist3 <= dist1 && dist3 <= dist2)
    {
        return 3;
    }

    return 0;
}

void PieMenu::leaveEvent(QEvent *pEvent)
{
    update();
    QWidget::leaveEvent(pEvent);
}

void PieMenu::mouseReleaseEvent(QMouseEvent *pEvent)
{
    update();

    const auto selected = Selected();

    if (mButtonsEnabled[selected])
    {
        emit PieMenuButtonClicked(selected);

        if (selected == 1 || selected == 3 || selected == 4)
        {
            hide();
        }
    }

    QWidget::mouseReleaseEvent(pEvent);
}

void PieMenu::mousePressEvent(QMouseEvent *pEvent)
{
    update();
    QWidget::mousePressEvent(pEvent);
}

void PieMenu::mouseMoveEvent(QMouseEvent *pEvent)
{
    update();
    QWidget::mouseMoveEvent(pEvent);
}

