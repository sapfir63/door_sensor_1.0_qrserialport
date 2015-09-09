#include <QSettings>

#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>

#include "mainwindow.h"
#include "settingsprogram.h"
#include "ui_settingsprogram.h"

SettingsProgram::SettingsProgram(QDialog *parent) :
    QDialog(parent),
	ui_program(new Ui::SettingsProgram)//,
	//MainWindow();
{
    ui_program->setupUi(this);
 //   setUiProgramSettings();
    setWindowTitle("Настройка программы");

	connect(ui_program->checkBoxAutoLoad, SIGNAL( clicked()) , this, SLOT( slotAutoRun()));
	connect(ui_program->pushButtonOk,     SIGNAL( clicked() ), this, SLOT( accept()));
	connect(ui_program->pushButtonCancel, SIGNAL( clicked() ), this, SLOT( reject()));

	connect(ui_program->btnFileOpen,  SIGNAL(clicked()), fileDialog,SLOT(exec()));
	connect(ui_program->btnStartProg, SIGNAL(clicked()), SLOT(slotStartProg()));

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


void SettingsProgram::slotStartProg()
{
	if(ui_program->filePath->text().length()!=0)
	{
		file=new QFile(ui_program->filePath->text());

		if(file->open(QFile::ReadOnly))
		{
			QByteArray byteArray=file->readAll();

			bufLenght=byteArray.length();
			buff=new unsigned char[bufLenght];
			//копируем образ
			for(int i=0; i<byteArray.length();i++)
				buff[i]=byteArray.data()[i];
			file->close();
			ui_program->progressBar->setValue(0);
	//		MainWindow::sendCommand(MainWindow::START_DOWNLOADER);
		}
	}
}
