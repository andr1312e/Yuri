#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QDebug>

class DoubleSlider : public QSlider
{
    Q_OBJECT
public:
    DoubleSlider(QWidget *parent);
    ~DoubleSlider();
Q_SIGNALS:
    void ToValueChanged(double value);
private Q_SLOTS:
    void OnValueChanged(int intValue);
public:
    void SetDoubleRange(double  min, double max);
    double GetCurrentDoubleRangeValue() const;
    QString GetCurrentDoubleRangeText() const;

    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // DOUBLESLIDER_H
