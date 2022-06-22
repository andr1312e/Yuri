#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QDebug>

#include <cmath>

class DoubleSlider : public QSlider
{
    Q_OBJECT
public:
    explicit DoubleSlider(QWidget *parent);
    ~DoubleSlider();
Q_SIGNALS:
    void ToValueChanged(double value);
private Q_SLOTS:
    void OnValueChanged(int intValue);
public:
    void SetDoubleRange(double  min, double max);
    double GetCurrentDoubleRangeValue() const noexcept;
    QString GetCurrentDoubleRangeText() const noexcept;
};

#endif // DOUBLESLIDER_H
