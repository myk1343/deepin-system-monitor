#ifndef SYSTEM_SERVICE_ENTRY_DATA_H
#define SYSTEM_SERVICE_ENTRY_DATA_H

#include <QSharedData>
#include <QString>

class SystemServiceEntry;

class SystemServiceEntryData : public QSharedData
{
public:
    SystemServiceEntryData();
    SystemServiceEntryData(const SystemServiceEntryData &);
    SystemServiceEntryData &operator=(const SystemServiceEntryData &);
    ~SystemServiceEntryData();

    friend class SystemServiceEntry;

private:
    // name =>
    // org.freedesktop.systemd1->/org/freedesktop/systemd1->org.freedesktop.systemd1.Manager->ListUnitFiles
    QString m_id {};
    // LOAD: (loaded | not-found | stub | bad-setting | error | merged | masked) =>
    // org.freedesktop.systemd1->%objpath%->org.freedesktop.systemd1.Unit->LoadState
    QString m_loadState {};
    // ACTIVE: (active | inactive | reloading | failed | activating | deactivating) =>
    // org.freedesktop.systemd1->%objpath%->org.freedesktop.systemd1.Unit->ActiveState
    QString m_activeState {};
    // running status SUB: (running | exited | dead | startpre | start | start-post | reload | stop
    // | stop-watchdog | stop-sigterm | stop-sigkill | stop-post | final-sigterm | final-sigkill |
    // failed | auto-restart) =>
    // org.freedesktop.systemd1->%objpath%->org.freedesktop.systemd1.Unit->SubState
    QString m_subState {};
    // STATE: (enabled | disabled | masked | static | generated | transient | indirect) =>
    // org.freedesktop.systemd1->/org/freedesktop/systemd1->org.freedesktop.systemd1.Manager->ListUnitFiles
    QString m_state {};

    // unit object path
    QString m_unitObjectPath {};

    // DESCRIPTION
    QString m_description {};  // org.freedesktop.systemd1.Unit
    // PID
    quint32 m_mainPID {0};  // org.freedesktop.systemd1.Service

    bool m_canReload {false};  // org.freedesktop.systemd1.Unit
    bool m_canStart {false};   // org.freedesktop.systemd1.Unit
    bool m_canStop {false};    // org.freedesktop.systemd1.Unit
};

#endif  // SYSTEM_SERVICE_ENTRY_DATA_H