#pragma once

// В этом файле собраны функции и переменные,
// используемые другими модулями

#include <QStringList>  // QStringList - список строк
#include <chrono>       // Для работы со временем (стандартная библиотека С++)

// Тип часов
using ClockType = std::chrono::system_clock;
// Тип временной точки
using TimePoint = std::chrono::time_point<ClockType>;

/**
 * @brief The NumberSystem enum - перечисление доступных систем счисления
 */
enum NumberSystem
{
    Two     = 0,   // 2-ичная система
    Eight   = 1,   // 8-ричная система
    Ten     = 2,   // 10-тичная система
    Sixteen = 3    // 16-ричная система
};

// Список доступных систем счисления
// переменная определена в cpp-файле.
// Ключевое слово extern указывает на то, что
// переменная определена в другом месте.
// Если бы extern не было переменная создавалась при
// каждом include этого заголовочного файла, а это ошибка,
// поскольку определение должно быть только одно.
extern const QStringList numberSystems;

/**
 * @brief valueOfNumberSystem - функция для получения десятичного значения системы счисления
 * @param ns - значение перечисления системы счисления
 */
unsigned valueOfNumberSystem(NumberSystem ns);

/**
 * @brief translateNumber - функция для перевода числа из одной системы в другую
 * @param fromStr - строка, содержащее число в изначальной системе счисления
 * @param numberSystemFrom - система, из которой нужно перевести
 * @param numberSystemTo - система, в которую нужно перевести
 * @return строку с числом в новой системе счисления
 */
QString translateNumber(const QString& fromStr, NumberSystem numberSystemFrom, NumberSystem numberSystemTo);

/**
 * @brief generateRandomNumber - сгенерировать случайное число из диапазона
 * @param min - минимальное возможное значение числа
 * @param max - максимальное возможное значение числа
 */
unsigned generateRandomNumber(unsigned min, unsigned max);

/**
 * @brief generateRandomNumberSystem - сгенерировать случайным образом одну из доступных
 * систем счисления
 * @param without - индекс системы, которую не нужно генерировать; -1, если
 * следует генерировать любую из доступных систем счисления
 * @return случайную систему счисления
 */
NumberSystem generateRandomNumberSystem(int without = -1);

/**
 * @brief getCountDigits - функция для получения количества цифр в десятичном числе
 * @param val - число
 * @return количество цифр в числе
 */
int getCountDigits(int val);

/**
 * @brief addColorToString - функция для добавления цвета тексту
 * с помощью тега html
 * @param color - цвет
 * @param text - текст, к которому нужно добавить цвет
 * @return текст со цветом
 */
QString addColorToString(const QString& color,const QString &text);

/**
 * @brief getTimeStrFromSec - функция для конвертации количества секунд в строку времени
 * @param sec - кол-во секунд
 * @return строку времени
 */
QString getTimeStrFromSec(quint64 sec);

/**
 * @brief getCurrentTime - функция для получения текущего времени
 * @return текущее время
 */
TimePoint getCurrentTime();

/**
 * @brief getDiffTime - функция для нахождения разницы между
 * текущем временем и временем, переданным в качестве аргумента
 * @param oldTime
 * @return
 */
quint64 getDiffTime(const TimePoint& oldTime);
