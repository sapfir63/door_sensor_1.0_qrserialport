/** @file Заголовочный файл класса для работы с версиями.
Версия: 1.0.0
Дата и время последнего изменения: 2012-02-23 22:54
Автор: Сергей Ткаченко (CanSee the Pain)
Сайт: http://life-warrior.org
*/

#ifndef LWVERSION_H
#define LWVERSION_H

#include <QString>

class LWVersion
{
public:
    LWVersion();
    LWVersion(QString sVersioIn);
    LWVersion(int iVerMajorIn, int iVerMinorIn = 0,
        int iVerRevisionIn = 0, int iVerBuildIn = 0);

    int major() const;
    int minor() const;
    int revision() const;
    int build() const;

    void setMajor(int iVerMajorIn);
    void setMinor(int iVerMinorIn);
    void setRevision(int iVerRevisionIn);
    void setBuild(int iVerBuildIn);

    QString toString() const;
    static QString toString(int iVerMajorIn, int iVerMinorIn = 0,
        int iVerRevisionIn = 0, int iVerBuildIn = 0);
    bool isHigherThan(const LWVersion & OtherVer) const;
    bool isLowerThan(const LWVersion & OtherVer) const;

private:
    int iVerMajor;
    int iVerMinor;
    int iVerRevision;
    int iVerBuild;

    void init();
};

#endif // LWVERSION_H
