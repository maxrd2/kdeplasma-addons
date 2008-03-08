/*
 *   Copyright (C) 2007 Ivan Cukic <ivan.cukic+kde@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser/Library General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser/Library General Public License for more details
 *
 *   You should have received a copy of the GNU Lesser/Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LANCELOTWINDOW_H_
#define LANCELOTWINDOW_H_

#include <plasma/svg.h>
#include <plasma/plasma.h>
#include <plasma/corona.h>
#include <KConfig>
#include <KConfigGroup>

#include "ui_LancelotWindowBase.h"

class CustomGraphicsView;

namespace Lancelot {
    class ActionListViewModel;
    class ResizeBordersPanel;
}

class LancelotWindow: public QWidget, public Ui::LancelotWindowBase
{
    Q_OBJECT
public:
    LancelotWindow();
    virtual ~LancelotWindow();
    bool eventFilter(QObject * object, QEvent * event);

public Q_SLOTS:
    /**
     * Shows full window.
     * @param x x coordinate of the mouse
     * @param y y coordinate of the mouse
     */
    void lancelotShow(int x, int y);

    /**
     * Shows window with only specified section
     * @param x x coordinate of the mouse
     * @param y y coordinate of the mouse
     * @param name the name of the section to be shown
     */
    void lancelotShowItem(int x, int y, QString name);

    /**
     * Hides window
     * @param immediate if not set, the window will start \
     *   the auto-timer for hiding. If true, window will be \
     *   hidden immediately
     */
    void lancelotHide(bool immediate = false);

    /**
     * Opens the search section and searches for
     * the specified string,
     * @param string search string
     */
    void search(const QString & string);

protected Q_SLOTS:
    /**
     * Shows specified section
     * @param item section to be shown
     */
    void sectionActivated(const QString & item);

    /**
     * Locks the system session
     */
    void systemLock();

    /**
     * Opens the log out/shut down dialog
     */
    void systemLogout();

    /**
     * Opens the switch user dialog
     */
    void systemSwitchUser();

private Q_SLOTS:
    // The following function do the real work for the
    // corresponding slots. They are invoked with a small
    // delay to avoid dead-locks.
    void systemDoLock();
    void systemDoLogout();
    //void systemDoSwitchUser();

public:
    /**
     * Returns a list of available sections - their IDs
     */
    QStringList sectionIDs();

    /**
     * Returns a list of available sections - their names
     */
    QStringList sectionNames();

    /**
     * Returns a list of available sections - their icons
     */
    QStringList sectionIcons();

protected:
    /**
     * Resizes window and relayouts everything.
     */
    void resizeWindow();

    /**
     * Shows window
     * @param x x coordinate of position to show at
     * @param y y coordinate of position to show at
     */
    void showWindow(int x, int y);

    /**
     * Creates and sets up data models
     */
    void setupModels();

    /**
     * Loads configuration options
     */
    void loadConfig();

    void focusOutEvent (QFocusEvent * event);
    void leaveEvent    (QEvent      * event);
    void enterEvent    (QEvent      * event);

private:
    Lancelot::ResizeBordersPanel * m_root;
    CustomGraphicsView  * m_view;
    Plasma::Corona      * m_corona;
    QVBoxLayout         * m_layout;

    QTimer                m_hideTimer;
    QString               m_searchString;

    bool                  m_hovered;
    bool                  m_showingFull;

    QSignalMapper       * m_sectionsSignalMapper;

    QMap < QString, Lancelot::ActionListViewModel * > m_models;
    QMap < QString, Lancelot::ActionListViewModel * > m_modelGroups;

    KConfig               m_config;
    KConfigGroup          m_mainConfig;

    Lancelot::Instance  * instance;

// Resize related code
protected:
    void mousePressEvent   (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    void mouseMoveEvent    (QMouseEvent * event);

private:
    enum BaseDirection {
        None = 0,
        Up   = 1, Down  = 2,
        Left = 4, Right = 8
    };

    Q_DECLARE_FLAGS(ResizeDirection, BaseDirection);

    ResizeDirection m_resizeDirection;
    QPoint          m_originalMousePosition;
    QPoint          m_originalWindowPosition;
    QSize           m_originalMainSize;
    QSize           m_mainSize;

    friend class CustomGraphicsView;
};

#endif /*LANCELOTWINDOW_H_*/
