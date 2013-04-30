#include "PeoplePlot.h"
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_legend_item.h>
#include <qwt/qwt_plot_canvas.h>

class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw( const QTime &base ):
        baseTime( base )
    {
    }
    virtual QwtText label( double v ) const
    {
        QTime upTime = baseTime.addSecs( ( int )v );
        return upTime.toString();
    }
private:
    QTime baseTime;
};

class Background: public QwtPlotItem
{
public:
    Background()
    {
        setZ( 0.0 );
    }

    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }

    virtual void draw( QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRectF &canvasRect ) const
    {
        QColor c( Qt::white );
        QRectF r = canvasRect;

        for ( int i = 100; i > 0; i -= 10 )
        {
            r.setBottom( yMap.transform( i - 10 ) );
            r.setTop( yMap.transform( i ) );
            painter->fillRect( r, c );

            c = c.dark( 110 );
        }
    }
};

class PeopleCurve: public QwtPlotCurve
{
public:
    PeopleCurve( const QString &title ):
        QwtPlotCurve( title )
    {
        setRenderHint( QwtPlotItem::RenderAntialiased );
    }

    void setColor( const QColor &color )
    {
        QColor c = color;
        c.setAlpha( 150 );

        setPen( c );
        setBrush( c );
    }
};

PeoplePlot::PeoplePlot( QWidget *parent ):
    QwtPlot( parent ),
    dataCount( 0 ){
    setAutoReplot( false );
    canvas()->setBorderRadius( 10 );
    plotLayout()->setAlignCanvasToScales( true );
    setAxisTitle( QwtPlot::xBottom, " System Time" );
//    setAxisScaleDraw( QwtPlot::xBottom,
//        new TimeScaleDraw( cpuStat.upTime() ) );
    setAxisScale( QwtPlot::xBottom, 0, HISTORY );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    QwtScaleWidget *scaleWidget = axisWidget( QwtPlot::xBottom );
    const int fmh = QFontMetrics( scaleWidget->font() ).height();
    scaleWidget->setMinBorderDist( 0, fmh / 2);

    setAxisTitle( QwtPlot::yLeft, "Occupancy Count" );
    setAxisScale( QwtPlot::yLeft, 0, 15 );

    PeopleCurve *curve;

    curve = new PeopleCurve( "People" );
    curve->setColor( Qt::red );
    curve->attach( this );
    data.curve = curve;

    for ( int i = 0; i < HISTORY; i++ )
        timeData[HISTORY - 1 - i] = i;

    ( void )startTimer( 1000 ); // 1 second
}

void PeoplePlot::timerEvent( QTimerEvent * ){
    for ( int i = dataCount; i > 0; i-- ){
        if ( i < HISTORY )
            data.data[i] = data.data[i-1];
    }
    if(m_walnut_detector != nullptr)
        data.data[0] = m_walnut_detector->get_count_belief().first;
    else
        data.data[0] = 0;
    if ( dataCount < HISTORY )
        dataCount++;

    for ( int j = 0; j < HISTORY; j++ )
        timeData[j]++;

    setAxisScale( QwtPlot::xBottom,
        timeData[HISTORY - 1], timeData[0] );
    data.curve->setRawSamples(
        timeData, data.data, dataCount );
    replot();
}
