// 24_cpp17_features.cpp
// 演示：C++17 对模板编程的改进
// 【注意】此文件需要 C++17 编译器支持！

#include <iostream>
#include <string>
#include <type_traits>
#include <tuple>

// ============================================================
// 1. if constexpr —— 编译期条件分支（C++17）
// ============================================================
// 在 C++11/14 中，需要用 enable_if 或标签分发来实现编译期分支
// C++17 的 if constexpr 让这件事变得极其简单

template <typename T>
std::string describe(const T& value) {
    if constexpr (std::is_integral<T>::value) {
        return std::to_string(value) + " 是整数";
    } else if constexpr (std::is_floating_point<T>::value) {
        return std::to_string(value) + " 是浮点数";
    } else if constexpr (std::is_same<T, std::string>::value) {
        return "\"" + value + "\" 是字符串";
    } else {
        return "未知类型";
    }
}

// 对比：C++11 需要 enable_if 实现同样功能（3个独立的函数）
// C++17 的 if constexpr 一个函数搞定！

// ============================================================
// 2. if constexpr 简化可变参数模板递归
// ============================================================

// C++11 方式：需要递归终止函数
// void print() { }  // 终止
// template <typename T, typename... Args>
// void print(T first, Args... rest) { ... print(rest...); }

// C++17 方式：不需要终止函数！
template <typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
        print(rest...);  // 只在还有参数时才递归
    } else {
        std::cout << std::endl;
    }
}

// ============================================================
// 3. 折叠表达式（Fold Expressions）—— C++17
// ============================================================

// C++11 方式的求和：需要递归
// template <typename T> T sum(T v) { return v; }
// template <typename T, typename... Rest>
// T sum(T first, Rest... rest) { return first + sum(rest...); }

// C++17 折叠表达式：一行搞定！
// (args + ...) 是「右折叠」: a + (b + (c + d))
// (... + args) 是「左折叠」: ((a + b) + c) + d

template <typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 右折叠
}

// 折叠表达式：逻辑与
template <typename... Args>
bool allTrue(Args... args) {
    return (args && ...);  // 右折叠
}

// 折叠表达式：逻辑或
template <typename... Args>
bool anyTrue(Args... args) {
    return (args || ...);  // 右折叠
}

// 折叠表达式：打印所有参数
template <typename... Args>
void printAll(const Args&... args) {
    ((std::cout << args << " "), ...);  // 逗号折叠
    std::cout << std::endl;
}

// 折叠表达式：检查某个值是否在给定值列表中
template <typename T, typename... Args>
bool isOneOf(const T& value, const Args&... args) {
    return ((value == args) || ...);
}

// ============================================================
// 4. 类模板参数推导 CTAD（C++17）
// ============================================================

template <typename T>
class Wrapper {
    T value;
public:
    Wrapper(T v) : value(v) {}
    T get() const { return value; }
};

// C++17 之前：Wrapper<int> w(42);
// C++17：Wrapper w(42);  // 自动推导 T = int

template <typename T1, typename T2>
class MyPair {
    T1 first;
    T2 second;
public:
    MyPair(T1 f, T2 s) : first(f), second(s) {}
    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }
};

// ============================================================
// 5. auto 非类型模板参数（C++17）
// ============================================================

// C++11: template <int N> struct Foo {};
// C++17: template <auto N> struct Foo {};  // N 的类型自动推导

template <auto Value>
struct Constant {
    static constexpr auto value = Value;

    void print() const {
        std::cout << "Constant<" << value << "> (size=" << sizeof(Value) << ")" << std::endl;
    }
};

// ============================================================
// 6. inline 变量（C++17）—— 解决模板静态成员 ODR 问题
// ============================================================

template <typename T>
struct TypeRegistry {
    // C++17: inline static 可以在头文件中直接初始化
    inline static int count = 0;
    inline static const std::string typeName = "unknown";
};

int main() {
    // ============================================================
    // 1. if constexpr
    // ============================================================
    std::cout << "=== if constexpr ===" << std::endl;
    std::cout << describe(42) << std::endl;
    std::cout << describe(3.14) << std::endl;
    std::cout << describe(std::string("hello")) << std::endl;
    std::cout << describe(true) << std::endl;  // bool 也是整数类型

    // ============================================================
    // 2. 简化递归
    // ============================================================
    std::cout << "\n=== if constexpr 简化递归 ===" << std::endl;
    print(1, 2.5, "hello", 'A');
    print("only one");

    // ============================================================
    // 3. 折叠表达式
    // ============================================================
    std::cout << "\n=== 折叠表达式 ===" << std::endl;
    std::cout << "sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "sum(1.5, 2.5) = " << sum(1.5, 2.5) << std::endl;

    std::cout << "allTrue(true, true, true) = " << allTrue(true, true, true) << std::endl;
    std::cout << "allTrue(true, false, true) = " << allTrue(true, false, true) << std::endl;

    std::cout << "anyTrue(false, false, true) = " << anyTrue(false, false, true) << std::endl;
    std::cout << "anyTrue(false, false, false) = " << anyTrue(false, false, false) << std::endl;

    std::cout << "printAll: ";
    printAll(1, 2.5, "hello", 'A');

    std::cout << "isOneOf(3, 1,2,3,4) = " << isOneOf(3, 1, 2, 3, 4) << std::endl;
    std::cout << "isOneOf(5, 1,2,3,4) = " << isOneOf(5, 1, 2, 3, 4) << std::endl;

    // 字符串连接
    std::string result = sum(std::string("Hello"), std::string(" "), std::string("World"));
    std::cout << "字符串折叠: " << result << std::endl;

    // ============================================================
    // 4. CTAD
    // ============================================================
    std::cout << "\n=== CTAD (类模板参数推导) ===" << std::endl;

    Wrapper w1(42);          // 推导为 Wrapper<int>
    Wrapper w2(3.14);        // 推导为 Wrapper<double>
    Wrapper w3(std::string("hello"));  // 推导为 Wrapper<string>
    std::cout << "w1 = " << w1.get() << std::endl;
    std::cout << "w2 = " << w2.get() << std::endl;
    std::cout << "w3 = " << w3.get() << std::endl;

    MyPair p1(42, 3.14);     // 推导为 MyPair<int, double>
    MyPair p2(std::string("age"), 25);  // 推导为 MyPair<string, int>
    std::cout << "p1 = (" << p1.getFirst() << ", " << p1.getSecond() << ")" << std::endl;
    std::cout << "p2 = (" << p2.getFirst() << ", " << p2.getSecond() << ")" << std::endl;

    // ============================================================
    // 5. auto 非类型模板参数
    // ============================================================
    std::cout << "\n=== auto 非类型模板参数 ===" << std::endl;
    Constant<42> ci;     // auto = int
    Constant<'A'> cc;    // auto = char
    Constant<true> cb;   // auto = bool
    ci.print();
    cc.print();
    cb.print();

    return 0;
}
