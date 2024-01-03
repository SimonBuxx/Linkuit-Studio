#include "PieMenu.h"
#include "HelperFunctions.h"

#include <QPainter>
#include <QApplication>
#include <QMouseEvent>

PieMenu::PieMenu(QWidget *pParent):
    QWidget(pParent),
    mDefaultButtonIcons{mButtonCount, QIcon()},
    mDisabledButtonIcons{mButtonCount, QIcon()},
    mPieButtonPaths{mButtonCount, QPainterPath()},
    mButtonsEnabled(mButtonCount, true),
    mBaseSize(mPieRadius * 2, mPieRadius * 2),
    mFullSize(mBaseSize + QSize(mStrokeWidth * 2, mStrokeWidth * 2)),
    mAnglePerButton(360.0f / mButtonCount),
    mCloseButtonIndex(mButtonCount + 1),
    mPinButtonIndex(mButtonCount + 2)
{
    setMinimumSize(mFullSize);
    setMaximumSize(mFullSize);
    setMouseTracking(true);
    hide();

    InitPainterPaths();
}

void PieMenu::InitPainterPaths()
{
    for (uint8_t i = 0; i < mButtonCount; i++)
    {
        QPainterPath path;

        qreal angle = mAnglePerButton * (i - 1) + mBaseAngle;

        path.moveTo(mFullSize.width() / 2.0f, mFullSize.height() / 2.0f);
        path.arcTo(QRectF(mStrokeWidth, mStrokeWidth, mFullSize.width() - mStrokeWidth * 2,
                          mFullSize.height() - mStrokeWidth * 2), -angle, mAnglePerButton);

        mPieButtonPaths[i] = path;
    }
}

void PieMenu::Display()
{
    auto geometryAdjusted = geometry();
    auto mappedPosition = mapToParent(mapFromGlobal(QCursor::pos()));

    geometryAdjusted.setTopLeft(mappedPosition - QPoint(mFullSize.width() / 2, mFullSize.height() / 2));
    setGeometry(geometryAdjusted);

    show();
    setFocus();
}

void PieMenu::HideIfNotPinned()
{
    if (!mIsPinned)
    {
        FadeOutWidget(*this, gui::PIE_MENU_ANIMATION_DURATION);
    }
}

void PieMenu::SetButtonEnabled(uint8_t pIndex, bool pEnable)
{
    Q_ASSERT(pIndex < mButtonsEnabled.size());
    mButtonsEnabled[pIndex] = pEnable;
}

void PieMenu::SetButtonIcon(uint8_t pIndex, const QImage& pIconImage)
{
    mDefaultButtonIcons[pIndex] = QIcon(QPixmap::fromImage(pIconImage));

    auto img = pIconImage;
    img.fill(Qt::transparent);

    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setOpacity(0.1);
    painter.drawImage(0, 0, pIconImage);

    mDisabledButtonIcons[pIndex] = QIcon(QPixmap::fromImage(img));
}

void PieMenu::SetCloseButtonIcon(const QIcon& pIcon)
{
    mCloseIcon = pIcon;
}

void PieMenu::SetPinButtonIcon(const QIcon& pIcon)
{
    mPinIcon = pIcon;
}

void PieMenu::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    const auto buttonUnderMouse = GetButtonUnderMouse();

    paintPieButtons(painter, buttonUnderMouse);

    paintCloseButton(painter, (buttonUnderMouse == mCloseButtonIndex));

    if (mShowPinButton)
    {
        paintPinButton(painter, (buttonUnderMouse == mPinButtonIndex));
    }
}

void PieMenu::paintPieButtons(QPainter& pPainter, int8_t pMouseover)
{
    for (int8_t i = 0; i < mButtonCount; i++)
    {
        if (pMouseover == i)
        {
            if (mAlternateColors && (i % 2 == 0))
            {
                pPainter.fillPath(mPieButtonPaths[i], QBrush(QApplication::mouseButtons() == Qt::LeftButton ? QColor(200, 200, 200) : QColor(220, 220, 220)));
            }
            else
            {
                pPainter.fillPath(mPieButtonPaths[i], QBrush(QApplication::mouseButtons() == Qt::LeftButton ? QColor(220, 220, 220) : QColor(230, 230, 230)));
            }
        }
        else
        {
            if (mAlternateColors && (i % 2 == 0))
            {
                pPainter.fillPath(mPieButtonPaths[i], QBrush(QColor(242, 242, 242)));
            }
            else
            {
                pPainter.fillPath(mPieButtonPaths[i], QBrush(QColor(250, 250, 250)));
            }
        }
    }

    for (int8_t i = 0; i < mButtonCount; i++)
    {
        pPainter.strokePath(mPieButtonPaths[i], QPen(QColor(200, 200, 200), mStrokeWidth));
    }

    for (int8_t i = 0; i < mButtonCount; i++)
    {
        qreal angle = mAnglePerButton * i + mBaseAngle - mAnglePerButton * 0.5f;

        QPoint referencePoint = QPoint(mPieRadius * qCos((angle * M_PI) / 180) + mPieRadius,
                                        mPieRadius * qSin((angle * M_PI) / 180) + mPieRadius);

        int8_t index = i + 1;

        if (index >= mButtonCount)
        {
            index = 0;
        }

        pPainter.drawPixmap(QRect((referencePoint.x() * 2 + mFullSize.width() / 2) / 3 - mPieIconSize / 2 + mStrokeWidth, (referencePoint.y() * 2 + mFullSize.height() / 2) / 3 - mPieIconSize / 2 + mStrokeWidth, mPieIconSize, mPieIconSize),
                           mButtonsEnabled[index] ? mDefaultButtonIcons[index].pixmap(mPieIconSize, mPieIconSize) : mDisabledButtonIcons[index].pixmap(mPieIconSize, mPieIconSize));
    }
}

void PieMenu::paintCloseButton(QPainter& pPainter, bool pMouseover)
{
    pPainter.setPen(QPen(QColor(200, 200, 200), mStrokeWidth));

    if (pMouseover)
    {
        pPainter.setBrush(QBrush(QApplication::mouseButtons() == Qt::LeftButton ? QColor(220, 220, 220) : QColor(230, 230, 230)));
    }
    else
    {
        pPainter.setBrush(QBrush(QColor(242, 242, 242)));
    }

    pPainter.drawEllipse(QRectF(mPieRadius - mCloseButtonRadius + mStrokeWidth, mPieRadius - mCloseButtonRadius + mStrokeWidth,
                               mCloseButtonRadius * 2, mCloseButtonRadius * 2));

    pPainter.drawPixmap(QRect(mFullSize.width() / 2 - mCloseIconSize / 2, mFullSize.height() / 2 - mCloseIconSize / 2,
                             mCloseIconSize, mCloseIconSize), mCloseIcon.pixmap(mCloseIconSize, mCloseIconSize));
}

void PieMenu::paintPinButton(QPainter& pPainter, bool pMouseover)
{
    pPainter.setPen(QPen(mIsPinned ? QColor(150, 150, 150) : QColor(200, 200, 200), mStrokeWidth));

    if (pMouseover)
    {
        if (QApplication::mouseButtons() == Qt::LeftButton)
        {
            pPainter.setBrush(mIsPinned ? QColor(190, 190, 190) : QBrush(QColor(200, 200, 200)));
        }
        else
        {
            pPainter.setBrush(mIsPinned ? QColor(190, 190, 190) : QBrush(QColor(220, 220, 220)));
        }
    }
    else
    {
        pPainter.setBrush(QBrush(mIsPinned ? QColor(190, 190, 190) : QColor(255, 255, 255)));
    }

    pPainter.drawEllipse(QRectF(mBaseSize.width() - mPinButtonRadius * 2, mStrokeWidth, mPinButtonRadius * 2, mPinButtonRadius * 2));

    pPainter.drawPixmap(QRect(mBaseSize.width() - mPinButtonRadius - mPinIconSize / 2, mStrokeWidth + mPinButtonRadius - mPinIconSize / 2,
                             mPinIconSize, mPinIconSize), mPinIcon.pixmap(mPinIconSize, mPinIconSize));
}
int8_t PieMenu::GetButtonUnderMouse() const
{
    const auto cursor = mapFromGlobal(QCursor::pos());

    if (cursor.x() <= 0 || cursor.x() >= mFullSize.width() || cursor.y() < 0 || cursor.y() >= mFullSize.height()) // out of widget
    {
        return -1;
    }

    int8_t nearest = -1;
    uint32_t distance = UINT_MAX;
    for (size_t i = 0; i < mButtonCount; i++)
    {
        qreal angle = mAnglePerButton * i + mBaseAngle - mAnglePerButton * 0.5f;

        QPoint referencePoint = QPoint(mPieRadius * qCos((angle * M_PI) / 180) + mPieRadius,
                                        mPieRadius * qSin((angle * M_PI) / 180) + mPieRadius);

        uint32_t current = sqrt(std::pow(referencePoint.x() - cursor.x(), 2) + std::pow(referencePoint.y() - cursor.y(), 2));

        if (current < distance)
        {
            distance = current;

            int8_t index = i + 1;

            if (index >= mButtonCount)
            {
                index = 0;
            }

            nearest = index;
        }
    }

    qreal distanceToPinButton = sqrt(std::pow(mBaseSize.width() - mPinButtonRadius - cursor.x(), 2) + std::pow(mPinButtonRadius - cursor.y(), 2));

    if (distanceToPinButton < mPinButtonRadius + mStrokeWidth)
    {
        return mPinButtonIndex;
    }

    qreal distanceToCloseButton = sqrt(std::pow(mBaseSize.width() / 2 - cursor.x(), 2) + std::pow(mBaseSize.height() / 2 - cursor.y(), 2));

    if (distanceToCloseButton < mCloseButtonRadius + mStrokeWidth)
    {
        return mCloseButtonIndex;
    }

    return nearest;
}

void PieMenu::mouseReleaseEvent(QMouseEvent *pEvent)
{
    update();

    if (pEvent->button() == Qt::LeftButton)
    {
        const auto buttonUnderMouse = GetButtonUnderMouse();

        if (buttonUnderMouse == mPinButtonIndex)
        {
            mIsPinned = !mIsPinned;
        }
        else if (buttonUnderMouse == mCloseButtonIndex)
        {
            FadeOutWidget(*this, gui::PIE_MENU_ANIMATION_DURATION);
        }
        else if (buttonUnderMouse >= 0 && buttonUnderMouse < mButtonCount && mButtonsEnabled[buttonUnderMouse])
        {
            emit ButtonClickedSignal(buttonUnderMouse);

            if (!mIsPinned && buttonUnderMouse != 1 && buttonUnderMouse != 3)
            {
                FadeOutWidget(*this, gui::PIE_MENU_ANIMATION_DURATION);
            }
        }
        QWidget::mouseReleaseEvent(pEvent);
    }
    else
    {
        pEvent->accept();
    }
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

void PieMenu::leaveEvent(QEvent *pEvent)
{
    update();
    QWidget::leaveEvent(pEvent);
}

