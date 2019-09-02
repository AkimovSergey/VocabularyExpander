#include "trainerwindow.h"
#include "ui_trainerwindow.h"
#include "globals.h"

TrainerWindow::TrainerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainerWindow)
{
    ui->setupUi(this);
    ui->tb_from->installEventFilter(this);
    SetProgresslabel();
}

TrainerWindow::~TrainerWindow()
{
    delete ui;
}

void TrainerWindow::StartExercise()
{
    m_coach.StartExercise();
    ui->bt_check->setText("Next");
    on_bt_check_clicked();
    exec();
}

void TrainerWindow::closeEvent(QCloseEvent * event)
{
    Globals::g_dictionary->SaveExercises();
}

bool TrainerWindow::eventFilter(QObject* o, QEvent* e)
{
   if(e->type() == QEvent::KeyPress)
   {
      QKeyEvent* eventKey = static_cast<QKeyEvent*>(e);
      if(eventKey->key() == Qt::Key_Return)
      {
         on_bt_check_clicked();
         return true;
      }
   }
   return o->eventFilter(o, e);
}
void TrainerWindow::on_bt_check_clicked()
{
    QPalette palette = ui->tb_from->palette();
    if(ui->bt_check->text() == "Check")
    {
        if(m_coach.CheckAnswer(ui->tb_from->toPlainText()))
            palette.setColor(QPalette::Base, Qt::green);
        else
            palette.setColor(QPalette::Base, Qt::red);
        ui->tb_from->setPalette(palette);
        ui->tb_from->setText(m_coach.ValueToCheck());
        ui->bt_check->setText("Next");
    }
    else
    {
        if(!m_coach.Next())
        {
            close();
            return;
        }
        palette.setColor(QPalette::Base, Qt::white);
        ui->tb_from->setPalette(palette);
        ui->tb_from->clear();
        ui->tb_to->setText(m_coach.ValueToTranslate());
        m_progress_label->setText(m_coach.GetCurrentProgress());
        ui->bt_check->setText("Check");
    }
}


void TrainerWindow::SetProgresslabel()
{
    static const int width = 48;
    static const int height = 24;
    m_progress_label = new QLabel("nope", ui->tb_from);
    // just put manually position
    // have to be installed in resizing
    m_progress_label->setGeometry(170, 45, width, height);
    m_progress_label->show();
}

