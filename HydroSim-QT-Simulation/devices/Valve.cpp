#include "Valve.h"

/**
 * @brief Constructs a Valve object.
 * @param zone Zone/group the valve belongs to
 * @param id Unique identifier for the valve
 * @param position 2D position (x,y)
 * @param status Initial open/closed state
 * @param flow Initial flow value
 * @param quality Initial quality value
 * @param parent Optional QObject parent
 */
Valve::Valve(int zone, int id, const QPointF &position, bool status, double flow, double quality, QObject *parent)
    : Device("Tcv", zone, id, position, parent), // Inherit from Device
      m_status(status),
      m_flow(flow),
      m_quality(quality)
{
    // Constructor body empty; all initialization done in initializer list
}

// Setter for valve status (open/closed)
void Valve::setStatus(bool status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(m_status); // Notify QML/UI
}

// Setter for valve flow
void Valve::setFlow(double flow)
{
    if (m_flow == flow)
        return;

    m_flow = flow;
    emit flowChanged(m_flow);
}

// Setter for valve quality
void Valve::setQuality(double quality)
{
    if (m_quality == quality)
        return;

    m_quality = quality;
    emit qualityChanged(m_quality);
}
