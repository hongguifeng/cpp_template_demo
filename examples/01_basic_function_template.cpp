// 01_basic_function_template.cpp
// 演示：最基本的函数模板定义和使用

#include <iostream>
#include <string>

// ============================================================
// 1. 没有模板时的问题：需要为每种类型写重复的函数
// ============================================================

// 比较两个 int 的最大值
int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

// 比较两个 double 的最大值 —— 逻辑完全相同，只是类型不同
double maxDouble(double a, double b) {
    return (a > b) ? a : b;
}

// ============================================================
// 2. 使用函数模板解决重复代码的问题
// ============================================================

// template 关键字告诉编译器：接下来是一个模板定义
// <typename T> 声明了一个「类型参数」T
// T 是一个占位符，代表"某种类型"，具体是什么类型在调用时确定
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}

// 注意：typename 也可以写成 class，两者在这里完全等价：
// template <class T>
// T myMax(T a, T b) { ... }

int main() {
    // ============================================================
    // 3. 调用函数模板
    // ============================================================

    // 调用方式一：显式指定模板参数类型
    // myMax<int> 告诉编译器："请把 T 替换为 int"
    std::cout << "myMax<int>(3, 5) = " << myMax<int>(3, 5) << std::endl;

    // 调用方式二：让编译器自动推导类型（推荐）
    // 编译器看到参数 3.14 和 2.72 都是 double，自动推导 T = double
    std::cout << "myMax(3.14, 2.72) = " << myMax(3.14, 2.72) << std::endl;

    // 对 char 类型也能工作（比较 ASCII 值）
    std::cout << "myMax('a', 'z') = " << myMax('a', 'z') << std::endl;

    // 对 std::string 也能工作（按字典序比较）
    std::string s1 = "apple";
    std::string s2 = "banana";
    std::cout << "myMax(\"apple\", \"banana\") = " << myMax(s1, s2) << std::endl;

    // ============================================================
    // 4. 与非模板函数的对比
    // ============================================================
    std::cout << "\n--- 非模板函数 ---" << std::endl;
    std::cout << "maxInt(3, 5) = " << maxInt(3, 5) << std::endl;
    std::cout << "maxDouble(3.14, 2.72) = " << maxDouble(3.14, 2.72) << std::endl;

    return 0;
}
