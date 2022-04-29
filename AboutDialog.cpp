#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "Configuration.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
#warning rename about dialog UI elements
    ui->label_2->setText(tr("Linkuit Studio ") + SW_VERSION_STRING);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
