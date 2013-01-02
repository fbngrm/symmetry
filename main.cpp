#include <QtGui/QApplication>
#include "mainwindow.h"
#include <GL/glut.h>
#include <glwidget.h>

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //GLWidget g;
    //g.show();
    
    return a.exec();
}
