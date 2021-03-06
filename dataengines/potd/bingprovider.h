/*
 *   Copyright 2017 Weng Xuetian <wengxt@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
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

#ifndef BINGPROVIDER_H
#define BINGPROVIDER_H

#include "potdprovider.h"

/**
 * This class provides the image for the Bing's homepage
 * url is obtained from http://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1
 */
class BingProvider : public PotdProvider
{
    Q_OBJECT

    public:
        /**
         * Creates a new Bing provider.
         *
         * @param date The date for which the image shall be fetched.
         * @param parent The parent object.
         */
        BingProvider( QObject *parent, const QVariantList &args );

        /**
         * Destroys the Bing provider.
         */
        ~BingProvider();

        /**
         * Returns the requested image.
         *
         * Note: This method returns only a valid image after the
         *       finished() signal has been emitted.
         */
        QImage image() const Q_DECL_OVERRIDE;

    private:
      class Private;
      Private* const d;

      Q_PRIVATE_SLOT( d, void pageRequestFinished( KJob* ) )
      Q_PRIVATE_SLOT( d, void imageRequestFinished( KJob* ) )
};

#endif

