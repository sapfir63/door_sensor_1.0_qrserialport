
#include <QUrl>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QDir>
#include <QSettings>
#include <QNetworkProxy>
#include <QStringBuilder>
#include <QDesktopServices>

#include "frmmain.h"
#include "frmabout.h"
#include "ui_frmmain.h"
#include "msgutilsmy.h"
#include "version.h"
#include "LWVersion.h"


/** @brief Строка с названием индексного файла справки. */
#define HELP_INDEX "index.html"
/** @brief Часть URL - протокол доступа к локальным файлам. */
#define FILE_PROT "file:///"

/** @brief Суффикс файла с настройками приложения */
#define INI_FILESUF ".ini"
/** @brief Директория для хранения конфигурационных файлов приложений */
#define CONFIG_FOLDER ".config"

// Константы путей настроек приложений в INI-файле

/** @brief Название в файле настроек настройки таймаута запроса. */
#define SETS_TIMEOUT "Timeout"
/** @brief Название в файле настроек флага использования прокси. */
#define SETS_USE_PROXY "UseProxy"
/** @brief Название в файле настроек программы настройки типа прокси. */
#define SETS_PROXY_TYPE "ProxyType"
/** @brief Название в файле настроек программы настройки имени хоста прокси. */
#define SETS_PROXY_HOST "ProxyHost"
/** @brief Название в файле настроек программы настройки номера порта прокси. */
#define SETS_PROXY_PORT "ProxyPort"
/** @brief Название в файле настроек программы аутентификации прокси. */
#define SETS_PROXY_AUTH "ProxyAuth"
/** @brief Название в файле настроек программы настройки имени пользователя прокси. */
#define SETS_PROXY_USER "ProxyUser"
/** @brief Название в файле настроек программы настройки пароля прокси. */
#define SETS_PROXY_PASS "ProxyPass"

// Константы значений по умолчанию настроек приложений

/** @brief Значение по умолчанию для времени таймаута запроса (в секундах). */
#define DEF_TIMEOUT 60
/** @brief Значение по умолчанию для флага использования прокси. */
#define DEF_USE_PROXY false
/** @brief Значение по умолчанию для настройки типа прокси. */
#define DEF_PROXY_TYPE 0
/** @brief Значение по умолчанию для настройки имени хоста прокси. */
#define DEF_PROXY_HOST "localhost"
/** @brief Значение по умолчанию для настройки номера порта прокси. */
#define DEF_PROXY_PORT 8080
/** @brief Значение по умолчанию для флага аутентификации прокси. */
#define DEF_PROXY_AUTH false
/** @brief Значение по умолчанию для настройки имени пользователя прокси. */
#define DEF_PROXY_USER ""
/** @brief Значение по умолчанию для настройки пароля прокси. */
#define DEF_PROXY_PASS ""



/** @brief Строка-маркер начала номера версии. */
#define VERSION_START "versionstart_"
/** @brief Строка-маркер окончания номера версии. */
#define VERSION_END "_versionend"

frmMain::frmMain(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    // Сбрасываем флаг выхода после выполнения запроса:
    bExitOnReply = false;
    // Назначаем заголовок для таба с настройками подключения:
    ui->tabWidget->setTabText(0, tr("Connection settings"));
    // Добавляем типы прокси:
    ui->cbProxyType->addItem(tr("HTTP proxy"));
    ui->cbProxyType->addItem(tr("SOCKS 5 proxy"));
    // Создаём объект QNetworkAccessManager и назначаем для него обработчик
    // завершения запроса:
    nman = new QNetworkAccessManager();
    connect(nman, SIGNAL(finished(QNetworkReply*)), this, SLOT(readReply(QNetworkReply*)));
    // Подключаем обработчик тика таймера таймаута:
    connect(&timer, SIGNAL(timeout()), this, SLOT(requestTimeout()));
    // Формируем путь к директории данных программы.
    // Получаем путь к директории пользователя:
    QString sAppDataFolder = QDir::homePath();
    // Добавляем универсальный разделитель путей:
    sAppDataFolder.append(QDir::separator());
    // Добавляем директорию с файлами конфигурации приложений:
    sAppDataFolder.append(CONFIG_FOLDER);
    // Снова добавляем универсальный разделитель путей:
    sAppDataFolder.append(QDir::separator());
    // Добавляем название программы (директория данных программы будет называться
    // так же, как и сама программа):
    sAppDataFolder.append(APP_NAME_SIGN);
    // Формируем путь к файлу с настройками приложения:
    QString sSetsFile = sAppDataFolder;
    sSetsFile.append(QDir::separator());
    sSetsFile.append(APP_NAME_SIGN);
    sSetsFile.append(INI_FILESUF);
    // Создаём объект для работы с настройками:
    QSettings sets(sSetsFile, QSettings::IniFormat, this);
    sets.setIniCodec(UTF8_ABBR);

    // Считываем настройки приложения.
    // Считываем таймаут:
    iTimeout = sets.value(SETS_TIMEOUT, DEF_TIMEOUT).toInt();
    if (iTimeout < 5 || iTimeout > 360)
        iTimeout = DEF_TIMEOUT;
    // Считываем флаг использования прокси:
    bUseProxy = sets.value(SETS_USE_PROXY, DEF_USE_PROXY).toBool();
    // Считываем тип прокси:
    iProxyType = sets.value(SETS_PROXY_TYPE, DEF_PROXY_TYPE).toInt();
    // Может использоваться только два типа прокси - SOCKS 5 и HTTP
    // (т.е., соответственно, 0 и 1). Если что-то другое - ставим тип 0 (HTTP):
    if (iProxyType < DEF_PROXY_TYPE || iProxyType > 1)
        iProxyType = DEF_PROXY_TYPE;
    // Считываем хост прокси:
    sProxyHost = sets.value(SETS_PROXY_HOST, DEF_PROXY_HOST).toString();
    // Считываем порт прокси:
    iProxyPort = sets.value(SETS_PROXY_PORT, DEF_PROXY_PORT).toInt();
    if (iProxyPort < 1 || iProxyType > 65535)
        iProxyPort = DEF_PROXY_PORT;
    // Считываем флаг аутентификации прокси:
    bProxyAuth = sets.value(SETS_PROXY_AUTH, DEF_PROXY_AUTH).toBool();
    // Считываем имя пользователя и пароль аутентификации прокси:
    sProxyUser = sets.value(SETS_PROXY_USER, DEF_PROXY_USER).toString();
    sProxyPass = sets.value(SETS_PROXY_PASS, DEF_PROXY_PASS).toString();
    // Настройки прочитаны. Выводим их в контролы:
    ui->sbTimeout->setValue(iTimeout);
    ui->cbUseProxy->setChecked(bUseProxy);
    ui->cbProxyType->setCurrentIndex(iProxyType);
    ui->leProxyHost->setText(sProxyHost);
    ui->sbProxyPort->setValue(iProxyPort);
    ui->cbAuth->setChecked(bProxyAuth);
    ui->leProxyUser->setText(sProxyUser);
    ui->leProxyPass->setText(sProxyPass);

	ui->curver->setText(qApp->applicationVersion());
	connect( ui->btnCheck, SIGNAL( clicked() ), this, SLOT( slotCheckUpdate() ) );
    // В случае, если включено использование прокси - устанавливаем прокси для
    // приложения:
    if (bUseProxy && !sProxyHost.isEmpty())
    {
        QNetworkProxy proxy;
        if (iProxyType > 0)
            proxy.setType(QNetworkProxy::Socks5Proxy);
        else
            proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(sProxyHost);
        proxy.setPort(iProxyPort);
        if (bProxyAuth && !sProxyUser.isEmpty())
        {
            proxy.setUser(sProxyUser);
            proxy.setPassword(sProxyPass);
        }
        QNetworkProxy::setApplicationProxy(proxy);
    }
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::slotCheckUpdate()
{
	QString sCurVerIn = ui->curver->text();
	QString sProgIn   = ui->prog->text();
	QString sPageForCheckIn   = ui->site->text();
	QString sPageGoToIn   = ui->goto_2->text();
	bool bExitOnReplyIn = 1;
	int iNupMsgIn = ui->nupmsg->text().toInt();
	checkSiteVer(sCurVerIn,sProgIn,sPageForCheckIn,sPageGoToIn,bExitOnReplyIn,iNupMsgIn);
}


/** @brief Функция для запуска проверки наличия обновлений для указанной программы.
@param sCurVerIn Строка с номером установленной версиеи программы.
@param sProgIn Строка с названием программы.
@param sPageForCheckIn URL сайта, к которому надо обращаться для получения номера последней версии программы.
@param sPageGoToIn URL сайта, на который надо перейти для скачивания новой версии програмы.
@param bExitOnReplyIn Флаг выхода из программы после проверки.
@param iNupMsgIn Режим отображения сообщений об отсутствии доступных обновлений. 0 - по умолчанию (не отображать сообщения), 1 - не отображать сообщения, 2 - отображать сообщения. */
void frmMain::checkSiteVer(const QString & sCurVerIn,
    const QString & sProgIn, const QString & sPageForCheckIn,
    const QString &sPageGoToIn, bool bExitOnReplyIn, int iNupMsgIn)
{
    // Устанавливаем флаг выхода после выполнения запроса:
    bExitOnReply = bExitOnReplyIn;
    // Сохраняем переменные:
    sCurVer = sCurVerIn;
    sPageForCheck = sPageForCheckIn;
    sProg = sProgIn;
    sPageGoTo = sPageGoToIn;
    iNupMsg = iNupMsgIn;
    // Отправляем запрос и сохраняем ссылку на него:
    reply = nman->get(QNetworkRequest((QUrl(sPageForCheckIn))));
}


/** @brief Функция получения директории для локализованных файлов справки.
@returns Возвращает строку с путём к директории, содержащей локализованные файлы справки. */
QString frmMain::getHelpDir() const
{
    // Получаем директорию с файлами справки:
    QString sHelpDir = QApplication::applicationDirPath();
    sHelpDir.append(QDir::separator());
    sHelpDir.append("docs_lwupchecker");
    sHelpDir.append(QDir::separator());
    sHelpDir.append(tr("default_lang"));
    sHelpDir.append(QDir::separator());
    return sHelpDir;
}


// Слоты

void frmMain::readReply(QNetworkReply * replyIn)
{
    // Проверяем, не произошло ли ошибки при выполнении запроса:
    if (replyIn->error() != QNetworkReply::NoError)
    {
        // Ошибка произошла - сообщаем о ней. Если поднят флаг выхода -
        // предлагаем пользователю настроить параметры соединения. Если флаг
        // выхода не поднять - значит, пользовательский интерфейс уже открыт,
        // и смысла предлагать пользователю его открыть нет.
        QString sMsg(tr("Failed to get version info from site."));
        QString sErr(replyIn->errorString());
        if (bExitOnReply)
        {
            sMsg.append(tr(" Open connection settings?"));
            if (MsgUtilsMy::showQuestion(this, false, sMsg,
                QString(), sErr))
                bExitOnReply = false;
        }
        else
            MsgUtilsMy::showErrorMsg(this, sMsg, QString(), sErr);
        // Если установлен флаг выхода - выходим из программы, иначе показываем
        // основную форму:
        if (bExitOnReply)
            qApp->exit(0);
        else
            this->show();
        // Завершаем функцию:
        return;
    }
    // Получаем текст полученного ответа на запрос:
    QString sVer(replyIn->readAll());
    // Маркер начала версии программы:
    QString sVerStartMark(VERSION_START);
    // Маркер окончания версии программы:
    QString sVerEndMark(VERSION_END);
    // Проверка наличия версии программы в нужном формате.
    // Ищем маркер начала строки версии:
    int iVStart = sVer.indexOf(sVerStartMark, 0, Qt::CaseInsensitive);
    // Позиция маркера окончания строки версии:
    int iVEnd = -1;
    // В случае, если маркер начала версии найден - ищем маркер окончания версии:
    if (iVStart > -1)
        iVEnd = sVer.indexOf(sVerEndMark, iVStart, Qt::CaseInsensitive);
    // В случае, если маркер начала или маркер окончания строки версии не найден -
    // сообщаем об ошибке и завершаем функцию:
    if (iVStart == -1 || iVEnd == -1)
    {
        // Ошибка произошла - сообщаем о ней:
        MsgUtilsMy::showErrorMsg(this, tr("Error while readin version from site."),
            QString(), tr("Version start or version end marker is not found."));
        // Если установлен флаг выхода - выходим из программы, иначе показываем
        // основную форму:
        if (bExitOnReply)
            qApp->exit(0);
        else
            this->show();
        // Завершаем функцию:
        return;
    }
    // Убираем окончание строки, начиная с первого символа маркера окончания строки версии:
    sVer.remove(iVEnd, sVer.length() - iVEnd);
    // Убираем начало строки до версии, включая маркер начала версии:
    sVer.remove(0, iVStart + sVerStartMark.length());
    // Создаём объекты версий:
    LWVersion verCurrent(sCurVer);
    LWVersion verSite(sVer);
    // Если версия на сайте больше - сообщаем об этом и предлагаем перейти на сайт:
    if (verSite.isHigherThan(verCurrent))
    {
        if (MsgUtilsMy::showQuestion(this, false,
            tr("New %1 version is available. Open download page?").arg(sProg)))
            QDesktopServices::openUrl(QUrl(sPageGoTo));
    }
    // Установлена последняя версия. В случае, если поднят флаг выхода
    // после проверки - сообщать о наличии последней версии пользователю
    // не нужно. В случае, если флаг выхода не поднят - значит, проверка
    // запущена пользователем, и надо сообщить ему, что у него самая
    // последняя версия:
    else
    {
        // Сообщаем об отстутствии обновлений в случае, если вывод
        // таких сообщений явно включен, и в случае, если вывод
        // не выключен и не поднят флаг выхода после проверки:
        if ((iNupMsg == 2) || ((iNupMsg != 1) && !bExitOnReply))
            // Сообщаем, что установлена последняя версия:
            MsgUtilsMy::showInfoMsg(this,
                tr("Latest %1 version is installed.").arg(sProg));
    }
    // Если поднят флаг завершения работы приложения после получения ответа -
    // завершаем работу приложения, иначе показываем главное окно:
    if (bExitOnReply)
        qApp->exit(0);
    else
        this->show();
}

void frmMain::requestTimeout()
{
    // Останавливаем запрос:
    reply->abort();
    // Останавливаем таймер:
    timer.stop();
    // Ошибка - превышен интервал запроса. Сообщаем пользвателю. Если поднят флаг
    // выхода - предлагаем пользователю настроить параметры соединения. Если флаг
    // выхода не поднять - значит, пользовательский интерфейс уже открыт,
    // и смысла предлагать пользователю его открыть нет.
    QString sMsg(tr("Failed to get version info from site."));
    QString sErr(tr("Request timeout."));
    if (bExitOnReply)
    {
        sMsg.append(tr(" Open connection settings?"));
        if (MsgUtilsMy::showQuestion(this, false, sMsg,
            QString(), sErr))
            bExitOnReply = false;
    }
    else
        MsgUtilsMy::showErrorMsg(this, sMsg, QString(), sErr);
    // Если установлен флаг выхода - выходим из программы, иначе показываем
    // основную форму:
    if (bExitOnReply)
        qApp->exit(0);
    else
        this->show();
    // Завершаем функцию:
    return;
}

void frmMain::on_cbUseProxy_toggled(bool checked)
{
    // Блокируем/разблокируем контролы в зависимости от включения/отключения
    // галочки использования прокси:
    ui->lblProxyType->setEnabled(checked);
    ui->cbProxyType->setEnabled(checked);
    ui->lblProxyHost->setEnabled(checked);
    ui->leProxyHost->setEnabled(checked);
    ui->lblProxyPort->setEnabled(checked);
    ui->sbProxyPort->setEnabled(checked);
    ui->cbAuth->setEnabled(checked);
    on_cbAuth_toggled(ui->cbAuth->isChecked());
    // Блокируем кнопку применения, если включено использование прокси,
    // но хост прокси - пустой:
    ui->btnOk->setEnabled(!(checked && ui->leProxyHost->text().isEmpty()));
}

void frmMain::on_cbAuth_toggled(bool checked)
{
    // Блокируем/разблокируем контролы аутентификации в зависимости от
    // включения/отключения аутентификации:
    ui->lblProxyUser->setEnabled(checked);
    ui->leProxyUser->setEnabled(checked);
    ui->lblProxyPass->setEnabled(checked);
    ui->leProxyPass->setEnabled(checked);
    ui->cbShowPassword->setEnabled(checked);
}

void frmMain::on_cbShowPassword_toggled(bool checked)
{
    if (checked)
        ui->leProxyPass->setEchoMode(QLineEdit::Normal);
    else
        ui->leProxyPass->setEchoMode(QLineEdit::Password);
}

void frmMain::on_btnCancel_clicked()
{
    qApp->exit();
}

void frmMain::on_btnOk_clicked()
{
    // Считываем настройки из пользовательских контролов:
    iTimeout = ui->sbTimeout->value();
    bUseProxy = ui->cbUseProxy->isChecked();
    iProxyType = ui->cbProxyType->currentIndex();
    sProxyHost = ui->leProxyHost->text();
    iProxyPort = ui->sbProxyPort->value();
    bProxyAuth = ui->cbAuth->isChecked();
    sProxyUser = ui->leProxyUser->text();
    sProxyPass = ui->leProxyPass->text();

    // Применяем новые настройки.
    QNetworkProxy proxy;
    // В случае, если включено использование прокси - устанавливаем прокси для
    // приложения:
    if (bUseProxy && !sProxyHost.isEmpty())
    {
        if (iProxyType > 0)
            proxy.setType(QNetworkProxy::Socks5Proxy);
        else
            proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(sProxyHost);
        proxy.setPort(iProxyPort);
        if (bProxyAuth && !sProxyUser.isEmpty())
        {
            proxy.setUser(sProxyUser);
            proxy.setPassword(sProxyPass);
        }
        QNetworkProxy::setApplicationProxy(proxy);
    }
    else
    {
        // Устанавливаем прокси с типом "Нет прокси":
        proxy.setType(QNetworkProxy::NoProxy);
        QNetworkProxy::setApplicationProxy(proxy);
    }

    // Создание объекта настроек.
    // Формируем путь к директории данных программы.
    // Получаем путь к директории пользователя:
    QString sAppDataFolder = QDir::homePath();
    // Добавляем универсальный разделитель путей:
    sAppDataFolder.append(QDir::separator());
    // Добавляем директорию с файлами конфигурации приложений:
    sAppDataFolder.append(CONFIG_FOLDER);
    // Снова добавляем универсальный разделитель путей:
    sAppDataFolder.append(QDir::separator());
    // Добавляем название программы (директория данных программы будет называться
    // так же, как и сама программа):
    sAppDataFolder.append(APP_NAME_SIGN);
    // Формируем путь к файлу с настройками приложения:
    QString sSetsFile = sAppDataFolder;
    sSetsFile.append(QDir::separator());
    sSetsFile.append(APP_NAME_SIGN);
    sSetsFile.append(INI_FILESUF);
    // Создаём объект для работы с настройками:
    QSettings sets(sSetsFile, QSettings::IniFormat, this);
    sets.setIniCodec(UTF8_ABBR);
    // Устанавливаем значения настроек.
    // Таймаут:
    sets.setValue(SETS_TIMEOUT, iTimeout);
    // Флаг использования прокси:
    sets.setValue(SETS_USE_PROXY, bUseProxy);
    // Тип прокси:
    sets.setValue(SETS_PROXY_TYPE, iProxyType);
    // Хост прокси:
    sets.setValue(SETS_PROXY_HOST, sProxyHost);
    // Порт прокси:
    sets.setValue(SETS_PROXY_PORT, iProxyPort);
    // Флаг аутентификации прокси:
    sets.setValue(SETS_PROXY_AUTH, bProxyAuth);
    // Имя пользователя и пароль аутентификации прокси:
    sets.setValue(SETS_PROXY_USER, sProxyUser);
    sets.setValue(SETS_PROXY_PASS, sProxyPass);
    // Записываем изменения в файл:
    sets.sync();

    // Завершаем работу программы:
    qApp->quit();
}

void frmMain::on_leProxyHost_textChanged(const QString &arg1)
{
    // Блокируем кнопку применения, если включено использование прокси,
    // но хост прокси - пустой:
    ui->btnOk->setEnabled(!(ui->cbUseProxy->isChecked() && arg1.isEmpty()));
}

void frmMain::on_btnHelp_clicked()
{
    //MsgUtilsMy::showInfoMsg(this, getHelpDir());
    //return;
    // Показываем индексную страничку справки:
    QString sHelpIndex = getHelpDir() % HELP_INDEX;
    if (!QFile::exists(sHelpIndex))
    {
        // Не найден индексный файл справки. Сообщаем пользователю об ошибке:
        MsgUtilsMy::showErrorMsg(this, tr("Help index file not found. "
            "Try to reinstall the program."), QString(),
            tr("File not found: ") % sHelpIndex);
        return;
    }
    // Открываем индексный файл справки:
    QDesktopServices::openUrl(QUrl(FILE_PROT % sHelpIndex));
}

void frmMain::on_btnAbout_clicked()
{
    // Создаём форму "О программе":
    frmAbout fabout(this);
    // Проверяем, есть ли локализованная страничка поддержки разработчика,
    // и если есть - устанавливаем её:
    QString sLocSupport = getHelpDir() % "support.html";
    if (QFile::exists(sLocSupport))
    {
        sLocSupport.prepend(FILE_PROT);
        fabout.setSupportUrl("<a href=\"" % sLocSupport %
            "\">" % tr("Support page") % "</a>");
    }
    // Показываем форму "О программе" в диалоговом режиме:
    fabout.exec();
}
