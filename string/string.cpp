#include <cassert>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <stdexcept>

class mystring {
private:
    size_t capacity;
    size_t len;
    char* buffer;

    void resize(size_t cap) {
        assert(cap >= len); // Изменил cap >= capacity на cap >= len
        char* newbuffer = new char[cap + 1]; // Добавил + 1 для '\0'
        if (!newbuffer) {
            throw std::bad_alloc();
        }
        if (buffer) {
            std::copy(buffer, buffer + len, newbuffer);
            delete[] buffer;
        }
        newbuffer[len] = '\0'; // Не забываем про нулевой терминатор
        buffer = newbuffer;
        capacity = cap;
    }

    void expand(size_t by) {
        if (len + by > capacity) {
            resize(std::max((len + by) * 2, capacity * 2));
        }
    }

public:
    // Конструкторы
    mystring() : capacity(0), len(0), buffer(nullptr) {}

    mystring(const char* str) {
        if (str == nullptr) {
            capacity = 0;
            len = 0;
            buffer = nullptr;
            return;
        }
        len = std::strlen(str);
        capacity = len;
        buffer = new char[capacity + 1];
        if (!buffer) throw std::bad_alloc();
        std::copy(str, str + len + 1, buffer);

    }
    mystring(const mystring& other) : capacity(other.capacity), len(other.len) {
        buffer = new char[capacity + 1];
        if (!buffer)
            throw std::bad_alloc();
        std::copy(other.buffer, other.buffer + len + 1, buffer);
    }
    // Деструктор
    ~mystring() {
        delete[] buffer;
    }
    size_t getLength() const { return len; }
    // Оператор []
    char& operator[](size_t index) {
        if (index >= len) {
            throw std::out_of_range("Out of range");
        }
        return buffer[index];
    }


};