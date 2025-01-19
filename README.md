# Операционные системы

## Задания:

###  Практическое задание 1

- [x] Установить на компьютер Oracle VM VirtualBox, на нем настроить виртуальную машину с ОС Windows 10, если на основном комьютере установлены ОС семейств Linux или macOS.
- [x] На основной компьютер и виртуальную машину установить компилятор gcc (mingw на windows) и систему сборки CMake
- [x] На основной компьютер и виртуальную машину установить git и создать git-репозиторий для проекта ""Hello world!""на C++. 
- [x] Создать скрипты cmd (для Windows) и sh (для Linux), автоматизирующие обновление исходных кодов проекта с GIT-репозитория, сборку и компиляцию проекта.
- [x] Настроить любимый редактор кода (Notepad++, Geany, MS Visual Studio Code, Eclipse, Qt Creator...) для компиляции и отладки своего проекта с использованием отладчика gdb.

[Код работы](https://github.com/Ru6en11/Operationg-Systems/tree/main/Lab1)

### Практическое задание 2

Написать на языке C или C++ библиотеку, которая
- [x] содержит функцию для запуска программ в фоновом режиме, с возможностью подождать завершения работы дочерней программы и получить код ответа. 
- [x] Написать тестовую утилиту для проверки библиотеки. 

И библиотека и утилита должны быть кроссплатформенными (собираться и работать на ОС семейств Windows и UNIX (POSIX)

[Код работы](https://github.com/Ru6en11/Operationg-Systems/tree/main/Lab2)

### Практическое задание 3

Написать на C\C++ без использования сторонних библиотек кросплатформенную (POSIX, Windows) программу, которая:
- [x] При старте открывает лог-файл и пишет в него строку со своим идентификатором процесса и временем запуска.
- [x] Запускает таймер и раз в 300 мс увеличивает счетчик на 1
- [x] Позволяет пользователю через интерфейс командной строки установить любое значение счетчика
- [x] Раз в 1 секунду пишет в лог-файл текущее время (дата, часы, минуты, секунды, миллисекунды), свой идентификатор процесса и значение счетчика
- [x] Раз в 3 секунды запускает 2 своих копии:
- [x] Копия 1 записывает в тот же лог файл строку со своим идентификатором процесса, временем своего запуска, увеличивает текущее значение счетчика на 10, записывает в лог файл время выхода и завершается.
- [x] Копия 2 записывает в тот же лог файл строку со своим идентификатором процесса, временем своего запуска, увеличивает текущее значение счетчика в 2 раза, через 2 секунды уменьшает значение счетчика в 2 раза, записывает в лог файл время выхода и завершается
- [x] Если какая-либо копия к следующему циклу запуска еще не завершила свою работу, программа не запускает копию, а пишет в лог сообщение.
- [x] Пользователь может запустить любое количество программ. В этом случае только одна из программ должна писать в лог текущее значение счетчика и порождать копии, но все запущенные программы должны модифицировать счетчик раз в 300 мс и по запросу пользователя.

[Код работы](https://github.com/Ru6en11/Operationg-Systems/tree/main/Lab3)

### Практическое задание 4

Есть устройство, которое публикует по серийному порту или интерфейсу USB текущую температуру окружающей среды. Необходимо написать на C\C++ кроссплатформенную программу, которая 
- [x] считывает информацию с порта и записывает ее в лог-файл. 
- [x] Каждый час программа считает среднюю температуру за час и записывает ее в другой лог-файл. 
- [x] Средняя температура за день записывается в 3й лог-файл. 
- [x] Лог файл со всеми измерениями должен хранить только измерения за последние 24 часа
- [x] лог-файл со средней температурой за час должен хранить только измерения за последний месяц, - [x] лог файл со средней дневной температурой 

- [x] симулировать работу устройства с помощью специально созданной вами программы.

[Код работы](https://github.com/Ru6en11/Operationg-Systems/tree/main/Lab4)