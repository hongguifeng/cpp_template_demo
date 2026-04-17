// 08_nontype_template_params.cpp
// 演示：非类型模板参数（Non-type Template Parameters）

#include <iostream>
#include <array>    // C++11
#include <string>

// ============================================================
// 1. 基本非类型模板参数
// ============================================================

// N 不是一个类型，而是一个编译期常量（整数值）
// 模板参数不仅可以是类型（typename T），也可以是值
template <typename T, int N>
class FixedArray {
private:
    T data[N];  // 数组大小在编译期确定

public:
    FixedArray() {
        for (int i = 0; i < N; ++i) {
            data[i] = T();  // 默认初始化
        }
    }

    T& operator[](int index) { return data[index]; }
    const T& operator[](int index) const { return data[index]; }

    int size() const { return N; }

    void fill(const T& value) {
        for (int i = 0; i < N; ++i) {
            data[i] = value;
        }
    }

    void print() const {
        std::cout << "[";
        for (int i = 0; i < N; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
};

// ============================================================
// 2. 非类型参数用于函数模板
// ============================================================

// 编译期计算幂运算：base^exp
template <int Base, int Exp>
struct Power {
    // 递归：Base^Exp = Base * Base^(Exp-1)
    static const int value = Base * Power<Base, Exp - 1>::value;
};

// 递归终止条件：任何数的 0 次幂等于 1
template <int Base>
struct Power<Base, 0> {
    static const int value = 1;
};

// ============================================================
// 3. 非类型参数作为默认值
// ============================================================

template <typename T, int InitSize = 16>
class DynamicBuffer {
private:
    T buffer[InitSize];
    int used;

public:
    DynamicBuffer() : used(0) {}

    bool add(const T& val) {
        if (used < InitSize) {
            buffer[used++] = val;
            return true;
        }
        return false;
    }

    int capacity() const { return InitSize; }
    int size() const { return used; }

    void print() const {
        std::cout << "Buffer(capacity=" << InitSize << ", used=" << used << "): [";
        for (int i = 0; i < used; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << buffer[i];
        }
        std::cout << "]" << std::endl;
    }
};

// ============================================================
// 4. 非类型参数的类型限制
// ============================================================
// C++11 允许的非类型模板参数类型：
//   - 整数类型（int, unsigned, long, char, bool, enum, etc.）
//   - 指向对象/函数的指针
//   - 左值引用
//   - std::nullptr_t
//   - 枚举类型
//
// 注意：float, double, class 类型在 C++11 中不能作为非类型参数
// （C++20 放宽了这个限制，允许浮点数和字面量类类型）

// bool 作为非类型参数
template <typename T, bool Ascending = true>
void sortTwo(T& a, T& b) {
    if (Ascending) {
        if (a > b) std::swap(a, b);
    } else {
        if (a < b) std::swap(a, b);
    }
}

int main() {
    // ============================================================
    // 1. 非类型参数的基本使用
    // ============================================================
    std::cout << "=== FixedArray ===" << std::endl;

    // FixedArray<int, 5> 创建大小为 5 的 int 数组
    FixedArray<int, 5> arr5;
    for (int i = 0; i < arr5.size(); ++i) {
        arr5[i] = (i + 1) * 10;
    }
    std::cout << "arr5: ";
    arr5.print();

    // FixedArray<int, 3> 和 FixedArray<int, 5> 是完全不同的类型！
    FixedArray<int, 3> arr3;
    arr3.fill(42);
    std::cout << "arr3: ";
    arr3.print();

    // 字符串类型的固定数组
    FixedArray<std::string, 2> strArr;
    strArr[0] = "Hello";
    strArr[1] = "World";
    std::cout << "strArr: ";
    strArr.print();

    // ============================================================
    // 2. 编译期计算
    // ============================================================
    std::cout << "\n=== 编译期幂运算 ===" << std::endl;

    // 这些值在编译期就已经计算好了
    std::cout << "2^0  = " << Power<2, 0>::value << std::endl;
    std::cout << "2^10 = " << Power<2, 10>::value << std::endl;
    std::cout << "3^5  = " << Power<3, 5>::value << std::endl;

    // 可以用于需要编译期常量的场景
    int powerArray[Power<2, 3>::value];  // 等价于 int powerArray[8]
    std::cout << "powerArray 大小 = " << sizeof(powerArray) / sizeof(int) << std::endl;

    // ============================================================
    // 3. 默认非类型参数
    // ============================================================
    std::cout << "\n=== DynamicBuffer 默认容量 ===" << std::endl;

    DynamicBuffer<int> buf1;  // 使用默认容量 16
    buf1.add(1);
    buf1.add(2);
    buf1.add(3);
    buf1.print();

    DynamicBuffer<int, 4> buf2;  // 指定容量 4
    buf2.add(10);
    buf2.add(20);
    buf2.add(30);
    buf2.add(40);
    bool ok = buf2.add(50);  // 超出容量
    std::cout << "添加第5个元素: " << (ok ? "成功" : "失败") << std::endl;
    buf2.print();

    // ============================================================
    // 4. bool 非类型参数
    // ============================================================
    std::cout << "\n=== bool 非类型参数 ===" << std::endl;

    int a = 5, b = 3;
    sortTwo<int, true>(a, b);  // 升序
    std::cout << "升序: a=" << a << ", b=" << b << std::endl;

    a = 5; b = 3;
    sortTwo<int, false>(a, b);  // 降序
    std::cout << "降序: a=" << a << ", b=" << b << std::endl;

    return 0;
}
