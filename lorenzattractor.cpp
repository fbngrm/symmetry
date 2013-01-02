#include "lorenzattractor.h"

double* LorenzAttractor::create()
{
        double x1,y1,z1;
        for (int i=0; i<3*m_iterations; i+=3) {
                x1 = m_start_x + m_coe_h * m_coe_a * (m_start_y - m_start_x);
                y1 = m_start_y + m_coe_h * (m_start_x * (m_coe_b - m_start_z) - m_start_y);
                z1 = m_start_z + m_coe_h * (m_start_x * m_start_y - m_coe_c * m_start_z);
                m_start_x = x1;
                m_start_y = y1;
                m_start_z = z1;

        //draw the new point
        m_vertices[i]   = m_start_x/15.0;
        m_vertices[i+1] = m_start_y/15.0;
        m_vertices[i+2] = m_start_z/15.0;
        }
        return m_vertices;
}
