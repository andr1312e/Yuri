#pragma once

#include <QtGlobal>
#include <QMap>
#include <QVector>

const quint8 pcbStateMessageBytesCount=1;
const int maxMessageBytesCount=255;

const double Fref=40000000.0;
const double c=299792458.0;
const double f=245000000.0;
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
const QVector<quint8> messagesIds={0,1,2,3,4,5,6,7,8};

static bool calculateDIV_rx(double Fvco)
{
    if (Fvco>2750000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
