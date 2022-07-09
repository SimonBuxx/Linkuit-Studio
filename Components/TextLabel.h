#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include "IBaseComponent.h"

#include <QPlainTextEdit>
#include <QGraphicsProxyWidget>

class TextLabel;

///
/// \brief PlainTextEdit is the class of the widget inside the TextLabel component
///
class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    /// \brief Sets the text state for which the latest ContentChangedSignal was emitted
    /// \param pText: The text string
    void SetLastTextState(const QString& pText);

protected:
    /// \brief Deselects the TextLabel and triggers an undo action if the text was changed
    /// \param pEvent: The focus event
    void focusOutEvent(QFocusEvent *pEvent) override;

    /// \brief Selects the TextLabel in which this widget is contained
    /// \param pEvent: The focus event
    void focusInEvent(QFocusEvent *pEvent) override;

signals:
    /// \brief Emitted when the parent item (TextLabel) should be selected
    void SelectParentItem(void);

    /// \brief Emitted when the parent item (TextLabel) should be deselected
    void DeselectParentItem(void);

    /// \brief Emitted after unfocusing when the widgets text content changed
    /// \param pPreviousText: The previous text
    /// \param pCurrentText: The current text
    void ContentChangedSignal(QString pPreviousText, QString pCurrentText);

protected:
    QString mLastTextState;
};

///
/// \brief The TextLabel class represents a text label component
///
class TextLabel : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for TextLabel
    /// \param pCoreLogic: Pointer to the core logic
    /// \param pText: The text to display in the label
    /// \param pTakeFocus: If true, the text label will be focused
    TextLabel(const CoreLogic* pCoreLogic, const QString& pText = "", bool pTakeFocus = true);

    /// \brief Copy constructor for TextLabel
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    TextLabel(const TextLabel& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the label component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

    /// \brief Sets the text of this label and updates its size
    /// \param pText: The text to display in the label
    void SetTextContent(const QString& pText);

signals:
    /// \brief Emitted when the content of the text label changes
    /// \param pTextLabel: Pointer to this text label
    /// \param pPreviousText: The previous text
    /// \param pCurrentText: The current text
    void TextLabelContentChangedSignal(TextLabel* pTextLabel, QString pPreviousText, QString pCurrentText);

protected slots:
    /// \brief Updates the size of this label and its text edit widget
    void UpdatePlainTextEditSize(void);

protected:
    /// \brief Paints the label component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

    /// \brief Initializes the proxy widget that contains the text edit widget
    /// \param pTakeFocus: If true, the text edit widget will be focused
    /// \param pText: The text to display in the text edit widget
    void InitProxyWidget(bool pTakeFocus, const QString& pText);

    /// \brief Connects this label to the core logic using signals and slots
    /// \param pCoreLogic: Pointer to the core logic
    void ConnectToCoreLogic(const CoreLogic* pCoreLogic);

protected:
    QGraphicsProxyWidget mPlainTextEditProxy;
    PlainTextEdit *mPlainTextEdit;
};

#endif // TEXTLABEL_H
