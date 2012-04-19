/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#include "rubberband.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QLineF>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPen>

RubberBand::RubberBand ( QWidget * parent )
    : QRubberBand ( QRubberBand::Rectangle, parent )
    , p_settings ( new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab" ) )
{
  setMinimumWidth ( 128 );
  setMinimumHeight ( 96 );
  setAutoFillBackground ( false );
  setContentsMargins ( 0, 0, 0, 0 );
  setWindowOpacity ( 1.0 ); // BUGFIX Composite extension behavior
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_OpaquePaintEvent, true );
  setAttribute ( Qt::WA_PaintOnScreen, true );
}

const QColor RubberBand::frameColor() const
{
  QColor color;
  color.setNamedColor ( p_settings.value ( "Rubberband/Color", "#800000" ).toString() );
  return color;
}

void RubberBand::initStyleOption ( QStyleOptionRubberBand * option ) const
{
  if ( option )
    option->initFrom ( this );
}

/**
* Es gibt eingige Distribution die der Meinung sind sie müssten
* Unbedingt den Rubberband Style an Windoof anpassen. :-/
* Hier mein Hack um dieses wieder zu umgehen!
*/
void RubberBand::paintEvent ( QPaintEvent * event )
{
  QRubberBand::paintEvent ( event );

  QStyleOptionRubberBand panel;
  initStyleOption ( &panel );

  QPainter painter ( this );
  painter.fillRect ( panel.rect, frameColor() );
}

bool RubberBand::isScalability()
{
  if ( ( size().width() % 2 ) != 0 )
  {
    QString str = QString::number ( size().width() );
    emit error ( trUtf8 ( "Scalability Failure Width" ),
                 trUtf8 ( "Frame Width must be a multiple of 2" ) );
    return false;
  }

  if ( ( size().height() % 2 ) != 0 )
  {
    QString str = QString::number ( size().height() );
    emit error ( trUtf8 ( "Scalability Failure Height" ),
                 trUtf8 ( "Frame Height must be a multiple of 2" ) );
    return false;
  }

  return true;
}

RubberBand::~RubberBand()
{}
