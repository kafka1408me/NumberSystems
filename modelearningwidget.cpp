#include "modelearningwidget.h"
#include "ui_modelearningwidget.h"
#include <QPainter>
//#include <QDebug>
#include <cmath>
#include "numbersysteminputvalidator.h"
#include <unordered_map>
// #include <algorithm>

// Строковые значения цветов, используемых для рисования
#define COLOR_ORDINARY_TEXT      "#474d47"
#define COLOR_HIGHLIGHTED_TEXT   "red"
#define COLOR_EXPLANATION        "#0660e7"

// Отсутпы при рисовании красной стрелочки
#define ARROW_PLUS_X 10
#define ARROW_CHANGE_Y 5

// Тип контейнеров, содержащих строки соответсвия
// значения из одной системы счисления другой
// std::unordered_map - хэш-словарь
using MapForTranslate = std::unordered_map<QString, QString>;

MapForTranslate mapTranslateFrom8To2;   // Сопостовляет значению из 8-ичной системы значение из 2-ой
MapForTranslate mapTranslateFrom2To8;   // Сопостовляет значению из 2-ичной системы значение из 8-ой
MapForTranslate mapTranslateFrom16To2;  // Сопостовляет значению из 16-ичной системы значение из 2-ой
MapForTranslate mapTranslateFrom2To16;  // Сопостовляет значению из 2-ичной системы значение из 8-ой

// Функция для инициализации контейнеров
// сопоставления строковых значений одной СС другой СС
void initMapsForTranslate()
{
    // Из 8-ричной в 2-ую
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
        std::swap(el.first, el.second);  // Меняем местами
        mapTranslateFrom2To8.insert(el);
    }

    // Из 16-ой в 2-ую
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
        std::swap(el.first, el.second);     // Меняем местами
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

    // Инициализируем контейнеры сопоставления чисел разных систем счисления
    initMapsForTranslate();

    connect(ui->translateBtn, &QPushButton::clicked, this, &ModeLearningWidget::slot_translate);
    // Соединяем сигнал изменения текущей системы счисления со слотом-обработчиком
    connect(ui->numberSystemFromCmbx, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ModeLearningWidget::slot_fromNumberSystemChanged);
}

ModeLearningWidget::~ModeLearningWidget()
{
    delete ui;
}

void ModeLearningWidget::drawTranslateAlgorithm(QString valStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo)
{
    // Если переводим из 16-й системы, преобразуем строку в нижний регистр
    if(numberSystemFrom == NumberSystem::Sixteen)
    {
        valStr = valStr.toLower();
    }

    QString startVal = valStr;    // Запоминаем строку, содержащее число
    const QString answer = translateNumber(valStr, numberSystemFrom, numberSystemTo);  // Переведенное в нужную СС число
    const int from = valueOfNumberSystem(numberSystemFrom);    // Из какой системы переводим
    const int to = valueOfNumberSystem(numberSystemTo);        // В какую систему переводим
    QString toStr = numberSystems.at(int(numberSystemTo));     // В какую систему переводим (строка)
    QString fromStr = numberSystems.at(int(numberSystemFrom)); // Из какой переводим (строка)

    // Создаем изображение, чтобы нарисовать на нем алгоритм перевода числа в систему счисления
    QImage image(QSize(1000, 1000), QImage::Format_ARGB32);

    QPainter painter;                 // Объект, осуществляющий рисование
    QFont font("Courier", 14);        // Шрифт для рисования
    QFont fontSmall("Courier", 8);    // Шрифт для рисования меньших деталей (системы счисления и степени чисел)
    // QFontMetrics дает информацию о шрифте
    QFontMetrics fm(font);
    QFontMetrics fmSmall(fontSmall);
    // Цвета для рисования
    QColor ordinaryColor(COLOR_ORDINARY_TEXT);
    QColor highlightedColor(COLOR_HIGHLIGHTED_TEXT);

    // Начинаем рисование
    // В метод begin передаем указатель на контекст рисования
    painter.begin(&image);

    painter.setFont(font);
    painter.setPen(ordinaryColor);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Значения x и y, где будем рисовать результат перевода
    int answerY = fm.height();
    int answerX = 15;

    // Лямбда-функция для рисования основания системы счисления
    // baseStr - строка, содержащее число - систему счисления;
    // otstupX - координата x для рисования;
    // y - координата основного текста
    auto paintBase = [&](const QString& baseStr, int& otstupX, int y)
    {
        int numberSystemTextY = y + fmSmall.height() / 2;      // y-координата основания системы счисления
        painter.setFont(fontSmall);                            // Устанавливаем меньший шрифт
        painter.drawText(otstupX, numberSystemTextY, baseStr);
        painter.setFont(font);                                 // Возвращаем обычный шрифт
        otstupX += fmSmall.horizontalAdvance(baseStr);         // Увеличиваем координату x на ширину основания СС
    };

    // Лямбда-функция для рисования алгоритма перевода в 2-ую систему счисления
    // textX - координата x, с которой нужно начать рисовать;
    // textY - координата y для текста;
    // val - число, которое нужно перевести
    auto paintTranslatingTo2 = [&](int textX, int textY, const QString& val)
    {
        painter.drawText(textX, textY, val);  // Сначала рисуем изначальное число
        textX += fm.horizontalAdvance(val);   // Увеличиваем координату рисования x на ширину нарисованного числа
        paintBase(fromStr, textX, textY);     // Рисуем систему счисления, из которой переводим

        // Рисуем равно
        QString s = " = ";
        painter.drawText(textX, textY, s);
        textX += fm.horizontalAdvance(s);

        int i = 0;                  // Индекс текущей цифры в числе
        int countNum = val.size();  // Количество цифр в числе

        s = "";

        // Запоминаем ссылку на контейнер, который будет использоваться
        // для сопоставления 8-ых или 16-ых чисел и 2-ых
        MapForTranslate& mapToTranslateTo2 = (from == 8? mapTranslateFrom8To2 : mapTranslateFrom16To2);

        // Пока не пройдем все цифры в числе
        while(i != countNum)
        {
            // Добавляем к строке 2-ое представление числа
            s += mapToTranslateTo2[val.at(i++)] + " ";
        }

        painter.drawText(textX, textY, s);
        textX += fm.horizontalAdvance(s);

        QString answer2 = translateNumber(val, numberSystemFrom, NumberSystem::Two);

        // Рисуем равно
        s = "= " + answer2;
        painter.drawText(textX, textY, s);
        textX += fm.horizontalAdvance(s);

        // Рисуем 2-ую систему счисления для результирующей записи числа
        paintBase(QString::number(2), textX, textY);

        return answer2;
    };

    // Лямбда-функция для рисования алгоритма перевода из двоичной системы счисления
    // (в 16-ую или 8-ую)
    // textX - начальная x-координата текста;
    // textY - y-координата текста
    auto paintTranslateFrom2 = [&](int textX, int textY){
        painter.drawText(textX, textY, valStr);          // Рисуем изначальное число
        textX += fm.horizontalAdvance(valStr);
        // Рисуем 2-ую систему счисления
        QString numberSystem2Str =  QString::number(2);
        paintBase(numberSystem2Str, textX, textY);

        QString s = "";

        // Количество двоичных цифр в наборе для перевода
        // для 8-й системе 3 цифры; 16-й 4 фицры
        int countDigitsForTranslate = log2(to);
        int count = valStr.size() / countDigitsForTranslate;    // Количество целых наборов
        int leftOst = valStr.size() % countDigitsForTranslate;  // Количество цифр, не вошедших в набор

//        qDebug() << "count = " << count << " ; leftOst = " << leftOst;

        // Запоминаем ссылку на контейнер, который будет испозоваться для
        // сопоставления набору доичных цифр цифры другой системы счисления
        MapForTranslate& mapTranslate = (from == 8 || (from == 2 && to == 16))? mapTranslateFrom2To16 : mapTranslateFrom2To8;

        QString s2 = "";
        QString tmp;
        // Делим двоичные цифры на наборы (по 3 или 4 цифры)
        // и этому набору сопоставляем 8-ую или 16-ую цифру
        while (count--)
        {
            tmp = valStr.right(countDigitsForTranslate);     // Берем набор цифр справа
            s = tmp + " " + s;
            s2 = mapTranslate[tmp] + " " + s2;                // Записываем соответствующую двоичному набору цифру
            valStr = valStr.chopped(countDigitsForTranslate); // Отбрасываем от строки учтенный набор двоичных цифр
        }

        // Если есть двоичные цифры, не вошедшие в набо
        if(leftOst)
        {
            tmp = valStr;
            // Дополняем оставшиеся двоичные цифры до набора
            tmp.insert(0, QString(countDigitsForTranslate - leftOst, '0'));
            s = tmp + " " + s;
            s2 = mapTranslate[tmp] + " " + s2;
        }

        s = " = " + s + "= " + s2 + "= " + answer;
        painter.drawText(textX, textY, s);
        textX += fm.horizontalAdvance(s);

        // Рисуем систему счисления, в которуюю перевели число
        paintBase(toStr, textX, textY);
    };

    // Если системы счисления, из которой переводим и в которую переводим не одинаковы
    if(numberSystemFrom != numberSystemTo)
    {
        // Перевод из 10-й системы счисления
        if(numberSystemFrom == NumberSystem::Ten)
        {
            int val = valStr.toInt();                  // Десятичное представление числа
            int countDigitsVal = getCountDigits(val);  // Количество цифр в числе

            QString minus = "-";
            int minusWidth = fm.horizontalAdvance(minus);

            // x и y координаты делимого
            int divisiblePosX = answerX;
            int divisiblePosY = fm.height() + 1;

            while(true)
            {
                int res = val / to;            // Результат деления на основание СС, в которую переводим

                int deductible = res*to;       // Вычитаемое

                int rest = val - deductible;   // Остаток от деления

                // Количество цифр в вычитаемом
                int countDigitsDeductible = getCountDigits(deductible);

                // Вставляем в строку такое количество пробелов,
                // чтобы правый край вычитаемого числа был вровень с правым краем делимого
                QString deductibleValStr(countDigitsVal-countDigitsDeductible, QChar(' '));
                deductibleValStr += QString::number(deductible);

                QString restStr = QString::number(rest, to);
                int countDigitsRest = countDigitsVal-restStr.size();
                if(countDigitsRest)
                {
                    // Вставляем в строку такое количество пробелов,
                    // чтобы правый край остатка был вровень с правым краем делимого
                    restStr.insert(0, QString(countDigitsRest, ' '));
                }

                // Ширина делимого
                int widthStr = fm.horizontalAdvance(valStr);

                // Рисуем минус
                painter.drawText(divisiblePosX - minusWidth, divisiblePosY + fm.height() / 2, minus);

                // Рисуем делимое
                painter.drawText(divisiblePosX, divisiblePosY, valStr);
                int deductibleY = divisiblePosY + fm.height();
                // Рисуем вычитаемое
                painter.drawText(divisiblePosX, deductibleY, deductibleValStr);
                int horLineUnderDeductibleY = deductibleY + 2;
                // Рисуем линию под вычитаемым
                painter.drawLine(divisiblePosX, horLineUnderDeductibleY, divisiblePosX + widthStr, horLineUnderDeductibleY);

                // Рисуем остаток (выделенным цветом)
                painter.setPen(highlightedColor);
                painter.drawText(divisiblePosX, horLineUnderDeductibleY + fm.height() - 1, restStr);
                painter.setPen(ordinaryColor);

                int deviderX = divisiblePosX + widthStr + 5;
                // Рисуем делитель
                painter.drawText(deviderX, divisiblePosY, toStr);
                int xLine = divisiblePosX + widthStr + 1;
                int yLine1 = divisiblePosY - fm.height();
                int yLine2 = divisiblePosY + fm.height()*2 + 3;

                // Рисуем линию, делящую делимое и делитель
                painter.drawLine(xLine, yLine1, xLine, yLine2);
                // Рисуем линию под делителем
                painter.drawLine(xLine + 2, yLine1 + fm.height() + 2, xLine + widthStr + 4, yLine1 + fm.height() + 2);

                divisiblePosX = deviderX + minusWidth;
                divisiblePosY = divisiblePosY + fm.height();
                val = res;

                valStr = QString::number(val);
                // Если число стало меньше основания системы счисления,
                // то заканчиваем рисование деления столбиками
                if(val < to)
                {
                    painter.setPen(highlightedColor);
                    painter.drawText(divisiblePosX, divisiblePosY, valStr);
                    break;
                }

                countDigitsVal = getCountDigits(val);
            }

            // Рисуем стрелочку (выделенным цветом)
            painter.setPen(highlightedColor);
            int arrowX1 = 10 + minusWidth;
            int arrowX2 = divisiblePosX + fm.horizontalAdvance(val) + 2;
            int arrowY = divisiblePosY + fm.height() * 2.5;
            // Рисуем /
            painter.drawLine(arrowX1, arrowY, arrowX1 + ARROW_PLUS_X, arrowY - ARROW_CHANGE_Y);
            // Рисуем -
            painter.drawLine(arrowX1, arrowY, arrowX2, arrowY);
            // Рисуем \линию
            painter.drawLine(arrowX1, arrowY, arrowX1 + ARROW_PLUS_X, arrowY + ARROW_CHANGE_Y);

            answerY = arrowY + ARROW_CHANGE_Y*2 + fm.height();
        }
        else if(numberSystemTo == NumberSystem::Ten)  // Перевод в 10-ую систему счисления
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;

            painter.drawText(textX, textY, startVal);  // Рисуем начальное значение
            textX += fm.horizontalAdvance(startVal);
            paintBase(fromStr, textX, textY);          // Рисуем основание системы счисления, откуда переводим

            QString s = " = ";
            painter.drawText(textX, textY, s);

            textX += fm.horizontalAdvance(s);

            const QString mulAndDegreeStr = "*" + fromStr;  // Строка "*основание_системы_счисления"; Если СС=2, то "*2"
            const int mulAndBaseWidth =  fm.horizontalAdvance(mulAndDegreeStr);
            const int degreeY = textY - fmSmall.height() / 2;  // y-координата текста степени

            // Лямбда-функция для рисования степени числа
            // degree - степень
            auto paintDegree = [&](int degree){
                painter.drawText(textX, textY, mulAndDegreeStr);  // Рисуем знак * и основание СС, откуда переводим
                textX += mulAndBaseWidth;                         // Увеличиваем x-координату на ширину нарисованного текста "*основание_системы_счисления"
                QString degreeStr = QString::number(degree);      // Строка, содержащая степень
                painter.setFont(fontSmall);                       // Установка малого шрифта
                painter.drawText(textX, degreeY, degreeStr);      // Рисуем степень
                painter.setFont(font);                            // Восстановление обычного шрифта
                textX += fm.horizontalAdvance(degreeStr);
            };

            int degree = valStr.size() - 1;              // Степень числа
            int baseInDegree = std::pow(from, degree);   // Основание СС в степени
            QString plusStr = "+";
            const int plusWidth = fm.horizontalAdvance(plusStr);
            s = " = ";
            int indx = 0;
            // В цикле перебираем разряды числа
            // и умножаем на основание СС в степени
            while (true)
            {
                QString vStr = QString(valStr.at(indx++));
                int v = vStr.toInt(nullptr, from);   // Цифра в текущем разряде

                // Если переводим из 16-ой системы и цифра больше 9,
                // то строку нужно преобразовать к десятичном представлению:
                // чтобы затем печатать 10 вместо a, 11 вместо b и т.д.
                if(numberSystemFrom == NumberSystem::Sixteen && v > 9)
                {
                    vStr = QString::number(v, to);
                }

                // Рисуем цифру (в десятичном представлении)
                painter.drawText(textX, textY, vStr);
                textX += fm.horizontalAdvance(vStr);
                paintDegree(degree);

                // В s записываем слагаемые, являющиеся результатом умножения цифры в разряде
                // на основание СС в степени
                s += QString::number(v * baseInDegree);

                // Если это не самый малый разряд (разряд единиц), тогда
                // рисуем плюс
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
                baseInDegree /= from; // Делим на основание системы счисления
                --degree;             // Уменьшаем степень
            }

            // Рисуем строку, содержащую слагаемые без степеней
            painter.drawText(textX, textY, s);

            // Если в строке больше, чем одно слагаемое, то
            // рисуем равно и окончательный ответ
            if(s.contains(plusStr))
            {
                textX += fm.horizontalAdvance(s);
                painter.drawText(textX, textY, " = " + answer);
            }

            answerY = textY + fm.height()*2;

        }
        else if(numberSystemTo == NumberSystem::Two)  // Перевод в 2-ую систему счисления (из 8 или 16)
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;
            paintTranslatingTo2(textX, textY, valStr);
            answerY = textY + fm.height() * 2;
        }
        else if(numberSystemFrom == NumberSystem::Two)  // Перевод из 2-й системы счисления (в 8 или 16)
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;
            paintTranslateFrom2(textX, textY);
            answerY = textY + fm.height() * 2;
        }
        else  // Перед из 8-й в 16-ую и наоборот
        {
            int textX = answerX;
            int textY = fm.height() * 1.8;

            // Цвет для рисования поясняющего текста
            QColor colorExplanation(COLOR_EXPLANATION);

            // Лямбда-функция для рисования поясняющего текста
            // explanationStr - поясняющий текст
            auto paintExplanation = [&](const QString& explanationStr)
            {
                painter.setPen(colorExplanation);
                painter.drawText(textX, textY, explanationStr);
                textY += fm.height() * 1.3;
                painter.setPen(ordinaryColor);
            };

            paintExplanation("Сначала переводим в 2-ичную систему:");

            valStr = paintTranslatingTo2(textX, textY, valStr);
            textY += fm.height() * 1.5;

            paintExplanation(QString("Теперь переводим в %1-ичную систему:").arg(toStr));

            paintTranslateFrom2(textX, textY);

            answerY = textY + fm.height() * 2;
        }
    }

    // Рисуем окончательный ответ
    painter.setPen(highlightedColor);
    QString s = "Ответ: " + startVal;
    painter.drawText(answerX, answerY, s);

    int otstup = answerX + fm.horizontalAdvance(s);

    // Рисуем СС, откуда переводили
    paintBase(fromStr, otstup, answerY);

    s = " = " + answer;
    painter.setFont(font);
    painter.drawText(otstup, answerY, s);
    otstup += fm.horizontalAdvance(s);

    // Рисуем СС, куда переводили
    paintBase(toStr, otstup, answerY);

    painter.end();  // Завершаем рисование

    // Устанавливаем в QLabel нарисованный алгоритм как картинку
    ui->algorithmLbl->setPixmap(QPixmap::fromImage(image));
}

void ModeLearningWidget::preparePage()
{
    setCurrentValidator();
}

void ModeLearningWidget::slot_translate()
{
    NumberSystem from = NumberSystem(ui->numberSystemFromCmbx->currentIndex());
    NumberSystem to = NumberSystem(ui->numberSystemToCmbx->currentIndex());
    QString valueStr = ui->numberLineEdit->text();

    // Если пользователь не заполнил поле ввода, то
    // считаем, что нужно перевести число 0
    if(valueStr.isEmpty())
    {
        valueStr = "0";
    }

    // std::move перемещает объект, избегая копирования
    drawTranslateAlgorithm(std::move(valueStr), from, to);
}

void ModeLearningWidget::slot_fromNumberSystemChanged(int index)
{
    // Исправляем пользовательский ввод в соответсвии с новой системой счисления
    fixInput(NumberSystem(index));
}

