/*
 * TerminalControl.h
 *
 *  Created on: Dec 6, 2014
 *      Author: matt
 */

#ifndef TERMINALCONTROL_H_
#define TERMINALCONTROL_H_

#include <QObject>
#include <bb/cascades/CustomControl>

namespace bb {
    namespace cascades {
        class Container;
        class Label;
        class ScrollView;
    }
}

class QString;

class TerminalControl: public bb::cascades::CustomControl
{
    Q_OBJECT


public:
    TerminalControl(bb::cascades::Container *parent = 0);

    virtual ~TerminalControl();

public slots:
    void update(QString data);

private:

    bb::cascades::Container *m_rootContainer;
    bb::cascades::ScrollView *m_scrollView;
    bb::cascades::Label *m_label;

};

#endif /* TERMINALCONTROL_H_ */
