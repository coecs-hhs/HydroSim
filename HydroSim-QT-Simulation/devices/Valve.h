#pragma once

#include "Device.h"

/**
 * @brief The Valve class represents a valve device in the system.
 * 
 * Inherits from Device, so it has a type, ID, zone, and position.
 * Additional properties for Valve include status, flow, and quality.
 * All properties are accessible from QML and notify changes dynamically.
 */
class Valve : public Device
{
    Q_OBJECT

    // QML-accessible properties
    Q_PROPERTY(bool status READ status WRITE setStatus NOTIFY statusChanged)    // Open/closed state
    Q_PROPERTY(double flow READ flow WRITE setFlow NOTIFY flowChanged)         // Flow through the valve
    Q_PROPERTY(double quality READ quality WRITE setQuality NOTIFY qualityChanged) // Quality of fluid passing

public:
    /**
     * @brief Constructs a Valve object.
     * @param id Unique identifier of the valve
     * @param zone Zone/group the valve belongs to
     * @param position Position in 2D space (x,y)
     * @param status Initial open/closed state
     * @param flow Initial flow value
     * @param quality Initial quality value
     * @param parent Optional QObject parent
     */
    Valve(int id, int zone, const QPointF &position, bool status, double flow, double quality, QObject *parent = nullptr);

    // Getters for QML binding
    bool status() const { return m_status; }
    double flow() const { return m_flow; }
    double quality() const { return m_quality; }

public slots:
    // Setters that also emit signals for QML binding
    void setStatus(bool status);
    void setFlow(double flow);
    void setQuality(double quality);

signals:
    // Signals emitted whenever a property changes
    void statusChanged(bool status);
    void flowChanged(double flow);
    void qualityChanged(double quality);

private:
    bool m_status;    // Current open/closed status
    double m_flow;    // Current flow through the valve
    double m_quality; // Current quality of fluid
};
