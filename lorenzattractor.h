#ifndef LORENZATTRACTOR_H
#define LORENZATTRACTOR_H

#include <QDebug>

class LorenzAttractor
{
public:
    // constructor
    LorenzAttractor()
    {}
    // destructor
    ~LorenzAttractor()
    {
        delete m_vertices;
    }

    //methods
    double *create();

    //accessors
    // start point
    double start_x() const
    {
        return m_start_x;
    }
    double start_y() const
    {
        return m_start_y;
    }

    double start_z() const
    {
        return m_start_z;
    }
    // coefficients
    double coe_a() const
    {
        return m_coe_a;
    }
    double coe_b() const
    {
        return m_coe_b;
    }
    double coe_c() const
    {
        return m_coe_c;
    }
    double coe_h() const
    {
        return m_coe_h;
    }
    // iterations
    int iterations() const
    {
        return m_iterations;
    }
    // vertices
    double* vertices() const
    {
        return m_vertices;
    }

    // setter
    // start point
    void set_start_x(double x)
    {
        qDebug() << "set_start_x: " << x;
        m_start_x = x;
    }
    void set_start_y(double y)
    {
        qDebug() << "set_start_y: " << y;
        m_start_y = y;
    }
    void set_start_z(double z)
    {
        qDebug() << "set_start_z: " << z;
        m_start_z = z;
    }
    // coefficients
    void set_coe_a(double a)
    {
        qDebug() << "set_coe_a: " << a;
        m_coe_a = a;
    }
    void set_coe_b(double b)
    {
        qDebug() << "set_coe_b: " << b;
        m_coe_b = b;
    }
    void set_coe_c(double c)
    {
        qDebug() << "set_coe_c: " << c;
        m_coe_c = c;
    }
    void set_coe_h(double h)
    {
        qDebug() << "set_coe_h: " << h;
        m_coe_h = h;
    }
    // iterations
    void set_iterations(int i)
    {
        qDebug() << "set_iterations: " << i;
        m_iterations = i;
    }
    // vertices
    void set_vertices(double* v)
    {
        qDebug() << "set_vertices";
        m_vertices = v;
    }
    void init_vertices(int size)
    {
        m_vertices = new double[size];
        qDebug() << "init_vertices: " ;
    }

private:
    double  m_start_x,
            m_start_y,
            m_start_z,
            m_coe_a,
            m_coe_b,
            m_coe_c,
            m_coe_h;
    int     m_iterations;
    double* m_vertices;
};

#endif // LORENZATTRACTOR_H
