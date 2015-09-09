#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>

#include "settings.h"
#include "settingsupdate.h"
#include "ui_settingsprogram.h"
#include "ui_settingsemail.h"

SettingsUpdate::SettingsUpdate( int act1) :
	ui_program(new Ui::SettingsProgram),
    ui_email(new Ui::SettingsEmail)
{

	if(act1 == 0)
	{
		ui_program->setupUi(this);
		setUiProgramSettings();
		setWindowTitle("Настройка программы");

        connect( ui_program->checkBoxAutoLoad, SIGNAL( clicked()) , this, SLOT( slotAutoRun()));
        connect( ui_program->pushButtonOk, SIGNAL( clicked() ), this, SLOT( accept()));
        connect( ui_program->pushButtonCancel, SIGNAL( clicked() ), this, SLOT( reject()));
    }
	else if (act1 == 1)
	{
		ui_email->setupUi(this);
		setUiEmailSettings();
		setWindowTitle("Настройка почты");

		ui_email->pushButtonOK->setDisabled (true);
		connect( ui_email->addAttachment, SIGNAL( clicked() ), this, SLOT( slotAddAttachment()));
		connect( ui_email->clearAttachments, SIGNAL( clicked() ), this, SLOT( slotClearAttachments()));
		connect( ui_email->checkEmail, SIGNAL( clicked() ), this, SLOT( slotCheckEmail()));
		connect( ui_email->pushButtonOK, SIGNAL( clicked() ), this, SLOT( accept()));
		connect( ui_email->pushButtonCancel, SIGNAL( clicked() ), this, SLOT( reject()));
	}
}

SettingsUpdate::~SettingsUpdate()
{
	delete ui_email;
	delete ui_program;
}

/*
 * чтение и вывод в форму настроек Program
 */
void SettingsUpdate::setUiProgramSettings()
{
	Settings::PROGRAM_SETTINGS_T prog = Settings::getInstance ().getProgramSettings();
	ui_program->checkBoxAutoLoad->setChecked(prog.auto_load);

	ui_program->checkBoxAlarmEmailEnable->setChecked(prog.alarm_email_enable);
	ui_program->spinBoxAlarmEmailTimeout->setValue(prog.alarm_email_timeout);

	ui_program->checkBoxJournalEmailEnable->setChecked(prog.journal_email_enable);
	ui_program->spinBoxJournalEmailTimeout->setValue(prog.journal_email_timeout);

	ui_program->checkBoxTrayEnable->setChecked(prog.tray_enable);
	ui_program->spinBoxTrayTimeout->setValue(prog.tray_timeout);
}

/*
 * запись настроек Program в файл
 */

void SettingsUpdate::setProgramSettings ()
{

	Settings::PROGRAM_SETTINGS_T prog ;//= Settings::getInstance ().getProgramSettings();
	prog.auto_load = ui_program->checkBoxAutoLoad->isChecked();
	prog.alarm_email_enable  = ui_program->checkBoxAlarmEmailEnable->isChecked();
	prog.alarm_email_timeout = ui_program->spinBoxAlarmEmailTimeout->value();

	prog.journal_email_enable  = ui_program->checkBoxJournalEmailEnable->isChecked();
	prog.journal_email_timeout = ui_program->spinBoxJournalEmailTimeout->value();

	prog.tray_enable  = ui_program->checkBoxTrayEnable->isChecked();
	prog.tray_timeout = ui_program->spinBoxTrayTimeout->value();

	Settings::getInstance ().updateProgramSettings(prog);
}

/*
 * установка автозагрузки при запуске системы
 */

void SettingsUpdate::slotAutoRun()
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

/*
 * чтение и вывод в форму настроек Email
 */

void SettingsUpdate::setUiEmailSettings()
{
	Settings::EMAIL_SETTINGS_T email = Settings::getInstance ().getEmailSettings();

	ui_email->host->setText (email.host);
	ui_email->port->setValue (email.port);
	ui_email->ssl->setChecked (email.ssl);
	ui_email->auth->setChecked (email.auth);
	ui_email->username->setText (email.username);
	ui_email->password->setText (email.password);
	ui_email->sender->setText (email.sender);
	ui_email->recipients->setText (email.recipients);
	ui_email->subject->setText (email.subject);
	ui_email->texteditor->setText (email.texteditor);
	for (int i = 0; i < email.attachments.count(); ++i)
	{
		ui_email->attachments->addItem (email.attachments.value (i));
	}
}
/*
 * добавление файла(ов) в оправляемое сообщение
 */

void SettingsUpdate::slotAddAttachment()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFiles);

	if (dialog.exec())
		ui_email->attachments->addItems(dialog.selectedFiles());
}

void SettingsUpdate::slotClearAttachments()
{
	ui_email->attachments->clear ();
}

/*
 * проверка отсылки Email
 */

void SettingsUpdate::slotCheckEmail()
{
	Settings::EMAIL_SETTINGS_T email;

	email.host = ui_email->host->text();
	email.port = ui_email->port->value();
	email.ssl = ui_email->ssl->isChecked();
	email.auth = ui_email->auth->isChecked();
	email.username = ui_email->username->text();
	email.password = ui_email->password->text();
	email.sender = ui_email->sender->text();
	email.recipients = ui_email->recipients->text();
	email.subject = ui_email->subject->text();
	email.texteditor = ui_email->texteditor->toHtml();

	QString attachment;
	for (int i = 0; i < ui_email->attachments->count(); ++i)
	{
		email.attachments.append(attachment);
		email.attachments[i] =ui_email->attachments->item(i)->text();
	}

	SendEmail *sendemail = new SendEmail(&email);
    sendemail->sendEmail(&email);

    if(email.enable)
        ui_email->pushButtonOK->setEnabled (true);
	QMessageBox errMessage ;
	errMessage.setText(email.error);
	errMessage.exec();
}
/*
 * запись настроек Email в файл
 */
void SettingsUpdate::setEmailSettings ()
{
	Settings::EMAIL_SETTINGS_T email;
	email.host       = ui_email->host->text();
	email.port       = ui_email->port->value();
	email.ssl        = ui_email->ssl->isChecked();
	email.auth       = ui_email->auth->isChecked();
	email.username   = ui_email->username->text();
	email.password   = ui_email->password->text();
	email.sender     = ui_email->sender->text();
	email.recipients = ui_email->recipients->text();
	email.subject    = ui_email->subject->text();
	qDebug() << ui_email->subject->text();;
	email.texteditor = ui_email->texteditor->toHtml();

	QString attachment;
	for (int i = 0; i < ui_email->attachments->count(); ++i)
	{
		email.attachments.append(attachment);
		email.attachments[i] = ui_email->attachments->item(i)->text();
	}
	Settings::getInstance ().updateEmailSettings(email);

}

