/*
 * TerminalEmulator.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: chip
 */

#include <src/TerminalEmulator.h>

#include <QDebug>
#include <QStringList>

TerminalEmulator::TerminalEmulator(QObject *parent)
    : QObject(parent),
      m_cursorpos(0, 0)
{
    m_width = 80;
    m_height = 25;
    adjustInternals();
}

TerminalEmulator::~TerminalEmulator()
{

}

void TerminalEmulator::addData(QString data) {
    for (int i = 0; i < data.size(); i++) {
        switch (data[i].unicode()) {
        case 010:
            cursorBackward();
            break;
        case 012:
            lineFeed();
            break;
        case 015:
            carriageReturn();
            break;
        default:
            m_data[m_cursorpos.y()][m_cursorpos.x()] = data[i];
            cursorForward();
        }
    }
    emit screenChanged(screen());
}

QString TerminalEmulator::screen() {
    return QStringList(m_data).join("\n");
}

int TerminalEmulator::width() {
    return m_width;
}

void TerminalEmulator::setWidth(int w) {
    m_width = w;
    adjustInternals();
    emit widthChanged(w);
}

int TerminalEmulator::height() {
    return m_height;
}

void TerminalEmulator::setHeight(int h) {
    m_height = h;
    adjustInternals();
    emit heightChanged(h);
}

// Adjust internal data structures to match width and height
void TerminalEmulator::adjustInternals() {
    if (m_height < m_data.size()) {
        QList<QString> overflow;
        while (m_height < m_data.size()) {
            overflow.append(m_data.takeFirst());
        }

        emit screenOverflowed(overflow);
    } else if (m_height > m_data.size()) {
        while (m_height > m_data.size()) {
            m_data.append(QString(m_width, ' '));
        }
    }

    for (int i = 0; i < m_data.size(); i++) {
        if (m_data[i].size() > m_width) {
            m_data[i].resize(m_width);
        } else if (m_data[i].size() < m_width) {
            m_data[i] += QString(m_width - m_data[i].size(), ' ');
        }
    }
}

void TerminalEmulator::cursorForward() {
    if (m_cursorpos.x() == m_width - 1) {
        // Wrap to the next line
        m_cursorpos.setX(0);
        if (m_cursorpos.y() == m_height - 1) {
            // On the last line, scroll down
            m_data.append(QString(m_width, ' '));
            adjustInternals();
        } else {
            m_cursorpos.ry()++;
        }
    } else {
        m_cursorpos.rx()++;
    }
}

void TerminalEmulator::cursorBackward() {
    if (m_cursorpos.x() > 0) {
        m_cursorpos.rx()--;
    }
}

void TerminalEmulator::carriageReturn() {
    m_cursorpos.setX(0);
}

void TerminalEmulator::lineFeed() {
    if (m_cursorpos.y() == m_height - 1) {
        // Scroll down
        QList<QString> overflow;
        overflow.append(m_data.takeFirst());
        m_data.append(QString(m_width, ' '));
        emit screenOverflowed(overflow);
    } else {
        m_cursorpos.ry()++;
    }
}
