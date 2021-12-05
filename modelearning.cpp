#include "modelearning.h"
#include "ui_modelearning.h"
#include <QPainter>
#include <QPicture>
#include <QDebug>

#define COLOR_ORDINARY_TEXT      "#474d47"
#define COLOR_HIGHLIGHTED_TEXT   "red"

#define ARROW_PLUS_X 10
#define ARROW_CHANGE_Y 5

ModeLearning::ModeLearning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeLearning)
{
    ui->setupUi(this);
}

ModeLearning::~ModeLearning()
{
    delete ui;
}

void ModeLearning::drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo)
{
    const QString answer = translateNumber(valStr, numberSystemFrom, numberSystemTo);
    const int to = valueOfNumberSystem(numberSystemTo);
    QString toStr = QString::number(to);

    QImage image(QSize(1000, 1000), QImage::Format_ARGB32);

    QPainter painter;
    QFont font("Courier", 10);
    QFontMetrics fm(font);
    QColor ordinaryColor(COLOR_ORDINARY_TEXT);
    QColor highlightedColor(COLOR_HIGHLIGHTED_TEXT);
    painter.begin(&image);

    painter.setFont(font);
    painter.setPen(ordinaryColor);
    painter.setRenderHint(QPainter::Antialiasing, true);

    switch (numberSystemFrom)
    {
    case NumberSystem::Ten:
    {
        int val = valStr.toInt();
        int countDigitsVal = getCountDigits(val);

        QString minus = "-";
        int minusWidth = fm.horizontalAdvance(minus);

        int divisiblePosX = 10 + minusWidth;
        int divisiblePosY = fm.height() + 1;

        while(true)
        {
            int res = val / to;

            int deductible = res*to;

            int rest = val - deductible;

            int countDigitsDeductible = getCountDigits(deductible);

            QString deductibleValStr(countDigitsVal-countDigitsDeductible, QChar(' '));
            deductibleValStr += QString::number(deductible);

            QString restStr = QString::number(rest, to);
            int countDigitsRest = valStr.size()-restStr.size();
            if(countDigitsRest)
            {
                restStr.insert(0, QString(countDigitsRest, ' '));
            }

            int widthStr = fm.horizontalAdvance(valStr);

            painter.drawText(divisiblePosX - minusWidth, divisiblePosY + fm.height() / 2, minus);

            painter.drawText(divisiblePosX, divisiblePosY, valStr);
            int deductibleY = divisiblePosY + fm.height();
            painter.drawText(divisiblePosX, deductibleY, deductibleValStr);
            int horLineUnderDeductibleY = deductibleY + 2;
            painter.drawLine(divisiblePosX, horLineUnderDeductibleY, divisiblePosX + widthStr, horLineUnderDeductibleY);

            painter.setPen(highlightedColor);
            painter.drawText(divisiblePosX, horLineUnderDeductibleY + fm.height() - 1, restStr);
            painter.setPen(ordinaryColor);

            int deviderX = divisiblePosX + widthStr + 5;
            painter.drawText(deviderX, divisiblePosY, toStr);
            int xLine = divisiblePosX + widthStr + 1;
            int yLine1 = divisiblePosY - fm.height();
            int yLine2 = divisiblePosY + fm.height()*2 + 3;

            painter.drawLine(xLine, yLine1, xLine, yLine2);
            painter.drawLine(xLine + 2, yLine1 + fm.height() + 2, xLine + widthStr + 4, yLine1 + fm.height() + 2);

            divisiblePosX = deviderX + minusWidth;
            divisiblePosY = divisiblePosY + fm.height();
            val = res;

            valStr = QString::number(val);
            if(val < to)
            {
                painter.setPen(highlightedColor);
                painter.drawText(divisiblePosX, divisiblePosY, valStr);
                break;
            }

            countDigitsVal = getCountDigits(val);
        }

        painter.setPen(highlightedColor);
        int arrowX1 = 10 + minusWidth;
        int arrowX2 = divisiblePosX + fm.horizontalAdvance(val) + 2;
        int arrowY = divisiblePosY + fm.height() * 2.5;
        painter.drawLine(arrowX1, arrowY, arrowX1 + ARROW_PLUS_X, arrowY - ARROW_CHANGE_Y);
        painter.drawLine(arrowX1, arrowY, arrowX2, arrowY);
        painter.drawLine(arrowX1, arrowY, arrowX1 + ARROW_PLUS_X, arrowY + ARROW_CHANGE_Y);

        painter.drawText(arrowX1, arrowY + ARROW_CHANGE_Y*2 + fm.height(), "Ответ: " + answer);

        break;
    }
    default:
        break;
    }

    painter.end();

    ui->label_2->setPixmap(QPixmap::fromImage(image));
}

