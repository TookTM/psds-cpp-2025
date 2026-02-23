#include <string>

class SimpleList {
private:
    // Структура узла
    struct Node {
        std::string val;
        Node* next;
        Node* prev;

        Node(const std::string& st) : val(st), next(nullptr), prev(nullptr) {}
        Node(std::string&& st) : val(std::move(st)), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    size_t count;
    
    // Создание узла 
    template<typename T>
    Node* NewNode(T&& st) {
        return new Node(std::forward<T>(st));
    }
    // Вспомогательные методы
    void Link(Node* new_n, Node* before_n) {
        new_n->next = before_n;
        new_n->prev = before_n->prev;
        before_n->prev->next = new_n;
        before_n->prev = new_n;
    }

    void Unlink(Node* n) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        delete n;
    }

public:
    // Конструктор по умолчанию
    SimpleList() : count(0) {
        head = new Node(std::string());
        head->next = head;
        head->prev = head;
    }
    
    // Копирующий конструктор
    SimpleList(const SimpleList& other) : SimpleList() {
        Node* current = other.head->next;
        while (current != other.head) {
            PushBack(current->val);
            current = current->next;
        }
    }
    
    // Перемещающий конструктор
    SimpleList(SimpleList&& other) noexcept 
        : head(other.head), count(other.count) {
        other.head = new Node(std::string());
        other.head->next = other.head;
        other.head->prev = other.head;
        other.count = 0;
    }
    
    // Деструктор
    ~SimpleList() {
        Clear();
        delete head;
    }
    
    // Копирующее присваивание
    SimpleList& operator=(const SimpleList& other) {
        if (this != &other) {
            //опробование метод copy-and-swap через внешнюю функцию Swap
            SimpleList temp(other);
            Swap(temp);
        }
        return *this;
    }
    
    // Перемещающее присваивание
    SimpleList& operator=(SimpleList&& other) noexcept {
        if (this != &other) {
            Clear();
            delete head;
            
            head = other.head;
            count = other.count;
            
            other.head = new Node(std::string());
            other.head->next = other.head;
            other.head->prev = other.head;
            other.count = 0;
        }
        return *this;
    }
    // Методы
    void Swap(SimpleList& other) noexcept {
        std::swap(head, other.head);
        std::swap(count, other.count);
    }
    
    size_t Size() const {
        return count;
    }
    
    bool Empty() const {
        return count == 0;
    }
    
    void PushBack(const std::string& value) {
        Node* new_node = NewNode(value);
        Link(new_node, head);
        ++count;
    }
    void PushBack(std::string&& value) {
        Node* new_node = NewNode(std::move(value));
        Link(new_node, head);
        ++count;
    }
    
    void PushFront(const std::string& value) {
        Node* new_node = NewNode(value);
        Link(new_node, head->next);
        ++count;
    }
    void PushFront(std::string&& value) {
        Node* new_node = NewNode(std::move(value));
        Link(new_node, head->next);
        ++count;
    }
    

    void PopBack() {
        if (!Empty()) {
            Unlink(head->prev);
            --count;
        }
    }
    
    void PopFront() {
        if (!Empty()) {
            Unlink(head->next);
            --count;
        }
    }
    
    std::string& Front() {
        return head->next->val;
    }
    const std::string& Front() const {
        return head->next->val;
    }
    

    std::string& Back() {
        return head->prev->val;
    }
    const std::string& Back() const {
        return head->prev->val;
    }
    
    void Clear() {
        while (!Empty()) {
            PopFront();
        }
    }
};

// Внешняя функция Swap
void Swap(SimpleList& lhs, SimpleList& rhs) noexcept {
    lhs.Swap(rhs);

};