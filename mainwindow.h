#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>
#include <strangeattractor.h>
#include <lorenzattractor.h>


namespace Ui {
class MainWindow;
}
class QPrinter;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init_strange();
    void init_lorenz();
    void init_roessler();
    void init_gl_widget();
    map<QString, QString> map_values();
    void set_values(map<QString, QString> values);

    void set_lorenz(LorenzAttractor* l)
    {
        if(l != 0) m_lorenz = l;
    }

    void set_strange(StrangeAttractor* s)
    {
        if(s != 0) m_strange = s;
    }
    /*
    void set_roessler(RoesslerAttractor* r)
    {
        if(r != 0) m_roessler = r;
    }
*/
    Ui::MainWindow* get_ui()
    {
        return ui;
    }

public slots:
    void open_file();
    void save_file();
    void closeEvent(QCloseEvent* event);
    void create_attractor();
    void close_gl_wid();
    void start_timer();
    void file_print_preview();
    void file_print();
    void print( QPrinter* );
    void save_image();

private:
    Ui::MainWindow *ui;
    QString m_file;
    GLWidget* m_gl_wid;
    StrangeAttractor* m_strange;
    LorenzAttractor* m_lorenz;
    bool m_save_state;
    //RoesslerAttractor* m_roessler;
    enum Tabs {
        STRANGE, LORENZ, ROESSLER
    };
};

#endif // MAINWINDOW_H
