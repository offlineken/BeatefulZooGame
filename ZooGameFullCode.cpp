#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <locale>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <regex>
#include <unordered_map>
#include <io.h>
#include <fcntl.h>

using namespace std;

// Константы игры
const int MAX_EMPLOYEES = 5;          // Максимальное количество сотрудников
const int BASE_FOOD_COST = 5;         // Базовая стоимость единицы еды
const int BASE_ENCLOSURE_COST = 500;   // Базовая стоимость вольера
const int BASE_ANIMAL_PRICE = 2000;    // Базовая цена животного
const int ADVERTISEMENT_COST = 1000;   // Стоимость рекламы
const int MAX_POPULARITY = 100;        // Максимальная популярность
const int MAX_AGE = 2000;              // Максимальный возраст животного
const int OLD_AGE_THRESHOLD = 1000;    // Порог старости животного
const int BREEDING_ENCLOSURE_COST = 800; // Стоимость вольера для размножения

// Перечисления для типов данных
enum class Diet { PREDATORS, HERBIVORES };    // Тип питания: хищники/травоядные
enum class Climate { CONTINENT, TROPIC, ARCTIC }; // Климат: континентальный/тропический/арктический
enum class WorkerRole { DIRECTOR, VETERINAR, CLEANER, FOODMEN }; // Роли сотрудников
enum class AnimalState { HEALTHY, SICK, DEAD }; // Состояния животных

// Предварительные объявления структур
struct Animal;
struct Enclosure;
struct Employee;
struct Zoo;

// Прототипы вспомогательных функций
wstring chooseClimate();
wstring chooseAnimalType();
wstring chooseSpecificAnimal(const wstring& type);
bool isPredatorAnimal(const wstring& specificType);
int calculateAnimalPrice(int age, int weight);
void displayEnclosureInfo(int index, const vector<Enclosure>& enclosures);
void displayDelegationMessage(bool initial);
wstring removeNonRussian(const wstring& text);
wstring localizeText(const wstring& text);
wstring removeExtraSpaces(const wstring& text);

// Класс Animal - представляет животное в зоопарке
class Animal {
public:
    // Основные атрибуты животного (вариант 1 - для английской версии)
    string name;        // Имя животного
    int age = 0;        // Возраст в днях
    int weight = 0;     // Вес в кг
    int price = 0;      // Цена животного
    Diet diet;          // Тип питания (хищник/травоядное)
    Climate climate;    // Предпочитаемый климат
    AnimalState state;  // Состояние здоровья
    int id = 0;         // Уникальный идентификатор

    // Дополнительные атрибуты (вариант 2 - для локализованной версии)
    wstring wname;      // Имя животного (unicode)
    wstring type;       // Общий тип (кошачьи, псовые и т.д.)
    wstring specificType; // Конкретный вид (лев, тигр и т.д.)
    bool isPredator = false; // Является ли хищником
    bool isSick = false;     // Болен ли
    bool isHungry = false;   // Голоден ли
    bool isUnhappy = false;  // Несчастлив ли
    int happiness = 50;      // Уровень счастья (0-100)
    wchar_t gender = 'M';    // Пол ('M' или 'F')
    Animal* parent1 = nullptr; // Указатель на первого родителя
    Animal* parent2 = nullptr; // Указатель на второго родителя
    bool bornInZoo = false;  // Родилось ли в зоопарке

    // Конструктор с параметрами
    Animal(string _name, int _age, int _weight, int _price, Diet _diet, Climate _climate, AnimalState _state, int _id) :
        name(_name), age(_age), weight(_weight), price(_price), diet(_diet), climate(_climate), state(_state), id(_id),
        parent1(nullptr), parent2(nullptr), bornInZoo(false) {
    }

    // Конструктор по умолчанию
    Animal() : parent1(nullptr), parent2(nullptr), bornInZoo(false) {}

    // Метод обновления состояния животного
    void update() {
        // Случайное заболевание животного с вероятностью 10%
        if (rand() % 10 == 0) {
            state = AnimalState::SICK;
        }
    }

    // Оператор для размножения животных
    Animal operator+(Animal& other) {
        // Проверка совместимости животных для размножения
        if (this->gender == other.gender) {
            throw runtime_error("Должен быть разный гендр");
        }
        if (this->age <= 5 || other.age <= 5) {
            throw runtime_error("Слишком молодые (должно быть возрасть больше 5 дн)");
        }

        // Создание потомка
        Animal offspring;
        offspring.type = this->type;
        offspring.specificType = this->specificType;
        offspring.age = 0;
        offspring.weight = (this->weight + other.weight) / 4;
        offspring.climate = this->climate;
        offspring.isPredator = this->isPredator;
        offspring.happiness = 100;
        offspring.gender = (rand() % 2 == 0) ? 'M' : 'F';
        
        // Сохранение информации о родителях
        offspring.parent1 = new Animal(*this);
        offspring.parent2 = new Animal(other);
        offspring.bornInZoo = true;

        // Генерация имени для потомка
        wstring names[] = { L"Малыш", L"Кроха", L"Детка", L"Малышка", L"Крошка" };
        offspring.wname = names[rand() % 5] + L" " + this->wname + L" и " + other.wname;

        return offspring;
    }

    // Деструктор - освобождает память от родителей, если животное родилось в зоопарке
    ~Animal() {
        if (bornInZoo) {
            delete parent1;
            delete parent2;
        }
    }
};

// Класс Enclosure - представляет вольер для животных
class Enclosure {
public:
    // Основные атрибуты вольера (вариант 1)
    int capacity = 0;           // Вместимость вольера
    Climate climate;            // Климат вольера
    vector<Animal> animals;     // Животные в вольере
    int dirty = 0;              // Уровень загрязнения

    // Дополнительные атрибуты (вариант 2)
    string name;                // Название вольера
    wstring animalType;         // Тип животных
    wstring specificAnimalType; // Конкретный вид животных
    int dailyCost = 50;         // Ежедневные расходы
    bool isPredatorEnclosure = false; // Для хищников ли
    bool isBreedingEnclosure = false; // Для размножения ли
    int upgradeLevel = 1;       // Уровень улучшения
    bool isDirty = false;       // Грязный ли

    // Конструктор с параметрами
    Enclosure(int _capacity, Climate _climate) :
        capacity(_capacity), climate(_climate), dirty(0),
        isDirty(false), upgradeLevel(1) {
    }

    // Конструктор по умолчанию
    Enclosure() : capacity(0), dirty(0), dailyCost(50),
        isPredatorEnclosure(false), isBreedingEnclosure(false),
        upgradeLevel(1), isDirty(false) {
    }

    // Метод добавления животного в вольер
    bool addAnimal(const Animal& animal) {
        // Проверка на переполнение вольера
        if (animals.size() >= capacity) return false;
        // Проверка совместимости по типу питания
        if (!animals.empty() && animal.diet != animals[0].diet) return false;
        // Проверка совместимости по климату
        if (animal.climate != climate) return false;
        
        animals.push_back(animal);
        return true;
    }

    // Метод продажи животного по ID
    void sellAnimal(int id) {
        for (size_t i = 0; i < animals.size(); i++) {
            if (animals[i].id == id) {
                animals.erase(animals.begin() + i);
                break;
            }
        }
    }

    // Проверка, нужна ли уборка вольера
    bool needsCleaning() {
        return dirty > 5;
    }

    // Метод уборки вольера
    void clean() {
        dirty = 0;
        isDirty = false;
    }

    // Метод обновления состояния вольера
    void update() {
        // Увеличение уровня загрязнения
        dirty += 2;

        // Подсчет животных и больных животных
        int totalAnimal = 0;
        int totalSickAnimal = 0;
        for (auto& animal : animals) {
            totalAnimal += (animal.state != AnimalState::DEAD) ? 1 : 0;
            totalSickAnimal += (animal.state == AnimalState::SICK) ? 1 : 0;
        }

        // Распространение болезни, если есть больные животные
        if (totalSickAnimal) {
            for (int i = 0; i < 2; i++) {
                for (auto& animal : animals) {
                    if (animal.state == AnimalState::HEALTHY) {
                        animal.state = AnimalState::SICK;
                        break;
                    }
                }
            }
        }
    }
};

// Класс Employee - представляет сотрудника зоопарка
class Employee {
public:
    // Основные атрибуты (вариант 1)
    string name;        // Имя сотрудника
    int price = 0;      // Зарплата
    WorkerRole role;    // Должность
    bool tired = false; // Устал ли

    // Дополнительные атрибуты (вариант 2)
    wstring wname;      // Имя (unicode)
    wstring position;   // Должность
    int salary = 0;     // Зарплата
    int efficiency = 0; // Эффективность

    // Конструктор с параметрами
    Employee(string _name, int _price, WorkerRole _role) :
        name(_name), price(_price), role(_role), tired(false) {
    }

    // Конструктор по умолчанию
    Employee() : price(0), tired(false), salary(0), efficiency(0) {}

    // Метод обновления состояния сотрудника
    void update() {
        tired = false; // Сотрудник отдохнул
    }
};

// Класс Zoo - основной класс, представляющий зоопарк
class Zoo {
public:
    // Основные атрибуты зоопарка (вариант 1)
    string name;                // Название зоопарка
    int day = 0;                // Текущий день
    int food = 0;               // Количество еды
    int money = 0;              // Деньги
    int popularity = 0;         // Популярность (0-100)
    vector<Animal> animals;     // Все животные
    vector<Employee> workers;   // Сотрудники
    vector<Enclosure> enclosures; // Вольеры

    // Дополнительные атрибуты (вариант 2)
    wstring wname;              // Название (unicode)
    int visitors = 0;           // Количество посетителей
    int animalsCount = 0;       // Количество животных
    int daysSurvived = 0;       // Дней выживания
    bool hasPlayerTakenAction = false; // Игрок совершил действие
    bool delegationSatisfied = false; // Довольна ли делегация
    int daysWithoutAction = 0;  // Дней без действий
    int animalsBoughtToday = 0; // Животных куплено сегодня

    // Вложенная структура для магазина животных
    struct AnimalShop {
        vector<Animal> availableAnimals; // Доступные животные
        int daysUntilRefresh = 0;        // Дней до обновления
    } animalShop;

    // Конструктор с параметрами (английская версия)
    Zoo(string _name, int start_money) :
        name(_name), day(0), food(0), money(start_money), popularity(50),
        visitors(0), animalsCount(0), daysSurvived(0),
        hasPlayerTakenAction(false), delegationSatisfied(false),
        daysWithoutAction(0), animalsBoughtToday(0) {

        animalShop.daysUntilRefresh = 0;
        refreshAnimalShop();
        // Добавление директора по умолчанию
        workers.push_back(Employee("Директор Егор", 100, WorkerRole::DIRECTOR));
    }

    // Конструктор с параметрами (локализованная версия)
    Zoo(wstring _wname) : wname(_wname), food(10), money(100000), popularity(50),
        visitors(0), animalsCount(0), daysSurvived(0), day(0),
        hasPlayerTakenAction(false), delegationSatisfied(false),
        daysWithoutAction(0), animalsBoughtToday(0) {

        animalShop.daysUntilRefresh = 0;
        refreshAnimalShop();
        // Добавление директора по умолчанию
        workers.push_back(Employee("Директор Егор", 100, WorkerRole::DIRECTOR));
        name = string(_wname.begin(), _wname.end());
    }

    // Метод получения количества больных животных
    int getCountSickAnimal() const {
        int totalSickAnimal = 0;
        for (const auto& enclosure : enclosures) {
            for (const auto& animal : enclosure.animals) {
                totalSickAnimal += (animal.state == AnimalState::SICK) ? 1 : 0;
            }
        }
        return totalSickAnimal;
    }

    // Метод получения общего количества животных
    int getCountAnimal() const {
        int totalAnimal = 0;
        for (const auto& enclosure : enclosures) {
            for (const auto& animal : enclosure.animals) {
                totalAnimal += (animal.state != AnimalState::DEAD) ? 1 : 0;
            }
        }
        return totalAnimal;
    }

    // Метод лечения животных
    void healthingAnimal() {
        int totalSickAnimal = getCountSickAnimal();

        if (totalSickAnimal == 0) {
            wcout << L"Нет больных животных.\n";
            return;
        }

        // Лечение животных ветеринарами
        for (auto& worker : workers) {
            if (worker.role == WorkerRole::VETERINAR && !worker.tired) {
                if (totalSickAnimal >= 20) worker.tired = true;
                int health = 0;
                wcout << L"Ветеринар " << worker.name.c_str() << L", начал лечить животных:\n";
                for (auto& animal : animals) {
                    if (animal.state == AnimalState::SICK) {
                        health++;
                        animal.state = AnimalState::HEALTHY;
                        wcout << L"ID: " << animal.id << L" | Имя: " << animal.name.c_str() << L" вылечили.\n";
                    }
                    if (health >= 20) break;
                }
                totalSickAnimal = getCountSickAnimal();
                if (totalSickAnimal == 0) {
                    wcout << L"Все животные здоровы!\n";
                    return;
                }
            }
        }

        if (totalSickAnimal) {
            wcout << L"Еще остались больные животные\n";
        }
    }

    // Метод перехода на следующий день
    void nextDay() {
        day++;
        daysSurvived++;

        // Обновление состояния сотрудников
        for (auto& worker : workers) {
            worker.update();
        }

        // Расчет ежедневных расходов
        int cost = 0;
        for (Employee& worker : workers) { cost += worker.price; }
        money -= cost;
        wcout << L"Расходы: " << cost << endl;

        // Подсчет животных
        int totalAnimal = getCountAnimal();
        int totalSickAnimal = getCountSickAnimal();

        // Кормление животных
        int animals2feed = totalAnimal * 2;
        if (food >= animals2feed) {
            food -= animals2feed;
            wcout << L"Кормление животных: " << animals2feed << endl;
        }
        else {
            // Животные умирают от голода с вероятностью 10%
            for (auto& animal : animals) {
                if (rand() % 10 == 0) {
                    animal.state = AnimalState::DEAD;
                }
            }
            wcout << L"Не хватает еды для животных!" << endl;
        }

        // Обновление состояния вольеров
        for (auto& enclosure : enclosures) {
            enclosure.update();
        }

        // Уборка вольеров
        int cleanerCount = 0;
        int dirtyZoo = 0;
        for (Employee& worker : workers) {
            if (worker.role == WorkerRole::CLEANER) cleanerCount++;
        }
        for (Enclosure& enclosure : enclosures) {
            if (enclosure.needsCleaning() && cleanerCount > 0) {
                enclosure.clean();
                cleanerCount--;
            }
            dirtyZoo += enclosure.dirty;
        }
        wcout << L"Загрязнение зоопарка: " << dirtyZoo << endl;

        // Обновление состояния животных
        for (auto& animal : animals) {
            animal.update();
        }

        // Смерть от болезни
        int* enclosureAnimal = new int;
        int* sickAnimal = new int;
        for (auto& enclosure : enclosures) {
            *enclosureAnimal = 0;
            *sickAnimal = 0;
            for (const auto& animal : enclosure.animals) {
                *enclosureAnimal += (animal.state != AnimalState::DEAD) ? 1 : 0;
                *sickAnimal += (animal.state == AnimalState::SICK) ? 1 : 0;
            }

            // Если больных больше половины, животные умирают с вероятностью 50%
            if ((*enclosureAnimal - *sickAnimal) < *sickAnimal) {
                for (auto& animal : enclosure.animals) {
                    if (animal.state == AnimalState::SICK && rand() % 2 == 0) {
                        wcout << L"ID: " << animal.id << L" | Имя: " << animal.name.c_str() << L" умерло.\n";
                        animal.state = AnimalState::DEAD;
                    }
                }
            }
        }
        delete enclosureAnimal;
        delete sickAnimal;

        // Расчет посетителей и дохода
        visitors = 2 * popularity;
        wcout << L"Прибыль = " << visitors * totalAnimal - (dirtyZoo * 2) - cost << endl;
        money += visitors * totalAnimal - (dirtyZoo * 2);

        // Обновление популярности
        popularity += (rand() % 21 - 10); // Случайное изменение -10..+10
        popularity -= totalSickAnimal;    // Уменьшение из-за больных животных
        popularity = max(10, min(100, popularity)); // Ограничение 10..100

        // Проверка условий победы/поражения
        if (money < 0) {
            wcout << L"\n!!! ВЫ БАНКРОТ !!!\n";
            wcout << L"Игра окончена. Вы продержались " << day << L" дней.\n";
            exit(0);
        }

        if (day >= 30) {
            wcout << L"\n=== ПОБЕДА! ===\n";
            wcout << L"Вы успешно управляли зоопарком 30 дней!\n";
            exit(0);
        }
    }

    // Метод покупки животного
    bool buyAnimal(int animalIndex) {
        if (animalIndex < 0 || animalIndex >= animals.size()) return false;

        Animal animal = animals[animalIndex];
        if (money < animal.price) return false;

        // Попытка добавить животное в первый подходящий вольер
        for (auto& enclosure : enclosures) {
            if (enclosure.addAnimal(animal)) {
                money -= animal.price;
                return true;
            }
        }

        return false;
    }

    // Метод найма сотрудника
    bool hireWorker(WorkerRole role) {
        int salary = 0;
        // Установка зарплаты в зависимости от должности
        switch (role) {
        case WorkerRole::VETERINAR: salary = 80; break;
        case WorkerRole::CLEANER: salary = 50; break;
        case WorkerRole::FOODMEN: salary = 60; break;
        default: return false;
        }

        if (money < salary) return false;

        // Генерация случайного имени для сотрудника
        string names[] = { "Иван", "Мария", "Петр", "Анна", "Сергей", "Костеннов" };
        workers.emplace_back(names[rand() % 5], salary, role);
        money -= salary;
        return true;
    }

    // Метод строительства вольера
    bool buildEnclosure(Climate climate, int capacity = 5) {
        int cost = capacity * 100;
        if (money < cost) return false;

        enclosures.emplace_back(capacity, climate);
        money -= cost;
        return true;
    }

    // Метод покупки еды
    void buyFood(int amount) {
        int cost = amount * 10;
        if (money >= cost) {
            food += amount;
            money -= cost;
        }
    }

    // Метод заказа рекламы
    void advertise() {
        if (money >= 50) {
            popularity = min(100, popularity + 15);
            money -= 50;
        }
    }

    // Метод обновления магазина животных
    void refreshAnimalShop() {
        animalShop.availableAnimals.clear();
        int animalCount = min(10, 5 + (rand() % 6)); // 5-10 животных

        for (int i = 0; i < animalCount; i++) {
            Animal newAnimal;

            // Выбор случайного типа животного
            wstring types[] = { L"Кошачьи", L"Псовые", L"Птицы", L"Пресмыкающиеся", L"Морские" };
            newAnimal.type = types[rand() % 5];

            // Генерация характеристик в зависимости от типа
            if (newAnimal.type == L"Кошачьи") {
                wstring specificTypes[] = { L"Лев", L"Тигр", L"Леопард", L"Рысь", L"Гепард" };
                newAnimal.specificType = specificTypes[rand() % 5];
                wstring names[] = { L"Рыжик", L"Полосатик", L"Пятнышко", L"Грива", L"Коготь" };
                newAnimal.wname = names[rand() % 5] + L" " + to_wstring(rand() % 1000);
                newAnimal.isPredator = true;
            }
            else if (newAnimal.type == L"Псовые") {
                wstring specificTypes[] = { L"Собака", L"Волк", L"Лиса", L"Шакал", L"Гиена" };
                newAnimal.specificType = specificTypes[rand() % 5];
                wstring names[] = { L"Бобик", L"Шарик", L"Рекс", L"Лорд", L"Тузик" };
                newAnimal.wname = names[rand() % 5] + L" " + to_wstring(rand() % 1000);
                newAnimal.isPredator = (newAnimal.specificType != L"Собака");
            }
            else if (newAnimal.type == L"Птицы") {
                wstring specificTypes[] = { L"Орел", L"Попугай", L"Пингвин", L"Сова", L"Фламинго" };
                newAnimal.specificType = specificTypes[rand() % 5];
                wstring names[] = { L"Крыло", L"Клюв", L"Перо", L"Коготок", L"Пташка" };
                newAnimal.wname = names[rand() % 5] + L" " + to_wstring(rand() % 1000);
                newAnimal.isPredator = (newAnimal.specificType == L"Орел" || newAnimal.specificType == L"Сова" || newAnimal.specificType == L"Пингвин");
            }
            else if (newAnimal.type == L"Пресмыкающиеся") {
                wstring specificTypes[] = { L"Змея", L"Черепаха", L"Ящерица", L"Крокодил", L"Динозавр" };
                newAnimal.specificType = specificTypes[rand() % 5];
                wstring names[] = { L"Шип", L"Чешуя", L"Змей", L"Клык", L"Хвост" };
                newAnimal.wname = names[rand() % 5] + L" " + to_wstring(rand() % 1000);
                newAnimal.isPredator = (newAnimal.specificType == L"Змея" || newAnimal.specificType == L"Крокодил" || newAnimal.specificType == L"Динозавр");
            }
            else if (newAnimal.type == L"Морские") {
                wstring specificTypes[] = { L"Дельфин", L"Акула", L"Моллюски", L"Осьминог", L"Кит" };
                newAnimal.specificType = specificTypes[rand() % 5];
                wstring names[] = { L"Волна", L"Плавник", L"Пузырь", L"Ракушка", L"Жемчуг" };
                newAnimal.wname = names[rand() % 5] + L" " + to_wstring(rand() % 1000);
                newAnimal.isPredator = (newAnimal.specificType == L"Акула" || newAnimal.specificType == L"Осьминог" || newAnimal.specificType == L"Дельфин");
            }

            // Установка случайного климата
            wstring climates[] = { L"Тропический", L"Умеренный", L"Арктический" };
            newAnimal.climate = static_cast<Climate>(rand() % 3);

            // Установка случайных характеристик
            newAnimal.age = rand() % MAX_AGE + 1;
            newAnimal.weight = rand() % 400 + 10;
            newAnimal.isSick = false;
            newAnimal.isHungry = false;
            newAnimal.isUnhappy = false;
            newAnimal.happiness = 70 + rand() % 31; // 70-100
            newAnimal.gender = (rand() % 2 == 0) ? 'M' : 'F';
            newAnimal.price = calculateAnimalPrice(newAnimal.age, newAnimal.weight);
            newAnimal.bornInZoo = false;

            animalShop.availableAnimals.push_back(newAnimal);
        }

        animalShop.daysUntilRefresh = 1 + (rand() % 3); // 1-3 дня до обновления
    }

    // Метод отображения статуса зоопарка
    void displayStatus() {
        wcout << L"\n=== Статус зоопарка ===\n";
        wcout << L"Название: " << wname << endl;
        wcout << L"Еда: " << food << endl;
        wcout << L"Деньги: " << money << endl;
        wcout << L"Популярность: " << popularity << L"/" << MAX_POPULARITY << endl;
        wcout << L"Посетители: " << visitors << endl;
        wcout << L"Животные: " << animalsCount << endl;
        wcout << L"Вольеров: " << enclosures.size() << endl;
        wcout << L"Количество сотрудников: " << workers.size() << L"/" << MAX_EMPLOYEES << endl;
        wcout << L"Прожито дней: " << daysSurvived << endl;
        wcout << L"До обновления магазина: " << animalShop.daysUntilRefresh << L" дней" << endl;
        wcout << L"Делегация TOI-1452 b довольна? " << (delegationSatisfied ? L"Да" : L"Нет") << endl;
        if (daysSurvived >= 10) {
            wcout << L"Животных куплено сегодня: " << animalsBoughtToday << L"/1" << endl;
        }
    }

    // Метод получения целочисленного ввода с проверкой
    int getIntInput(const wstring& prompt) {
        int value;
        while (true) {
            wcout << prompt;
            wcin >> value;

            if (wcin.fail()) {
                wcout << L"Ошибка: Введите число.\n";
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    // Метод получения булевого ввода с проверкой
    bool getBoolInput(const wstring& prompt) {
        int value;
        while (true) {
            value = getIntInput(prompt);
            if (value == 0 || value == 1) {
                return (value == 1);
            }
            else {
                wcout << L"Ошибка: Введите 0 или 1.\n";
            }
        }
    }

    // Метод генерации случайного имени
    wstring generateRandomName() {
        wstring names[] = { L"Анна", L"Борис", L"Виктория", L"Глеб", L"Дарья", L"Егор", L"Жанна", L"Ирина", L"Константин" };
        return names[rand() % 9];
    }

    // Метод отображения списка животных
    void displayAnimalsList(const vector<Animal>& animals) {
        wcout << L"\n=== Список животных ===\n";
        if (animals.empty()) {
            wcout << L"Нет животных.\n";
            return;
        }

        for (size_t i = 0; i < animals.size(); ++i) {
            const auto& animal = animals[i];
            wcout << i << L". " << animal.wname << L" (" << animal.specificType << L")" << endl;
            wcout << L"   Тип: " << animal.type << L", Пол: " << animal.gender << endl;
            wcout << L"   Возраст: " << animal.age << L"д, Вес: " << animal.weight << L"кг" << endl;
            wcout << L"   Климат: ";
            switch (animal.climate) {
            case Climate::CONTINENT: wcout << L"Континент"; break;
            case Climate::TROPIC: wcout << L"Тропики"; break;
            case Climate::ARCTIC: wcout << L"Арктика"; break;
            }
            wcout << L", Хищник: " << (animal.isPredator ? L"Да" : L"Нет") << endl;
            wcout << L"   Здоровье: " << (animal.isSick ? L"Больное" : L"Здоровое") << endl;
            wcout << L"   Сытость: " << (animal.isHungry ? L"Голодное" : L"Сытое") << endl;
            wcout << L"   Счастье: " << animal.happiness << L"/100" << endl;
        }
    }

    // Метод отображения списка сотрудников
    void displayEmployeesList() {
        wcout << L"\n=== Список работников ===\n";
        if (workers.empty()) {
            wcout << L"Нет работников.\n";
            return;
        }

        for (size_t i = 0; i < workers.size(); ++i) {
            const auto& worker = workers[i];
            wcout << i << L". " << worker.name.c_str();
            wcout << L" | Зарплата: " << worker.price;
            wcout << L" | Роль: ";
            switch (worker.role) {
            case WorkerRole::DIRECTOR: wcout << L"Директор"; break;
            case WorkerRole::VETERINAR: wcout << L"Ветеринар"; break;
            case WorkerRole::CLEANER: wcout << L"Уборщик"; break;
            case WorkerRole::FOODMEN: wcout << L"Кормильщик"; break;
            }
            wcout << endl;
        }
    }

    // Метод отображения списка вольеров
    void displayEnclosuresList() {
        wcout << L"\n=== Список вольеров ===\n";
        if (enclosures.empty()) {
            wcout << L"Нет вольеров.\n";
            return;
        }

        for (size_t i = 0; i < enclosures.size(); ++i) {
            const auto& enclosure = enclosures[i];
            wcout << i << L". " << enclosure.name.c_str() << endl;
            wcout << L"   Вместимость: " << enclosure.animals.size() << L"/" << enclosure.capacity << endl;
            wcout << L"   Климат: ";
            switch (enclosure.climate) {
            case Climate::CONTINENT: wcout << L"Континент"; break;
            case Climate::TROPIC: wcout << L"Тропики"; break;
            case Climate::ARCTIC: wcout << L"Арктика"; break;
            }
            wcout << endl;
            wcout << L"   Для хищников: " << (enclosure.isPredatorEnclosure ? L"Да" : L"Нет") << endl;
            wcout << L"   Для размножения: " << (enclosure.isBreedingEnclosure ? L"Да" : L"Нет") << endl;
            wcout << L"   Уровень: " << enclosure.upgradeLevel << endl << endl;
        }
    }

    // Метод отображения детальной информации о вольере
    void displayEnclosureDetails(int index) {
        if (index < 0 || index >= (int)enclosures.size()) {
            wcout << L"Некорректный номер вольера.\n";
            return;
        }

        const auto& enclosure = enclosures[index];
        wcout << L"\n=== Подробная информация о вольере ===\n";
        wcout << L"Название: " << enclosure.name.c_str() << endl;
        wcout << L"Вместимость: " << enclosure.animals.size() << L"/" << enclosure.capacity << endl;
        wcout << L"Климат: ";
        switch (enclosure.climate) {
        case Climate::CONTINENT: wcout << L"Континент"; break;
        case Climate::TROPIC: wcout << L"Тропики"; break;
        case Climate::ARCTIC: wcout << L"Арктика"; break;
        }
        wcout << endl;
        wcout << L"Для хищников: " << (enclosure.isPredatorEnclosure ? L"Да" : L"Нет") << endl;
        wcout << L"Для размножения: " << (enclosure.isBreedingEnclosure ? L"Да" : L"Нет") << endl;
        wcout << L"Уровень: " << enclosure.upgradeLevel << endl;
        wcout << L"Состояние: " << (enclosure.isDirty ? L"Грязный" : L"Чистый") << endl;

        wcout << L"\nЖивотные в вольере:\n";
        if (enclosure.animals.empty()) {
            wcout << L"Нет животных.\n";
        }
        else {
            for (const auto& animal : enclosure.animals) {
                wcout << L"Имя: " << animal.name.c_str() << endl;
                wcout << L"Тип: " << (animal.diet == Diet::PREDATORS ? L"Хищник" : L"Травоядное") << endl;
                wcout << L"Состояние: ";
                switch (animal.state) {
                case AnimalState::HEALTHY: wcout << L"Здоров"; break;
                case AnimalState::SICK: wcout << L"Болен"; break;
                case AnimalState::DEAD: wcout << L"Мертв"; break;
                }
                wcout << endl << endl;
            }
        }
    }

    // Метод переименования животного
    void renameAnimal() {
        if (enclosures.empty()) {
            wcout << L"Нет вольеров с животными.\n";
            return;
        }

        displayEnclosuresList();
        int enclosureIndex = getIntInput(L"Введите номер вольера (0-" + to_wstring(enclosures.size() - 1) + L"): ");

        if (enclosureIndex < 0 || enclosureIndex >= (int)enclosures.size()) {
            wcout << L"Некорректный номер вольера.\n";
            return;
        }

        if (enclosures[enclosureIndex].animals.empty()) {
            wcout << L"В этом вольере нет животных.\n";
            return;
        }

        for (size_t i = 0; i < enclosures[enclosureIndex].animals.size(); ++i) {
            const auto& animal = enclosures[enclosureIndex].animals[i];
            wcout << i << L". " << animal.name.c_str() << endl;
        }

        int animalIndex = getIntInput(L"Введите номер животного для переименования (0-" + to_wstring(enclosures[enclosureIndex].animals.size() - 1) + L"): ");
        if (animalIndex < 0 || animalIndex >= (int)enclosures[enclosureIndex].animals.size()) {
            wcout << L"Некорректный номер животного.\n";
            return;
        }

        wcout << L"Текущее имя: " << enclosures[enclosureIndex].animals[animalIndex].name.c_str() << endl;
        wcout << L"Введите новое имя: ";
        wstring newName;
        wcin >> ws;
        getline(wcin, newName);

        enclosures[enclosureIndex].animals[animalIndex].name = string(newName.begin(), newName.end());
        wcout << L"Имя успешно изменено.\n";
    }

    // Метод размножения животных
    void breedAnimals() {
        if (enclosures.empty()) {
            wcout << L"Нет вольеров с животными.\n";
            return;
        }

        displayEnclosuresList();
        int enclosureIndex = getIntInput(L"Введите номер вольера (0-" + to_wstring(enclosures.size() - 1) + L"): ");

        if (enclosureIndex < 0 || enclosureIndex >= (int)enclosures.size()) {
            wcout << L"Некорректный номер вольера.\n";
            return;
        }

        Enclosure& enclosure = enclosures[enclosureIndex];
        if (enclosure.animals.size() < 2) {
            wcout << L"В вольере должно быть как минимум 2 животных для размножения.\n";
            return;
        }

        // Проверка специального вольера для размножения
        if (enclosure.isBreedingEnclosure && !enclosure.specificAnimalType.empty()) {
            bool hasSameType = true;
            for (const auto& animal : enclosure.animals) {
                if (animal.specificType != enclosure.specificAnimalType) {
                    hasSameType = false;
                    break;
                }
            }
            if (!hasSameType) {
                wcout << L"В этом вольере можно размножать только животных типа: " << enclosure.specificAnimalType << endl;
                return;
            }
        }

        for (size_t i = 0; i < enclosure.animals.size(); ++i) {
            const auto& animal = enclosure.animals[i];
            wcout << i << L". " << animal.name.c_str() << endl;
        }

        int animalIndex1 = getIntInput(L"Введите номер первого животного (0-" + to_wstring(enclosure.animals.size() - 1) + L"): ");
        int animalIndex2 = getIntInput(L"Введите номер второго животного (0-" + to_wstring(enclosure.animals.size() - 1) + L"): ");

        if (animalIndex1 < 0 || animalIndex1 >= (int)enclosure.animals.size() ||
            animalIndex2 < 0 || animalIndex2 >= (int)enclosure.animals.size()) {
            wcout << L"Некорректные номера животных.\n";
            return;
        }

        if (animalIndex1 == animalIndex2) {
            wcout << L"Нельзя размножать животное с самим собой.\n";
            return;
        }

        Animal& animal1 = enclosure.animals[animalIndex1];
        Animal& animal2 = enclosure.animals[animalIndex2];

        if (animal1.specificType != animal2.specificType) {
            wcout << L"Животные должны быть одного типа для размножения.\n";
            return;
        }

        try {
            // Попытка размножения с помощью перегруженного оператора +
            Animal offspring = animal1 + animal2;

            if ((int)enclosure.animals.size() >= enclosure.capacity) {
                wcout << L"В вольере нет места для потомства.\n";
                return;
            }

            // Добавление потомка в вольер
            enclosure.animals.push_back(offspring);
            animalsCount++;
            wcout << L"Родился новый " << offspring.specificType << L" по имени " << offspring.wname << endl;
            wcout << L"Пол: " << offspring.gender << L", вес: " << offspring.weight << L"кг" << endl;
        }
        catch (const exception& e) {
            wcout << L"Ошибка: " << e.what() << endl;
        }
    }

    // Метод управления животными (основное меню)
    void manageAnimals() {
        hasPlayerTakenAction = true;
        int choice;

        while (true) {
            wcout << L"\n=== Управление животными ===\n";
            wcout << L"1. Купить животное\n";
            wcout << L"2. Продать животное\n";
            wcout << L"3. Просмотреть всех животных\n";
            wcout << L"4. Переместить животное\n";
            wcout << L"5. Магазин животных\n";
            wcout << L"6. Переименовать животное\n";
            wcout << L"7. Размножить животных\n";
            wcout << L"8. Просмотреть вольер\n";
            wcout << L"9. Лечить животных\n";
            wcout << L"10. Вернуться\n";
            wcout << L"Выберите действие: ";
            wcin >> choice;

            if (wcin.fail()) {
                wcout << L"Ошибка: Введите число.\n";
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
            case 1: {
                // Ограничение на покупку животных после 10 дня
                if (daysSurvived >= 10 && animalsBoughtToday >= 1) {
                    wcout << L"После 10 дня можно покупать не более 1 животного в день.\n";
                    break;
                }

                wcout << L"\n=== Магазин животных ===\n";
                wcout << L"До следующего обновления: " << animalShop.daysUntilRefresh << L" дней\n";

                if (animalShop.availableAnimals.empty()) {
                    wcout << L"Нет доступных животных в магазине.\n";
                    break;
                }

                displayAnimalsList(animalShop.availableAnimals);

                int buyChoice = getIntInput(L"\nВведите номер животного для покупки (или -1 для отмены): ");
                if (buyChoice == -1) break;

                if (buyChoice < 0 || buyChoice >= (int)animalShop.availableAnimals.size()) {
                    wcout << L"Некорректный номер животного.\n";
                    break;
                }

                const Animal& animalToBuy = animalShop.availableAnimals[buyChoice];

                if (money < animalToBuy.price) {
                    wcout << L"Недостаточно денег для покупки.\n";
                    break;
                }

                if (food < 1) {
                    wcout << L"Недостаточно еды для нового животного.\n";
                    break;
                }

                wcout << L"\nВыберите вольер для размещения животного:\n";
                displayEnclosuresList();

                int enclosureIndex = getIntInput(L"Введите номер вольера (0-" + to_wstring(enclosures.size() - 1) + L"): ");

                if (enclosureIndex < 0 || enclosureIndex >= (int)enclosures.size()) {
                    wcout << L"Некорректный номер вольера.\n";
                    break;
                }

                Enclosure& enclosure = enclosures[enclosureIndex];

                // Проверка совместимости животного и вольера
                if (enclosure.animalType != animalToBuy.type) {
                    wcout << L"Тип вольера не подходит для этого животного.\n";
                    break;
                }

                if (!enclosure.specificAnimalType.empty() && enclosure.specificAnimalType != animalToBuy.specificType) {
                    wcout << L"Этот вольер предназначен только для: " << enclosure.specificAnimalType << L"\n";
                    break;
                }

                if (enclosure.isPredatorEnclosure != animalToBuy.isPredator) {
                    wcout << L"Этот вольер " << (enclosure.isPredatorEnclosure ? L"для хищников" : L"не для хищников")
                        << L", а это животное " << (animalToBuy.isPredator ? L"хищник" : L"не хищник") << L".\n";
                    break;
                }

                if ((int)enclosure.animals.size() >= enclosure.capacity) {
                    wcout << L"В вольере нет свободного места.\n";
                    break;
                }

                // Покупка животного
                money -= animalToBuy.price;
                food -= 1;
                animalsCount++;
                animalsBoughtToday++;
                enclosure.animals.push_back(animalToBuy);
                checkDelegationSatisfaction();

                wcout << L"Животное " << animalToBuy.specificType << L" \"" << animalToBuy.wname << L"\" куплено и помещено в вольер \"" << enclosure.name.c_str() << L"\"\n";
                animalShop.availableAnimals.erase(animalShop.availableAnimals.begin() + buyChoice);
                break;
            }
            case 2: {
                // Продажа животного
                if (enclosures.empty()) {
                    wcout << L"Нет вольеров.\n";
                    break;
                }

                displayEnclosuresList();
                int enclosureIndex = getIntInput(L"Введите номер вольера для продажи животного (0-" + to_wstring(enclosures.size() - 1) + L"): ");

                if (enclosureIndex < 0 || enclosureIndex >= (int)enclosures.size()) {
                    wcout << L"Некорректный номер вольера.\n";
                    break;
                }

                if (enclosures[enclosureIndex].animals.empty()) {
                    wcout << L"В этом вольере нет животных.\n";
                    break;
                }

                for (size_t i = 0; i < enclosures[enclosureIndex].animals.size(); ++i) {
                    const auto& animal = enclosures[enclosureIndex].animals[i];
                    wcout << i << L". " << animal.name.c_str() << L" ($" << animal.price / 2 << L")\n";
                }

                int animalIndex = getIntInput(L"Введите номер животного для продажи (0-" + to_wstring(enclosures[enclosureIndex].animals.size() - 1) + L"): ");

                if (animalIndex < 0 || animalIndex >= (int)enclosures[enclosureIndex].animals.size()) {
                    wcout << L"Некорректный номер животного.\n";
                    break;
                }

                wcout << L"Вы уверены, что хотите продать " << enclosures[enclosureIndex].animals[animalIndex].name.c_str() << L"? (1 - да, 0 - нет): ";
                int confirm = getIntInput(L"");

                if (confirm == 1) {
                    animalsCount--;
                    money += enclosures[enclosureIndex].animals[animalIndex].price / 2;
                    wcout << L"Животное продано.\n";
                    enclosures[enclosureIndex].animals.erase(enclosures[enclosureIndex].animals.begin() + animalIndex);
                    checkDelegationSatisfaction();
                }
                else {
                    wcout << L"Продажа отменена.\n";
                }
                break;
            }
            case 3: {
                // Просмотр всех животных
                if (enclosures.empty()) {
                    wcout << L"Нет вольеров с животными.\n";
                    break;
                }

                for (size_t i = 0; i < enclosures.size(); ++i) {
                    wcout << L"\nВольер " << i << L": " << enclosures[i].name.c_str() << endl;
                    for (const auto& animal : enclosures[i].animals) {
                        wcout << L"Имя: " << animal.name.c_str() << endl;
                        wcout << L"Тип: " << (animal.diet == Diet::PREDATORS ? L"Хищник" : L"Травоядное") << endl;
                        wcout << L"Состояние: ";
                        switch (animal.state) {
                        case AnimalState::HEALTHY: wcout << L"Здоров"; break;
                        case AnimalState::SICK: wcout << L"Болен"; break;
                        case AnimalState::DEAD: wcout << L"Мертв"; break;
                        }
                        wcout << endl << endl;
                    }
                }
                break;
            }
            case 4: {
                // Перемещение животного между вольерами
                if (enclosures.size() < 2) {
                    wcout << L"Необходимо минимум два вольера для перемещения животных.\n";
                    break;
                }
                wcout << L"\n=== Перемещение животных ===\n";
                displayEnclosuresList();

                int sourceEnclosureIndex = getIntInput(L"Введите номер вольера, из которого хотите переместить животное: ");
                if (sourceEnclosureIndex < 0 || sourceEnclosureIndex >= (int)enclosures.size()) {
                    wcout << L"Некорректный номер вольера.\n";
                    break;
                }
                if (enclosures[sourceEnclosureIndex].animals.empty()) {
                    wcout << L"В этом вольере нет животных.\n";
                    break;
                }

                for (size_t i = 0; i < enclosures[sourceEnclosureIndex].animals.size(); ++i) {
                    const auto& animal = enclosures[sourceEnclosureIndex].animals[i];
                    wcout << i << L". " << animal.name.c_str() << endl;
                }

                int animalIndex = getIntInput(L"Введите номер животного для перемещения (0-" + to_wstring(enclosures[sourceEnclosureIndex].animals.size() - 1) + L"): ");
                if (animalIndex < 0 || animalIndex >= (int)enclosures[sourceEnclosureIndex].animals.size()) {
                    wcout << L"Некорректный номер животного.\n";
                    break;
                }

                displayEnclosuresList();
                int destinationEnclosureIndex = getIntInput(L"Введите номер вольера, в который хотите переместить животное: ");
                if (destinationEnclosureIndex < 0 || destinationEnclosureIndex >= (int)enclosures.size()) {
                    wcout << L"Некорректный номер вольера.\n";
                    break;
                }
                if (sourceEnclosureIndex == destinationEnclosureIndex) {
                    wcout << L"Нельзя переместить животное в тот же самый вольер.\n";
                    break;
                }

                if ((int)enclosures[destinationEnclosureIndex].animals.size() >= enclosures[destinationEnclosureIndex].capacity) {
                    wcout << L"В вольере назначения недостаточно места.\n";
                    break;
                }

                Animal& animalToMove = enclosures[sourceEnclosureIndex].animals[animalIndex];
                Enclosure& destEnclosure = enclosures[destinationEnclosureIndex];

                // Проверка совместимости
                if (destEnclosure.animalType != animalToMove.type) {
                    wcout << L"Тип вольера не подходит для этого животного.\n";
                    break;
                }

                if (!destEnclosure.specificAnimalType.empty() && destEnclosure.specificAnimalType != animalToMove.specificType) {
                    wcout << L"Этот вольер предназначен только для: " << destEnclosure.specificAnimalType << L"\n";
                    break;
                }

                if (destEnclosure.isPredatorEnclosure != animalToMove.isPredator) {
                    wcout << L"Этот вольер " << (destEnclosure.isPredatorEnclosure ? L"для хищников" : L"не для хищников")
                        << L", а это животное " << (animalToMove.isPredator ? L"хищник" : L"не хищник") << L".\n";
                    break;
                }

                // Перемещение животного
                destEnclosure.animals.push_back(animalToMove);
                enclosures[sourceEnclosureIndex].animals.erase(enclosures[sourceEnclosureIndex].animals.begin() + animalIndex);
                wcout << L"Животное успешно перемещено.\n";
                checkDelegationSatisfaction();
                break;
            }
            case 5: {
                // Магазин животных
                wcout << L"\n=== Магазин животных ===\n";
                wcout << L"До следующего обновления: " << animalShop.daysUntilRefresh << L" дней\n";
                wcout << L"1. Просмотреть животных\n";
                wcout << L"2. Обновить магазин (1000 денег)\n";
                wcout << L"3. Вернуться\n";
                wcout << L"Выберите действие: ";
                int shopChoice = getIntInput(L"");

                if (shopChoice == 1) {
                    displayAnimalsList(animalShop.availableAnimals);
                }
                else if (shopChoice == 2) {
                    if (money >= 1000) {
                        money -= 1000;
                        refreshAnimalShop();
                        wcout << L"Магазин животных обновлен!\n";
                    }
                    else {
                        wcout << L"Недостаточно денег для обновления магазина.\n";
                    }
                }
                break;
            }
            case 6:
                renameAnimal();
                break;
            case 7:
                breedAnimals();
                break;
            case 8: {
                // Просмотр информации о вольере
                if (enclosures.empty()) {
                    wcout << L"Нет вольеров.\n";
                    break;
                }
                displayEnclosuresList();
                int enclosureIndex = getIntInput(L"Введите номер вольера для просмотра (0-" + to_wstring(enclosures.size() - 1) + L"): ");
                displayEnclosureDetails(enclosureIndex);
                break;
            }
            case 9:
                healthingAnimal();
                break;
            case 10:
                return;
            default:
                wcout << L"Некорректный ввод.\n";
            }
        }
    }

    // Метод управления закупками (еда и реклама)
    void managePurchases() {
        hasPlayerTakenAction = true;
        int choice;

        while (true) {
            wcout << L"\n=== Управление закупками ===\n";
            wcout << L"1. Купить еду\n";
            wcout << L"2. Заказать рекламу\n";
            wcout << L"3. Вернуться\n";
            wcout << L"Выберите действие: ";
            wcin >> choice;

            if (wcin.fail()) {
                wcout << L"Ошибка: Введите число.\n";
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
            case 1: {
                // Покупка еды
                int amount = getIntInput(L"Сколько еды купить? (1 еда = " + to_wstring(BASE_FOOD_COST) + L" денег): ");
                int cost = amount * BASE_FOOD_COST;
                if (money >= cost) {
                    money -= cost;
                    food += amount;
                    wcout << L"Куплено " << amount << L" еды.\n";
                }
                else {
                    wcout << L"Недостаточно денег для покупки.\n";
                }
                break;
            }
            case 2: {
                // Заказ рекламы
                if (money >= ADVERTISEMENT_COST) {
                    money -= ADVERTISEMENT_COST;
                    popularity = min(MAX_POPULARITY, popularity + 5);
                    visitors += 10;
                    wcout << L"Рекламная кампания проведена. Популярность +5, посетители +10.\n";
                }
                else {
                    wcout << L"Недостаточно денег для рекламы.\n";
                }
                break;
            }
            case 3:
                return;
            default:
                wcout << L"Некорректный ввод.\n";
            }
        }
    }

    // Метод управления вольерами
    void manageBuildings() {
        hasPlayerTakenAction = true;
        int choice;

        while (true) {
            wcout << L"\n=== Управление вольерами ===\n";
            wcout << L"1. Построить обычный вольер\n";
            wcout << L"2. Построить вольер для размножения\n";
            wcout << L"3. Улучшить вольер\n";
            wcout << L"4. Продать вольер\n";
            wcout << L"5. Просмотреть вольер\n";
            wcout << L"6. Вернуться\n";
            wcout << L"Выберите действие: ";
            wcin >> choice;

            if (wcin.fail()) {
                wcout << L"Ошибка: Введите число.\n";
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
            case 1: {
                // Строительство обычного вольера
                if (money < BASE_ENCLOSURE_COST) {
                    wcout << L"Недостаточно денег для строительства вольера.\n";
                    break;
                }

                wstring enclosureName;
                wcout << L"Введите название вольера: ";
                wcin >> ws;
                getline(wcin, enclosureName);

                wstring climate = chooseClimate();
                wstring animalType = chooseAnimalType();

                // Определение, будет ли вольер для хищников
                bool isPredatorEnclosure = false;
                if (animalType == L"Кошачьи") isPredatorEnclosure = true;
                else if (animalType == L"Псовые") isPredatorEnclosure = (rand() % 5 != 0);
                else if (animalType == L"Птицы") isPredatorEnclosure = (rand() % 3 == 0);
                else if (animalType == L"Пресмыкающиеся") isPredatorEnclosure = (rand() % 2 == 0);
                else if (animalType == L"Морские") isPredatorEnclosure = (rand() % 3 != 0);

                // Создание нового вольера
                Enclosure newEnclosure;
                newEnclosure.name = string(enclosureName.begin(), enclosureName.end());
                newEnclosure.capacity = 2;
                newEnclosure.animalType = animalType;
                newEnclosure.climate = static_cast<Climate>(rand() % 3);
                newEnclosure.dailyCost = 50;
                newEnclosure.isPredatorEnclosure = isPredatorEnclosure;
                newEnclosure.isBreedingEnclosure = false;
                newEnclosure.upgradeLevel = 1;

                money -= BASE_ENCLOSURE_COST;
                enclosures.push_back(newEnclosure);
                wcout << L"Вольер \"" << enclosureName << L"\" построен.\n";
                wcout << L"Этот вольер " << (isPredatorEnclosure ? L"для хищников" : L"не для хищников") << L".\n";
                checkDelegationSatisfaction();
                break;
            }
            case 2: {
                // Строительство вольера для размножения
                if (money < BREEDING_ENCLOSURE_COST) {
                    wcout << L"Недостаточно денег для строительства вольера для размножения.\n";
                    break;
                }

                wstring enclosureName;
                wcout << L"Введите название вольера: ";
                wcin >> ws;
                getline(wcin, enclosureName);

                wstring climate = chooseClimate();
                wstring animalType = chooseAnimalType();
                wstring specificType = chooseSpecificAnimal(animalType);

                bool isPredatorEnclosure = isPredatorAnimal(specificType);

                // Создание вольера для размножения
                Enclosure newEnclosure;
                newEnclosure.name = string(enclosureName.begin(), enclosureName.end());
                newEnclosure.capacity = 3;
                newEnclosure.animalType = animalType;
                newEnclosure.specificAnimalType = specificType;
                newEnclosure.climate = static_cast<Climate>(rand() % 3);
                newEnclosure.dailyCost = 80;
                newEnclosure.isPredatorEnclosure = isPredatorEnclosure;
                newEnclosure.isBreedingEnclosure = true;
                newEnclosure.upgradeLevel = 1;

                money -= BREEDING_ENCLOSURE_COST;
                enclosures.push_back(newEnclosure);
                wcout << L"Вольер для размножения \"" << enclosureName << L"\" построен.\n";
                wcout << L"Этот вольер предназначен для: " << specificType << L" (" << (isPredatorEnclosure ? L"хищник" : L"не хищник") << L")\n";
                checkDelegationSatisfaction();
                break;
            }
            case 3: {
                // Улучшение вольера
                if (enclosures.empty()) {
                    wcout << L"Нет вольеров для улучшения.\n";
                    break;
                }

                displayEnclosuresList();
                int enclosureIndex = getIntInput(L"Введите номер вольера для улучшения (0-" + to_wstring(enclosures.size() - 1) + L"): ");

                if (enclosureIndex < 0 || enclosureIndex >= (int)enclosures.size()) {
                    wcout << L"Некорректный номер вольера.\n";
                    break;
                }

                Enclosure& enclosure = enclosures[enclosureIndex];
                if (enclosure.upgradeLevel >= 5) {
                    wcout << L"Этот вольер уже максимально улучшен.\n";
                    break;
                }

                int upgradeCost = 200 * enclosure.upgradeLevel;
                wcout << L"Стоимость улучшения: " << upgradeCost << L" денег\n";
                wcout << L"Текущая вместимость: " << enclosure.capacity << endl;
                wcout << L"Новая вместимость: " << enclosure.capacity + (enclosure.isBreedingEnclosure ? 3 : 2) << endl;

                if (money < upgradeCost) {
                    wcout << L"Недостаточно денег для улучшения.\n";
                    break;
                }

                money -= upgradeCost;
                enclosure.capacity += (enclosure.isBreedingEnclosure ? 3 : 2);
                enclosure.dailyCost += 20;
                enclosure.upgradeLevel++;
                wcout << L"Вольер улучшен до уровня " << enclosure.upgradeLevel << L". Новая вместимость: " << enclosure.capacity << endl;
                break;
            }
            case 4: {
                // Продажа вольера
                if (enclosures.empty()) {
                    wcout << L"Нет вольеров для продажи.\n";
                    break;
                }

                displayEnclosuresList();
                int enclosureIndex = getIntInput(L"Введите номер вольера для продажи (0-" + to_wstring(enclosures.size() - 1) + L"): ");

                if (enclosureIndex < 0 || enclosureIndex >= (int)enclosures.size()) {
                    wcout << L"Некорректный номер вольера.\n";
                    break;
                }

                Enclosure& enclosure = enclosures[enclosureIndex];
                if (!enclosure.animals.empty()) {
                    wcout << L"Нельзя продать вольер с животными. Сначала переместите или продайте животных.\n";
                    break;
                }

                // Расчет стоимости продажи
                int sellPrice = 300 + (enclosure.upgradeLevel - 1) * 100;
                if (enclosure.isBreedingEnclosure) {
                    sellPrice += 200;
                }
                wcout << L"Вы получите " << sellPrice << L" за продажу этого вольера.\n";
                wcout << L"Вы уверены, что хотите продать вольер \"" << enclosure.name.c_str() << L"\"? (1 - да, 0 - нет): ";
                int confirm = getIntInput(L"");

                if (confirm == 1) {
                    money += sellPrice;
                    enclosures.erase(enclosures.begin() + enclosureIndex);
                    wcout << L"Вольер продан.\n";
                }
                else {
                    wcout << L"Продажа отменена.\n";
                }
                break;
            }
            case 5: {
                // Просмотр информации о вольере
                if (enclosures.empty()) {
                    wcout << L"Нет вольеров.\n";
                    break;
                }
                displayEnclosuresList();
                int enclosureIndex = getIntInput(L"Введите номер вольера для просмотра (0-" + to_wstring(enclosures.size() - 1) + L"): ");
                displayEnclosureDetails(enclosureIndex);
                break;
            }
            case 6:
                return;
            default:
                wcout << L"Некорректный ввод.\n";
            }
        }
    }

    // Метод управления персоналом
    void manageEmployees() {
        hasPlayerTakenAction = true;
        int choice;

        while (true) {
            wcout << L"\n=== Управление персоналом ===\n";
            wcout << L"1. Нанять сотрудника\n";
            wcout << L"2. Просмотреть сотрудников\n";
            wcout << L"3. Уволить сотрудника\n";
            wcout << L"4. Вернуться\n";
            wcout << L"Выберите действие: ";
            wcin >> choice;

            if (wcin.fail()) {
                wcout << L"Ошибка: Введите число.\n";
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
            case 1: {
                // Наем нового сотрудника
                if (workers.size() >= MAX_EMPLOYEES) {
                    wcout << L"Достигнуто максимальное количество сотрудников (" << MAX_EMPLOYEES << L").\n";
                    break;
                }

                wstring name = generateRandomName();
                wstring position;
                int salary;
                int efficiency;

                wcout << L"\nВыберите должность нового сотрудника:\n";
                wcout << L"1. Ветеринар (лечит животных)\n";
                wcout << L"2. Уборщик (поддерживает чистоту)\n";
                wcout << L"3. Смотритель (ухаживает за животными)\n";
                int positionChoice = getIntInput(L"Ваш выбор: ");

                // Создание сотрудника в зависимости от выбранной должности
                switch (positionChoice) {
                case 1:
                    position = L"Ветеринар";
                    salary = 60 + rand() % 41; // 60-100
                    efficiency = 50 + rand() % 51; // 50-100
                    workers.emplace_back(string(name.begin(), name.end()), salary, WorkerRole::VETERINAR);
                    break;
                case 2:
                    position = L"Уборщик";
                    salary = 40 + rand() % 31; // 40-70
                    efficiency = 70 + rand() % 31; // 70-100
                    workers.emplace_back(string(name.begin(), name.end()), salary, WorkerRole::CLEANER);
                    break;
                case 3:
                    position = L"Смотритель";
                    salary = 50 + rand() % 51; // 50-100
                    efficiency = 60 + rand() % 41; // 60-100
                    workers.emplace_back(string(name.begin(), name.end()), salary, WorkerRole::FOODMEN);
                    break;
                default:
                    wcout << L"Некорректный выбор. Назначаем смотрителем.\n";
                    position = L"Смотритель";
                    salary = 50 + rand() % 51;
                    efficiency = 60 + rand() % 41;
                    workers.emplace_back(string(name.begin(), name.end()), salary, WorkerRole::FOODMEN);
                }

                wcout << L"Нанят новый сотрудник: " << name << L" (" << position << L")\n";
                wcout << L"Зарплата: " << salary << L", Эффективность: " << efficiency << endl;
                break;
            }
            case 2: {
                displayEmployeesList();
                break;
            }
            case 3: {
                // Увольнение сотрудника
                if (workers.empty()) {
                    wcout << L"Нет сотрудников для увольнения.\n";
                    break;
                }

                displayEmployeesList();
                int employeeIndex = getIntInput(L"Введите номер сотрудника для увольнения (0-" + to_wstring(workers.size() - 1) + L"): ");

                if (employeeIndex < 0 || employeeIndex >= (int)workers.size()) {
                    wcout << L"Некорректный номер сотрудника.\n";
                    break;
                }

                wcout << L"Вы уверены, что хотите уволить " << workers[employeeIndex].name.c_str() << L"? (1 - да, 0 - нет): ";
                int confirm = getIntInput(L"");

                if (confirm == 1) {
                    string name = workers[employeeIndex].name;
                    workers.erase(workers.begin() + employeeIndex);
                    wcout << L"Сотрудник " << name.c_str() << L" уволен.\n";
                }
                else {
                    wcout << L"Увольнение отменено.\n";
                }
                break;
            }
            case 4:
                return;
            default:
                wcout << L"Некорректный ввод.\n";
            }
        }
    }

    // Метод обновления уровня счастья животных
    void updateAnimalHappiness() {
        for (auto& enclosure : enclosures) {
            for (auto& animal : enclosure.animals) {
                // Уменьшение счастья в зависимости от состояния
                if (animal.isHungry) {
                    animal.happiness = max(0, animal.happiness - 15);
                }

                if (animal.isSick) {
                    animal.happiness = max(0, animal.happiness - 20);
                }

                if (enclosure.isDirty) {
                    animal.happiness = max(0, animal.happiness - 10);
                }

                // Увеличение счастья, если все хорошо
                if (!animal.isHungry && !animal.isSick && !enclosure.isDirty) {
                    animal.happiness = min(100, animal.happiness + 5);
                }

                animal.isUnhappy = (animal.happiness < 50);
            }
        }
    }

    // Метод проверки старения и смерти животных
    void checkAnimalAgingAndDeath() {
        for (auto& enclosure : enclosures) {
            for (size_t i = 0; i < enclosure.animals.size(); ) {
                Animal& animal = enclosure.animals[i];
                animal.age++;

                // Проверка на смерть от старости
                if (animal.age > OLD_AGE_THRESHOLD) {
                    int deathChance = min(99, (animal.age - OLD_AGE_THRESHOLD) / 10);
                    if (rand() % 100 < deathChance) {
                        wcout << L"Животное " << animal.name.c_str() << L" (" << animal.specificType << L") умерло от старости в возрасте " << animal.age << L" дней.\n";
                        enclosure.animals.erase(enclosure.animals.begin() + i);
                        animalsCount--;
                        continue;
                    }
                }
                i++;
            }
        }
    }

    // Метод проверки удовлетворенности делегации
    void checkDelegationSatisfaction() {
        bool hasMarineEnclosure = false;
        for (const auto& enclosure : enclosures) {
            if (enclosure.animalType == L"Морские") {
                hasMarineEnclosure = true;
                break;
            }
        }

        bool hasMarineAnimals = false;
        for (const auto& enclosure : enclosures) {
            for (const auto& animal : enclosure.animals) {
                if (animal.type == L"Морские") {
                    hasMarineAnimals = true;
                    break;
                }
            }
            if (hasMarineAnimals) break;
        }

        delegationSatisfied = hasMarineEnclosure && hasMarineAnimals;

        if (delegationSatisfied) {
            wcout << L"\nДелегация с TOI-1452 b начинает проявлять интерес к вашему зоопарку!\n";
            wcout << L"Они видят, что вы заботитесь о морских видах животных.\n";
        }
    }

    // Главное меню игры
    void menu() {
        int choice;
        while (true) {
            displayStatus();

            wcout << L"\n=== Главное меню ===\n";
            wcout << L"1. Управление животными\n";
            wcout << L"2. Управление закупками\n";
            wcout << L"3. Управление вольерами\n";
            wcout << L"4. Управление персоналом\n";
            wcout << L"5. Следующий день\n";
            wcout << L"6. Выход\n";
            wcout << L"Выберите действие: ";
            wcin >> choice;

            if (wcin.fail()) {
                wcout << L"Ошибка: Введите число.\n";
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
            case 1:
                manageAnimals();
                break;
            case 2:
                managePurchases();
                break;
            case 3:
                manageBuildings();
                break;
            case 4:
                manageEmployees();
                break;
            case 5:
                nextDay();
                break;
            case 6:
                return;
            default:
                wcout << L"Некорректный ввод.\n";
            }
        }
    }
};

// Вспомогательная функция для проверки, является ли животное хищником
bool isPredatorAnimal(const wstring& specificType) {
    if (specificType == L"Лев" || specificType == L"Тигр" || specificType == L"Леопард" ||
        specificType == L"Рысь" || specificType == L"Гепард" ||
        specificType == L"Волк" || specificType == L"Лиса" || specificType == L"Шакал" ||
        specificType == L"Гиена" ||
        specificType == L"Орел" || specificType == L"Сова" || specificType == L"Пингвин" ||
        specificType == L"Змея" || specificType == L"Крокодил" || specificType == L"Динозавр" ||
        specificType == L"Акула" || specificType == L"Осьминог" || specificType == L"Дельфин") {
        return true;
    }
    return false;
}

// Функция выбора климата через меню
wstring chooseClimate() {
    int choice;
    while (true) {
        wcout << L"\nВыберите климат:\n";
        wcout << L"1. Тропический\n";
        wcout << L"2. Умеренный\n";
        wcout << L"3. Арктический\n";
        wcout << L"Выберите номер климата: ";
        wcin >> choice;

        if (wcin.fail()) {
            wcout << L"Ошибка: Введите число.\n";
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            return L"Тропический";
        case 2:
            return L"Умеренный";
        case 3:
            return L"Арктический";
        default:
            wcout << L"Некорректный ввод. Повторите попытку.\n";
        }
    }
}

// Функция выбора типа животного через меню
wstring chooseAnimalType() {
    int choice;
    while (true) {
        wcout << L"\nВыберите тип животного:\n";
        wcout << L"1. Кошачьи\n";
        wcout << L"2. Псовые\n";
        wcout << L"3. Птицы\n";
        wcout << L"4. Пресмыкающиеся\n";
        wcout << L"5. Морские\n";
        wcout << L"Выберите номер типа: ";
        wcin >> choice;

        if (wcin.fail()) {
            wcout << L"Ошибка: Введите число.\n";
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            return L"Кошачьи";
        case 2:
            return L"Псовые";
        case 3:
            return L"Птицы";
        case 4:
            return L"Пресмыкающиеся";
        case 5:
            return L"Морские";
        default:
            wcout << L"Некорректный ввод. Повторите попытку.\n";
        }
    }
}

// Функция выбора конкретного вида животного через меню
wstring chooseSpecificAnimal(const wstring& type) {
    if (type == L"Кошачьи") {
        int choice;
        wcout << L"\nВыберите конкретное животное:\n";
        wcout << L"1. Лев\n";
        wcout << L"2. Тигр\n";
        wcout << L"3. Леопард\n";
        wcout << L"4. Рысь\n";
        wcout << L"5. Гепард\n";
        wcout << L"Выберите номер: ";
        wcin >> choice;

        switch (choice) {
        case 1: return L"Лев";
        case 2: return L"Тигр";
        case 3: return L"Леопард";
        case 4: return L"Рысь";
        case 5: return L"Гепард";
        default: return L"Лев";
        }
    }
    else if (type == L"Псовые") {
        int choice;
        wcout << L"\nВыберите конкретное животное:\n";
        wcout << L"1. Собака\n";
        wcout << L"2. Волк\n";
        wcout << L"3. Лиса\n";
        wcout << L"4. Шакал\n";
        wcout << L"5. Гиена\n";
        wcout << L"Выберите номер: ";
        wcin >> choice;

        switch (choice) {
        case 1: return L"Собака";
        case 2: return L"Волк";
        case 3: return L"Лиса";
        case 4: return L"Шакал";
        case 5: return L"Гиена";
        default: return L"Собака";
        }
    }
    else if (type == L"Птицы") {
        int choice;
        wcout << L"\nВыберите конкретное животное:\n";
        wcout << L"1. Орел\n";
        wcout << L"2. Попугай\n";
        wcout << L"3. Пингвин\n";
        wcout << L"4. Сова\n";
        wcout << L"5. Фламинго\n";
        wcout << L"Выберите номер: ";
        wcin >> choice;

        switch (choice) {
        case 1: return L"Орел";
        case 2: return L"Попугай";
        case 3: return L"Пингвин";
        case 4: return L"Сова";
        case 5: return L"Фламинго";
        default: return L"Орел";
        }
    }
    else if (type == L"Пресмыкающиеся") {
        int choice;
        wcout << L"\nВыберите конкретное животное:\n";
        wcout << L"1. Змея\n";
        wcout << L"2. Черепаха\n";
        wcout << L"3. Ящерица\n";
        wcout << L"4. Крокодил\n";
        wcout << L"5. Динозавр\n";
        wcout << L"Выберите номер: ";
        wcin >> choice;

        switch (choice) {
        case 1: return L"Змея";
        case 2: return L"Черепаха";
        case 3: return L"Ящерица";
        case 4: return L"Крокодил";
        case 5: return L"Динозавр";
        default: return L"Змея";
        }
    }
    else if (type == L"Морские") {
        int choice;
        wcout << L"\nВыберите конкретное животное:\n";
        wcout << L"1. Дельфин\n";
        wcout << L"2. Акула\n";
        wcout << L"3. Моллюски\n";
        wcout << L"4. Осьминог\n";
        wcout << L"5. Кит\n";
        wcout << L"Выберите номер: ";
        wcin >> choice;

        switch (choice) {
        case 1: return L"Дельфин";
        case 2: return L"Акула";
        case 3: return L"Моллюски";
        case 4: return L"Осьминог";
        case 5: return L"Кит";
        default: return L"Дельфин";
        }
    }
    return L"";
}

// Функция расчета цены животного на основе возраста и веса
int calculateAnimalPrice(int age, int weight) {
    double ageFactor = max(0.0, 1.0 - (double)age / MAX_AGE);
    double weightFactor = max(0.0, 1.0 - (double)weight / 500.0);
    int price = (int)(BASE_ANIMAL_PRICE * ageFactor * weightFactor);
    return max(price, 100);
}

// Функция отображения информации о вольере
void displayEnclosureInfo(int index, const vector<Enclosure>& enclosures) {
    if (index >= 0 && index < (int)enclosures.size()) {
        wcout << L"  Вольер " << index << L": " << enclosures[index].name.c_str() << endl;
        wcout << L"    Тип животных: " << enclosures[index].animalType << endl;
        wcout << L"    Конкретный тип: " << (enclosures[index].specificAnimalType.empty() ? L"Любой" : enclosures[index].specificAnimalType) << endl;
        wcout << L"    Вместимость: " << enclosures[index].capacity << endl;
        wcout << L"    Климат: ";
        switch (enclosures[index].climate) {
        case Climate::CONTINENT: wcout << L"Континент"; break;
        case Climate::TROPIC: wcout << L"Тропики"; break;
        case Climate::ARCTIC: wcout << L"Арктика"; break;
        }
        wcout << endl;
        wcout << L"    Для хищников: " << (enclosures[index].isPredatorEnclosure ? L"Да" : L"Нет") << endl;
        wcout << L"    Для размножения: " << (enclosures[index].isBreedingEnclosure ? L"Да" : L"Нет") << endl;
        wcout << L"    Животные: ";
        if (enclosures[index].animals.empty()) {
            wcout << L"Нет\n";
        }
        else {
            for (const auto& animal : enclosures[index].animals) {
                wcout << animal.name.c_str() << L" ";
            }
            wcout << endl;
        }
    }
    else {
        wcout << L"Некорректный номер вольера.\n";
    }
}

// Функция отображения сообщения о делегации
void displayDelegationMessage(bool initial) {
    if (initial) {
        wcout << L"\nВ ваш зоопарк прибыла делегация с Суперземли TOI-1452 b!\n";
        wcout << L"Их очень расстраивает отсутствие морских видов животных.\n";
        wcout << L"Они великодушно предлагают выслать вам своих самых интересных морских животных,\n";
        wcout << L"если вы сможете предоставить им подходящие условия!\n";
        wcout << L"Помните, что для морских животных требуется отдельный вольер с типом 'Морские'!\n";
    }
}

// Функция удаления не-русских символов из строки
wstring removeNonRussian(const wstring& text) {
    wregex re(L"[^а-яА-ЯёЁ\\s]");
    return regex_replace(text, re, L"");
}

// Функция локализации текста (замены непонятных символов)
wstring localizeText(const wstring& text) {
    unordered_map<wstring, wstring> localizationMap = {
        {L"т√їюф", L"привет"},
        {L"шыш", L"что"},
        {L"чртхЁ°хэш", L"читаешь"}
    };

    wstring result = text;
    for (const auto& pair : localizationMap) {
        size_t pos = result.find(pair.first);
        while (pos != wstring::npos) {
            result.replace(pos, pair.first.length(), pair.second);
            pos = result.find(pair.first, pos + pair.second.length());
        }
    }
    return result;
}

// Функция удаления лишних пробелов из строки
wstring removeExtraSpaces(const wstring& text) {
    wstring result = text;
    result.erase(unique(result.begin(), result.end(), [](wchar_t a, wchar_t b) {
        return iswspace(a) && iswspace(b);
        }), result.end());

    if (!result.empty() && iswspace(result.front())) {
        result.erase(result.begin());
    }
    if (!result.empty() && iswspace(result.back())) {
        result.pop_back();
    }
    return result;
}

// Главная функция программы
int wmain(int argc, wchar_t* argv[]) {
    // Настройка консоли для поддержки Unicode
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    // Инициализация генератора случайных чисел
    srand((unsigned int)time(nullptr));

    // Приветствие и ввод названия зоопарка
    wcout << L"\n=== Зоопарк Менеджер ===\n";
    wcout << L"Введите название зоопарка: ";

    wstring name;
    getline(wcin >> ws, name);

    // Обработка пустого названия
    if (name.empty()) {
        wcout << L"Название зоопарка не может быть пустым. Используется название 'Мой Зоопарк'." << endl;
        name = L"Мой Зоопарк";
    }

    // Очистка и нормализация названия
    wstring cleanedName = removeNonRussian(name);
    wstring localizedName = localizeText(cleanedName);
    wstring finalName = removeExtraSpaces(localizedName);

    if (finalName.empty()) {
        finalName = L"Мой Зоопарк";
    }

    // Создание зоопарка и запуск игры
    Zoo zoo(finalName);
    displayDelegationMessage(true);
    zoo.menu();

    return 0;
}
