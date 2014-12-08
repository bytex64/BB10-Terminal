/*
 * Pty.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: chip
 */

#include <src/Pty.h>
#include <QDebug>
#include <unix.h>
#include <process.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

PtyIOWorker::PtyIOWorker(int fd, QObject *parent)
    : QObject(parent)
{
    _fd = fd;
    // Recursing via a signal loop like this is probably not kosher, but
    // it works.
    connect(this, SIGNAL(dataReady()), this, SLOT(waitForInput()), Qt::QueuedConnection);
}

void PtyIOWorker::waitForInput() {
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(_fd, &rfd);
    int r = select(_fd + 1, &rfd, NULL, NULL, NULL);
    if (r == -1) {
        perror("PtyIOWorker select");
        this->thread()->quit();
        emit ptyErrored();
        return;
    }
    emit dataReady();
}


Pty::Pty(QObject *parent)
    : QObject(parent)
{
    master_fd = -1;
    // TODO: set terminal size based on actual window size
    ws.ws_row = 80;
    ws.ws_col = 25;
    ws.ws_xpixel = 0;
    ws.ws_ypixel = 0;
    child_pid = 0;
}

Pty::~Pty()
{
    if (master_fd > 0) {
        close(master_fd);
    }
    wait_thread.quit();
    wait_thread.wait();
}

bool Pty::open() {
    child_pid = forkpty(&master_fd, slavepty, NULL, &ws);
    if (child_pid == -1) {
        perror("forkpty");
        return false;
    } else if (child_pid == 0) {
        execl("/bin/sh", "sh", NULL);
    }

    // Set non-blocking mode on the pty
    fcntl(master_fd, F_SETFL, O_NONBLOCK);

    PtyIOWorker *worker = new PtyIOWorker(master_fd);
    worker->moveToThread(&wait_thread);
    connect(&wait_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(startWait()), worker, SLOT(waitForInput()));
    connect(worker, SIGNAL(dataReady()), this, SLOT(handleWorkerDataReady()));
    connect(worker, SIGNAL(ptyErrored()), this, SLOT(handlePtyError()));
    wait_thread.start();

    emit startWait();

    return true;
}

pid_t Pty::getPID() {
    return child_pid;
}

bool Pty::read(char *buf, int *len) {
    ssize_t n;

    n = ::read(master_fd, buf, *len);
    if (n == -1) {
        if (errno == EAGAIN) {
            *len = 0;
            return true;
        } else {
            perror("reading pty");
            return false;
        }
    } else if (n == 0) {
        *len = 0;
        return true;
    }

    *len = n;
    return true;
}

bool Pty::write(const char *buf, int *len) {
    ssize_t n;

    n = ::write(master_fd, buf, *len);
    if (n == -1) {
        if (errno == EAGAIN) {
            *len = 0;
            return true;
        } else {
            perror("writing pty");
            return false;
        }
    }
    // According to the QNX docs, a non-blocking write will either
    // succeed completely or fail if the number of bytes is less than
    // PIPE_BUF (several kilobytes, it seems), so I'm not checking for
    // any non-error case.

    *len = n;
    return true;
}

QString Pty::read() {
    char buf[255];
    int len = 255;

    if (Pty::read(buf, &len)) {
        return QString::fromUtf8(buf, len);
    } else {
        return QString("");
    }
}

bool Pty::write(QString data) {
    QByteArray b = data.toUtf8();
    const char *buf = b.constData();
    int len = b.length();

    return Pty::write(buf, &len);
}

int Pty::width() {
    return ws.ws_col;
}

void Pty::setWidth(int w) {
    ws.ws_col = w;
    ioctl(master_fd, TIOCSWINSZ, &ws);
}

int Pty::height() {
    return ws.ws_row;
}

void Pty::setHeight(int h) {
    ws.ws_row = h;
    ioctl(master_fd, TIOCSWINSZ, &ws);
}

void Pty::setSize(int w, int h) {
    ws.ws_col = w;
    ws.ws_row = h;
    ioctl(master_fd, TIOCSWINSZ, &ws);
}

void Pty::handleWorkerDataReady() {
    QString str = Pty::read();
    emit dataReady(str);
}

void Pty::handlePtyError() {
    int child_status;

    int pid = waitpid(child_pid, &child_status, WNOHANG);
    if (pid == 0) {
        qDebug() << "Pty error but child still alive?";
    } else if (pid == -1 && errno == EINTR) {
        qDebug() << "Interrupted";
    } else {
        qDebug() << "Child exited" << child_status;
        QString msg = "[Child process exited with status " + QString::number(child_status) + "]";
        emit dataReady(msg);
    }
}
