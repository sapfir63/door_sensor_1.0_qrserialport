#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

/** @brief Строка с именем кодека utf-8 для использования его там, где требуется текстовый кодек (например, в файлах настроек) */
#define UTF8_ABBR "UTF-8"

namespace Ui {
    class frmMain;
}

class frmMain : public QDialog
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

    void checkSiteVer(const QString & sCurVerIn, const QString & sProgIn,
        const QString & sPageForCheckIn, const QString & sPageGoToIn,
        bool bExitOnReplyIn = false, int iNupMsgIn = 0);

private:
    Ui::frmMain *ui;

    /** @brief Таймер для отслеживания таймаута запроса. */
    QTimer timer;
    /** @brief Ссылка на объект ответа - нужна для завершения запроса по таймауту. */
    QNetworkReply * reply;

    /** @brief Флаг завершения работы после выполнения запроса.
    Нужен будет после обеспечения работы программы с конфигурационными файлами для обновления приложений и возможности проверять обновления сразу нескольких приложений из графического интерфейса. */
    bool bExitOnReply;

    // Текущие параметры проверки

    /** @brief Текущая версия программы, наличие новой версии которой надо проверить. */
    QString sCurVer;
    /** @brief Название программы, наличие версии для которой надо проверить. */
    QString sProg;
    /** @brief Страница сайта, с которой надо получить номер последней версии программы. */
    QString sPageForCheck;
    /** @brief Страница сайта, переход на которую надо предложить пользователю в случае наличия новой версии программы. */
    QString sPageGoTo;
    /** @brief Режим оповещения об отсутствии обновлений.

    0 означает поведение по умолчанию. 1 - не показывать сообщение об отсутствии обновлений. 2 - показывать сообщение. */
    int iNupMsg;


    // Настройки приложения

    /** @brief Таймаут запроса. */
    int iTimeout;
    /** @brief Флаг использования прокси. */
    bool bUseProxy;
    /** @brief Тип прокси. */
    int iProxyType;
    /** @brief Хост прокси. */
    QString sProxyHost;
    /** @brief Порт прокси. */
    int iProxyPort;
    /** @brief Флаг аутентификации прокси. */
    bool bProxyAuth;
    /** @brief Имя пользователя прокси. */
    QString sProxyUser;
    /** @brief Тип прокси. */
    QString sProxyPass;

    /** @brief Объект QNetworkAccessManager для запроса номера последней версии программы. */
    QNetworkAccessManager * nman;

    QString getHelpDir() const;

private slots:
    void readReply(QNetworkReply * replyIn);
    void requestTimeout();
    void on_cbUseProxy_toggled(bool checked);
    void on_cbAuth_toggled(bool checked);
    void on_cbShowPassword_toggled(bool checked);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_leProxyHost_textChanged(const QString &arg1);
    void on_btnHelp_clicked();
    void on_btnAbout_clicked();
	void slotCheckUpdate();
};

#endif // FRMMAIN_H
