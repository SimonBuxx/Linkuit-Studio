#ifndef INPUTEVENTHANDLER_H
#define INPUTEVENTHANDLER_H

#include "View.h"

class InputEventHandler
{
public:
    InputEventHandler(View &pView);

    void wheelEvent(QWheelEvent *pEvent);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
protected:
    View &mView;
};

#endif // INPUTEVENTHANDLER_H
