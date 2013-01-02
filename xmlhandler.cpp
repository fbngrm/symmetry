#include "xmlhandler.h"
#include <QtCore>
#include <QtXml>
#include <QDebug>
#include <mainwindow.h>

using namespace std;

XMLHandler::XMLHandler()
{}

bool XMLHandler::write(QString & path, map<QString, QString> values)
{
   // write XML
    QDomDocument document;
    // set root
    QDomElement root = document.createElement("attractor");
    root.setAttribute("type", values.find("type")->second);
    document.appendChild(root);
    // set start point
    QDomElement start = document.createElement("start_point");
    root.appendChild(start);
    QDomElement start_x = document.createElement("start_x");
    start.appendChild(start_x);
    start_x.setAttribute("value", values.find("start_x")->second);
    QDomElement start_y = document.createElement("start_y");
    start.appendChild(start_y);
    start_y.setAttribute("value", values.find("start_y")->second);
    QDomElement start_z = document.createElement("start_z");
    start.appendChild(start_z);
    start_z.setAttribute("value", values.find("start_z")->second);
    // window size
    QDomElement window_size = document.createElement("window_size");
    root.appendChild(window_size);
    QDomElement width = document.createElement("width");
    window_size.appendChild(width);
    width.setAttribute("value", values.find("width")->second);
    QDomElement height = document.createElement("height");
    window_size.appendChild(height);
    height.setAttribute("value", values.find("height")->second);
    // set coefficients
    QDomElement coef = document.createElement("coefficients");
    root.appendChild(coef);
    QDomElement coef_a = document.createElement("coef_a");
    coef.appendChild(coef_a);
    coef_a.setAttribute("value", values.find("coef_a")->second);
    QDomElement coef_b = document.createElement("coef_b");
    coef.appendChild(coef_b);
    coef_b.setAttribute("value", values.find("coef_b")->second);
    QDomElement coef_c = document.createElement("coef_c");
    coef.appendChild(coef_c);
    coef_c.setAttribute("value", values.find("coef_c")->second);
    QDomElement coef_d = document.createElement("coef_d");
    coef.appendChild(coef_d);
    coef_d.setAttribute("value", values.find("coef_d")->second);
    // set iterations
    QDomElement iterations = document.createElement("iterations");
    root.appendChild(iterations);
    iterations.setAttribute("value", values.find("iterations")->second);
    // set colors
    QDomElement colors = document.createElement("colors");
    root.appendChild(colors);
    // set background
    QDomElement background = document.createElement("background");
    colors.appendChild(background);
    QDomElement back_col_r = document.createElement("back_col_r");
    background.appendChild(back_col_r);
    back_col_r.setAttribute("value", values.find("back_col_r")->second);
    QDomElement back_col_g = document.createElement("back_col_g");
    background.appendChild(back_col_g);
    back_col_g.setAttribute("value", values.find("back_col_g")->second);
    QDomElement back_col_b = document.createElement("back_col_b");
    background.appendChild(back_col_b);
    back_col_b.setAttribute("value", values.find("back_col_b")->second);
    QDomElement back_col_a = document.createElement("back_col_a");
    background.appendChild(back_col_a);
    back_col_a.setAttribute("value", values.find("back_col_a")->second);
    // set foreground
    QDomElement foreground = document.createElement("foreground");
    colors.appendChild(foreground);
    QDomElement fore_col_r = document.createElement("fore_col_r");
    foreground.appendChild(fore_col_r);
    fore_col_r.setAttribute("value", values.find("fore_col_r")->second);
    QDomElement fore_col_g = document.createElement("fore_col_g");
    foreground.appendChild(fore_col_g);
    fore_col_g.setAttribute("value", values.find("fore_col_g")->second);
    QDomElement fore_col_b = document.createElement("fore_col_b");
    foreground.appendChild(fore_col_b);
    fore_col_b.setAttribute("value", values.find("fore_col_b")->second);
    QDomElement fore_col_a = document.createElement("fore_col_a");
    foreground.appendChild(fore_col_a);
    fore_col_a.setAttribute("value", values.find("fore_col_a")->second);
    // set rotation
    QDomElement rotation = document.createElement("rotation");
    root.appendChild(rotation);
    QDomElement rot_a = document.createElement("rot_a");
    rotation.appendChild(rot_a);
    rot_a.setAttribute("value", values.find("rot_a")->second);
    QDomElement rot_b = document.createElement("rot_b");
    rotation.appendChild(rot_b);
    rot_b.setAttribute("value", values.find("rot_b")->second);
    QDomElement rot_c = document.createElement("rot_c");
    rotation.appendChild(rot_c);
    rot_c.setAttribute("value", values.find("rot_c")->second);
    QDomElement rot_d = document.createElement("rot_d");
    rotation.appendChild(rot_d);
    rot_d.setAttribute("value", values.find("rot_d")->second);
    QDomElement speed = document.createElement("speed");
    rotation.appendChild(speed);
    speed.setAttribute("value", values.find("speed")->second);
    QDomElement rot_auto = document.createElement("rot_auto");
    rotation.appendChild(rot_auto);
    rot_auto.setAttribute("value", values.find("rot_auto")->second);
    QDomElement rot_mouse = document.createElement("rot_mouse");
    rotation.appendChild(rot_mouse);
    rot_mouse.setAttribute("value", values.find("rot_mouse")->second);


    // write to file
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "faild to write file";
        return false;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.flush();
        file.close();
        qDebug() << "finished";
    }

    return true;

}

map<QString, QString> XMLHandler::read(QString & path)
{
    // read XML
    QDomDocument document;
    QFile file(path);
    map<QString, QString> values;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "failed to open file";
        return values;
    }
    else
    {
        if(!document.setContent(&file))
        {
            qDebug() << "failed to open file";
            return values;
        }
        file.close();
    }

    // get root element
    QDomElement root = document.firstChildElement();

    values = listElements(root);
    /*map<QString, QString>::iterator it;
    for ( it=values.begin() ; it != values.end(); it++ )
            qDebug() << (*it).first << " => " << (*it).second << endl;
            */
    return values;
}

map<QString, QString> XMLHandler::listElements(QDomElement root)
{
    map<QString, QString> values;

    QDomNodeList elements = root.childNodes();

    qDebug() << "total elements = " << elements.count();

    for (int i=0; i<elements.count(); i++)
    {
        QDomNode itemnode = elements.at(i);
        // convert to element
        if(itemnode.isElement())
        {
            QDomElement element = itemnode.toElement();
            // add iterations
            if(element.hasAttribute("value"))
            {
                values.insert(pair<QString, QString>(element.nodeName(),element.attribute("value", "-1")));
                qDebug() << element.nodeName() << " : " << element.attribute("value", "-1");
            }

            QDomNodeList elements2 = element.childNodes();
            qDebug() << "total elements 2 = " << elements2.count();
            // second
            for (int i=0; i<elements2.count(); i++)
            {
                QDomNode itemnode2 = elements2.at(i);
                // convert to element
                if(itemnode2.isElement())
                {
                    QDomElement element2 = itemnode2.toElement();
                    // add iterations
                    if(element2.hasAttribute("value"))
                    {
                        values.insert(pair<QString, QString>(element2.nodeName(),element2.attribute("value", "-1")));
                        qDebug() << element2.nodeName() << " : " << element2.attribute("value", "-1");
                    }
                }
            }
        }
    }

    return values;
}
