#ifndef SETTINGSEMAIL_H
#define SETTINGSEMAIL_H
#include <QDialog>

namespace Ui
{
    class SettingsEmail;
}


class SettingsEmail : public QDialog {
Q_OBJECT

public:
    SettingsEmail(QDialog * parent = 0);
    ~SettingsEmail();

    typedef struct
    {
        bool enable;
        QString error;
        QString host;
        int port;
        bool ssl;
        bool auth;
        QString username;
        QString password;
        QString sender;
        QString recipients;
        QString subject;
        QString texteditor;
        QList<QString> attachments;
    }EMAIL_SETTINGS_T;


private slots:
    void slotCheckEmail();
    void slotAddAttachment();
    void slotClearAttachments();

private:
    Ui::SettingsEmail   *ui_email;
    void defaultEmail();
    EMAIL_SETTINGS_T   email;

};

#endif // SETTINGSEMAIL_H
