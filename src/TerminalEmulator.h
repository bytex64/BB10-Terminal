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
#include <QList>
#include <QPoint>

class TerminalEmulator: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString screen READ screen NOTIFY screenChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
public:
    TerminalEmulator(QObject *parent = 0);
    virtual ~TerminalEmulator();

    Q_INVOKABLE void addData(QString data);
    QString screen();

    int width();
    void setWidth(int w);
    int height();
    void setHeight(int h);
    Q_INVOKABLE void setSize(int h, int w);

signals:
    void screenChanged(QString data);
    void screenOverflowed(QList<QString> lines);
    void widthChanged(int w);
    void heightChanged(int h);
    void sizeChanged(int w, int h);

private:
    QList<QString> m_data;
    int m_width;
    int m_height;
    QPoint m_cursorpos;

    void adjustInternals();

    // Cursor movement
    void cursorForward();
    void cursorBackward();
    void carriageReturn();
    void lineFeed();
};

#endif /* TERMINALEMULATOR_H_ */
