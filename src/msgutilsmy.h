/** @file Заголовочный файл для утилит, облегчающих отображение диалогов для пользователя и задание вопросов пользователю. */
#ifndef MSGUTILSMY_H
#define MSGUTILSMY_H

#include <QObject>

class MsgUtilsMy : public QObject
{
    Q_OBJECT
public:
    static void showErrorMsg(QWidget *parent = 0,
        QString sTextIn  = QString(), QString sInfoTextIn = QString(),
        QString sDetailedTextIn = QString());
    static bool showQuestion(QWidget *parent = 0,
        bool isDefaultCancel = false, QString sTextIn  = QString(),
        QString sInfoTextIn = QString(),
        QString sDetailedTextIn = QString());
    static void showInfoMsg(QWidget *parent = 0,
        QString sTextIn  = QString(), QString sInfoTextIn = QString(),
        QString sDetailedTextIn = QString());
    static void showWarningMsg(QWidget *parent = 0,
        QString sTextIn  = QString(), QString sInfoTextIn = QString(),
        QString sDetailedTextIn = QString());
};

#endif // MSGUTILSMY_H
