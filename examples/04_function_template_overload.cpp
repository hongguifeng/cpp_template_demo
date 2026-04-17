// 04_function_template_overload.cpp
// 演示：函数模板的重载和与普通函数的关系

#include <iostream>
#include <cstring>

// ============================================================
// 1. 函数模板与普通函数重载
// ============================================================

// 普通函数（非模板）
int myMax(int a, int b) {
    std::cout << "  [调用了普通函数 myMax(int, int)]" << std::endl;
    return (a > b) ? a : b;
}

// 函数模板
template <typename T>
T myMax(T a, T b) {
    std::cout << "  [调用了模板函数 myMax<T>(T, T)]" << std::endl;
    return (a > b) ? a : b;
}

// ============================================================
// 2. 函数模板之间的重载
// ============================================================

// 单参数版本
template <typename T>
T myMax(T a) {
    std::cout << "  [调用了单参数模板 myMax<T>(T)]" << std::endl;
    return a;
}

// 三参数版本
template <typename T>
T myMax(T a, T b, T c) {
    std::cout << "  [调用了三参数模板 myMax<T>(T, T, T)]" << std::endl;
    return myMax(myMax(a, b), c);
}

// ============================================================
// 3. 针对 C 字符串的特殊重载
// ============================================================

// 对 const char* 的特化处理（普通函数重载）
const char* myMax(const char* a, const char* b) {
    std::cout << "  [调用了 const char* 重载]" << std::endl;
    return (std::strcmp(a, b) > 0) ? a : b;
}

int main() {
    // ============================================================
    // 规则：当模板函数和普通函数都能匹配时，优先选择普通函数
    // （除非模板能提供更好的匹配）
    // ============================================================

    std::cout << "=== 普通函数 vs 模板函数 ===" << std::endl;

    // 两个 int 参数：普通函数和模板都能匹配，优先普通函数
    std::cout << "myMax(3, 5) = " << myMax(3, 5) << std::endl;

    // 显式指定模板参数，强制使用模板版本
    std::cout << "myMax<>(3, 5) = " << myMax<>(3, 5) << std::endl;

    // 两个 double 参数：普通函数不匹配，使用模板
    std::cout << "myMax(3.0, 5.0) = " << myMax(3.0, 5.0) << std::endl;

    // ============================================================
    // 函数模板之间的重载
    // ============================================================
    std::cout << "\n=== 模板之间的重载 ===" << std::endl;

    std::cout << "myMax(42) = " << myMax(42) << std::endl;
    std::cout << "myMax(1, 2, 3) = " << myMax(1, 2, 3) << std::endl;

    // ============================================================
    // C 字符串的特殊处理
    // ============================================================
    std::cout << "\n=== C 字符串特殊处理 ===" << std::endl;

    // 使用 const char* 重载，按字符串内容比较
    std::cout << "myMax(\"apple\", \"banana\") = "
              << myMax("apple", "banana") << std::endl;

    return 0;
}
