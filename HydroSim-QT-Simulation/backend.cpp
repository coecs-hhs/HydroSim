#include "backend.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

// Function to create all devices and append them to the given list
void createDevices(QList<Device*>& devices) {
    // -------------------------
    // Zone 0 Devices
    // -------------------------
    devices.append(new Valve(0, 0, QPointF(0.075, 0.345), false, 0.0, 0.0)); // Valve near source

    // Pumps for various zones
    devices.append(new Pump(0, 1, QPointF(0.340, 0.100), false, 1.0, 1.0, 1.0)); // Zone 1 pump
    devices.append(new Pump(0, 2, QPointF(0.340, 0.305), false, 1.0, 1.0, 1.0)); // Zone 2 pump
    devices.append(new Pump(0, 4, QPointF(0.340, 0.380), false, 1.0, 1.0, 1.0)); // Zone 4 pump
    devices.append(new Pump(0, 3, QPointF(0.340, 0.590), false, 1.0, 1.0, 1.0)); // Zone 3 pump

    // Valves before tanks
    devices.append(new Valve(0, 5, QPointF(0.440, 0.680), false, 0.0, 0.0)); // Before zone 3 tank
    devices.append(new Valve(0, 7, QPointF(0.539, 0.680), false, 0.0, 0.0)); // Before zone 4 tank
    devices.append(new Valve(0, 3, QPointF(0.637, 0.680), false, 0.0, 0.0)); // Before zone 2 tank
    devices.append(new Valve(0, 1, QPointF(0.735, 0.680), false, 0.0, 0.0)); // Before zone 1 tank

    // Valves after tanks
    devices.append(new Valve(0, 6, QPointF(0.440, 0.825), false, 0.0, 0.0)); // After zone 3 tank
    devices.append(new Valve(0, 8, QPointF(0.539, 0.825), false, 0.0, 0.0)); // After zone 4 tank
    devices.append(new Valve(0, 4, QPointF(0.637, 0.825), false, 0.0, 0.0)); // After zone 2 tank
    devices.append(new Valve(0, 2, QPointF(0.735, 0.825), false, 0.0, 0.0)); // After zone 1 tank

    // -------------------------
    // Zone 1 Devices
    // -------------------------
    devices.append(new Valve(1, 1, QPointF(0.130, 0.770), false, 0.0, 0.0)); // Valve before pump
    devices.append(new Valve(1, 2, QPointF(0.350, 0.790), false, 0.0, 0.0)); // Valve after pump
    devices.append(new Pump(1, 1, QPointF(0.240, 0.645), false, 1.0, 1.0, 1.0)); // Pump

    // -------------------------
    // Zone 2 Devices
    // -------------------------
    devices.append(new Valve(2, 1, QPointF(0.130, 0.770), false, 0.0, 0.0)); // Valve before pump
    devices.append(new Valve(2, 2, QPointF(0.350, 0.790), false, 0.0, 0.0)); // Valve after pump
    devices.append(new Pump(2, 1, QPointF(0.240, 0.645), false, 1.0, 1.0, 1.0)); // Pump

    // -------------------------
    // Zone 3 Devices
    // -------------------------
    devices.append(new Valve(3, 1, QPointF(0.130, 0.230), false, 0.0, 0.0)); // Valve before pump
    devices.append(new Valve(3, 2, QPointF(0.350, 0.255), false, 0.0, 0.0)); // Valve after pump
    devices.append(new Pump(3, 1, QPointF(0.240, 0.380), false, 1.0, 1.0, 1.0)); // Pump

    // -------------------------
    // Zone 4 Devices
    // -------------------------
    devices.append(new Valve(4, 1, QPointF(0.105, 0.195), false, 0.0, 0.0)); // Valve before pump
    devices.append(new Valve(4, 2, QPointF(0.290, 0.205), false, 0.0, 0.0)); // Valve after pump
    devices.append(new Pump(4, 1, QPointF(0.195, 0.320), false, 1.0, 1.0, 1.0)); // Pump
}

// -------------------------
// Backend constructor
// -------------------------
Backend::Backend(QObject* parent) : QObject(parent) {
    // Create devices
    createDevices(devices);

    // Initialize MQTT client
    m_mqtt = new MqttClientWrapper(this);

    // TLS certificate paths
    QString caCert     = ":/certs/ca.crt";       // CA certificate
    QString clientCert = ":/certs/client.crt";   // Client certificate
    QString clientKey  = ":/certs/client.key";   // Client private key

    // Uncomment to connect to broker
    m_mqtt->connectToBroker("100.109.40.23", 8883, caCert, clientCert, clientKey);

    // Log when a message is published
    connect(m_mqtt, &MqttClientWrapper::messagePublished, this,
            [](const QString &topic, const QString &msg){
                qDebug() << "Published to" << topic << ":" << msg;
            });
}

// -------------------------
// Helper functions
// -------------------------
void Backend::sendMqttMessage(const QString &msg) const {
    m_mqtt->publishMessage("test/topic", msg);
}

void Backend::printMessage(const QString &msg) const {
    qDebug() << "Backend message:" << msg;
}

// -------------------------
// Toggle device status
// -------------------------
void Backend::toggleDevice(Device* device) {
    if (!device) return;

    if (device->type().toLower() == "pump") {
        auto pump = qobject_cast<Pump*>(device);
        if (pump) {
            pump->setStatus(!pump->status());
            turnPump(pump); // Send updated pump state via MQTT
        }
    }
    else if (device->type().toLower() == "tcv") {
        auto valve = qobject_cast<Valve*>(device);
        if (valve) {
            valve->setStatus(!valve->status());
            turnValve(valve); // Send updated valve state via MQTT
        }
    }
}

// -------------------------
// Send valve state via MQTT
// -------------------------
Q_INVOKABLE void Backend::turnValve(Valve* valve) {
    QJsonObject root, obj;

    // JSON key format: z<zone>-tcv<id>
    QString key = QString("z%1-tcv%2").arg(valve->zone()).arg(valve->id());

    // Fill JSON object with valve info
    obj["type"] = valve->type().toUpper();
    obj["id"] = valve->id();
    obj["status"] = static_cast<int>(valve->status());
    obj["flow"] = valve->flow();
    obj["quality"] = valve->quality();

    root.insert(key, obj);

    QJsonDocument doc(root);
    QString jsonString = doc.toJson(QJsonDocument::Compact);

    sendMqttMessage(jsonString);
}

// -------------------------
// Send pump state via MQTT
// -------------------------
Q_INVOKABLE void Backend::turnPump(Pump* pump) {
    QJsonObject root, obj;

    // JSON key format: z<zone>-pump<id>
    QString key = QString("z%1-pump%2").arg(pump->zone()).arg(pump->id());

    // Fill JSON object with pump info
    obj["type"] = pump->type().toUpper();
    obj["id"] = pump->id();
    obj["status"] = static_cast<int>(pump->status());
    obj["flow"] = pump->flow();
    obj["speed"] = pump->speed();
    obj["quality"] = pump->quality();

    root.insert(key, obj);

    QJsonDocument doc(root);
    QString payload = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

    sendMqttMessage(payload);
}
