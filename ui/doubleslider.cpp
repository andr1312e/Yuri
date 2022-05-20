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
    const double doubleValue = intValue / 10.0;
    Q_EMIT  ToValueChanged(doubleValue);
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

double DoubleSlider::GetCurrentDoubleRangeValue() const
{
    const double value = QSlider::value();
    return value / 10;
}

QString DoubleSlider::GetCurrentDoubleRangeText() const
{
    const double value = GetCurrentDoubleRangeValue();
    return QString::number(value);
}

void DoubleSlider::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << event->x();
    if ((event->x()) <= 0)
    {
        setValue(0);
    }
    else
    {
        if (event->x() >= width())
        {
            qDebug() << "event->x()" << event->x();
            setValue(maximum());
        }
        else
        {
            double abstPos = event->x();
            abstPos = abstPos / width();
            int newSliderPos = abstPos * maximum();
            if (newSliderPos % 5 <= 2)
            {
                newSliderPos = newSliderPos / 5;
                newSliderPos = newSliderPos * 5;
            }
            else
            {
                newSliderPos = newSliderPos / 5;
                newSliderPos = (newSliderPos + 1) * 5;
            }
            setValue(newSliderPos);
            qDebug() << newSliderPos;
        }
    }


    event->accept();
}
