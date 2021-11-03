#ifndef FIRMWARESOURCEVIEWER_H
#define FIRMWARESOURCEVIEWER_H

#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;


class FirmwareSourceViewer : public QPlainTextEdit
{
    Q_OBJECT

public:
    FirmwareSourceViewer(QWidget *parent);
    ~FirmwareSourceViewer();

    void LineNumberAreaPaintEvent(QPaintEvent *event);
    int LineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;


private slots:
    void UpdateLineNumberAreaWidth(int newBlockCount);
    void HighlightCurrentLine();
    void UpdateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *m_lineNumberArea;

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *wheel);
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(FirmwareSourceViewer *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    FirmwareSourceViewer *m_codeEditor;
};

#endif
