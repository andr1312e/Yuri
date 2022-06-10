#pragma once

#include <QtGlobal>
#include <QMap>



const QMap<quint8, quint8> atteniatorTable { {0, 15},
    {1, 14},
    {2, 13},
    {3, 12},
    {4, 11},
    {5, 10},
    {6, 9},
    {9, 8},
    {12, 7},
    {15, 6},
    {18, 5},
    {21, 4},
    {24, 3},
    {27, 2},
    {30, 1},};

const QStringList noiseValues = {"все Отключено", "ответчик ВКЛЮЧЕН", "шумогенератор ВКЛЮЧЕН", "Синус ВКЛЮЧЕН", "М-сигнал ВКЛЮЧЕН", "БПАР"};



