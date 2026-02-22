#include <string>

// Хранит свединия об объекте
struct ControlCount 
{
    size_t shared_count;
    size_t weak_count;
    std::string* ptr_cc;

    explicit ControlCount(std::string* p) : shared_count(1), weak_count(0), ptr_cc(p) {}

    ~ControlCount() { delete ptr_cc; }
};



class SharedPtr {
private:
std::string* ptr_;
ControlCount* control;

void ResourceRelease() {
    if (control) { 
        --control->shared_count; 
        if (!control->shared_count) {
            if (!control->weak_count) {
                delete control;
            }
        }
        ptr_ = nullptr;
        control = nullptr;
    }
}

friend class WeakPtr;
public:
// Дефолтный конструктор
SharedPtr() : ptr_(nullptr), control(nullptr) {}

// Конструктор от сырого указателя
explicit SharedPtr(std::string* p) : ptr_(p), control(nullptr) {
    if (ptr_) {
        control = new ControlCount(ptr_);
    }
}

//  Конструктор копирования
SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), control(other.control) {
    if (control) {
        ++control->shared_count;
    } 
}

// Конструктор перемещения
SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), control(other.control) {
    other.ptr_ = nullptr;
    other.control = nullptr;
}

// Деструктор
~SharedPtr() {   
    if (!control) { return; }
    --control->shared_count;

    if (control->shared_count > 0) { return; }

    if (control->weak_count > 0) { return; }
    delete control;
}

// Оператор присвоения при копировании
SharedPtr& operator=(const SharedPtr& other);

// Оператор присвоения при перемещении
SharedPtr& operator=(SharedPtr&& other) noexcept;

// Оператор bool, *, ->
explicit operator bool() const { return ptr_ != nullptr; }

std::string& operator*() const { return *ptr_; }

std::string* operator->() const { return ptr_; }

// Методы
std::string* Get() const { return ptr_; }
void Reset(std::string* p);
void Reset();
void Swap(SharedPtr& other);
size_t UseCount() const;
};

// Реализация методов
void SharedPtr::Reset(std::string* p) {
    ResourceRelease();
    ptr_ = p;
    (ptr_) ? control = new ControlCount(p) : control = nullptr;
}

void SharedPtr::Reset() {
    ResourceRelease();
    ptr_ = nullptr;
    control = nullptr;
}

void SharedPtr::Swap(SharedPtr& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(control, other.control);
}

size_t SharedPtr::UseCount() const {
    return (control) ? control->shared_count : 0;
}

// Реализация операторов
SharedPtr& SharedPtr::operator=(const SharedPtr& other) {
    if (this != &other) {
        ResourceRelease();

        ptr_ = other.ptr_;
        control = other.control;
        if (control) {
            ++control->shared_count;
        }
    }
    return *this;
}

SharedPtr& SharedPtr::operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
        ResourceRelease();

        ptr_ = other.ptr_;
        control = other.control;
        other.ptr_ = nullptr;
        other.control = nullptr;
    }
    return *this;
}



class WeakPtr {
private:
    ControlCount* control_w;

    void ResourceRelease_w() {
        if (control_w) {
            --control_w->weak_count;
            if (control_w->weak_count == 0 && control_w->shared_count == 0) {
                delete control_w;
            }
        }
    }

public:
    // Дефолтный конструктор
    WeakPtr() : control_w(nullptr) {} 

    // Констрктор от SharedPtr
    WeakPtr(const SharedPtr& sp) : control_w(sp.control) {
        if (control_w) {
            ++control_w->weak_count;
        }
    }

    // Копирующий констрктор
    WeakPtr(const WeakPtr& other) : control_w(other.control_w)  {
      if (control_w) {
            ++control_w->weak_count;
        }
    }

    // Конструтор перемещения
    WeakPtr(WeakPtr&& other) noexcept : control_w(other.control_w) {
        other.control_w = nullptr;

    }

    // Деструтор
    ~WeakPtr() {
        if (control_w) {
            --control_w->weak_count;
            if (control_w->shared_count == 0 && control_w->weak_count == 0) {
                delete control_w;
            }
        }

    }

    // Операторы
    WeakPtr& operator=(const WeakPtr& other);
    WeakPtr& operator=(WeakPtr&& other) noexcept;
    WeakPtr& operator=(const SharedPtr& sp);

    // Методы
    void Reset();
    void Swap(WeakPtr& other) noexcept;
    size_t UseCount() const;
    bool Expired() const;
    SharedPtr Lock() const;
    
};

// Реализация операторов
WeakPtr& WeakPtr::operator=(const WeakPtr& other) {
    if (this != &other) {
        ResourceRelease_w();
        control_w = other.control_w;
        if (control_w) {
            ++control_w->weak_count;
        }
    }
    return *this;
}

WeakPtr& WeakPtr::operator=(WeakPtr&& other) noexcept {
    if (this != &other) {
        ResourceRelease_w();
        control_w = other.control_w;
        other.control_w = nullptr;
    }
    return *this;
}

WeakPtr& WeakPtr::operator=(const SharedPtr& sp) {
    ResourceRelease_w();
    control_w = sp.control;
    if (control_w) {
        ++control_w->weak_count;
    }
    return *this;
}

// Реализация методов
void WeakPtr::Reset() {
    ResourceRelease_w();
    control_w = nullptr;
}

void WeakPtr::Swap(WeakPtr& other) noexcept {
    std::swap(control_w, other.control_w);
}

size_t WeakPtr::UseCount() const {
    return (control_w) ? control_w->shared_count : 0;
}

bool WeakPtr::Expired() const {
    return !control_w || control_w->shared_count == 0;
}

SharedPtr WeakPtr::Lock() const {
    if (Expired()) return SharedPtr();

    SharedPtr sp;
    sp.ptr_ = control_w->ptr_cc;
    sp.control = control_w;
    ++sp.control->shared_count;
    return sp;
}

// Функции

SharedPtr MakeShared(const std::string& s) {
    return SharedPtr(new std::string(s));
}

SharedPtr MakeShared(std::string&& s) {
    return SharedPtr(new std::string(std::move(s)));
}

void Swap(SharedPtr& lhs, SharedPtr& rhs) noexcept {
    lhs.Swap(rhs);
}

void Swap(WeakPtr& lhs, WeakPtr& rhs) noexcept {
    lhs.Swap(rhs);
}