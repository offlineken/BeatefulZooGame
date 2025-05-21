# ZooSIMS — Симулятор управления зоопарком

## Зачем?
Целью проекта было закрепить знания по ООП (объектно-ориентированному программированию), научиться проектировать сложные системы из объектов, использовать STL-контейнеры, указатели, работу с памятью и реализовать игровой цикл. Проект демонстрирует умение моделировать реальные процессы.

## Почему?
Потому что преподаватель так сказал сделать 

---

## 🛠 Технологии и инструменты

- **C++17** — основной язык программирования.
- **STL (Standard Template Library)** — использовались `vector`, `map`, `unordered_map`, `set`, `shared_ptr`, `unique_ptr`.
- **Windows API (частично)** — для поддержки юникода в консоли (`_setmode`).
- **Git + GitHub** — система контроля версий и хостинг проекта.

---

## 📁 Структура проекта
├── ZooSIMS.cpp # Основной файл программы

├── README.md # Этот файл

└── .gitignore # Файл исключений для Git

---

📚 Документация: 
```bash
Animal* findAnimal(int id)
Возвращает указатель на животное по его ID.

Enclosure* findEnclosure(int id)
Возвращает указатель на вольер по его ID.

Worker* findWorker(int id)
Возвращает указатель на работника по его ID.

void refreshMarket()
Обновляет список доступных к покупке животных на рынке.

void showMainMenu()
Отображает главное меню игры с текущим состоянием зоопарка.

void showAnimalsMenu()
Отображает подменю управления животными.

void showEnclosuresMenu()
Отображает подменю управления вольерами.

void showWorkersMenu()
Отображает подменю управления персоналом.

void showMarketAnimals()
Показывает список животных, доступных на рынке.

void buyAnimal()
Позволяет купить животное с рынка и поместить его в подходящий вольер.

void sellAnimal()
Продажа выбранного животного за 70% от его цены.

void showAnimals()
Отображает список всех животных в зоопарке.

void renameAnimal()
Позволяет переименовать животное.

void tryBreedAnimals()
Пытается размножить двух выбранных животных.

void buyEnclosure()
Покупка нового вольера заданного типа и климата.

void showEnclosures()
Отображает список всех вольеров.

void hireWorker()
Нанимает нового работника определённого типа.

void showWorkers()
Отображает список всех работников.

void buyFood()
Покупка корма для животных.

void advertise()
Проведение рекламной кампании для увеличения популярности.

void showAnimalHappiness()
Отображает уровень счастья каждого животного.

void refreshMarketForMoney()
Обновляет рынок животных за деньги.

void nextDay()
Переход к следующему дню в игре.
Увеличивает возраст животных, проверяет здоровье, обновляет вольеры, работает персонал, рассчитывает доход и многое другое.

void payWorkers()
Выплачивает зарплаты всем сотрудникам.

void updateDisplayedHappiness()
Обновляет отображаемый уровень счастья.

wstring getHappinessColor()
Возвращает цветовой код для уровня счастья животного.

void updateHappiness(bool isFed, bool isAlone, bool isClean)
Изменяет уровень счастья животного в зависимости от условий содержания.

bool tryEscape()
Проверяет, не убежит ли животное из-за низкого уровня счастья.

bool checkAge()
Проверяет, не умерло ли животное от старости.

void rename(const wstring& newName)
Переименовывает животное.

bool canReproduce()
Проверяет, может ли животное размножаться.

vector<unique_ptr<Animal>> operator+(Animal& other)
Перегруженный оператор, реализует размножение животных.

void checkDisease()
Случайным образом присваивает болезнь или вылечивает животное.

Методы получения данных (get...)
getDisplayedHappiness(), getTrueHappiness(), getGender(), getParentId1(), getParentId2(), getIsAlive(), getSpecies(), getHealth(), getType()

wstring getTypeName()
Возвращает название типа вольера.

bool isSuitableForAnimalType(AnimalType animalType)
Проверяет, подходит ли тип вольера для данного животного.

void updateCleanliness()
Случайно делает вольер грязным.

bool addAnimal(Animal* animal)
Добавляет животное в вольер, если есть место и условия подходят.

bool removeAnimal(int animalId)
Удаляет животное из вольера по его ID.

void clean()
Чистит вольер.

vector<Animal*> getAnimalsByGender(wchar_t gender)
Возвращает список животных в вольере по полу.

void spreadDisease()
Распространяет болезнь среди животных вольера.

wstring generateAnimalName(const wstring& species, wchar_t gender)
Генерирует половое имя животного (например, "Тигрица").

bool isNumber(const wstring& str)
Проверяет, является ли строка числом.

wstring safeInputString(const wstring& prompt)
Безопасный ввод строки от пользователя.

int safeInputInt(const wstring& prompt)
Безопасный ввод целого числа от пользователя.

int wmain()
Точка входа в программу.
Инициализирует игру и запускает основной цикл.

AnimalHealth: состояние здоровья животного.
AnimalType: тип животного (наземное, водное и т.д.).

EnclosureType: типы вольеров.

WorkerType: типы работников.

Константные массивы и словари :
healthStatus, animalTypes, climates, animalSpecies — списки текстовых представлений.
speciesClimates, speciesTypes, workerTypeNames, workerBaseSalaries, speciesMaxAge, enclosureTypes — маппинги данных для создания объектов.

```

---

## Пример использования

Купить животное из магазина

Нанять ветеринара и уборщика

Покормить животных

Повысить популярность через рекламу

Разводить животных в одном вольере

Пройти 100 дней успешного управления зоопарком

---

**Цель игры**

Успешно управлять зоопарком не менее 100 дней , сохраняя баланс денег, еды, здоровья животных и популярности.

---


## 🧪 Как запустить проект?

### Требования:
- Windows (или совместимая платформа с поддержкой `_setmode`)
- Компилятор C++17 (MSVC / g++ / clang++)
- Поддержка Unicode в консоли

### Сборка:
```bash
g++ -std=c++17 -o ZooSIMS main.cpp