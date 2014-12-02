/*
 * Pty.h
 *
 *  Created on: Nov 30, 2014
 *      Author: chip
 */

#ifndef PTY_H_
#define PTY_H_

#include <QObject>
#include <QThread>
#include <QMetaType>
#include <ioctl.h>

class PtyIOWorker : public QObject {
    Q_OBJECT
public:
    PtyIOWorker(int fd, QObject *parent = 0);

public slots:
    void waitForInput();

signals:
    void dataReady();

private:
    int _fd;
};

class Pty: public QObject
{
    Q_OBJECT
public:
    Pty(QObject *parent = 0);
    virtual ~Pty();

    bool open();
    pid_t getPID();

    bool read(char *buf, int *len);
    bool write(const char *buf, int *len);

    Q_INVOKABLE QString read();
    Q_INVOKABLE bool write(QString data);

private slots:
    void handleWorkerDataReady();

signals:
    void dataReady(QString data);
    void startWait();

private:
    int master_fd;
    char slavepty[256];
    struct winsize ws;
    pid_t child_pid;
    QThread wait_thread;
};

#endif /* PTY_H_ */
