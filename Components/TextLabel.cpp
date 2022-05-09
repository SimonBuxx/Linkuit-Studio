#include "TextLabel.h"
#include "Configuration.h"
#include "CoreLogic.h"

TextLabel::TextLabel(const CoreLogic* pCoreLogic, QString pText, bool pTakeFocus):
    IBaseComponent(pCoreLogic, nullptr),
    mPlainTextEditProxy(this)
{
    setZValue(components::zvalues::TEXT_LABEL);

    InitProxyWidget(pTakeFocus, pText);

    ConnectToCoreLogic(pCoreLogic);
}

void TextLabel::InitProxyWidget(bool pTakeFocus, QString pText)
{
    mPlainTextEdit = new PlainTextEdit();

    mPlainTextEdit->SetLastTextState(pText);
    mPlainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    mPlainTextEdit->document()->setDocumentMargin(1);

    mPlainTextEdit->setFont(components::text_label::FONT);
    mPlainTextEdit->move(10, canvas::GRID_SIZE * -0.5f + 1);
    mPlainTextEdit->setUndoRedoEnabled(false);
    mPlainTextEdit->setContextMenuPolicy(Qt::NoContextMenu);
    mPlainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mPlainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mPlainTextEdit->setCursor(Qt::IBeamCursor);
    mPlainTextEdit->viewport()->setCursor(Qt::IBeamCursor);

    SetTextContent(QString(pText));

    mPlainTextEditProxy.setWidget(mPlainTextEdit);

#warning mind label focus when implementing circuit loading
    if (pTakeFocus)
    {
        mPlainTextEdit->setFocus(); // Take focus if not generated via copy constructor
    }

    QObject::connect(mPlainTextEdit, &PlainTextEdit::SelectParentItem, this, [&]()
    {
        scene()->clearSelection(); // Prevent editing when multiple components are selected
        if (hasFocus())
        {
            qDebug() << "select";
            setSelected(true);
        }
    });

    QObject::connect(mPlainTextEdit, &PlainTextEdit::DeselectParentItem, this, [&]()
    {
        setSelected(false);
    });

    QObject::connect(mPlainTextEdit, &PlainTextEdit::ContentChangedSignal, this, [&](QString pPreviousText, QString pCurrentText)
    {
        emit TextLabelContentChangedSignal(this, pPreviousText, pCurrentText);
    });

    QObject::connect(mPlainTextEdit, &PlainTextEdit::textChanged, this, &TextLabel::UpdatePlainTextEditSize);
}

void TextLabel::ConnectToCoreLogic(const CoreLogic* pCoreLogic)
{
    QObject::connect(pCoreLogic, &CoreLogic::ControlModeChangedSignal, this, [&](ControlMode pNewMode)
    {
        if (mPlainTextEdit != nullptr)
        {
            if (pNewMode == ControlMode::EDIT)
            {
                // Make editable
                mPlainTextEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
                mPlainTextEdit->setCursor(Qt::IBeamCursor);
                mPlainTextEdit->viewport()->setCursor(Qt::IBeamCursor);
            }
            else
            {
                // Make read-only
                mPlainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
                mPlainTextEdit->setCursor(Qt::ArrowCursor);
                mPlainTextEdit->viewport()->setCursor(Qt::ArrowCursor);
            }
        }
    });

    QObject::connect(pCoreLogic, &CoreLogic::ComponentTypeChangedSignal, this, [&](ComponentType pNewType)
    {
        if (mPlainTextEdit != nullptr)
        {
            if (pNewType == ComponentType::TEXT_LABEL)
            {
                // Make editable
                mPlainTextEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
                mPlainTextEdit->setCursor(Qt::IBeamCursor);
                mPlainTextEdit->viewport()->setCursor(Qt::IBeamCursor);
            }
        }
    });

    QObject::connect(this, &TextLabel::TextLabelContentChangedSignal, pCoreLogic, &CoreLogic::OnTextLabelContentChanged);
}

TextLabel::TextLabel(const TextLabel& pObj, const CoreLogic* pCoreLogic):
    TextLabel(pCoreLogic, pObj.mPlainTextEdit ? pObj.mPlainTextEdit->document()->toPlainText(): "", false)
{};

IBaseComponent* TextLabel::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new TextLabel(*this, pCoreLogic);
}

void TextLabel::ResetZValue()
{
    setZValue(components::zvalues::TEXT_LABEL);
}

void TextLabel::UpdatePlainTextEditSize()
{
    Q_ASSERT(mPlainTextEdit);
    QFontMetrics metrics(mPlainTextEdit->font());

    // Update QPlainTextEdit height
    uint32_t rowHeight = metrics.lineSpacing() + 1;
    QMargins margins = mPlainTextEdit->contentsMargins();
    double newHeight = rowHeight * mPlainTextEdit->document()->lineCount()
            + (mPlainTextEdit->document()->documentMargin() + mPlainTextEdit->frameWidth()) * 2 + margins.top() + margins.bottom() + 2;
    mPlainTextEdit->setFixedHeight(newHeight);

    // Update QPlainTextEdit width (Minimum width to provide click space if label is empty)
    mPlainTextEdit->setFixedWidth(std::max(mPlainTextEdit->document()->size().width() + 12, 20.0));

    // Hide temporarily to update canvas after label shrunk
    this->setOpacity(0);
    update();

    mHeight = std::ceil((newHeight - 5) / canvas::GRID_SIZE) * canvas::GRID_SIZE;
    mWidth = mPlainTextEdit->width() + canvas::GRID_SIZE * 0.5f;

    mShape.clear();
    mShape.addRect(0, canvas::GRID_SIZE * -0.5f, mWidth, mHeight);

    this->setOpacity(1);
    update();
}

void TextLabel::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    if (levelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD)
    {
        {
            QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR, 2, Qt::SolidLine, Qt::SquareCap);
            pPainter->setPen(pen);

            pPainter->drawLine(0, canvas::GRID_SIZE * -0.45f, 0, mHeight - canvas::GRID_SIZE * 0.45f - 1);
        }
        {
            QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::wires::WIRE_LOW_COLOR, 1, Qt::SolidLine, Qt::RoundCap);
            pPainter->setPen(pen);

            pPainter->drawPoint(4, mHeight / 2 - canvas::GRID_SIZE * 0.45f - 4);
            pPainter->drawPoint(4, mHeight / 2 - canvas::GRID_SIZE * 0.45f - 1);
            pPainter->drawPoint(4, mHeight / 2 - canvas::GRID_SIZE * 0.45f + 2);
            pPainter->drawPoint(6, mHeight / 2 - canvas::GRID_SIZE * 0.45f - 4);
            pPainter->drawPoint(6, mHeight / 2 - canvas::GRID_SIZE * 0.45f - 1);
            pPainter->drawPoint(6, mHeight / 2 - canvas::GRID_SIZE * 0.45f + 2);
        }
    }

    if (mPlainTextEdit)
    {
        mPlainTextEdit->setVisible(levelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD);
    }
}

QRectF TextLabel::boundingRect() const
{
    return QRectF(0, canvas::GRID_SIZE * -0.5f, mWidth, mHeight);
}

void TextLabel::SetTextContent(QString pText)
{
    if (mPlainTextEdit != nullptr)
    {
        mPlainTextEdit->document()->setPlainText(pText);
        mPlainTextEdit->document()->setModified(false);
        UpdatePlainTextEditSize();
    }
}

void PlainTextEdit::SetLastTextState(QString pText)
{
    mLastTextState = pText;
}

void PlainTextEdit::focusOutEvent(QFocusEvent *pEvent)
{
    if (document()->isModified())
    {
        document()->setModified(false);
        emit ContentChangedSignal(mLastTextState, document()->toPlainText());
        mLastTextState = document()->toPlainText();
    }

    // Clear selected text
    auto cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);

    emit DeselectParentItem();
    QPlainTextEdit::focusOutEvent(pEvent);
}

void PlainTextEdit::focusInEvent(QFocusEvent *pEvent)
{
    emit SelectParentItem();
    QPlainTextEdit::focusInEvent(pEvent);
}
