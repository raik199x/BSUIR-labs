from Sources.Ui.frontend.MainWindow import ShowWindow

"""
ЧАСТЬ 1
Написать программу, выполняющую следующие операции над изображениями (обязательна обработка как цветных, так и полутоновых изображений):
    1. Загрузить и отобразить изображение на экране. Реализовать возможность сохранения и вывода на экран обработанного изображения.
    2. Вывести гистограмму яркости изображения.
    3. Выполнить поэлементную обработку изображения в соответствии с вариантом, выданным преподавателем.
    Вывести гистограмму яркости обработанного изображения.
    4. Выполнить фильтрацию изображения (либо иной пространственный процесс) в соответствии с вариантом,
    выданным преподавателем. Вывести гистограмму яркости обработанного изображения.

ЧАСТЬ 2
Выбрать одну из своих фотографий (изображение студента на данной фотографии приветствуется),
которая требует определенной обработки (например, изменения контраста, яркости и т.д.),
выполнить данную обработку. Показать преподавателю исходную фотографию, результат,
обосновать необходимость определенной обработки, пояснить алгоритм. Можно использовать один из алгоритмов,
приведенный в методичке, либо иной, но в рамках тематики лабораторной работы

Вариант Поэлементная обработка                          Примечание
6       Препарирование изображения (рис. 1.3, д, е)     При реализации интерфейса предусмотреть возможность ввода параметров:
                                                        рис. 1.3, д, е - gmin и gmax ;  fmin и fmax

Вариант Фильтрация, пространственные процессы           Примечание
6       Min-фильтр, max-фильтр, min-max-фильтр          -
"""

if __name__ == "__main__":
    ShowWindow()