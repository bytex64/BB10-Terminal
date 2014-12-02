/*
 * TerminalEmulator.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: chip
 */

#include <src/TerminalEmulator.h>

#include <QDebug>

TerminalEmulator::TerminalEmulator(QObject *parent)
    : QObject(parent)
{

}

TerminalEmulator::~TerminalEmulator()
{

}

void TerminalEmulator::addData(QString data) {
    for (int i = 0; i < data.size(); i++) {
        if (data[i] == 8) {
            _data.chop(1);
        } else {
            _data.append(data[i]);
        }
    }
    emit screenChanged(_data);
}

QString TerminalEmulator::screen() {
    return _data;
}
