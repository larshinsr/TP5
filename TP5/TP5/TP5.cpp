#include <iostream>
#include <string>
#include <list>

//const char MAX_LINE_LENGTH = 50;

bool isNumber(std::string value) {
    for (size_t i = 0; i < value.size(); i++) {
        if (!isdigit(value[i])) {
            return false;
        }
    }
    return true;
}

#pragma region LIST

template <typename T>
struct Node {
    T data;
    Node* next;

    Node(const T& newData) : data(newData), next(nullptr) {}
};

template <typename T>
class List {
private:
    Node<T>* head;

public:
    // Конструктор
    List() : head(nullptr) {}

    // Деструктор
    ~List() {
        clear();
    }

    // Добавление элемента в конец списка
    void push_back(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (!head) {
            head = newNode;
            return;
        }

        Node<T>* current = head;
        while (current->next) {
            current = current->next;
        }

        current->next = newNode;
    }

    // Очистка списка
    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Удаление первого вхождения элемента из списка
    void remove(const T& value) {
        Node<T>* current = head;
        Node<T>* previous = nullptr;

        while (current) {
            if (current->data == value) {
                // Удаляем элемент из списка
                if (previous) {
                    previous->next = current->next;
                    delete current;
                    return;
                }
                else {
                    // Если удаляется первый элемент
                    Node<T>* temp = head;
                    head = head->next;
                    delete temp;
                    return;
                }
            }
            else {
                // Переходим к следующему элементу
                previous = current;
                current = current->next;
            }
        }
    }

    // Итератор для перебора элементов списка
    class Iterator {
    private:
        Node<T>* current;

    public:
        Iterator(Node<T>* start) : current(start) {}

        // Переход к следующему элементу
        Iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        // Получение значения текущего элемента
        T& operator*() {
            return current->data;
        }

        // Проверка на равенство
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    // Методы для работы с итератором
    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

#pragma endregion

#pragma region HASH_TABLE

class HashTable {
private:
    int tableSize;
    List<std::pair<std::string, std::string>>* table;
    int itemCount;
    float loadFactorThreshold;
    size_t hashFunction(const std::string& input) {
        size_t hashValue = 0;

        for (char ch : input) {
            hashValue += ch; // Используем функцию деления, просто суммируя коды символов
        }

        return hashValue % tableSize;
    }
    void rehash() {
        int newSize = tableSize * 2;
        List<std::pair<std::string, std::string>>* newTable = new List<std::pair<std::string, std::string>>[newSize];

        for (int i = 0; i < tableSize; ++i) {
            for (const auto& entry : table[i]) {
                int newIndex = hashFunction(entry.first) % newSize;
                newTable[newIndex].push_back(entry);
            }
        }
        delete[] table;
        table = newTable;
        tableSize = newSize;
    }
public:
    HashTable(int initialSize = 10, float loadFactor = 0.7) : tableSize(initialSize), itemCount(0), loadFactorThreshold(loadFactor) {
        table = new List<std::pair<std::string, std::string>>[tableSize];
    }

    // Деструктор
    ~HashTable() {
        delete[] table;
    }
    // Вставка элемента в хэш-таблицу
    void insert(const std::string& key, const std::string& value) {
        if (static_cast<float>(itemCount) / tableSize >= loadFactorThreshold) {
            rehash();
        }
        int index = hashFunction(key);
        table[index].push_back(std::make_pair(key, value));
        itemCount++;
    }

    // Получение значения по ключу из хэш-таблицы
    std::string get(const std::string& key) {
        int index = hashFunction(key);
        for (const auto& entry : table[index]) {
            if (entry.first == key) {
                return entry.second;
            }
        }
        return "Key not found";
    }

    // Удаление элемента по ключу из хэш-таблицы
    bool try_remove(const std::string& key) {
        int index = hashFunction(key);
        List<std::pair<std::string, std::string>>& entries = table[index];
        for (const auto& entry : entries) {
            if (entry.first == key) {
                entries.remove(entry);
                itemCount--;
                return true;
                
            }
        }
        return false;
    }

    // Вывод хэш-таблицы
    void displayTable() {
        for (int i = 0; i < tableSize; ++i) {
            std::cout << "List " << i << ": ";
            for (const auto& entry : table[i]) {
                std::cout << "{" << entry.first << ": " << entry.second << "} ";
            }
            std::cout << std::endl;
        }
    }
};

#pragma endregion

int main() {
    setlocale(LC_ALL, "RU");

    HashTable myHashTable;
    int choice = -1;
    std::string input;
    std::string key;
    std::string value;

    do {
        std::cout << "\n----------------------------\n\n";
        std::cout << "Menu:\n1. Add element \n2. Remove element\n3. Show tab\n0. Exit\n";
        std::cout << "Choice: ";

        std::cin >> input;
        if (isNumber(input)) {
            choice = atoi(input.c_str());
        }
        else {
            choice = -1;
        }

        switch (choice) {
        case 1:
            std::cout << "Add element\n";
            std::cout << "Input key: ";
            std::cin.get();
            std::getline(std::cin, key);
            
            std::cout << "input value: ";
            std::getline(std::cin, value);
            myHashTable.insert(key, value);
            break;

        case 2:
            std::cout << "Removeing element \n";
            std::cout << "Input key for deleteing: ";
            std::cin.get();
            std::getline(std::cin, key);
            if (myHashTable.try_remove(key)) {
                std::cout << "Key \"" + key + "\" successfully deleted\n";
            }
            else {
                std::cout << "Key \"" + key + "\" Not Found\n";
            }
            break;

        case 3:
            std::cout << "Printing nums tab \n";
            myHashTable.displayTable();
            break;

        case 0:
            std::cout << "Exit from program.\n";
            break;

        default:
            std::cout << "incorrect choice.\n";
        }

    } while (choice != 0);

    return 0;
}
