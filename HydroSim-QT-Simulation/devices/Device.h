#pragma once

#include <QObject>
#include <QPointF>
#include <QString>

/**
 * @brief The Device class represents a generic device in the system.
 * 
 * This is the base class for all devices like Pump and Valve.
 * It provides core properties that all devices share:
 * - Type (string)
 * - Unique ID
 * - Zone or group ID
 * - Position in 2D space
 * 
 * All properties are read-only and constant, making them ideal for QML.
 */
class Device : public QObject
{
    Q_OBJECT

    // QML-accessible, read-only properties
    Q_PROPERTY(int id READ id CONSTANT)            // Unique device identifier
    Q_PROPERTY(int zone READ zone CONSTANT)        // Zone/group the device belongs to
    Q_PROPERTY(QPointF position READ position CONSTANT) // 2D position as (x, y)
    Q_PROPERTY(QString type READ type CONSTANT)   // Device type (e.g., "Pump" or "Valve")

public:
    /**
     * @brief Constructs a generic Device object.
     * @param type The type of device (used for identification in QML/UI)
     * @param zone The zone or group the device belongs to
     * @param id Unique identifier for the device
     * @param position Position in 2D space (x,y)
     * @param parent Optional QObject parent
     */
    explicit Device(QString type, int zone, int id, const QPointF &position, QObject *parent = nullptr)
        : QObject(parent), m_type(type), m_id(id), m_zone(zone), m_position(position) {}

    // Getters for QML binding
    QString type() const { return m_type; }
    int id() const { return m_id; }
    int zone() const { return m_zone; }
    QPointF position() const { return m_position; }

protected:
    QString m_type;   // Device type (e.g., Pump", "Valve")
    int m_id;         // Unique ID
    int m_zone;       // Zone/group ID
    QPointF m_position; // 2D position (x, y)
};
