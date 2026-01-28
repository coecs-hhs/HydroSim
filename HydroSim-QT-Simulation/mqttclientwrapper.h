#pragma once

#include <QObject>
#include <QMqttClient>
#include <QSslConfiguration>

/**
 * @brief Wrapper around QMqttClient with optional TLS support
 *        for easy MQTT publishing and connection handling.
 */
class MqttClientWrapper : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent QObject parent
     */
    explicit MqttClientWrapper(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~MqttClientWrapper() override = default;

    /**
     * @brief Connect to an MQTT broker
     * @param hostname Broker hostname or IP
     * @param port Broker port
     * @param caCertPath Path to CA certificate (optional, default: empty)
     * @param clientCertPath Path to client certificate (optional)
     * @param clientKeyPath Path to client private key (optional)
     */
    void connectToBroker(const QString &hostname, quint16 port,
                         const QString &caCertPath = QString(),
                         const QString &clientCertPath = QString(),
                         const QString &clientKeyPath = QString());

    /**
     * @brief Publish a message to a topic
     * @param topic Topic name
     * @param message Message payload
     * @param qos Quality of Service (0, 1, 2)
     * @param retained Retained flag
     */
    void publishMessage(const QString &topic, const QString &message, quint8 qos = 0, bool retained = false);

signals:
    /// Emitted when the client successfully connects to the broker
    void connected();

    /// Emitted when the client disconnects from the broker
    void disconnected();

    /// Emitted after a message is published
    void messagePublished(const QString &topic, const QString &message);

private:
    QMqttClient* m_client; ///< The internal MQTT client

    /// Setup internal connections for signals and debug messages
    void setupConnections();
};
