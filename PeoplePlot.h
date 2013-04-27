#ifndef PEOPLEPLOT_H
#define PEOPLEPLOT_H

#include <QPainter>
#include <QTime>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include "WalnutDetector.h"
#define HISTORY 60
class PeoplePlot : public QwtPlot
{
    Q_OBJECT
public:
    PeoplePlot( QWidget * = 0 );
    const QwtPlotCurve* get_curve() const{
        return data.curve;
    }
    void set_walnut_detector(std::shared_ptr<WalnutDetector> walnut_detector){m_walnut_detector = walnut_detector;}
protected:
    void timerEvent( QTimerEvent *e );

private Q_SLOTS:

private:
    struct{
        QwtPlotCurve *curve;
        double data[HISTORY];
    } data;

    double timeData[HISTORY];

    int dataCount;
    std::shared_ptr<WalnutDetector> m_walnut_detector;
};

#endif // PEOPLEPLOT_H
