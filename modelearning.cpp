#include "modelearning.h"
#include "ui_modelearning.h"
#include <QPainter>
#include <QPicture>
#include <QDebug>
#include <cmath>
#include "numbersysteminputvalidator.h"


#define COLOR_ORDINARY_TEXT      "#474d47"
#define COLOR_HIGHLIGHTED_TEXT   "red"

#define ARROW_PLUS_X 10
#define ARROW_CHANGE_Y 5

ModeLearning::ModeLearning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeLearning)
{
    ui->setupUi(this);

    ui->numberLineEdit->setValidator(MyValidator);

    init();
}

ModeLearning::~ModeLearning()
{
    delete ui;
}

void ModeLearning::drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo)
{
    QString startVal = valStr;
    const QString answer = translateNumber(valStr, numberSystemFrom, numberSystemTo);
    const int from = valueOfNumberSystem(numberSystemFrom);
    const int to = valueOfNumberSystem(numberSystemTo);
    QString toStr = QString::number(to);
    QString fromStr = QString::number(from);

    QImage image(QSize(1000, 1000), QImage::Format_ARGB32);

    QPainter painter;
    QPainter painterSmall;
    QFont font("Courier", 14);
    QFont fontSmall("Courier", 8);
    QFontMetrics fm(font);
    QFontMetrics fmSmall(fontSmall);
    QColor ordinaryColor(COLOR_ORDINARY_TEXT);
    QColor highlightedColor(COLOR_HIGHLIGHTED_TEXT);

    painter.begin(&image);

    painter.setFont(font);
    painter.setPen(ordinaryColor);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int answerY = 0;
    int answerX = 15;

    auto paintBase = [&](const QString& baseStr, int& otstupX, int y)
    {
        int numberSystemTextY = y + fmSmall.height() / 2;
        painter.setFont(fontSmall);
        painter.drawText(otstupX, numberSystemTextY, baseStr);
        painter.setFont(font);
        otstupX += fmSmall.horizontalAdvance(baseStr);
    };

    if(numberSystemFrom != numberSystemTo)
    {
        if(numberSystemFrom == NumberSystem::Ten)
        {
            int val = valStr.toInt();
            int countDigitsVal = getCountDigits(val);

            QString minus = "-";
            int minusWidth = fm.horizontalAdvance(minus);

            int divisiblePosX = answerX;
            //     answerX = divisiblePosX;
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

            answerY = arrowY + ARROW_CHANGE_Y*2 + fm.height();
        }
        else if(numberSystemTo == NumberSystem::Ten)
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;

            painter.drawText(textX, textY, startVal);
            textX += fm.horizontalAdvance(startVal);
            QString baseStr = QString::number(from);
            paintBase(baseStr, textX, textY);

            QString s = " = ";
            painter.drawText(textX, textY, s);

            textX += fm.horizontalAdvance(s);

            const QString mulAndDegreeStr = "*" + fromStr;
            const int mulAndBaseWidth =  fm.horizontalAdvance(mulAndDegreeStr);
            const int degreeY = textY - fmSmall.height() / 2;

            auto paintDegree = [&](int degree){
                painter.drawText(textX, textY, mulAndDegreeStr);
                textX += mulAndBaseWidth;
                painter.setFont(fontSmall);
                QString degreeStr = QString::number(degree);
                painter.drawText(textX, degreeY, degreeStr);
                painter.setFont(font);
                textX += fm.horizontalAdvance(degreeStr);
            };

            int degree = valStr.size() - 1;
            int baseInDegree = std::pow(from, degree);
            QString plusStr = "+";
            const int plusWidth = fm.horizontalAdvance(plusStr);
            s = " = ";
            while (true)
            {
                QString vStr = QString(valStr.at(0));
                int v = vStr.toInt(nullptr, from);

                if(numberSystemFrom == NumberSystem::Sixteen)
                {
                    vStr = QString::number(v, to);
                }

                painter.drawText(textX, textY, vStr);
                textX += fm.horizontalAdvance(vStr);
                paintDegree(degree);

                s += QString::number(v * baseInDegree);

                if(degree > 0)
                {
                    painter.drawText(textX, textY, plusStr);
                    textX += plusWidth;
                    s += plusStr;
                }
                else
                {
                    break;
                }
                valStr = valStr.right(1);
                baseInDegree /= from;
                --degree;
            }

            painter.drawText(textX, textY, s);

            if(s.contains(plusStr))
            {
                textX += fm.horizontalAdvance(s);
                painter.drawText(textX, textY, " = " + answer);
            }

            answerY = textY + fm.height()*2;

        }
    }

    painter.setPen(highlightedColor);
    QString s = "Ответ: " + startVal;
    painter.drawText(answerX, answerY, s);

    int otstup = answerX + fm.horizontalAdvance(s);

    paintBase(fromStr, otstup, answerY);

    s = " = " + answer;
    painter.setFont(font);
    painter.drawText(otstup, answerY, s);
    otstup += fm.horizontalAdvance(s);

    paintBase(toStr, otstup, answerY);

    painter.end();

    ui->label_2->setPixmap(QPixmap::fromImage(image));
}

