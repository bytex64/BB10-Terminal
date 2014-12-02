/*
 * TerminalEmulator.h
 *
 *  Created on: Nov 30, 2014
 *      Author: chip
 */

#ifndef TERMINALEMULATOR_H_
#define TERMINALEMULATOR_H_

#include <QObject>
#include <QMetaType>

class TerminalEmulator: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString screen READ screen NOTIFY screenChanged)
public:
    TerminalEmulator(QObject *parent = 0);
    virtual ~TerminalEmulator();

    Q_INVOKABLE void addData(QString data);
    QString screen();

signals:
    void screenChanged(QString data);

private:
    QString _data;
};

#endif /* TERMINALEMULATOR_H_ */
