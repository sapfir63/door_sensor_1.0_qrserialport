#ifndef SETTINGSPROGRAM_H
#define SETTINGSPROGRAM_H

#include <QDialog>
#include <QFileDialog>
#include <stdint.h>

namespace Ui
{
    class SettingsProgram;
}

class SettingsProgram : public QDialog {
Q_OBJECT

public:
    SettingsProgram(QDialog * parent = 0);
    ~SettingsProgram();

    typedef struct
    {
        bool    auto_load;
        bool    alarm_email_enable;		// флаг отправки email
        quint8  alarm_email_timeout;	// время отправки email
        bool    journal_email_enable;	// флаг ведения журнала
        quint8  journal_email_timeout;	// время отправки журнала
        bool    door_state;
        quint16 dust_threshold;			// порог пыли
        qint8   ow_threshold;			// порог температуры
        bool    tray_enable;			// флаг вывода в трей
        quint8  tray_timeout;			// пауза между выводом сообщения в трее (мин)
    }PROGRAM_SETTINGS_T;



private slots:
    void slotAutoRun();
	void slotStartProg();
private:
    Ui::SettingsProgram *ui_program;
  //  void defaultEmail();
 //   EMAIL_SETTINGS_T   email;
	QFile* file;
	QFileDialog *fileDialog;
	unsigned char* buff;
	unsigned int bufLenght;
	unsigned int dataCounter;

};

#endif // SETTINGSPROGRAM_H
