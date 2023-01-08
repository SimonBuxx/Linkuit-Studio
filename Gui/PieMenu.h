#ifndef PIEMENU_H
#define PIEMENU_H

#include "QtAwesome.h"

#include <QWidget>
#include <QPainterPath>

class PieMenu : public QWidget
{
    Q_OBJECT
public:
    explicit PieMenu(QtAwesome &pAwesome, QWidget *pParent = nullptr);

    void SetIcons(uint8_t pButtonIndex, const QIcon& pIcon, const QIcon& pDisabledIcon);

    void SetButtonEnabled(uint8_t pButtonIndex, bool pEnable);

    void HideIfNotPinned(void);

    void Hide(void);

signals:
    void PieMenuButtonClicked(int8_t pButtonIndex);

protected:
    void paintEvent(QPaintEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void leaveEvent(QEvent *pEvent) override;

    int8_t CurrentSelection(void) const;

    void DrawPinButton(QPainter& pPainter, bool pMouseOver);

protected:
    std::vector<QIcon> mButtonIcons;
    std::vector<QIcon> mButtonDisabledIcons;
    std::vector<QPainterPath> mButtonPaths;

    QtAwesome &mAwesome; // Contains Fontawesome icons

    QVariantMap mStandardIconVariant;
    QVariantMap mPinnedIconVariant;

    std::vector<bool> mButtonsEnabled;

    bool mIsPinned = false;
};

#endif // PIEMENU_H
