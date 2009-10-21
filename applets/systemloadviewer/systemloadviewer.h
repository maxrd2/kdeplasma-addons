/****************************************************************************
* System Monitor: Plasmoid and data engines to monitor CPU/Memory/Swap Usage.
* Copyright (C) 2008  Matthew Dawson <matthewjd@gmail.com>
* Copyright (C) 2008-2009 Dominik Haumann <dhaumann kde org>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
*****************************************************************************/

#ifndef SYSTEMLOADVIEWER_H
#define SYSTEMLOADVIEWER_H

#include <Plasma/Applet>
#include <Plasma/DataEngine>

#include <QtCore/QVector>

#include "ui_coloursconfig.h"
#include "ui_generalconfig.h"

class QString;
class QSizeF;
class QPainter;
class QStyleOptionGraphicsItem;
class QPoint;

struct cpuInfo
{
    double user, sys, nice, idle, disk, clock;
    bool clockValid;
    cpuInfo(): user(0), sys(0), nice(0), idle(0), disk(0), clock(0), clockValid(false) {}
};

class SystemLoadViewer : public Plasma::Applet
{
    Q_OBJECT

public:
    SystemLoadViewer(QObject *parent, const QVariantList &args);
    virtual ~SystemLoadViewer();

    virtual void constraintsEvent(Plasma::Constraints constraints);
    virtual void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect& contentsRect);
    virtual void init();

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
protected slots:
    void dataUpdated(const QString& source, const Plasma::DataEngine::Data &data);
    void configUpdated();
    void sourcesAdded(const QString& source);
    void toolTipAboutToShow();

private:
    void paintCPUUsage(QPainter *p, const QStyleOptionGraphicsItem *option, const QRect& contentsRect, const cpuInfo &cpu);
    void paintSwapUsage(QPainter *p, const QStyleOptionGraphicsItem *option, const QRect& contentsRect);
    void paintRAMUsage(QPainter *p, const QStyleOptionGraphicsItem *option, const QRect& contentsRect);

    void updateSize();
    qreal widthForHeight(qreal h) const;
    qreal heightForWidth(qreal w) const;
    inline int cpuCount() const;
    inline int barCount() const;

    void readConfig();
    void createConfigurationInterface(KConfigDialog *parent);

    void reconnectSources();
    void reconnectCPUSources();
    void disconnectSources();
    void disconnectCPUSources();

    void setVerticalBars(bool verticalOrientation);
    bool verticalBars() const;

    QVector<cpuInfo> m_cpuInfo;
    uint m_numCPUs;
    double m_ramfree;
    double m_ramused;
    double m_ramapps;
    double m_rambuffers;
    double m_ramcached;
    double m_ramtotal;
    double m_swapfree;
    double m_swapused;
    double m_swaptotal;

    Ui::generalConfig uiGeneral;
    Ui::coloursConfig uiColours;

    Qt::Orientation m_barOrientation;
    bool m_showMultiCPU;
    bool m_swapAvailable;
    int m_updateInterval;

    QColor m_cpuUserColour;
    QColor m_cpuNiceColour;
    QColor m_cpuDiskColour;
    QColor m_cpuSysColour;
    QColor m_ramCachedColour;
    QColor m_ramBuffersColour;
    QColor m_ramUserColour;

    QColor m_swapUsedColour;
    QColor m_freeResourceColor;
    
    QPoint m_mousePressLoc;

    Plasma::DataEngine *sys_mon;
};

K_EXPORT_PLASMA_APPLET(systemloadviewer, SystemLoadViewer)

#endif // SYSTEMLOADVIEWER_H
