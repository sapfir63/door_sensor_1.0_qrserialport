#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>

#include "sendemail.h"

#include "settingsemail.h"
#include "ui_settingsemail.h"

SettingsEmail::SettingsEmail(QDialog *parent) :
    QDialog(parent),
    ui_email(new Ui::SettingsEmail)
{
    ui_email->setupUi(this);
    setWindowTitle("Настройка почты");
    defaultEmail();
    ui_email->pushButtonOK->setDisabled(true);
    connect( ui_email->addAttachment, SIGNAL( clicked() ), this, SLOT( slotAddAttachment()));
    connect( ui_email->clearAttachments, SIGNAL( clicked() ), this, SLOT( slotClearAttachments()));
    connect( ui_email->checkEmail, SIGNAL( clicked() ), this, SLOT( slotCheckEmail()));
    connect( ui_email->pushButtonOK, SIGNAL( clicked() ), this, SLOT( accept()));
    connect( ui_email->pushButtonCancel, SIGNAL( clicked() ), this, SLOT( reject()));

}

SettingsEmail::~SettingsEmail()
{
    delete ui_email;
}

void SettingsEmail::defaultEmail()
{
 //   ui_email->enable     = false;
//    this->email.error      = "The email was succesfully sended.";
    ui_email->host->setText("smtp.mastermail.ru");
    ui_email->port->setValue(465);
    ui_email->ssl->setChecked(true);
    ui_email->auth->setChecked(true);

    ui_email->username->setText( "sosnin@sapfir.biz");
    ui_email->password->setText( "cjcyby1963");

    ui_email->sender->setText("sosnin@sapfir.biz");
    ui_email->recipients->setText("sosnin1963@gmail.com;sosnin@sapfir.biz");

    ui_email->subject->setText("тест");
    ui_email->texteditor->setText("тест");

}


void SettingsEmail::slotAddAttachment()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec())
        ui_email->attachments->addItems(dialog.selectedFiles());
}

void SettingsEmail::slotClearAttachments()
{
    ui_email->attachments->clear ();
}


void SettingsEmail::slotCheckEmail()
{
    EMAIL_SETTINGS_T email;

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

