#include <QSettings>

#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>

#include "settingsprogram.h"
#include "ui_settingsprogram.h"

SettingsProgram::SettingsProgram(QDialog *parent) :
    QDialog(parent),
    ui_program(new Ui::SettingsProgram)
{
    ui_program->setupUi(this);
 //   setUiProgramSettings();
    setWindowTitle("Настройка программы");

    connect( ui_program->checkBoxAutoLoad, SIGNAL( clicked()) , this, SLOT( slotAutoRun()));
    connect( ui_program->pushButtonOk, SIGNAL( clicked() ), this, SLOT( accept()));
    connect( ui_program->pushButtonCancel, SIGNAL( clicked() ), this, SLOT( reject()));


}

SettingsProgram::~SettingsProgram()
{
    delete ui_program;
}

void SettingsProgram::slotAutoRun()
{
//#ifdef OS_WIN32
    //Добавляем в автозагрузку пользователя
    QSettings *autorun = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(ui_program->checkBoxAutoLoad->isChecked())
    {
        autorun->setValue("Sapfir", QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
        autorun->sync();
        ui_program->checkBoxAutoLoad->setToolTip("Отключить автозагрузку");
    }
    else
    {
        autorun->remove("Sapfir");
        ui_program->checkBoxAutoLoad->setToolTip("Включить автозагрузку");
    }
    delete autorun;
//#endif
}
