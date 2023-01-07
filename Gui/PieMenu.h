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

signals:
    void PieMenuButtonClicked(int8_t pButtonIndex);

protected:
    void paintEvent(QPaintEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void leaveEvent(QEvent *pEvent) override;

    int8_t Selected(void) const;

protected:
    std::vector<QIcon> mButtonIcons;
    std::vector<QIcon> mButtonDisabledIcons;
    std::vector<QPainterPath> mButtonPaths;

    QtAwesome &mAwesome; // Contains Fontawesome icons

    QVariantMap mStandardIconVariant;

    std::vector<bool> mButtonsEnabled;
};

#endif // PIEMENU_H
