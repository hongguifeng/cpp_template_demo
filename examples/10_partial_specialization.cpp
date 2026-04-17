// 10_partial_specialization.cpp
// 演示：模板的偏特化（Partial Specialization）
// 注意：只有类模板可以偏特化，函数模板不能偏特化

#include <iostream>
#include <string>

// ============================================================
// 1. 基本偏特化：针对指针类型
// ============================================================

// 主模板
template <typename T>
class Storage {
    T value;
public:
    Storage(const T& v) : value(v) {}

    void print() const {
        std::cout << "Storage<T>: value = " << value << std::endl;
    }

    const T& get() const { return value; }
};

// 偏特化：当 T 是指针类型 U* 时
// 注意语法：template <typename U> 仍有一个参数，但 Storage<U*> 偏特化了主模板
template <typename U>
class Storage<U*> {
    U* ptr;
    bool ownsMemory;
public:
    // 指针版本：需要管理内存
    Storage(U* p, bool own = false) : ptr(p), ownsMemory(own) {}

    ~Storage() {
        if (ownsMemory) {
            delete ptr;
        }
    }

    void print() const {
        std::cout << "Storage<U*>: 指针指向 " << *ptr
                  << " (地址: " << ptr << ")" << std::endl;
    }

    U& get() const { return *ptr; }
};

// ============================================================
// 2. 多参数的偏特化
// ============================================================

// 主模板：两个类型参数
template <typename T1, typename T2>
class Pair {
    T1 first;
    T2 second;
public:
    Pair(const T1& f, const T2& s) : first(f), second(s) {}

    void print() const {
        std::cout << "Pair<T1, T2>: (" << first << ", " << second << ")" << std::endl;
    }
};

// 偏特化 1：两个类型相同
template <typename T>
class Pair<T, T> {
    T first;
    T second;
public:
    Pair(const T& f, const T& s) : first(f), second(s) {}

    void print() const {
        std::cout << "Pair<T, T> (同类型): (" << first << ", " << second << ")" << std::endl;
    }

    // 同类型版本可以提供额外功能
    bool areEqual() const { return first == second; }
};

// 偏特化 2：第二个类型是 int
template <typename T>
class Pair<T, int> {
    T first;
    int second;
public:
    Pair(const T& f, int s) : first(f), second(s) {}

    void print() const {
        std::cout << "Pair<T, int>: (" << first << ", " << second << ")" << std::endl;
    }
};

// 偏特化 3：两个都是指针
template <typename T1, typename T2>
class Pair<T1*, T2*> {
    T1* first;
    T2* second;
public:
    Pair(T1* f, T2* s) : first(f), second(s) {}

    void print() const {
        std::cout << "Pair<T1*, T2*> (双指针): (*" << *first
                  << ", *" << *second << ")" << std::endl;
    }
};

// ============================================================
// 3. 实际应用：remove_pointer 类型特征
// ============================================================

// 主模板：T 不是指针，结果就是 T 本身
template <typename T>
struct RemovePointer {
    typedef T type;
};

// 偏特化：T 是 U*，去掉一层指针
template <typename U>
struct RemovePointer<U*> {
    typedef U type;
};

// 偏特化：T 是 const U*
template <typename U>
struct RemovePointer<const U*> {
    typedef U type;
};

// ============================================================
// 4. 实际应用：判断两个类型是否相同
// ============================================================

// 主模板：两个不同的类型
template <typename T, typename U>
struct IsSame {
    static const bool value = false;
};

// 偏特化：两个相同的类型
template <typename T>
struct IsSame<T, T> {
    static const bool value = true;
};

int main() {
    // ============================================================
    // 1. 指针偏特化
    // ============================================================
    std::cout << "=== Storage 偏特化 ===" << std::endl;

    // 使用主模板 Storage<T>
    Storage<int> s1(42);
    s1.print();

    Storage<std::string> s2("hello");
    s2.print();

    // 使用偏特化版本 Storage<U*>
    int x = 100;
    Storage<int*> s3(&x);
    s3.print();

    double d = 3.14;
    Storage<double*> s4(&d);
    s4.print();

    // ============================================================
    // 2. Pair 的多种偏特化
    // ============================================================
    std::cout << "\n=== Pair 偏特化 ===" << std::endl;

    // 使用主模板：Pair<string, double>
    Pair<std::string, double> p1("pi", 3.14);
    p1.print();

    // 使用偏特化 Pair<T, T>：两个类型都是 double
    Pair<double, double> p2(1.5, 2.5);
    p2.print();
    std::cout << "  areEqual: " << (p2.areEqual() ? "是" : "否") << std::endl;

    // 使用偏特化 Pair<T, int>：第二个是 int
    Pair<std::string, int> p3("age", 25);
    p3.print();

    // 注意：Pair<int, int> 同时匹配 Pair<T,T> 和 Pair<T,int>，会导致歧义！
    // 编译器无法确定选哪个 -> 编译错误
    // Pair<int, int> ambiguous(1, 2);  // 这行会报错

    // 使用偏特化 Pair<T1*, T2*>：两个都是指针
    int a = 10;
    double b = 3.14;
    Pair<int*, double*> p4(&a, &b);
    p4.print();

    // 说明：Pair<int, int> 同时匹配 Pair<T,T> 和 Pair<T,int>
    // 两个偏特化同样"特殊"，编译器无法决定用哪个，所以会报歧义错误

    // ============================================================
    // 3. RemovePointer
    // ============================================================
    std::cout << "\n=== RemovePointer ===" << std::endl;

    // RemovePointer<int>::type 就是 int
    std::cout << "RemovePointer<int>: "
              << IsSame<RemovePointer<int>::type, int>::value << std::endl;

    // RemovePointer<int*>::type 就是 int
    std::cout << "RemovePointer<int*>: "
              << IsSame<RemovePointer<int*>::type, int>::value << std::endl;

    // RemovePointer<const int*>::type 就是 int
    std::cout << "RemovePointer<const int*>: "
              << IsSame<RemovePointer<const int*>::type, int>::value << std::endl;

    // ============================================================
    // 4. IsSame
    // ============================================================
    std::cout << "\n=== IsSame ===" << std::endl;

    std::cout << "IsSame<int, int>:       " << IsSame<int, int>::value << std::endl;
    std::cout << "IsSame<int, double>:    " << IsSame<int, double>::value << std::endl;
    std::cout << "IsSame<int, const int>: " << IsSame<int, const int>::value << std::endl;

    return 0;
}
