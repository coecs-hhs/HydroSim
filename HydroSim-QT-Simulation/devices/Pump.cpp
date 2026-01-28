#include "Pump.h"

/**
 * @brief Constructs a Pump object.
 * @param zone Zone/group the pump belongs to
 * @param id Unique identifier for the pump
 * @param position 2D position (x,y)
 * @param status Initial on/off state
 * @param flow Initial flow value
 * @param quality Initial quality value
 * @param speed Initial pump speed
 * @param parent Optional QObject parent
 */
Pump::Pump(int zone, int id, const QPointF &position, bool status, double flow, double quality, double speed, QObject *parent)
    : Device("Pump", zone, id, position, parent), // Inherit from Device
      m_status(status),
      m_flow(flow),
      m_quality(quality),
      m_speed(speed)
{
    // Constructor body is empty because initialization is done in initializer list
}

// Setter for pump status (on/off)
void Pump::setStatus(bool status)
{
    if (m_status == status) // No change, skip
        return;

    m_status = status;
    emit statusChanged(m_status); // Notify QML/UI
}

// Setter for pump flow
void Pump::setFlow(double flow)
{
    if (m_flow == flow)
        return;

    m_flow = flow;
    emit flowChanged(m_flow);
}

// Setter for pump quality
void Pump::setQuality(double quality)
{
    if (m_quality == quality)
        return;

    m_quality = quality;
    emit qualityChanged(m_quality);
}

// Setter for pump speed
void Pump::setSpeed(double speed)
{
    if (m_speed == speed)
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}
