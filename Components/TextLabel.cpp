#include "TextLabel.h"
#include "Configuration.h"
#include "CoreLogic.h"

TextLabel::TextLabel(const CoreLogic* pCoreLogic, QString pText):
    IBaseComponent(pCoreLogic, nullptr),
    mText(pText),
    mPlainTextEditProxy(this)
{
    setZValue(components::zvalues::TEXT_LABEL);

    mHeight = canvas::GRID_SIZE;
    mWidth = canvas::GRID_SIZE * 2;

    InitProxyWidget();

    QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, this, [&](){
        if (mPlainTextEdit)
        {
            mPlainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
            mPlainTextEdit->setCursor(Qt::ArrowCursor);
            mPlainTextEdit->viewport()->setCursor(Qt::ArrowCursor);
        }
    });
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, [&](){
        if (mPlainTextEdit)
        {
            mPlainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
            mPlainTextEdit->setTextInteractionFlags(Qt::TextEditable);
            mPlainTextEdit->setCursor(Qt::IBeamCursor);
            mPlainTextEdit->viewport()->setCursor(Qt::IBeamCursor);
        }
    });
}

void TextLabel::InitProxyWidget()
{
    mPlainTextEdit = new PlainTextEdit();

    mPlainTextEdit->document()->setPlainText(mText);
    mPlainTextEdit->document()->setModified(false);
    mPlainTextEdit->document()->setDocumentMargin(1);

    mPlainTextEdit->setFont(components::text_label::FONT);

    mPlainTextEdit->move(5, canvas::GRID_SIZE * -0.5f + 1);
    mPlainTextEdit->setUndoRedoEnabled(false);
    mPlainTextEdit->setContextMenuPolicy(Qt::NoContextMenu);
    mPlainTextEdit->setStyleSheet("QPlainTextEdit { color: white; background: transparent; border: none; selection-color: rgb(0, 88, 61); selection-background-color: transparent;}");
    mPlainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mPlainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mPlainTextEdit->setCursor(Qt::IBeamCursor);
    mPlainTextEdit->viewport()->setCursor(Qt::IBeamCursor);

    QObject::connect(mPlainTextEdit, &PlainTextEdit::textChanged, this, &TextLabel::OnTextChanged);

    mPlainTextEditProxy.setWidget(mPlainTextEdit);

    UpdatePlainTextEditSize();
    mPlainTextEdit->setFocus();
    mPlainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    mPlainTextEdit->setTextInteractionFlags(Qt::TextEditable);

    QObject::connect(mPlainTextEdit, &PlainTextEdit::SelectParentItem, this, [&](){
        setSelected(true);
    });

    QObject::connect(mPlainTextEdit, &PlainTextEdit::DeselectParentItem, this, [&](){
        setSelected(false);
    });
}

TextLabel::TextLabel(const TextLabel& pObj, const CoreLogic* pCoreLogic):
    TextLabel(pCoreLogic, pObj.mText)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

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

    // Update QPlainTextEdit width
    mPlainTextEdit->setFixedWidth(mPlainTextEdit->document()->size().width() + 12);

    // Hide temporarily to update canvas after label shrunk
    this->setOpacity(0);
    update();

    mHeight = std::ceil((newHeight - 5) / canvas::GRID_SIZE) * canvas::GRID_SIZE;
    mWidth = mPlainTextEdit->document()->size().width() + canvas::GRID_SIZE / 2;

    this->setOpacity(1);
    update();
}

void TextLabel::OnTextChanged()
{
    Q_ASSERT(mPlainTextEdit);

    mText = mPlainTextEdit->document()->toPlainText();
    UpdatePlainTextEditSize();
}

void TextLabel::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    if (levelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD)
    {
        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR, 2, Qt::SolidLine, Qt::FlatCap);

        pPainter->setPen(pen);
        pPainter->setBrush(Qt::NoBrush);

        pPainter->drawLine(0, canvas::GRID_SIZE * -0.45f, 0, mHeight - canvas::GRID_SIZE * 0.45f);
    }
}

QRectF TextLabel::boundingRect() const
{
    return QRectF(0, canvas::GRID_SIZE * -0.5f, mWidth, mHeight);
}

QPainterPath TextLabel::shape() const
{
    QPainterPath path;
    path.addRect(0, canvas::GRID_SIZE * -0.5f, mWidth, mHeight);
    return path;
}

void PlainTextEdit::focusOutEvent(QFocusEvent *pEvent)
{
    if (document()->isModified())
    {
        qDebug() << "Add undo action!";
        document()->setModified(false);
    }
    emit DeselectParentItem();
    QPlainTextEdit::focusOutEvent(pEvent);
}

void PlainTextEdit::focusInEvent(QFocusEvent *pEvent)
{
    emit SelectParentItem();
    QPlainTextEdit::focusInEvent(pEvent);
}

