#include "AbstractGate.h"

AbstractGate::AbstractGate(uint8_t pInputCount, Direction pDirection):
    mInputCount(pInputCount),
    mDirection(pDirection),
    mInputInverted(pInputCount, false)
{
    Q_ASSERT(mInputCount >= 1);

    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT)
    {
        mWidth = components::gates::GRID_WIDTH * canvas::GRID_SIZE;
        mHeight = mInputCount * canvas::GRID_SIZE * 2;
    }
    else
    {
        mWidth = mInputCount * canvas::GRID_SIZE * 2;
        mHeight = components::gates::GRID_WIDTH * canvas::GRID_SIZE;
    }
}

void AbstractGate::PaintSpecifics(QPainter *pPainter, const double pLevelOfDetail)
{
    if (pLevelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD)
    {
        pPainter->setFont(components::gates::FONT);
        pPainter->drawText(boundingRect(), mComponentText, Qt::AlignHCenter | Qt::AlignVCenter);
    }
    if (pLevelOfDetail >= components::CONNECTORS_MIN_LOD)
    {
        switch (mDirection)
        {
            case Direction::RIGHT:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawLine(-10, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
                    if (mInputInverted.at(i))
                    {
                        pPainter->drawEllipse(-10, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 5, 10, 10);
                    }
                }
                pPainter->drawLine(mWidth, mHeight / 2, mWidth + 10, mHeight / 2);
                if (mOutputInverted)
                {
                    pPainter->drawEllipse(mWidth, mHeight / 2 - 5, 10, 10);
                }
                break;
            }
            case Direction::DOWN:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, -10, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0);
                    if (mInputInverted.at(i))
                    {
                        pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 5, -10, 10, 10);
                    }
                }
                pPainter->drawLine(mWidth / 2, mHeight, mWidth / 2, mHeight + 10);
                if (mOutputInverted)
                {
                    pPainter->drawEllipse(mWidth / 2 - 5, mHeight, 10, 10);
                }
                break;
            }
            case Direction::LEFT:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawLine(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mWidth + 10, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
                    if (mInputInverted.at(i))
                    {
                        pPainter->drawEllipse(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 5, 10, 10);
                    }
                }
                pPainter->drawLine(-10, mHeight / 2, 0, mHeight / 2);
                if (mOutputInverted)
                {
                    pPainter->drawEllipse(-10, mHeight / 2 - 5, 10, 10);
                }
                break;
            }
            case Direction::UP:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight + 10);
                    if (mInputInverted.at(i))
                    {
                        pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 5, mHeight, 10, 10);
                    }
                }
                pPainter->drawLine(mWidth / 2, -10, mWidth / 2, 0);
                if (mOutputInverted)
                {
                    pPainter->drawEllipse(mWidth / 2 - 5, -10, 10, 10);
                }
                break;
            }
            default:
            {
                Q_ASSERT(false);
                break;
            }
        }
    }
}

QRectF AbstractGate::boundingRect() const
{
    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT)
    {
        return QRectF(-13, -3, mWidth + 26, mHeight + 6);
    }
    else
    {
        return QRectF(-3, -13, mWidth + 6, mHeight + 26);
    }
}

QPainterPath AbstractGate::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);
    return path;
}
