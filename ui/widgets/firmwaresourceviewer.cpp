#include "firmwaresourceviewer.h"

#include <QPainter>
#include <QTextBlock>
#include <QWheelEvent>

//![constructor]

FirmwareSourceViewer::FirmwareSourceViewer(QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);

    connect(this, &FirmwareSourceViewer::blockCountChanged, this, &FirmwareSourceViewer::UpdateLineNumberAreaWidth);
    connect(this, &FirmwareSourceViewer::updateRequest, this, &FirmwareSourceViewer::UpdateLineNumberArea);
    connect(this, &FirmwareSourceViewer::cursorPositionChanged, this, &FirmwareSourceViewer::HighlightCurrentLine);

    UpdateLineNumberAreaWidth(0);
    HighlightCurrentLine();
}

FirmwareSourceViewer::~FirmwareSourceViewer()
{
    delete m_lineNumberArea;
}

int FirmwareSourceViewer::LineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(u'9') * digits;

    return space;
}

void FirmwareSourceViewer::UpdateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(LineNumberAreaWidth(), 0, 0, 0);
}

void FirmwareSourceViewer::UpdateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        UpdateLineNumberAreaWidth(0);
}

void FirmwareSourceViewer::wheelEvent(QWheelEvent *wheel)
{
    if( wheel->modifiers() == Qt::ControlModifier )
    {
        if(wheel->angleDelta().y() > 0)
        {
            this->zoomIn(2);
        }
        else
        {
            this->zoomOut(2);
        }
    }
}

void FirmwareSourceViewer::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), LineNumberAreaWidth(), cr.height()));
}

void FirmwareSourceViewer::HighlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void FirmwareSourceViewer::LineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}


LineNumberArea::LineNumberArea(FirmwareSourceViewer *editor)
    : QWidget(editor)
    , m_codeEditor(editor)
{

}

QSize LineNumberArea::sizeHint() const
{
    return QSize(m_codeEditor->LineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    m_codeEditor->LineNumberAreaPaintEvent(event);
}
