#include "strangeattractor.h"
#include <QDebug>
#include <math.h>

double *StrangeAttractor::create()
{
    qDebug() << "strange attractor - create vertices: ";

    // iterate through the equations many times, drawing one point for each iteration
    for (int i = 0; i < 2*m_iterations; i+=2) {

        // compute a new point using the strange attractor equations
        double xnew = sin(m_start_y * m_coe_b) + m_coe_c * sin(m_start_x * m_coe_b);
        double ynew = sin(m_start_x * m_coe_a) + m_coe_d * sin(m_start_y * m_coe_a);

        m_start_x = xnew;
        m_start_y = ynew;

        //qDebug() << xnew << " : " << ynew ;

        // save the new point
        m_vertices[i]   = xnew;
        m_vertices[i+1] = ynew;
    }
    return m_vertices;
}
