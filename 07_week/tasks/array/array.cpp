#include <initializer_list>
#include <utility>
#include <algorithm>

template <typename T, size_t N>

class Array {
private:
    T arr_[N];
    
public:
// Конструторы
    Array() = default;
    
    Array(std::initializer_list<T> il) {
        size_t j = 0;
        for (auto it = il.begin(); it != il.end() && j < N; ++it, ++j) {
            arr_[j] = *it;
        }
    }
    
    Array(const Array& other) {
        for (size_t i = 0; i < N; ++i) {
            arr_[i] = other.arr_[i];
        }
    }
    
    Array(Array&& other) noexcept {
        for (size_t i = 0; i < N; ++i) {
            arr_[i] = std::move(other.arr_[i]);
        }
    }
// Деструктор
    ~Array() = default;
    
// Операторы присваивания и индексирования
    Array& operator=(const Array& other) {
        if (this != &other) {
            for (size_t i = 0; i < N; ++i) {
                arr_[i] = other.arr_[i];
            }
        }
        return *this;
    }
    
   Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < N; ++i) {
                arr_[i] = std::move(other.arr_[i]);
            }
        }
        return *this;
   }
   
   T& operator[](size_t i) {
        return arr_[i];   
   }
   
   const T& operator[](size_t i) const {
        return arr_[i];   
   }
   
// Методы
    T& Front() { return arr_[0]; }
    
    const T& Front() const { return arr_[0]; }
    T& Back() { return arr_[N-1]; }
    
    const T& Back() const { return arr_[N-1]; }
    
    T* Data() { return arr_; }
    
    const T* Data() const { return arr_; }
    
    bool Empty() const { return N == 0; }
    
    size_t Size() const { return N; }
    
    void Fill(const T& val) {
        for (size_t i = 0; i < N; ++i) {
            arr_[i] = val;
        }
    }
    
    void Swap(Array& other) noexcept {
        for (size_t i = 0; i < N; ++i) {
            std::swap(arr_[i], other.arr_[i]);
        }
    }

// Методы для rang-based for
    T* begin() { return arr_; }
    const T* begin() const { return arr_; }

    T* end() { return arr_ + N; }
    const T* end() const { return arr_ + N; }

    const T* cbegin() const { return arr_; }
    const T* cend() const { return arr_ + N; }
};

//Операторы сравнения
template <typename T, size_t N>
bool operator==(const Array<T,N>& lhs, const Array<T,N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template <typename T, size_t N>
bool operator!=(const Array<T,N>& lhs, const Array<T,N>& rhs) {
    return !(lhs == rhs);
}

template <typename T, size_t N>
bool operator<(const Array<T,N>& lhs, const Array<T,N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] < rhs[i]) return true;
        if (rhs[i] < lhs[i]) return false;
    }
    return false;
}

template <typename T, size_t N>
bool operator<=(const Array<T,N>& lhs, const Array<T,N>& rhs) {
    return !(rhs < lhs);
}

template <typename T, size_t N>
bool operator>(const Array<T,N>& lhs, const Array<T,N>& rhs) {
    return rhs < lhs;
}

template <typename T, size_t N>
bool operator>=(const Array<T,N>& lhs, const Array<T,N>& rhs) {
    return !(lhs < rhs);
}

// Внешние Методы
template <typename T, size_t N>
void swap(Array<T,N>& lhs, Array<T,N>& rhs) noexcept {
    lhs.Swap(rhs);
}   

template <size_t I, typename T, size_t N>
T& get(Array<T,N>& arr) {
    return arr[I];
}

template <size_t I, typename T, size_t N>
const T& get(const Array<T,N>& arr) {
    return arr[I];
}

template <size_t I, typename T, size_t N>
T&& get(Array<T,N>&& arr) {
    return std::move(arr[I]);
}