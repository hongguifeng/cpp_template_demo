// 12_variadic_templates_basic.cpp
// 演示：可变参数模板（Variadic Templates）—— C++11 引入
// 这是 C++11 最重要的新特性之一

#include <iostream>
#include <string>

// ============================================================
// 1. 可变参数模板的基本语法
// ============================================================

// typename... Args 声明了一个「参数包」（parameter pack）
// Args 可以代表 0 个或多个类型
// args 可以包含 0 个或多个对应的值

// 递归终止条件：0 个参数的版本
void print() {
    std::cout << "(结束)" << std::endl;
}

// 可变参数版本
// T 是第一个参数的类型，Args... 是剩余参数的类型包
// first 是第一个参数，rest... 是剩余参数包
template <typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    std::cout << first;
    if (sizeof...(rest) > 0) {
        std::cout << ", ";
    }
    // 递归调用：把剩余参数包展开传递
    print(rest...);
}

// ============================================================
// 2. sizeof... 操作符
// ============================================================

// sizeof...(Args) 返回参数包中类型的个数（编译期常量）
// sizeof...(args) 返回参数包中值的个数

template <typename... Args>
void countArgs(const Args&... args) {
    std::cout << "类型参数个数: " << sizeof...(Args) << std::endl;
    std::cout << "值参数个数:   " << sizeof...(args) << std::endl;
}

// ============================================================
// 3. 可变参数的求和
// ============================================================

// 递归终止：单个参数
template <typename T>
T sum(T value) {
    return value;
}

// 递归展开：取出第一个，对剩余求和
template <typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

// ============================================================
// 4. 可变参数的最大值
// ============================================================

template <typename T>
T myMax(T value) {
    return value;
}

template <typename T, typename... Args>
T myMax(T first, Args... rest) {
    T restMax = myMax(rest...);
    return (first > restMax) ? first : restMax;
}

// ============================================================
// 5. 可变参数的类型安全格式化输出
// ============================================================

// 类似 printf，但是类型安全
void typeSafePrintf(const char* format) {
    // 终止条件：格式字符串遍历完毕
    while (*format) {
        if (*format == '%') {
            // 有占位符但没有对应的参数
            std::cout << "[MISSING_ARG]";
            ++format;
        } else {
            std::cout << *format;
        }
        ++format;
    }
}

template <typename T, typename... Args>
void typeSafePrintf(const char* format, const T& value, const Args&... args) {
    while (*format) {
        if (*format == '%') {
            // 遇到 %，输出当前参数，递归处理剩余
            std::cout << value;
            typeSafePrintf(format + 1, args...);
            return;
        }
        std::cout << *format;
        ++format;
    }
}

int main() {
    // ============================================================
    // 1. 基本 print
    // ============================================================
    std::cout << "=== 可变参数 print ===" << std::endl;

    print(1);                           // 1 个参数
    print(1, 2.5);                      // 2 个参数
    print(1, 2.5, "hello");            // 3 个参数
    print(1, 2.5, "hello", 'A', true); // 5 个参数

    // ============================================================
    // 2. sizeof...
    // ============================================================
    std::cout << "\n=== sizeof... ===" << std::endl;
    countArgs();                  // 0 个参数
    countArgs(1);                 // 1 个参数
    countArgs(1, 2.5, "hello");  // 3 个参数

    // ============================================================
    // 3. 可变参数求和
    // ============================================================
    std::cout << "\n=== 可变参数求和 ===" << std::endl;
    std::cout << "sum(1) = " << sum(1) << std::endl;
    std::cout << "sum(1, 2, 3) = " << sum(1, 2, 3) << std::endl;
    std::cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "sum(1.5, 2.5, 3.0) = " << sum(1.5, 2.5, 3.0) << std::endl;

    // ============================================================
    // 4. 可变参数最大值
    // ============================================================
    std::cout << "\n=== 可变参数最大值 ===" << std::endl;
    std::cout << "myMax(3) = " << myMax(3) << std::endl;
    std::cout << "myMax(3, 7, 2) = " << myMax(3, 7, 2) << std::endl;
    std::cout << "myMax(3, 7, 2, 9, 1) = " << myMax(3, 7, 2, 9, 1) << std::endl;

    // ============================================================
    // 5. 类型安全 printf
    // ============================================================
    std::cout << "\n=== 类型安全 printf ===" << std::endl;
    typeSafePrintf("Name: %, Age: %, Score: %\n",
                   std::string("Alice"), 25, 98.5);
    typeSafePrintf("% + % = %\n", 1, 2, 3);

    return 0;
}
