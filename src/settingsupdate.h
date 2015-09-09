#ifndef SETTINGSUPDATE_H
#define SETTINGSUPDATE_H
#include <QDialog>

#include "sendemail.h"
#include "settings.h"

namespace Ui
{
	class SettingsProgram;
	class SettingsEmail;
}

class SettingsUpdate : public QDialog
{
	Q_OBJECT

public:
    explicit SettingsUpdate(int act1);
	~SettingsUpdate();

    /* функции записи значений из формы */
	void setProgramSettings();
	void setEmailSettings();

private slots:
	void slotAutoRun();
	void slotCheckEmail();
	void slotAddAttachment();
	void slotClearAttachments();

private:
	Ui::SettingsProgram *ui_program;
	Ui::SettingsEmail   *ui_email;

    /* функции заполнения формы */
	void setUiProgramSettings();
	void setUiEmailSettings();
};


#endif // SETTINGSUPDATE_H
