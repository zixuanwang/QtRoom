#ifndef PEOPLEPLOT_H
#define PEOPLEPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#define HISTORY 60
class PeoplePlot : public QwtPlot
{
    Q_OBJECT
public:
    PeoplePlot( QWidget * = 0 );
    const QwtPlotCurve* get_curve() const{
        return data.curve;
    }
protected:
    void timerEvent( QTimerEvent *e );

private Q_SLOTS:
    void showCurve( QwtPlotItem *, bool on );

private:
    struct{
        QwtPlotCurve *curve;
        double data[HISTORY];
    } data;

    double timeData[HISTORY];

    int dataCount;
};

#endif // PEOPLEPLOT_H
