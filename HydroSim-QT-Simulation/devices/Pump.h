#pragma once

#include "Device.h"

/**
 * @brief The Pump class represents a pump device in the system.
 * 
 * Inherits from Device, so it has a type, ID, zone, and position.
 * Additional properties for Pump include status, flow, quality, and speed.
 * All properties are accessible from QML and notify changes dynamically.
 */
class Pump : public Device
{
    Q_OBJECT

    // QML-accessible properties
    Q_PROPERTY(bool status READ status WRITE setStatus NOTIFY statusChanged)    // On/off state
    Q_PROPERTY(double flow READ flow WRITE setFlow NOTIFY flowChanged)         // Flow rate of the pump
    Q_PROPERTY(double quality READ quality WRITE setQuality NOTIFY qualityChanged) // Quality of pumped medium
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged)     // Pump speed (RPM or other units)

public:
    /**
     * @brief Constructs a Pump object.
     * @param id Unique identifier of the pump
     * @param zone Zone/group the pump belongs to
     * @param position Position in 2D space (x,y)
     * @param status Initial on/off state
     * @param flow Initial flow value
     * @param quality Initial quality value
     * @param speed Initial speed value
     * @param parent Optional QObject parent
     */
    Pump(int id, int zone, const QPointF &position, bool status, double flow, double quality, double speed, QObject *parent = nullptr);

    // Getters for QML binding
    bool status() const { return m_status; }
    double flow() const { return m_flow; }
    double quality() const { return m_quality; }
    double speed() const { return m_speed; }

public slots:
    // Setters that also emit signals for QML binding
    void setStatus(bool status);
    void setFlow(double flow);
    void setQuality(double quality);
    void setSpeed(double speed);

signals:
    // Signals emitted whenever a property changes
    void statusChanged(bool status);
    void flowChanged(double flow);
    void qualityChanged(double quality);
    void speedChanged(double speed);

private:
    bool m_status;    // Current on/off status
    double m_flow;    // Current flow value
    double m_quality; // Current quality value
    double m_speed;   // Current speed value
};
