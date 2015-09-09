/** @file Файл с реализацией функций формы с информацией о программе. */

#include "frmabout.h"
#include "ui_frmabout.h"
#include "version.h"
#include "base_settings.h"

/** @brief Конструктор формы с информацией о программе.
@param parent Виджет-родитель формы. */
frmAbout::frmAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmAbout)
{
	BaseSettings::BASE_DEVICE_SETTINGS_S device = BaseSettings::getInstance().getDeviceSettings();
    ui->setupUi(this);
    // Отключаем изменение размера формы:
    this->setFixedSize(this->size());
    // Отключаем все кнопки, кроме системного меню и кнопки закрытия:
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint |
        Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint | Qt::Dialog);
    // Устанавливаем название программы:
    ui->lblProgNam->setText(qApp->applicationName());
    // Формируем строку с версией программы:
    QString sVer = QString::number(VER_MAJOR);
    sVer.append('.');
    sVer.append(QString::number(VER_MINOR));
    sVer.append('.');
    sVer.append(QString::number(VER_BUILD));
    // Выводим версию программы:
    ui->lblVersion->setText(sVer);
	// Выводим автора:
	ui->lblAuthor->setText(qApp->organizationName());
	// Выводим сайт:
	ui->lblProgDevice->setText(device.VERSION);

}

/** @brief Деструктор формы с информацией о программе. */
frmAbout::~frmAbout()
{
    delete ui;
}


/** @brief Функция для установки новой ссылки на страничку о поддержке автора программы.
@param sUrl Новый адрес странички поддержки автора программы. */
void frmAbout::setSupportUrl(QString sUrl)
{
    // Устанавливаем новую ссылку на страничку поддержки:
 //   ui->lblSupport->setText(sUrl);
}

// Слоты

/** @brief Слот-обработчик нажатия кнопки закрытия формы. */
void frmAbout::on_btnClose_clicked()
{
    // Закрываем диалог:
    this->reject();
}
