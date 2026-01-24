#include "mqttclientwrapper.h"
#include <QDebug>
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>

MqttClientWrapper::MqttClientWrapper(QObject* parent) 
    : QObject(parent) 
{
    // Create the MQTT client
    m_client = new QMqttClient(this);

    // Setup signals for connection state changes
    setupConnections();
}

// Connect internal signals to debug output and own signals
void MqttClientWrapper::setupConnections() {
    connect(m_client, &QMqttClient::connected, this, [this]() {
        qDebug() << "MQTT connected!";
        emit connected();
    });

    connect(m_client, &QMqttClient::disconnected, this, [this]() {
        qDebug() << "MQTT disconnected!";
        emit disconnected();
    });
}

/**
 * Connect to an MQTT broker with optional TLS configuration
 * @param hostname: broker hostname
 * @param port: broker port
 * @param caCertPath: path to CA certificate (optional)
 * @param clientCertPath: path to client certificate (optional)
 * @param clientKeyPath: path to client private key (optional)
 */
void MqttClientWrapper::connectToBroker(const QString &hostname, quint16 port,
                                        const QString &caCertPath,
                                        const QString &clientCertPath,
                                        const QString &clientKeyPath)
{
    m_client->setHostname(hostname);
    m_client->setPort(port);

    if (!caCertPath.isEmpty()) {
        // Setup TLS/SSL configuration
        QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();

        // Load CA certificate
        QFile caFile(caCertPath);
        if (caFile.open(QIODevice::ReadOnly)) {
            QSslCertificate caCert(&caFile, QSsl::Pem);
            sslConfig.addCaCertificate(caCert);
        } else {
            qDebug() << "Failed to open CA certificate:" << caCertPath;
        }

        // Load client certificate and private key if provided
        if (!clientCertPath.isEmpty() && !clientKeyPath.isEmpty()) {
            QFile certFile(clientCertPath);
            QFile keyFile(clientKeyPath);

            if (certFile.open(QIODevice::ReadOnly) && keyFile.open(QIODevice::ReadOnly)) {
                QSslCertificate clientCert(&certFile, QSsl::Pem);
                QSslKey clientKey(&keyFile, QSsl::Rsa, QSsl::Pem);

                sslConfig.setLocalCertificate(clientCert);
                sslConfig.setPrivateKey(clientKey);
            } else {
                qDebug() << "Failed to open client certificate or key";
            }
        }

        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
        m_client->connectToHostEncrypted(sslConfig);  // Connect securely
    } else {
        // No TLS
        qDebug() << "Connecting without TLS";
        m_client->connectToHost();  // Plain connection
    }
}

/**
 * Publish a message to a topic
 * @param topic: topic name
 * @param message: payload
 * @param qos: quality of service
 * @param retained: retained flag
 */
void MqttClientWrapper::publishMessage(const QString &topic, const QString &message, quint8 qos, bool retained) {
    if (m_client->state() == QMqttClient::Connected) {
        m_client->publish(topic, message.toUtf8(), qos, retained);
        emit messagePublished(topic, message);
    } else {
        qDebug() << "MQTT not connected, cannot send:" << message;
    }
}
