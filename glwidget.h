#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QDebug>
class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void set_iterations(int i)
    {
        //qDebug() << i;
        if(i>0) m_iterations = i;
        //qDebug() << m_iterations;
    }
    // setter
    void set_depth(bool b)
    {
        m_depth = b;
    }
    void set_rotate(bool b)
    {
        m_rotate = b;
    }
    void set_vertices(double* d)
    {
        if(d != 0) m_vertices = d;
    }
    void set_speed(float f)
    {
        if(f >= 0.0f) m_speed = f;
        qDebug() << "speed: " << m_speed;
    }
    void set_rot_a(float f)
    {
        if(f >= 0.0f) m_rot_a = f;
    }
    void set_rot_b(float f)
    {
        if(f >= 0.0f) m_rot_b = f;
    }
    void set_rot_c(float f)
    {
        if(f >= 0.0f) m_rot_c = f;
    }
    void set_rot_d(float f)
    {
        if(f >= 0.0f) m_rot_d = f;
    }
    //background color
    void set_b_col_r(float f)
    {
        if(f>=0.0f && f<=1.0f) m_b_col_r = f;
    }
    void set_b_col_g(float f)
    {
        if(f >= 0.0f && f <= 1.0f) m_b_col_g = f;
    }
    void set_b_col_b(float f)
    {
        if(f >= 0.0f && f <= 1.0f) m_b_col_b = f;
    }
    //foreground color
    void set_f_col_r(float f)
    {
        if(f >= 0.0f && f <= 1.0f) m_f_col_r = f;
    }
    void set_f_col_g(float f)
    {
        if(f >= 0.0f && f <= 1.0f) m_f_col_g = f;
    }
    void set_f_col_b(float f)
    {
        if(f >= 0.0f && f <= 1.0f) m_f_col_b = f;
    }
    void set_f_col_a(float f)
    {
        if(f >= 0.0f && f <= 1.0f) m_f_col_a = f;
    }
    void set_invert(bool b)
    {
        m_invert = b;
    }

    // getter
    bool rotate()
    {
        return m_rotate;
    }

    void start_timer()
    {
        timer.start(m_speed);
        qDebug() << "started rotation: " << m_speed;
    }
    void stop_timer()
    {
        timer.stop();
        qDebug() << "stopped rotation" ;
    }

private:
    QTimer timer;
    int m_iterations;
    bool m_depth;
    double* m_vertices;
    // rotation
    bool m_rotate;
    float m_speed;
    float m_rot_a;
    float m_rot_b;
    float m_rot_c;
    float m_rot_d;
    // background color
    float m_b_col_r;
    float m_b_col_g;
    float m_b_col_b;
    // foreground color
    float m_f_col_r;
    float m_f_col_g;
    float m_f_col_b;
    float m_f_col_a;

    bool m_invert;
};

#endif // GLWIDGET_H
