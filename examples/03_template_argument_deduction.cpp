// 03_template_argument_deduction.cpp
// 演示：模板参数推导的规则和注意事项

#include <iostream>
#include <string>

// ============================================================
// 1. 基本类型推导
// ============================================================
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}

// ============================================================
// 2. 引用和 const 的推导
// ============================================================
template <typename T>
void showType(T param) {
    // 按值传递时，T 推导的结果会去掉 const 和引用
    std::cout << "按值传递 - sizeof(T) = " << sizeof(param) << std::endl;
}

template <typename T>
void showTypeRef(T& param) {
    // 按引用传递时，T 保留 const 属性（但去掉引用本身）
    std::cout << "按引用传递 - sizeof(T) = " << sizeof(param) << std::endl;
}

template <typename T>
void showTypeConstRef(const T& param) {
    // const 引用传递时，T 推导为去掉 const 后的类型
    std::cout << "按const引用传递 - sizeof(T) = " << sizeof(param) << std::endl;
}

// ============================================================
// 3. 数组和指针的推导
// ============================================================
template <typename T>
void passArray(T param) {
    // 按值传递时，数组退化为指针
    std::cout << "数组按值: sizeof(param) = " << sizeof(param)
              << " (指针大小)" << std::endl;
}

template <typename T>
void passArrayRef(T& param) {
    // 按引用传递时，保留数组类型（含大小信息）
    std::cout << "数组按引用: sizeof(param) = " << sizeof(param)
              << " (数组实际大小)" << std::endl;
}

// 利用引用传递获取数组大小的技巧
template <typename T, int N>
int getArraySize(T (&arr)[N]) {
    (void)arr;  // 避免未使用变量警告
    return N;
}

int main() {
    // ============================================================
    // 1. 基本推导
    // ============================================================
    std::cout << "=== 基本推导 ===" << std::endl;

    // 两个参数类型相同，推导成功
    std::cout << "myMax(3, 5) = " << myMax(3, 5) << std::endl;

    // 错误示例（已注释）：两个参数类型不同，无法推导 T
    // myMax(3, 5.0);  // 编译错误！int 和 double 都想做 T

    // 解决方法一：显式指定 T
    std::cout << "myMax<double>(3, 5.0) = " << myMax<double>(3, 5.0) << std::endl;

    // 解决方法二：手动转换使两个参数类型一致
    std::cout << "myMax(static_cast<double>(3), 5.0) = "
              << myMax(static_cast<double>(3), 5.0) << std::endl;

    // ============================================================
    // 2. 引用和 const 推导
    // ============================================================
    std::cout << "\n=== 引用和 const 推导 ===" << std::endl;

    int x = 42;
    const int cx = 100;
    const int& rcx = cx;

    // 按值传递：T 推导为 int（忽略 const 和引用）
    std::cout << "showType(x):   ";   showType(x);    // T = int
    std::cout << "showType(cx):  ";   showType(cx);   // T = int (const 被忽略)
    std::cout << "showType(rcx): ";   showType(rcx);  // T = int (const & 都忽略)

    // 按引用传递：T 保留 const
    std::cout << "showTypeRef(x):   "; showTypeRef(x);    // T = int
    std::cout << "showTypeRef(cx):  "; showTypeRef(cx);   // T = const int

    // ============================================================
    // 3. 数组推导
    // ============================================================
    std::cout << "\n=== 数组推导 ===" << std::endl;

    int arr[10] = {};
    passArray(arr);     // T = int*（数组退化为指针）
    passArrayRef(arr);  // T = int[10]（保留完整数组类型）

    // 利用模板获取数组大小
    int nums[] = {1, 2, 3, 4, 5};
    std::cout << "数组大小 = " << getArraySize(nums) << std::endl;

    const char str[] = "Hello";
    std::cout << "字符串数组大小 = " << getArraySize(str) << " (含 '\\0')" << std::endl;

    return 0;
}
