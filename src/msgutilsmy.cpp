/** @file Файл с реализацией функций для утилит, облегчающих работу с базами данных. */

#include <QMessageBox>
#include "msgutilsmy.h"

/** @brief Функция для отображения пользователю сообщения об ошибке.

@param parent Виджет-родитель диалога сообщения. Нужен, чтобы диалог показывался по центру указанного виджета, а также для системы Qt.
@param sTextIn Текст сообщения.
@param sInfoTextIn Дополнительный, поясняющий текст. Отображается чуть ниже основного. В некоторых системах может отображаться меньшим шрифтом.
@param sDetailedTextIn Детализированный текст. После открытия диалога не показывается. Отображается, когда пользователь нажмёт на кнопку "Show details...". Может быть выделен и скопирован. */
void MsgUtilsMy::showErrorMsg(QWidget *parent, QString sTextIn,
    QString sInfoTextIn, QString sDetailedTextIn)
{
    // Создаём диалог сообщения пользователю:
    QMessageBox msgBox(parent);
    // Устанавливаем различные тексты, если они указаны:
    if (!sTextIn.isNull())
        msgBox.setText(sTextIn);
    if (!sInfoTextIn.isNull())
        msgBox.setInformativeText(sInfoTextIn);
    if (!sDetailedTextIn.isNull())
        msgBox.setDetailedText(sDetailedTextIn);
    // Устанавливаем кнопки:
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    msgBox.setEscapeButton(QMessageBox::Close);
    msgBox.setIcon(QMessageBox::Critical);
    // Показываем сообщение:
    msgBox.exec();
}

/** @brief Функция для отображения сообщения с вопросом пользователю.

Вопрос подразумевает два варианта ответа: "Ок" и "Отмена". В случае, если пользователь выберет "Ок", функция возвращает true, если пользователь выберет "Cancel" - функция вернёт false.

@param parent Виджет-родитель диалога сообщения. Нужен, чтобы диалог показывался по центру указанного виджета, а также для системы Qt.
@param isDefaultCancel Определяет, какой вариант ответа будет выбран по умолчанию. Если этот параметр равен true - по умолчанию будет выбрана кнопка отмены, если false - по умолчанию будет выбрана кнопка подтверждения. Значение параметра по умолчанию равно false (т.е. по умолчанию выбрана кнопка подтверждения).
@param sTextIn Текст вопроса пользователю.
@param sInfoTextIn Дополнительный, поясняющий текст. Отображается чуть ниже основного. В некоторых системах может отображаться меньшим шрифтом.
@param sDetailedTextIn Детализированный текст. После открытия диалога не показывается. Отображается, когда пользователь нажмёт на кнопку "Show details...". Может быть выделен и скопирован.

@returns В случае, если пользователь выберет "Ок", функция возвращает true, если пользователь выберет "Cancel" - функция вернёт false. */
bool MsgUtilsMy::showQuestion(QWidget *parent,
    bool isDefaultCancel, QString sTextIn,
    QString sInfoTextIn, QString sDetailedTextIn)
{
    // Создаём диалог сообщения пользователю:
    QMessageBox msgBox(parent);
    // Устанавливаем различные тексты, если они указаны:
    if (!sTextIn.isNull())
        msgBox.setText(sTextIn);
    if (!sInfoTextIn.isNull())
        msgBox.setInformativeText(sInfoTextIn);
    if (!sDetailedTextIn.isNull())
        msgBox.setDetailedText(sDetailedTextIn);
    // Устанавливаем кнопки:
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if (isDefaultCancel)
        msgBox.setDefaultButton(QMessageBox::Cancel);
    else
        msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setEscapeButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Question);
    // Показываем диалог и возвращаем true, если пользователь нажал кнопку
    // Ok, и false, если нажал Cancel::
    return (msgBox.exec() == QMessageBox::Ok);
}

/** @brief Функция для отображения пользователю информационного сообщения.

@param parent Виджет-родитель диалога сообщения. Нужен, чтобы диалог показывался по центру указанного виджета, а также для системы Qt.
@param sTextIn Текст сообщения.
@param sInfoTextIn Дополнительный, поясняющий текст. Отображается чуть ниже основного. В некоторых системах может отображаться меньшим шрифтом.
@param sDetailedTextIn Детализированный текст. После открытия диалога не показывается. Отображается, когда пользователь нажмёт на кнопку "Show details...". Может быть выделен и скопирован. */
void MsgUtilsMy::showInfoMsg(QWidget *parent, QString sTextIn,
    QString sInfoTextIn, QString sDetailedTextIn)
{
    // Создаём диалог сообщения пользователю:
    QMessageBox msgBox(parent);
    // Устанавливаем различные тексты, если они указаны:
    if (!sTextIn.isNull())
        msgBox.setText(sTextIn);
    if (!sInfoTextIn.isNull())
        msgBox.setInformativeText(sInfoTextIn);
    if (!sDetailedTextIn.isNull())
        msgBox.setDetailedText(sDetailedTextIn);
    // Устанавливаем кнопки:
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    msgBox.setEscapeButton(QMessageBox::Close);
    msgBox.setIcon(QMessageBox::Information);
    // Показываем сообщение:
    msgBox.exec();
}

/** @brief Функция для отображения пользователю предупреждающего сообщения.

@param parent Виджет-родитель диалога сообщения. Нужен, чтобы диалог показывался по центру указанного виджета, а также для системы Qt.
@param sTextIn Текст сообщения.
@param sInfoTextIn Дополнительный, поясняющий текст. Отображается чуть ниже основного. В некоторых системах может отображаться меньшим шрифтом.
@param sDetailedTextIn Детализированный текст. После открытия диалога не показывается. Отображается, когда пользователь нажмёт на кнопку "Show details...". Может быть выделен и скопирован. */
void MsgUtilsMy::showWarningMsg(QWidget *parent, QString sTextIn,
    QString sInfoTextIn, QString sDetailedTextIn)
{
    // Создаём диалог сообщения пользователю:
    QMessageBox msgBox(parent);
    // Устанавливаем различные тексты, если они указаны:
    if (!sTextIn.isNull())
        msgBox.setText(sTextIn);
    if (!sInfoTextIn.isNull())
        msgBox.setInformativeText(sInfoTextIn);
    if (!sDetailedTextIn.isNull())
        msgBox.setDetailedText(sDetailedTextIn);
    // Устанавливаем кнопки:
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    msgBox.setEscapeButton(QMessageBox::Close);
    msgBox.setIcon(QMessageBox::Warning);
    // Показываем сообщение:
    msgBox.exec();
}
