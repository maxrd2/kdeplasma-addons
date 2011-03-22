/*
 *   Copyright 2008,2011 Sebastian Kügler <sebas@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "kdevelopsessions.h"


#include <KDebug>
#include <KDirWatch>
#include <KStandardDirs>
#include <KToolInvocation>
#include <KIcon>
#include <KConfig>
#include <KConfigGroup>
#include <KUrl>
#include <KStringHandler>
#include <QFile>

bool kdevelopsessions_runner_compare_sessions(const QString &s1, const QString &s2) {
    return KStringHandler::naturalCompare(s1,s2)==-1;
}

KDevelopSessions::KDevelopSessions(QObject *parent, const QVariantList& args)
    : Plasma::AbstractRunner(parent, args),
    m_sessionWatch(0)
{
    setObjectName(QLatin1String("KDevelop Sessions"));
    setIgnoredTypes(Plasma::RunnerContext::File | Plasma::RunnerContext::Directory | Plasma::RunnerContext::NetworkLocation);
    Plasma::RunnerSyntax s(QLatin1String(":q:"), i18nc("syntax description", "Finds KDevelop sessions matching :q:."));
    s.addExampleQuery(QLatin1String("kdevelop :q:"));
    addSyntax(s);

    addSyntax(Plasma::RunnerSyntax(QLatin1String("kdevelop"), i18nc("syntax description", "Lists all the KDevelop sessions in your account.")));

    connect(this, SIGNAL(prepare()), SLOT(slotPrepare()));
    connect(this, SIGNAL(teardown()), SLOT(slotTeardown()));
}

void KDevelopSessions::slotPrepare()
{
    loadSessions();

    // listen for changes to the list of kdevelop sessions
    if (!m_sessionWatch) {
        m_sessionWatch = new KDirWatch(this);
    }
    const QStringList sessiondirs = KGlobal::dirs()->findDirs("data", QLatin1String("kdevelop/sessions/"));
    foreach (const QString &dir, sessiondirs) {
        m_sessionWatch->addDir(dir);
    }
    connect(m_sessionWatch,SIGNAL(dirty(QString)),this,SLOT(loadSessions()));
    connect(m_sessionWatch,SIGNAL(created(QString)),this,SLOT(loadSessions()));
    connect(m_sessionWatch,SIGNAL(deleted(QString)),this,SLOT(loadSessions()));
}

void KDevelopSessions::slotTeardown()
{
    delete m_sessionWatch;
    m_sessionWatch = 0;
    m_sessions.clear();
}

KDevelopSessions::~KDevelopSessions()
{
}

void KDevelopSessions::loadSessions()
{
    QStringList sessions = QStringList();
    const QStringList list = KGlobal::dirs()->findAllResources( "data", QLatin1String("kdevelop/sessions/*/sessionrc"), KStandardDirs::Recursive );

    foreach (const QString &sessionfile, list) {
        KConfig cfg(sessionfile);
        QString sessionName = cfg.entryMap()["SessionName"];
        if (!sessionName.isEmpty()) {
            sessions.append(sessionName);
        }
    }
    qSort(sessions.begin(),sessions.end(),kdevelopsessions_runner_compare_sessions);
    m_sessions = sessions;

}

void KDevelopSessions::match(Plasma::RunnerContext &context)
{
    if (m_sessions.isEmpty()) {
        return;
    }

    QString term = context.query();
    if (term.length() < 3) {
        return;
    }

    bool listAll = false;

    if (term.startsWith(QLatin1String("kdevelop"), Qt::CaseInsensitive)) {
        if (term.trimmed().compare(QLatin1String("kdevelop"), Qt::CaseInsensitive) == 0) {
            listAll = true;
            term.clear();
        } else if (term.at(8) == QLatin1Char(' ') ) {
            term.remove(QLatin1String("kdevelop "), Qt::CaseInsensitive);
        } else {
            term.clear();
        }
    }

    if (term.isEmpty() && !listAll) {
        return;
    }

    foreach (const QString &session, m_sessions) {
        if (!context.isValid()) {
            return;
        }

        if (listAll || (!term.isEmpty() && session.contains(term, Qt::CaseInsensitive))) {
            Plasma::QueryMatch match(this);
            if (listAll) {
                // All sessions listed, but with a low priority
                match.setType(Plasma::QueryMatch::ExactMatch);
                match.setRelevance(0.8);
            } else {
                if (session.compare(term, Qt::CaseInsensitive) == 0) {
                    // parameter to kdevelop matches session exactly, bump it up!
                    match.setType(Plasma::QueryMatch::ExactMatch);
                    match.setRelevance(1.0);
                } else {
                    // fuzzy match of the session in "kdevelop $session"
                    match.setType(Plasma::QueryMatch::PossibleMatch);
                    match.setRelevance(0.8);
                }
            }
            match.setIcon(KIcon(QLatin1String("kdevelop")));
            match.setData(session);
            match.setText(session);
            match.setSubtext(i18n("Open KDevelop Session"));
            context.addMatch(term, match);
        }
    }
}

void KDevelopSessions::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context)
    QString session = match.data().toString();
    kDebug() << "Open KDevelop Session " << session;

    if (!session.isEmpty()) {
        QStringList args;
       	args << QLatin1String("--open-session") << session;
        KToolInvocation::kdeinitExec(QLatin1String("kdevelop"), args);
    }
}

#include "kdevelopsessions.moc"