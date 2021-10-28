/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     yukuan  <yukuan@uniontech.com>
*
* Maintainer: yukuan  <yukuan@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef DATADEALSINGLETON_H
#define DATADEALSINGLETON_H
#include <QMutex>
#include <QReadWriteLock>
#include <QObject>
#include <QTimer>

class CPUInfoModel;

class DataDealSingleton : public QObject
{
    Q_OBJECT
public:
    static DataDealSingleton &getInstance();

    //公开的接口
    //!
    //! \brief readCpuPer
    //! \param cpuPer
    //! \return
    //!
    bool readCpuPer(qreal &cpuPer);

    //!
    //! \brief readMemInfo
    //! \param memUsage
    //! \param memTotal
    //! \param memPercent
    //! \param swapUsage
    //! \param swapTotal
    //! \param swapPercent
    //! \return
    //!
    bool readMemInfo(QString &memUsage, QString &memTotal, QString &memPercent, QString &swapUsage, QString &swapTotal, QString &swapPercent);

    //!
    //! \brief readNetInfo
    //! \param netReceive
    //! \param netTotalReceive
    //! \param netSend
    //! \param totalSend
    //! \return
    //!
    bool readNetInfo(QString &netReceive, QString &netTotalReceive, QString &netSend, QString &totalSend);

    //!
    //! \brief readDiskInfo
    //! \param diskRead
    //! \param diskTotalSize
    //! \param diskWrite
    //! \param diskAvailable
    //! \return
    //!
    bool readDiskInfo(QString &diskRead, QString &diskTotalSize, QString &diskWrite, QString &diskAvailable);

    //!
    //! \brief sendJumpWidgetMessage 发送跳转命令消息
    //! \param dbusMessage 消息名称字符串
    //! \return 返回dbus信号是否发送成功
    //!
    bool sendJumpWidgetMessage(const QString &dbusMessage);

signals:
    void sigDataUpdate();

private:
    DataDealSingleton(QObject *parent = nullptr);
    ~DataDealSingleton();
    DataDealSingleton(const DataDealSingleton &) = delete;
    DataDealSingleton &operator=(const DataDealSingleton &) = delete;

    QReadWriteLock internalMutex;

    static QMutex mutex;
    static QAtomicPointer<DataDealSingleton> instance;

    //防止300ms内重复按键
    QTimer* m_popupTrickTimer;
};

#endif // DATADEALSINGLETON_H
