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
    void ptyErrored();

private:
    int _fd;
};

class Pty: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int width READ width WRITE setWidth())
    Q_PROPERTY(int height READ height WRITE setHeight())
public:
    Pty(QObject *parent = 0);
    virtual ~Pty();

    Q_INVOKABLE bool open();
    pid_t getPID();

    bool read(char *buf, int *len);
    bool write(const char *buf, int *len);

    Q_INVOKABLE QString read();
    Q_INVOKABLE bool write(QString data);

    int width();
    void setWidth(int w);
    int height();
    void setHeight(int h);

public slots:
    void setSize(int w, int h);

private slots:
    void handleWorkerDataReady();
    void handlePtyError();

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
