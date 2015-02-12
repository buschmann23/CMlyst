/***************************************************************************
 *   Copyright (C) 2014-2015 Daniel Nicoletti <dantti12@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; see the file COPYING. If not, write to       *
 *   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,  *
 *   Boston, MA 02110-1301, USA.                                           *
 ***************************************************************************/

#include "engine.h"
#include "menu.h"

#include <QRegularExpression>
#include <QStringList>
#include <QDateTime>
#include <QDebug>

using namespace CMS;

Engine::Engine(QObject *parent) : QObject(parent)
{

}

Engine::~Engine()
{

}

bool Engine::init(const QHash<QString, QString> &settings)
{
    return true;
}

Page *Engine::getPage(const QString &path)
{
    return 0;
}

Page *Engine::getPageToEdit(const QString &path)
{
    Q_UNUSED(path)
    return 0;
}

bool Engine::savePage(Page *page)
{
    return false;
}

QList<Page *> Engine::listPages(Filters filters, SortFlags sort, int depth, int limit)
{
    Q_UNUSED(filters)
    Q_UNUSED(sort)
    Q_UNUSED(depth)
    Q_UNUSED(limit)
    return QList<Page *>();
}

QList<Menu *> Engine::menus()
{
    return QList<Menu *>();
}

Menu *Engine::menu(const QString &name)
{
    Q_FOREACH (Menu *menu, menus()) {
        if (menu->name() == name) {
            return menu;
        }
    }
    return 0;
}

QHash<QString, Menu *> Engine::menuLocations()
{
    QHash<QString, Menu *> ret;

    Q_FOREACH (Menu *menu, menus()) {
        Q_FOREACH (const QString &location, menu->locations()) {
            if (!ret.contains(location)) {
                ret.insert(location, menu);
            }
        }
    }

    return ret;
}

bool Engine::saveMenu(Menu *menu, bool replace)
{
    Q_UNUSED(menu)
    Q_UNUSED(replace)
    return false;
}

bool Engine::removeMenu(const QString &name)
{
    Q_UNUSED(name)
    return false;
}

bool Engine::saveMenus(const QList<Menu *> &menus)
{
    Q_FOREACH (Menu *menu, menus) {
        if (!saveMenu(menu, true)) {
            return false;
        }
    }

    return true;
}

QDateTime Engine::lastModified()
{
    return QDateTime();
}

bool Engine::settingsIsWritable()
{
    return false;
}

QHash<QString, QString> Engine::settings()
{
    return QHash<QString, QString>();
}

QString Engine::settingsValue(const QString &key, const QString &defaultValue)
{
    Q_UNUSED(key)
    return defaultValue;
}

bool Engine::setSettingsValue(const QString &key, const QString &value)
{
    Q_UNUSED(key)
    Q_UNUSED(value)
    return false;
}

QString Engine::normalizePath(const QString &path)
{
    // "/foo/bar/Iam a big...path" turns into
    // "/foo/bar/iam-a-big-path"
    QStringList parts = path.split(QChar('/'));
    for (int i = 0; i < parts.size(); ++i) {
        parts.replace(i, normalizeTitle(parts.at(i)));
    }
    return parts.join(QChar('/'));
}

QString Engine::normalizeTitle(const QString &title)
{
    // "Iam a big/small...path" turns into
    // "/foo/bar/iam-a-bigsmall-path"
    QString ret = title.toLower();

    // turn separators into space
    ret.replace(QChar('.'), QChar::Space);
    ret.replace(QChar('-'), QChar::Space);

    // remove everything that is not a word or space
    ret.remove(QRegularExpression("[^\\w\\s]"));

    // remove abused space
    ret = ret.simplified();

    // turn space into dashes
    ret.replace(QChar::Space, QChar('-'));

    return ret;
}

QString CMS::Engine::title()
{
    return settingsValue(QStringLiteral("title"));
}

QString Engine::description()
{
    return settingsValue(QStringLiteral("tagline"));
}
