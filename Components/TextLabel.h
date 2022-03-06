#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include "IBaseComponent.h"

#include <QPlainTextEdit>
#include <QGraphicsProxyWidget>

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
protected:
    void focusOutEvent(QFocusEvent *pEvent) override;
    void focusInEvent(QFocusEvent *pEvent) override;

signals:
    void SelectParentItem(void);
    void DeselectParentItem(void);
};

class TextLabel : public IBaseComponent
{
    Q_OBJECT
public:
    TextLabel(const CoreLogic* pCoreLogic, QString pText = "");
    TextLabel(const TextLabel& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the label component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Defines the shape of this component
    /// \return A rectangular path describing the component shape
    QPainterPath shape(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

public slots:
    void OnTextChanged(void);

protected:
    /// \brief Paints the label component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

    void InitProxyWidget(void);

    void UpdatePlainTextEditSize(void);

protected:
    QString mText = "";

    QGraphicsProxyWidget mPlainTextEditProxy;
    PlainTextEdit *mPlainTextEdit;
};

#endif // TEXTLABEL_H
