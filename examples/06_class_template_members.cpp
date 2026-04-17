// 06_class_template_members.cpp
// 演示：类模板的成员函数在类外定义、静态成员、嵌套类型

#include <iostream>
#include <string>
#include <vector>

// ============================================================
// 1. 成员函数在类外定义
// ============================================================

template <typename T>
class Container {
private:
    std::vector<T> data;

public:
    // 类内声明
    void add(const T& value);
    T get(int index) const;
    int size() const;
    void print() const;
};

// 类外定义成员函数时，需要重复 template <typename T>
// 并使用 Container<T>:: 来限定
template <typename T>
void Container<T>::add(const T& value) {
    data.push_back(value);
}

template <typename T>
T Container<T>::get(int index) const {
    return data.at(index);
}

template <typename T>
int Container<T>::size() const {
    return static_cast<int>(data.size());
}

template <typename T>
void Container<T>::print() const {
    std::cout << "[";
    for (int i = 0; i < static_cast<int>(data.size()); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << data[i];
    }
    std::cout << "]" << std::endl;
}

// ============================================================
// 2. 类模板中的静态成员
// ============================================================

template <typename T>
class Counter {
private:
    T value;

public:
    // 静态成员：每个模板实例有自己独立的一份
    static int instanceCount;

    Counter(T v = T()) : value(v) {
        ++instanceCount;
    }

    ~Counter() {
        --instanceCount;
    }

    T getValue() const { return value; }

    static int getCount() { return instanceCount; }
};

// 静态成员在类外初始化，同样需要模板前缀
template <typename T>
int Counter<T>::instanceCount = 0;

// ============================================================
// 3. 类模板中的嵌套类型（typedef / using）
// ============================================================

template <typename T>
class DataWrapper {
public:
    // 定义嵌套类型别名
    typedef T value_type;               // C++98 风格
    using reference = T&;               // C++11 风格（推荐）
    using const_reference = const T&;

    // 嵌套类
    class Iterator {
    private:
        T* ptr;
    public:
        Iterator(T* p) : ptr(p) {}
        T& operator*() { return *ptr; }
        Iterator& operator++() { ++ptr; return *this; }
        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }
    };

private:
    T data[10];
    int count;

public:
    DataWrapper() : count(0) {}

    void add(const_reference val) {
        if (count < 10) {
            data[count++] = val;
        }
    }

    value_type get(int i) const { return data[i]; }

    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + count); }
};

int main() {
    // ============================================================
    // 1. 成员函数在类外定义
    // ============================================================
    std::cout << "=== Container ===" << std::endl;
    Container<int> c;
    c.add(10);
    c.add(20);
    c.add(30);
    std::cout << "容器内容: ";
    c.print();
    std::cout << "索引1的元素: " << c.get(1) << std::endl;

    Container<std::string> sc;
    sc.add("C++");
    sc.add("Template");
    sc.add("Tutorial");
    std::cout << "字符串容器: ";
    sc.print();

    // ============================================================
    // 2. 静态成员 —— 每个实例化类型有独立的静态成员
    // ============================================================
    std::cout << "\n=== Counter 静态成员 ===" << std::endl;

    Counter<int> ci1(10), ci2(20), ci3(30);
    Counter<double> cd1(1.1), cd2(2.2);

    // Counter<int> 有 3 个实例
    std::cout << "Counter<int> 实例数: " << Counter<int>::getCount() << std::endl;
    // Counter<double> 有 2 个实例（和 Counter<int> 是独立的）
    std::cout << "Counter<double> 实例数: " << Counter<double>::getCount() << std::endl;

    // ============================================================
    // 3. 嵌套类型
    // ============================================================
    std::cout << "\n=== DataWrapper 嵌套类型 ===" << std::endl;

    DataWrapper<int> dw;
    dw.add(100);
    dw.add(200);
    dw.add(300);

    // 使用嵌套类型别名
    DataWrapper<int>::value_type val = dw.get(0);
    std::cout << "value_type val = " << val << std::endl;

    // 使用嵌套 Iterator 类
    std::cout << "遍历: ";
    for (DataWrapper<int>::Iterator it = dw.begin(); it != dw.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
