/** @file Заголовочный файл класса для работы с версиями. */

#include <QStringBuilder>
#include "LWVersion.h"


LWVersion::LWVersion()
{
    init();
}

LWVersion::LWVersion(QString sVersioIn)
{
    init();
    // Пробуем разобрать переданную строку.
    // Получаем позицию первой точки:
    int iDot = sVersioIn.indexOf('.');
    // Получаем строку с номером мажорной версии:
    QString sTmp = sVersioIn;
    // Убираем из строки с номером мажорной версии всё лишнее:
    if (iDot > -1)
        sTmp.remove(iDot, sVersioIn.length() - iDot);
    // Получаем мажорную версию:
    iVerMajor = sTmp.toInt();
    // Если первой точки нету - завершаем функцию:
    if (iDot == -1) return;
    // Убираем из строки с версией всё до первой точки и первую точку:
    sVersioIn.remove(0, iDot + 1);
    // Получаем позицию второй точки:
    iDot = sVersioIn.indexOf('.');
    // Получаем строку с номером минорной версии:
    sTmp = sVersioIn;
    // Убираем из строки с номером минорной версии всё лишнее:
    if (iDot > -1)
        sTmp.remove(iDot, sVersioIn.length() - iDot);
    // Получаем минорную версию:
    iVerMinor = sTmp.toInt();
    // Если второй точки нету - завершаем функцию:
    if (iDot == -1) return;
    // Убираем из строки с версией всё до второй точки и вторую точку:
    sVersioIn.remove(0, iDot + 1);
    // Получаем позицию третьей точки:
    iDot = sVersioIn.indexOf('.');
    // Получаем строку с номером ревизии:
    sTmp = sVersioIn;
    // Убираем из строки с номером ревизии всё лишнее:
    if (iDot > -1)
        sTmp.remove(iDot, sVersioIn.length() - iDot);
    // Получаем номер ревизии:
    iVerRevision = sTmp.toInt();
    // Если третьей точки нету - завершаем функцию:
    if (iDot == -1) return;
    // Убираем из строки с версией всё до третьей точки и третью точку:
    sVersioIn.remove(0, iDot + 1);
    // Получаем номер сборки:
    iVerBuild = sVersioIn.toInt();
}

LWVersion::LWVersion(int iVerMajorIn, int iVerMinorIn,
    int iVerRevisionIn, int iVerBuildIn)
{
    iVerMajor = iVerMajorIn;
    iVerMinor = iVerMinorIn;
    iVerRevision = iVerRevisionIn;
    iVerBuild = iVerBuildIn;
}


int LWVersion::major() const
{
    return iVerMajor;
}

int LWVersion::minor() const
{
    return iVerMinor;
}

int LWVersion::revision() const
{
    return iVerRevision;
}

int LWVersion::build() const
{
    return iVerBuild;
}

void LWVersion::setMajor(int iVerMajorIn)
{
    iVerMajor = iVerMajorIn;
}

void LWVersion::setMinor(int iVerMinorIn)
{
    iVerMinor = iVerMinorIn;
}

void LWVersion::setRevision(int iVerRevisionIn)
{
    iVerRevision = iVerRevisionIn;
}

void LWVersion::setBuild(int iVerBuildIn)
{
    iVerBuild = iVerBuildIn;
}

QString LWVersion::toString() const
{
    return QString::number(iVerMajor) % '.' %  QString::number(iVerMinor) %
            '.' % QString::number(iVerRevision) % '.' % QString::number(iVerBuild);
}

QString LWVersion::toString(int iVerMajorIn, int iVerMinorIn,
    int iVerRevisionIn, int iVerBuildIn)
{
    return QString::number(iVerMajorIn) % '.' %  QString::number(iVerMinorIn) %
            '.' % QString::number(iVerRevisionIn) % '.' % QString::number(iVerBuildIn);
}

void LWVersion::init()
{
    iVerMajor = 0;
    iVerMinor = 0;
    iVerRevision = 0;
    iVerBuild = 0;
}

bool LWVersion::isHigherThan(const LWVersion & OtherVer) const
{
    // Если мажорная версия этого объекта больше - значит, и вся версия больше:
    if (iVerMajor > OtherVer.major())
        return true;
    // Если мажорная версия этого объекта меньше - значит, и вся версия меньше:
    if (iVerMajor < OtherVer.major())
        return false;
    // Мажорная версия этого объекта равна мажорной версии другого.
    // Сравниваем минорные версии.
    // Если минорная версия другого объекта больше - значит, и вся версия больше:
    if (iVerMinor > OtherVer.minor())
        return true;
    // Если минорная версия этого объекта меньше - значит, и вся версия меньше:
    if (iVerMinor < OtherVer.minor())
        return false;
    // Минорная версия этого объекта также равна минорной версии другого.
    // Сравниваем номера ревизий.
    // Если номер ревизии другого объекта больше - значит, и вся версия больше:
    if (iVerRevision > OtherVer.revision())
        return true;
    // Если минорная версия этого объекта меньше - значит, и вся версия меньше:
    if (iVerRevision < OtherVer.revision())
        return false;
    // Номера ревизий также равны.
    // Сравниваем номера сборки.
    if (iVerBuild > OtherVer.build())
        return true;
    // Всё равно, ну или номер сборки другого объекта больше - в любом случае
    // надо вернуть false:
    return false;
}

bool LWVersion::isLowerThan(const LWVersion & OtherVer) const
{
    // Если мажорная версия другого объекта больше - значит, и вся версия больше:
    if (OtherVer.major() > iVerMajor)
        return true;
    // Если мажорная версия другого объекта меньше - значит, и вся версия меньше:
    if (OtherVer.major() < iVerMajor)
        return false;
    // Мажорная версия другого объекта равна мажорной версии этого.
    // Сравниваем минорные версии.
    // Если минорная версия другого объекта больше - значит, и вся версия больше:
    if (OtherVer.minor() > iVerMinor)
        return true;
    // Если минорная версия другого объекта меньше - значит, и вся версия меньше:
    if (OtherVer.minor() < iVerMinor)
        return false;
    // Минорная версия другого объекта также равна минорной версии этого.
    // Сравниваем номера ревизий.
    // Если номер ревизии другого объекта больше - значит, и вся версия больше:
    if (OtherVer.revision() > iVerRevision)
        return true;
    // Если номер ревизии другого объекта меньше - значит, и вся версия меньше:
    if (OtherVer.revision() < iVerRevision)
        return false;
    // Номер ревизии другого объекта также равна минорной версии этого.
    // Сравниваем номера сборки.
    if (OtherVer.build() > iVerBuild)
        return true;
    // Всё равно, ну или номер сборки этого объекта больше - в любом случае
    // надо вернуть false:
    return false;
}
