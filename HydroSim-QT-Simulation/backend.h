#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include "mqttclientwrapper.h"
#include "devices/Pump.h"
#include "devices/Valve.h"

// -------------------------
// Backend class
// -------------------------
// This class manages all devices (pumps and valves),
// handles MQTT communication, and exposes functions to QML.
// -------------------------
class Backend : public QObject
{
    Q_OBJECT

public:
    // Constructor
    // 'explicit' prevents implicit conversions from QObject* to Backend
    explicit Backend(QObject *parent = nullptr);

    // -------------------------
    // QML-invokable functions
    // -------------------------

    // Print a message to the console (useful for debugging)
    Q_INVOKABLE void printMessage(const QString &msg) const;

    // Get a list of all devices (pumps and valves) as QObject* for QML
    Q_INVOKABLE QVector<QObject*> getDevices() const {
        QVector<QObject*> list;
        for (Device* device : devices)
            list.append(device);
        return list;
    }

    // Toggle the status of a given device (pump or valve)
    Q_INVOKABLE void toggleDevice(Device* device);

    // Send the state of a valve to MQTT
    Q_INVOKABLE void turnValve(Valve* valve);

    // Send the state of a pump to MQTT
    Q_INVOKABLE void turnPump(Pump* pump);

signals:
    // Signal emitted when a button/device state changes
    void buttonStateChanged(int index, bool state);

private:
    // List of all devices managed by the backend
    QList<Device*> devices;

    // MQTT client wrapper for publishing messages
    MqttClientWrapper* m_mqtt;

    // Helper function to send a message via MQTT
    void sendMqttMessage(const QString &msg) const;
};
