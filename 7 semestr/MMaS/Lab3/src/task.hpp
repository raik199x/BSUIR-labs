#pragma once

/**
 * @brief Общее задание
 * 
 * В соответствии с вариантом написать программу, которая по нажатию одной
 * кнопки последовательно переключает заданные тактовые частоты и напряжение
 * питания, второй — входит и выходит в заданный режим пониженного
 * энергопотребления. Для индикации активного режима использовать LED1, режима
 * пониженного энергопотребления — LED3. Для индикации высокого напряжения
 * питания использовать LED4, низкого — LED5. Дополнительно тактовую частоту
 * подать на таймер, прерывание от таймера управляет LED8, и подобрать режим
 * таймера так, чтобы наглядно было видно изменение частоты. При написании кода
 * максимально использовать принципы разработки приложения для пониженного
 * энергопотребления. Обязательно соблюдать условие соответствия частоты и
 * напряжения, а также пошагового переключения режимов питания.
 */

/**
 * @brief Задание по варианту
 * 
 * |------------|-----|-----------|--------------------------------------------|
 * | № варианта | LPM | V_core, В | Частота                                    |
 * |------------|-----|-----------|--------------------------------------------|
 * | 15         | 4   | 1,9 — 1,4 | DCOCLK = 0,9 МГц, ACLK – смена источника   |
 * |            |     |           | DCOCLK / VLOCLK                            |
 * |------------|-----|-----------|--------------------------------------------|
 */