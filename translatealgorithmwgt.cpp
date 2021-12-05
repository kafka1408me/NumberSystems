#include "translatealgorithmwgt.h"
#include <QPainter>
#include <QtDebug>


#define COLOR_TEXT "#474d47"

TranslateAlgorithmWgt::TranslateAlgorithmWgt(QWidget *parent) :
    QWidget(parent)
{

}

TranslateAlgorithmWgt::~TranslateAlgorithmWgt()
{
    qDebug() << "end";
}

void TranslateAlgorithmWgt::drawTranslateAlgorithm(const QString &valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo)
{
    QPainter painter;
    QFont font("Courier", 10);
    QFontMetrics fm(font);
    QBrush brush(QColor(COLOR_TEXT));
    painter.begin(this);

    painter.setFont(font);
    painter.setBrush(brush);

    QString s = "15";
    painter.drawText(QPoint{0,fm.height() + 1}, s);

    painter.drawLine(fm.width(s) + 1, 1, fm.width(s) + 1, fm.height());

    painter.end();
    repaint();
}

void TranslateAlgorithmWgt::paintEvent(QPaintEvent *)
{

}
