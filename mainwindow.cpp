#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <QToolBar>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QDateTime>
#include <xmlhandler.h>

using namespace std;

// =============================
//          Constructor
// =============================

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // ask to save before quit
    m_save_state = false;

    // file menu
    QMenu*  file_menu = menuBar()->addMenu( "&File" );
    // open file action
    QAction* open_action = file_menu->addAction( "&Open", this, SLOT(open_file()) );
    open_action->setShortcut( QKeySequence::Open );
    // save file action
    QAction* save_action = file_menu->addAction( "&Save", this, SLOT(save_file()) );
    save_action->setShortcut( QKeySequence::Save );
    file_menu->addSeparator();
    // save image
    QAction* save_image = file_menu->addAction( "&Save Image", this, SLOT(save_image()) );
    save_image->setShortcut( QKeySequence::SaveAs );
    file_menu->addSeparator();
    // add close action
    QAction* quit_action = file_menu->addAction( "&Quit", this, SLOT(close()) );
    quit_action->setShortcut( QKeySequence::Quit );
    file_menu->addSeparator();
    // add print actions
    file_menu->addAction( "&Print preview", this, SLOT(file_print_preview()) );
    QAction* print_action   = file_menu->addAction( "&Print", this, SLOT(file_print()) );
    print_action->setShortcut( QKeySequence::Print );

    // create toolbar, set icon size, and add actions
     QToolBar*   toolBar = addToolBar( "Standard" );
     QStyle*     style   = this->style();
     QSize       size    = style->standardIcon(QStyle::SP_DesktopIcon).actualSize( QSize(20, 20) );
     toolBar->setIconSize(size );
     open_action->setIcon(style->standardIcon(QStyle::SP_DialogOpenButton));
     save_action->setIcon(style->standardIcon(QStyle::SP_DialogSaveButton));
     save_image->setIcon(style->standardIcon(QStyle::SP_ArrowDown));
     toolBar->addAction(open_action);
     toolBar->addAction(save_action);
     toolBar->addAction(save_image);

    // setup the ui
    ui->setupUi(this);

    // render
    connect(ui->but_ren, SIGNAL(clicked()),
            this, SLOT(create_attractor()));

    connect(ui->but_stop, SIGNAL(clicked()),
            this, SLOT(close_gl_wid()));

    connect(ui->but_load, SIGNAL(clicked()),
            this, SLOT(start_timer()));


}

// =============================
//          Destructor
// =============================

MainWindow::~MainWindow()
{
    delete ui;
    delete m_gl_wid;
    delete m_strange;
    delete m_lorenz;
}

// =============================
//            Slots
// =============================

void MainWindow::open_file()
{
    statusBar()->showMessage("Open file ...");
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), getenv( "HOME" ), tr("Files (*.xml)"));
    if( !filename.endsWith( ".xml" ) && !filename.endsWith( ".XML" ))
    {
        QMessageBox::warning( this, "Read Values", "Wrong file type" );
        return;
    }
    XMLHandler handler;
    map<QString, QString> values = handler.read(filename);
    if(values.empty())
    {
        QMessageBox::warning( this, "Read values", "Error reading data." );
        return;
    }
    else
    {
        qDebug() << "set values";
        set_values(values);
        statusBar()->showMessage("Read values~ complete");
    }
}


void MainWindow::save_file()
{
    statusBar()->showMessage("Save file ...");
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), getenv( "HOME" ), tr("Files (*.xml*)"));

    if( !filename.endsWith( ".xml" ) && !filename.endsWith( ".XML" ) )
    {
        filename.append( ".xml" );
    }
    XMLHandler handler;
    if(!handler.write(filename, map_values()))
    {
        QMessageBox::warning( this, "Save Values", "Error saving data." );
        return;
    }
    statusBar()->showMessage("Saving file ~ complete");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // if nothing has changed ~> quit
    if(!m_save_state)
    {
        event->accept();
        return;
    }
        // check if user wants to save before quitting
      while (true)
        switch ( QMessageBox::warning( this, "Attractors",
            "Do you want to save the values before you quit?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel ) )
        {
          case QMessageBox::Save:
            // if save not successful ask again
            save_file();
            return;
          case QMessageBox::Discard:
            event->accept();
            return;
          default:    // "Cancel"
            event->ignore();
            return;
        }
}

void  MainWindow::file_print_preview()
{
    if(!m_save_state)
    {
        statusBar()->showMessage("No image to print");
        return;
    }
  // display print preview dialog
  QPrinter             printer( QPrinter::HighResolution );
  QPrintPreviewDialog  preview( &printer, this );
  connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(print(QPrinter*)) );
  preview.exec();
}

void  MainWindow::file_print()
{
    if(!m_save_state)
    {
        statusBar()->showMessage("No image to print");
        return;
    }
    statusBar()->showMessage("Print Image");
  // display print dialog and if accepted print
  QPrinter       printer( QPrinter::HighResolution );
  QPrintDialog   dialog( &printer, this );
  if ( dialog.exec() == QDialog::Accepted ) print( &printer );
}

void  MainWindow::print( QPrinter* printer )
{
  // create painter for drawing print page
  QPainter painter( printer );
  int      w = printer->pageRect().width();
  int      h = printer->pageRect().height();
  QRect    page( 0, 0, w, h );

  // create a font appropriate to page size
  QFont    font = painter.font();
  font.setPixelSize( (w+h) / 100 );
  painter.setFont( font );

  // draw labels in corners of page
  painter.drawText(page, Qt::AlignTop    | Qt::AlignLeft, "Attractor");
  painter.drawText(page, Qt::AlignBottom | Qt::AlignLeft, QString(getenv("USER")));
  painter.drawText(page, Qt::AlignBottom | Qt::AlignRight,
                    QDateTime::currentDateTime().toString(Qt::DefaultLocaleShortDate));

  // draw simulated landscape
  page.adjust( w/20, h/20, -w/20, -h/20 );
  QPixmap image = QPixmap::grabWidget(m_gl_wid);
  painter.drawPixmap(page, image);
  statusBar()->showMessage("");
}

void MainWindow::create_attractor()
{
    qDebug() << "create_attractor";
    int index = ui->tabWidget->currentIndex();
    qDebug() << "index: " << index;
    switch (index) {
      case STRANGE:
        init_strange();
        qDebug() << "TAB: 0";
        break;
      case LORENZ:
        init_lorenz();
        qDebug() << "TAB: 1";
        break;
      case ROESSLER:
        init_roessler();
        qDebug() << "TAB: 2";
        break;
      default:
        qDebug() << "no tab selected";
    }
}

void MainWindow::close_gl_wid()
{
    m_gl_wid->stop_timer();
}

void MainWindow::start_timer()
{
    m_gl_wid->start_timer();
}

void MainWindow::save_image( )
{
    if(!m_save_state)
    {
        statusBar()->showMessage("No image to save");
        return;
    }
    statusBar()->showMessage("Save Image");

    QString filename = QFileDialog::getSaveFileName( this, "Save File", getenv( "HOME" ), "PNG Image (*.png *.PNG)" );
    if( !filename.endsWith( ".png" ) && !filename.endsWith( ".PNG" ) )
    {
        filename.append( ".png" );
    }

    QPixmap image = QPixmap::grabWidget(m_gl_wid);
    if( !image.save( filename, "PNG", 100 ))
    {
        QMessageBox::warning( this, "Save Image", "Error saving image." );
    }
    statusBar()->showMessage("");
}

// =============================
//           Methods
// =============================

// create a map with the gui values to write the xml file
map<QString, QString> MainWindow::map_values()
{
    map<QString, QString> values;
    QString number;

    int index = ui->tabWidget->currentIndex();
    switch (index) {
      case STRANGE:
        values.insert(pair<QString,QString>("type", "strange_attractor"));
        // start point
        values.insert(pair<QString,QString>("start_x", number.setNum(ui->spin_sta_x->value())));
        values.insert(pair<QString,QString>("start_y", number.setNum(ui->spin_sta_y->value())));
        values.insert(pair<QString,QString>("start_z", "-1"));
        // coefficients
        values.insert(pair<QString,QString>("coef_a", number.setNum(ui->spin_coe_a->value())));
        values.insert(pair<QString,QString>("coef_b", number.setNum(ui->spin_coe_b->value())));
        values.insert(pair<QString,QString>("coef_c", number.setNum(ui->spin_coe_c->value())));
        values.insert(pair<QString,QString>("coef_d", number.setNum(ui->spin_coe_d->value())));
        break;
      case LORENZ:
        values.insert(pair<QString,QString>("type", "lorenz_attractor"));
        // start point
        values.insert(pair<QString,QString>("start_x", number.setNum(ui->spin_sta_x_l->value())));
        values.insert(pair<QString,QString>("start_y", number.setNum(ui->spin_sta_y_l->value())));
        values.insert(pair<QString,QString>("start_z", number.setNum(ui->spin_sta_z_l->value())));
        // coefficients
        values.insert(pair<QString,QString>("coef_a", number.setNum(ui->spin_coe_a_l->value())));
        values.insert(pair<QString,QString>("coef_b", number.setNum(ui->spin_coe_b_l->value())));
        values.insert(pair<QString,QString>("coef_c", number.setNum(ui->spin_coe_c_l->value())));
        values.insert(pair<QString,QString>("coef_d", number.setNum(ui->spin_coe_h_l->value())));
        break;
      default:
        values.insert(pair<QString,QString>("type", "error"));
        return values;
    }

    // window size
    values.insert(pair<QString,QString>("width", number.setNum(ui->spin_win_wid->value())));
    values.insert(pair<QString,QString>("height", number.setNum(ui->spin_win_hei->value())));
    // iterations
    values.insert(pair<QString,QString>("iterations", number.setNum(ui->spin_iter->value())));
    // colors
    values.insert(pair<QString,QString>("back_col_r", number.setNum(ui->spin_back_col_r->value())));
    values.insert(pair<QString,QString>("back_col_g", number.setNum(ui->spin_back_col_g->value())));
    values.insert(pair<QString,QString>("back_col_b", number.setNum(ui->spin_back_col_b->value())));
    values.insert(pair<QString,QString>("back_col_a", "1"));
    values.insert(pair<QString,QString>("fore_col_r", number.setNum(ui->spin_fore_col_r->value())));
    values.insert(pair<QString,QString>("fore_col_g", number.setNum(ui->spin_fore_col_g->value())));;
    values.insert(pair<QString,QString>("fore_col_b", number.setNum(ui->spin_fore_col_b->value())));
    values.insert(pair<QString,QString>("fore_col_a", number.setNum(ui->spin_fore_col_a->value())));
    // rotation
    values.insert(pair<QString,QString>("rot_a", number.setNum(ui->spin_rot_a->value())));
    values.insert(pair<QString,QString>("rot_b", number.setNum(ui->spin_rot_b->value())));
    values.insert(pair<QString,QString>("rot_c", number.setNum(ui->spin_rot_c->value())));
    values.insert(pair<QString,QString>("rot_d", number.setNum(ui->spin_rot_d->value())));
    values.insert(pair<QString,QString>("speed", number.setNum(ui->spin_rot_spe->value())));
    values.insert(pair<QString,QString>("rot_auto", ui->check_rot ? "false" : "true"));
    values.insert(pair<QString,QString>("rot_mouse", ui->check_rot_2 ? "false" : "true"));
/*    map<QString, QString>::iterator it;
    for ( it=values.begin() ; it != values.end(); it++ )
        qDebug() << (*it).first << " => " << (*it).second << endl;
*/
    return values;
}
// write values from xml file to gui
void MainWindow::set_values(map<QString, QString> values)
{
    QString type = values.find("type")->second;
    qDebug() << "type: " << type;
    int index;
    if(type.compare("strange_attractor"))
    {
        qDebug() << "STRANGE";
        index = STRANGE;
        ui->tabWidget->setCurrentIndex(STRANGE);
    }
    else if(type.compare("lorenz_attractor"))
    {
        qDebug() << "LORENZ";
        index = LORENZ;
        ui->tabWidget->setCurrentIndex(LORENZ);
    }
    else
    {
        index = ROESSLER;
    }

    switch (index) {
      case STRANGE:
        ui->spin_sta_x->setValue(values.find("start_x")->second.toDouble());
        ui->spin_sta_y->setValue(values.find("start_y")->second.toDouble());

        // coefficients
        ui->spin_coe_a->setValue(values.find("coef_a")->second.toDouble());
        ui->spin_coe_b->setValue(values.find("coef_b")->second.toDouble());
        ui->spin_coe_c->setValue(values.find("coef_c")->second.toDouble());
        ui->spin_coe_d->setValue(values.find("coef_d")->second.toDouble());
        break;
      case LORENZ:
        ui->spin_sta_x_l->setValue(values.find("start_x")->second.toDouble());
        ui->spin_sta_y_l->setValue(values.find("start_y")->second.toDouble());
        ui->spin_sta_z_l->setValue(values.find("start_z")->second.toDouble());

        // coefficients
        ui->spin_coe_a_l->setValue(values.find("coef_a")->second.toDouble());
        ui->spin_coe_b_l->setValue(values.find("coef_b")->second.toDouble());
        ui->spin_coe_c_l->setValue(values.find("coef_c")->second.toDouble());
        ui->spin_coe_h_l->setValue(values.find("coef_d")->second.toDouble());
        break;
      default:
        QMessageBox::warning( this, "Load data", "Error loading data!" );

    }
    // iterations
    ui->spin_iter->setValue(values.find("iterations")->second.toInt());
}

void MainWindow::init_strange()
{
    qDebug() << "init_strange ~ start";

    //statusBar()->showMessage("Rendering Strange Atrractor...");

    m_strange = new StrangeAttractor;
    m_strange->set_start_x(ui->spin_sta_x->value());
    m_strange->set_start_y(ui->spin_sta_y->value());
    m_strange->set_coe_a(ui->spin_coe_a->value());
    m_strange->set_coe_b(ui->spin_coe_b->value());
    m_strange->set_coe_c(ui->spin_coe_c->value());
    m_strange->set_coe_d(ui->spin_coe_d->value());
    m_strange->set_iterations(ui->spin_iter->value());
    m_strange->init_vertices(2*ui->spin_iter->value());
    m_strange->create();

    statusBar()->showMessage("Rendering Strange-Atrractor ~ complete");

    m_gl_wid = new GLWidget;
    m_gl_wid->set_vertices(m_strange->vertices());
    m_gl_wid->set_depth(false);

    qDebug() << "init_strange ~ end";
    init_gl_widget();

    m_save_state = true;
}

void  MainWindow::init_lorenz()
{
    qDebug() << "init_lorenz ~ start";

    m_lorenz = new LorenzAttractor;
    m_lorenz->set_start_x(ui->spin_sta_x_l->value());
    m_lorenz->set_start_y(ui->spin_sta_y_l->value());
    m_lorenz->set_start_z(ui->spin_sta_z_l->value());
    m_lorenz->set_coe_a(ui->spin_coe_a_l->value());
    m_lorenz->set_coe_b(ui->spin_coe_b_l->value());
    m_lorenz->set_coe_c(ui->spin_coe_c_l->value());
    m_lorenz->set_coe_h(ui->spin_coe_h_l->value());
    m_lorenz->set_iterations(ui->spin_iter->value());
    m_lorenz->init_vertices(3*ui->spin_iter->value());
    m_lorenz->create();

    statusBar()->showMessage("Rendering Lorenz-Atrractor ~ complete");

    m_gl_wid = new GLWidget;
    m_gl_wid->set_vertices(m_lorenz->vertices());
    m_gl_wid->set_depth(true);

    qDebug() << "init_lorenz ~ end";
    init_gl_widget();

    m_save_state = true;
}

void  MainWindow::init_roessler()
{

}
void MainWindow::init_gl_widget()
{
    qDebug() << "init_gl_widget";

    // iterations
    m_gl_wid->set_iterations(ui->spin_iter->value());
    // init rotation
    m_gl_wid->set_rotate(ui->check_rot->checkState());
    m_gl_wid->set_speed(ui->spin_rot_spe->value());
    m_gl_wid->set_rot_a(ui->spin_rot_a->value());
    m_gl_wid->set_rot_b(ui->spin_rot_b->value());
    m_gl_wid->set_rot_c(ui->spin_rot_c->value());
    m_gl_wid->set_rot_d(ui->spin_rot_d->value());
    // init the background color
    m_gl_wid->set_b_col_r(ui->spin_back_col_r->value());
    m_gl_wid->set_b_col_g(ui->spin_back_col_g->value());
    m_gl_wid->set_b_col_b(ui->spin_back_col_b->value());
    // init the foreground color
    qDebug() << "col: " << ui->spin_fore_col_r->value();
    m_gl_wid->set_f_col_r(ui->spin_fore_col_r->value());
    m_gl_wid->set_f_col_g(ui->spin_fore_col_g->value());
    m_gl_wid->set_f_col_b(ui->spin_fore_col_b->value());
    m_gl_wid->set_f_col_a(ui->spin_fore_col_a->value());
    // invert
    m_gl_wid->set_invert(ui->check_inv->checkState());
    // init point size
    // ... later
    // init size
    m_gl_wid->setMinimumSize(ui->spin_win_wid->value(), ui->spin_win_hei->value());
    m_gl_wid->show();

    // start the rotation ~ timer
    if(m_gl_wid->rotate())
    {   qDebug() << "timer start ...";
        m_gl_wid->start_timer();
        qDebug() << "... m_speed";
    }
}
