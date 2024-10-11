# 1С отбор на кафдеру

ФИО: Аминов Шахром Илхомджонович </br>
Группа: Б05-252 </br>
Почта: [aminoff.sh@gmail.com](mailto:aminoff.sh@gmail.com) </br>
Номер решаемой задачи: 2 (Алгоритмы)

## Как запустить проект?
* Скомпилировать </br>
  `g++ main.cpp -o main.exe`
* Запустить </br>
  `./main`
## Как работать с программой?
* Запросы вида "Добавь текст _Zdraviya zhelayu_" вводятся так: (текст в новой строке)
```
1
Zdraviya zhelayu
```
* Запросы вида "Ищи слово начинающееся _Slov_" вводятся так:
```
2
Slov
```
* Запросы вида "Припиши к последнему слову которое искали, букву _c_" вводятся так: (до этого должен быть хотя бы один запрос второго типа)
```
3
c
```
* Чтобы остановить программу, напишите: 
```
0
```
* При попытке ввести другого типа запрос, будет выведено **Please enter a valid option.**
## Основная идея алгоритма
* Воспользуемся структурой данных бор.
* Пусть мы в вершине бора с префиксом **cur**, будем хранить индекс строки начинающиеся на **cur** с максимальным вхождением, назовём её **max_index**
* При добавлении текста, делим его на строки с разделителем "пробел" и добавляем их в бор
* При поиске слова, мы проходимся вглубь бора, пытаясь искать нужный префикс. 
Если находим, то возвращаем вершину бора. Иначе возвращаем **nullptr**. 
Воспользуясь **max_index** выведем искомое слово, если такого слова не было выводем его обратно. В **last** сохраним последнюю вершину бора в которой мы были.
* При добавлении буквы, попробуем перейти по ней из **last**. Действия аналогичные предыдущему пункту.
### Детали алгоритма
* Асимптотика первого вида запросов: `O(длина текста)` по времени и `O(длина текста)` по памяти
* Асимптотика второго вида запросов: `O("длина префикса" + "длина слова которое мы найдём")` по времени и `O(1)` по памяти
* Асимптотика третьего вида запросов: `O("длина слова которое мы найдём")` по времени и `O(1)` по памяти
* Храню три `std::unordered_map`: </br>
`std::unordered_map<int64_t, int64_t> count` - для подсчёта количества вхождений слова по её хэшу
`std::unordered_map<int64_t, int64_t> hashs` - для хранения хэша слова по её индексу
`std::unordered_map<std::string, int64_t> index` - для хранения индекса по строке
* Использую хэш вместо самой строки, чтобы каждый раз заново не высчитывать её (при каждом поиске по мапе и т.п.)
* Использую `std::shared_ptr<Node>` вместо `Node*`, чтобы обезопасить программу от memory leak
