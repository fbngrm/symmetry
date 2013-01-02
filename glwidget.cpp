#include "glwidget.h"
#include <GL/glut.h>
#include <math.h>
#include <strangeattractor.h>
#include <lorenzattractor.h>
#include <QDebug>
#include <mainwindow.h>

using namespace std;

// =============================
//          Constructor
// =============================

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

// =============================
//          Destructor
// =============================

GLWidget::~GLWidget()
{
    delete m_vertices;
}

// =============================
//           Methods
// =============================

void GLWidget::initializeGL()
{
    qDebug() << m_f_col_r;

    if(!m_invert)
    {
        glClearColor(m_b_col_r, m_b_col_g, m_b_col_b, 1.0f);
        glColor4f(m_f_col_r, m_f_col_g, m_f_col_b, m_f_col_a);

    }
    else
    {
        glClearColor(m_f_col_r, m_f_col_g, m_f_col_b, 1.0f);
        glColor4f(m_b_col_r, m_b_col_g, m_b_col_b, m_f_col_a);
    }
    if(m_depth)
    {
        glEnable(GL_DEPTH_TEST);
    }

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::paintGL()
{

    qDebug() << "paintGL";
    qDebug() << "iterations: " << m_iterations;
    qDebug() << "depth: " << m_depth;
    qDebug() << "rotate: " << m_rotate;

    // clear the screen
    if(m_depth)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        qDebug() << "set depth buffer";
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    // rotation
    if(m_rotate)
    {
        glRotatef(m_rot_a, m_rot_b, m_rot_c, m_rot_d);
    }
    // draw some points
    glBegin(GL_POINTS);

    // iterate through the equations many times, drawing one point for each iteration
    if(m_depth)
    {
        qDebug() << "draw 3d attractor...";
        for (int i=0; i<3*m_iterations; i+=3) {
            //draw the new point
            glVertex3d(m_vertices[i], m_vertices[i+1], m_vertices[i+2]);
            //qDebug() << m_vertices[i] << " : " <<  m_vertices[i+1] << " : " << m_vertices[i+2];
        }
    }
    else
    {
        qDebug() << "draw 2d attractor...";
        for (int i=0; i<2*m_iterations; i+=2) {
            //draw the new point
            glVertex2d(m_vertices[i], m_vertices[i+1]);
            //qDebug() << m_vertices[i] << " : " <<  m_vertices[i+1];
        }
    }
    glEnd();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/(float)h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5, 0,0,0, 0,1,0);
}
