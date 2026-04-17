// 02_multiple_type_params.cpp
// 演示：多个模板类型参数

#include <iostream>
#include <string>

// ============================================================
// 1. 两个类型参数的函数模板
// ============================================================

// T1 和 T2 可以是不同的类型
template <typename T1, typename T2>
void printPair(T1 first, T2 second) {
    std::cout << "(" << first << ", " << second << ")" << std::endl;
}

// ============================================================
// 2. 返回类型与多类型参数
// ============================================================

// 问题：当 T1 和 T2 不同时，返回类型应该是什么？
// 方法一：手动指定返回类型为其中一个类型（可能丢失精度）
template <typename T1, typename T2>
T1 addV1(T1 a, T2 b) {
    return a + b;  // 返回类型固定为 T1
}

// 方法二：使用第三个模板参数作为返回类型
template <typename ReturnType, typename T1, typename T2>
ReturnType addV2(T1 a, T2 b) {
    return a + b;
}

// 方法三（C++11）：使用尾置返回类型 + decltype
// decltype(a + b) 让编译器自动推导 a + b 的结果类型
template <typename T1, typename T2>
auto addV3(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}

int main() {
    // ============================================================
    // 1. 两个不同类型的参数
    // ============================================================
    std::cout << "=== printPair ===" << std::endl;
    printPair(42, 3.14);           // T1=int, T2=double
    printPair("hello", 100);       // T1=const char*, T2=int
    printPair(std::string("world"), 'A');  // T1=std::string, T2=char

    // ============================================================
    // 2. 不同版本的 add 函数
    // ============================================================
    std::cout << "\n=== add 函数的不同版本 ===" << std::endl;

    // addV1：返回类型是 T1 (int)，double 部分会被截断
    int result1 = addV1(1, 2.7);
    std::cout << "addV1(1, 2.7) = " << result1 << " (返回 int，小数被截断)" << std::endl;

    // addV1：返回类型是 T1 (double)，结果正确
    double result1b = addV1(1.0, 2);
    std::cout << "addV1(1.0, 2) = " << result1b << " (返回 double)" << std::endl;

    // addV2：显式指定返回类型为 double
    double result2 = addV2<double>(1, 2.7);
    std::cout << "addV2<double>(1, 2.7) = " << result2 << " (显式指定返回 double)" << std::endl;

    // addV3：使用 decltype 自动推导，int + double = double
    auto result3 = addV3(1, 2.7);
    std::cout << "addV3(1, 2.7) = " << result3 << " (decltype 自动推导)" << std::endl;

    return 0;
}
