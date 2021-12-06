#include "modelearningwidget.h"
#include "ui_modelearningwidget.h"
#include <QPainter>
#include <QPicture>
#include <QDebug>
#include <cmath>
#include "numbersysteminputvalidator.h"
#include <unordered_map>
#include <algorithm>


#define COLOR_ORDINARY_TEXT      "#474d47"
#define COLOR_HIGHLIGHTED_TEXT   "red"
#define COLOR_EXPLANATION        "#0660e7"

#define ARROW_PLUS_X 10
#define ARROW_CHANGE_Y 5

using MapForTranslate = std::unordered_map<QString, QString>;

MapForTranslate mapTranslateFrom8To2;
MapForTranslate mapTranslateFrom2To8;
MapForTranslate mapTranslateFrom16To2;
MapForTranslate mapTranslateFrom2To16;

void initMapsForTranslate()
{
    QVector<std::pair<QString, QString>> initCont = {
        {"0", "000"},
        {"1", "001"},
        {"2", "010"},
        {"3", "011"},
        {"4", "100"},
        {"5", "101"},
        {"6", "110"},
        {"7", "111"}
    };


    for(auto& el: initCont)
    {
        mapTranslateFrom8To2.insert(el);
        std::swap(el.first, el.second);
        mapTranslateFrom2To8.insert(el);
    }

    initCont = {
        {"0", "0000"},
        {"1", "0001"},
        {"2", "0010"},
        {"3", "0011"},
        {"4", "0100"},
        {"5", "0101"},
        {"6", "0110"},
        {"7", "0111"},
        {"8", "1000"},
        {"9", "1001"},
        {"a", "1010"},
        {"b", "1011"},
        {"c", "1100"},
        {"d", "1101"},
        {"e", "1110"},
        {"f", "1111"}
    };

    for(auto& el: initCont)
    {
        mapTranslateFrom16To2.insert(el);
        std::swap(el.first, el.second);
        mapTranslateFrom2To16.insert(el);
    }
}





ModeLearningWidget::ModeLearningWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeLearningWidget)
{
    ui->setupUi(this);

    ui->numberLineEdit->setValidator(MyValidator);

    init();

    initMapsForTranslate();

    connect(ui->translateBtn, &QPushButton::clicked, this, &ModeLearningWidget::slot_translate);
    connect(ui->numberSystemFromCmbx, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ModeLearningWidget::slot_fromNumberSystemChanged);
}

ModeLearningWidget::~ModeLearningWidget()
{
    delete ui;
}

void ModeLearningWidget::drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo)
{
    if(numberSystemFrom == NumberSystem::Sixteen)
    {
        valStr = valStr.toLower();
    }

    QString startVal = valStr;
    const QString answer = translateNumber(valStr, numberSystemFrom, numberSystemTo);
    const int from = valueOfNumberSystem(numberSystemFrom);
    const int to = valueOfNumberSystem(numberSystemTo);
    QString toStr = QString::number(to);
    QString fromStr = QString::number(from);

    QImage image(QSize(1000, 1000), QImage::Format_ARGB32);

    static QPainter painter;
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

    int answerY = fm.height();
    int answerX = 15;

    auto paintBase = [&](const QString& baseStr, int& otstupX, int y)
    {
        int numberSystemTextY = y + fmSmall.height() / 2;
        painter.setFont(fontSmall);
        painter.drawText(otstupX, numberSystemTextY, baseStr);
        painter.setFont(font);
        otstupX += fmSmall.horizontalAdvance(baseStr);
    };

    auto paintTranslatingTo2 = [&](int textX, int textY, const QString& answer, const QString& baseAnswer, const QString& val)
    {
        painter.drawText(textX, textY, startVal);
        textX += fm.horizontalAdvance(startVal);
        paintBase(fromStr, textX, textY);

        QString s = " = ";
        painter.drawText(textX, textY, s);

        textX += fm.horizontalAdvance(s);

        int i = 0;
        int countNum = val.size();

        s = "";

        MapForTranslate& mapToTranslateTo2 = (from == 8? mapTranslateFrom8To2 : mapTranslateFrom16To2);

        while(i != countNum)
        {
            s += mapToTranslateTo2[val.at(i++)] + " ";
        }

        painter.drawText(textX, textY, s);
        textX += fm.horizontalAdvance(s);

        s = "= " + answer;
        painter.drawText(textX, textY, s);
        textX += fm.horizontalAdvance(s);

        paintBase(baseAnswer, textX, textY);

        return translateNumber(val, numberSystemFrom, NumberSystem::Two);
    };

    auto paintTranslateFrom2 = [&](int textX, int textY){
        painter.drawText(textX, textY, valStr);
        textX += fm.horizontalAdvance(valStr);
        QString numberSystem2Str =  QString::number(2);
        paintBase(numberSystem2Str, textX, textY);

        QString s = "";

        int countDigitsForTranslate = log2(to);
        int count = valStr.size() / countDigitsForTranslate;
        int leftOst = valStr.size() % countDigitsForTranslate;

        qDebug() << "count = " << count << " ; leftOst = " << leftOst;

        MapForTranslate& mapTranslate = (from == 8 || (from == 2 && to == 16))? mapTranslateFrom2To16 : mapTranslateFrom2To8;

        QString s2 = "";
        QString tmp;
        while (count--)
        {
            tmp = valStr.right(countDigitsForTranslate);
            s = tmp + " " + s;
            s2 = mapTranslate[tmp] + " " + s2;
        }

        if(leftOst)
        {
            tmp = valStr.left(leftOst);
            tmp.insert(0, QString(countDigitsForTranslate - leftOst, '0'));
            s = tmp + " " + s;
            s2 = mapTranslate[tmp] + " " + s2;
        }

        s = " = " + s + "= " + s2 + "= " + answer;
        painter.drawText(textX, textY, s);
        textX += fm.horizontalAdvance(s);

        paintBase(toStr, textX, textY);
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
        else if(numberSystemTo == NumberSystem::Two)
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;

            paintTranslatingTo2(textX, textY, answer, toStr, valStr);

            answerY = textY + fm.height() * 2;
        }
        else if(numberSystemFrom == NumberSystem::Two)
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;
            paintTranslateFrom2(textX, textY);
            answerY = textY + fm.height() * 2;
        }
        else
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;

            QColor colorExplanation(COLOR_EXPLANATION);

            auto paintExplanation = [&](const QString& explanationStr)
            {
                painter.setPen(colorExplanation);
                painter.drawText(textX, textY, explanationStr);
                textY += fm.height() * 1.3;
                painter.setPen(ordinaryColor);
            };

            paintExplanation("Сначала переводим в 2-ичную систему:");

            QString answer2 = translateNumber(valStr, numberSystemFrom, NumberSystem::Two);
            QString numberSystem2Str =  QString::number(2);
            valStr = paintTranslatingTo2(textX, textY, answer2, numberSystem2Str, valStr);
            textY += fm.height() * 1.5;

            paintExplanation(QString("Теперь переводим в %1-ичную систему:").arg(toStr));

            paintTranslateFrom2(textX, textY);

            answerY = textY + fm.height() * 2;
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

    ui->algorithmLbl->setPixmap(QPixmap::fromImage(image));
}

void ModeLearningWidget::slot_translate()
{
    NumberSystem from = NumberSystem(ui->numberSystemFromCmbx->currentIndex());
    NumberSystem to = NumberSystem(ui->numberSystemToCmbx->currentIndex());
    QString valueStr = ui->numberLineEdit->text();

    if(valueStr.isEmpty())
    {
        valueStr = "0";
    }

    drawTranslateAlgorithm(std::move(valueStr), from, to);
}

void ModeLearningWidget::slot_fromNumberSystemChanged(int index)
{
    fixInput(NumberSystem(index));
}

