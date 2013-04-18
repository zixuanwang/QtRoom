#include "PeoplePlot.h"
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>

PeoplePlot::PeoplePlot( QWidget *parent ):
    QwtPlot( parent ),
    dataCount( 0 ){

}

void PeoplePlot::timerEvent( QTimerEvent * ){
    replot();
}

void PeoplePlot::showCurve( QwtPlotItem *item, bool on ){
    item->setVisible( on );

    QwtLegendItem *legendItem =
        qobject_cast<QwtLegendItem *>( legend()->find( item ) );

    if ( legendItem )
        legendItem->setChecked( on );

    replot();
}
