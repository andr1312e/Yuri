#include "doubleslider.h"

DoubleSlider::DoubleSlider(QWidget *parent)
    : QSlider(parent)
{
    connect(this, &QSlider::valueChanged, this, &DoubleSlider::OnValueChanged);
    QSlider::setTickPosition(QSlider::TicksLeft);
    QSlider::setOrientation(Qt::Horizontal);
}

DoubleSlider::~DoubleSlider()
{

}

void DoubleSlider::OnValueChanged(int intValue)
{
    qDebug()<< intValue;
    const int divRemainder=intValue%5;
    if(0==divRemainder)
    {
        const double doubleValue = intValue / 10.0;
        Q_EMIT  ToValueChanged(doubleValue);
    }
    else
    {
       setValue(intValue-divRemainder);
    }

}

void DoubleSlider::SetDoubleRange(double min, double max)
{
    const int minValue = min * 10;
    const int maxValue = max * 10;
    QSlider::setRange(minValue, maxValue);
    setTickInterval(50);
    setSingleStep(5);
    setPageStep(5);
}

double DoubleSlider::GetCurrentDoubleRangeValue() const noexcept
{
    const double value = QSlider::value();
    return value / 10;
}

QString DoubleSlider::GetCurrentDoubleRangeText() const noexcept
{
    const double value = GetCurrentDoubleRangeValue();
    return QString::number(value);
}
