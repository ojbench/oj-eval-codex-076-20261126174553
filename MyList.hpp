#ifndef MY_LIST_HPP
#define MY_LIST_HPP

#include <utility>

template <typename ValueType>
class MyList {
    struct Node {
        ValueType value;
        Node *prev = nullptr;
        Node *next = nullptr;
        explicit Node(const ValueType &v) : value(v) {}
    };

    Node *head_ = nullptr;
    Node *tail_ = nullptr;
    int size_ = 0;
    mutable Node *cursor_ = nullptr;
    mutable int cursor_index_ = 0;

    Node *node_at(int index) const {
        Node *p = head_;
        int position = 0;
        int distance = index;
        if (size_ - 1 - index < distance) {
            p = tail_;
            position = size_ - 1;
            distance = position - index;
        }
        int cursor_distance = cursor_index_ > index ? cursor_index_ - index
                                                   : index - cursor_index_;
        if (cursor_ && cursor_distance < distance) {
            p = cursor_;
            position = cursor_index_;
        }
        while (position < index) { p = p->next; ++position; }
        while (position > index) { p = p->prev; --position; }
        cursor_ = p;
        cursor_index_ = index;
        return p;
    }

public:
    MyList() = default;
    MyList(MyList &&obj) noexcept { swap(obj); }
    MyList(const MyList &obj) {
        try {
            for (Node *p = obj.head_; p; p = p->next) push_back(p->value);
        } catch (...) {
            clear();
            throw;
        }
    }
    ~MyList() { clear(); }

    MyList &operator=(const MyList &obj) {
        if (this != &obj) {
            MyList copy(obj);
            swap(copy);
        }
        return *this;
    }
    MyList &operator=(MyList &&obj) noexcept {
        if (this != &obj) {
            clear();
            swap(obj);
        }
        return *this;
    }
    void swap(MyList &obj) noexcept {
        using std::swap;
        swap(head_, obj.head_);
        swap(tail_, obj.tail_);
        swap(size_, obj.size_);
        swap(cursor_, obj.cursor_);
        swap(cursor_index_, obj.cursor_index_);
    }
    void push_back(const ValueType &value) {
        Node *p = new Node(value);
        p->prev = tail_;
        if (tail_) tail_->next = p;
        else head_ = p;
        tail_ = p;
        ++size_;
    }
    void push_front(const ValueType &value) {
        Node *p = new Node(value);
        if (cursor_) ++cursor_index_;
        p->next = head_;
        if (head_) head_->prev = p;
        else tail_ = p;
        head_ = p;
        ++size_;
    }
    void pop_back() {
        Node *p = tail_;
        if (cursor_ == p) cursor_ = nullptr;
        tail_ = p->prev;
        if (tail_) tail_->next = nullptr;
        else head_ = nullptr;
        delete p;
        --size_;
    }
    void pop_front() {
        Node *p = head_;
        if (cursor_ == p) cursor_ = nullptr;
        else if (cursor_) --cursor_index_;
        head_ = p->next;
        if (head_) head_->prev = nullptr;
        else tail_ = nullptr;
        delete p;
        --size_;
    }
    ValueType &front() const { return head_->value; }
    ValueType &back() const { return tail_->value; }
    int size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void insert(int index, const ValueType &value) {
        if (index == size_) return push_back(value);
        if (index == 0) return push_front(value);
        Node *next = node_at(index);
        Node *p = new Node(value);
        p->prev = next->prev;
        p->next = next;
        next->prev->next = p;
        next->prev = p;
        cursor_ = p;
        cursor_index_ = index;
        ++size_;
    }
    void erase(int index) {
        if (index == 0) return pop_front();
        if (index == size_ - 1) return pop_back();
        Node *p = node_at(index);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        cursor_ = p->next;
        cursor_index_ = index;
        delete p;
        --size_;
    }
    void clear() noexcept {
        while (head_) {
            Node *p = head_;
            head_ = p->next;
            delete p;
        }
        tail_ = nullptr;
        size_ = 0;
        cursor_ = nullptr;
    }
    void link(const MyList &obj) {
        // Copy first so self-linking is finite and allocation failure is atomic.
        MyList copy(obj);
        if (copy.empty()) return;
        if (tail_) {
            tail_->next = copy.head_;
            copy.head_->prev = tail_;
        } else {
            head_ = copy.head_;
        }
        tail_ = copy.tail_;
        size_ += copy.size_;
        copy.head_ = copy.tail_ = nullptr;
        copy.size_ = 0;
    }
    MyList cut(int index) {
        MyList result;
        if (index == size_) return result;
        if (index == 0) {
            result.swap(*this);
            return result;
        }
        result.head_ = node_at(index);
        result.tail_ = tail_;
        result.size_ = size_ - index;
        tail_ = result.head_->prev;
        tail_->next = nullptr;
        result.head_->prev = nullptr;
        size_ = index;
        cursor_ = nullptr;
        return result;
    }
};

#endif
