/*
    This file is part of KDE.

    Copyright (c) 2009 Eckhart Wörner <ewoerner@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#ifndef PERSONSERVICE_H
#define PERSONSERVICE_H

#include <Plasma/Service>

#include "provider.h"


class PersonService : public Plasma::Service
{
    public:
        PersonService(const Attica::Provider& provider, const QString& id, QObject* parent = 0);

    protected:
        virtual Plasma::ServiceJob* createJob(const QString& operation, QMap<QString, QVariant>& parameters);

    private:
        QString m_id;
        Attica::Provider m_provider;
};


#endif
