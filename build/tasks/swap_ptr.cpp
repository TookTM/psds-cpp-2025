#include <stdexcept>

// Шаблонная функция для обмена значений любых типов
template<typename T>
void SwapPtr(T*& a, T*& b) {
    T* temp = a;
    a = b;
    b = temp;
}