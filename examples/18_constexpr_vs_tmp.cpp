// 18_constexpr_vs_tmp.cpp
// 演示：constexpr（C++11）与传统模板元编程的对比
// constexpr 是更现代、更易读的编译期计算方式

#include <iostream>

// ============================================================
// 1. constexpr 函数 vs 模板元编程
// ============================================================

// 传统 TMP 方式：阶乘
template <int N>
struct FactorialTMP {
    static const int value = N * FactorialTMP<N - 1>::value;
};

template <>
struct FactorialTMP<0> {
    static const int value = 1;
};

// constexpr 方式：阶乘（C++11）
// constexpr 函数看起来像普通函数，但可以在编译期求值
// C++11 的 constexpr 函数有严格限制：只能有一个 return 语句
constexpr int factorialCE(int n) {
    return (n <= 1) ? 1 : n * factorialCE(n - 1);
}

// ============================================================
// 2. constexpr 斐波那契
// ============================================================

// TMP 方式
template <int N>
struct FibTMP {
    static const long long value = FibTMP<N-1>::value + FibTMP<N-2>::value;
};
template <>
struct FibTMP<0> { static const long long value = 0; };
template <>
struct FibTMP<1> { static const long long value = 1; };

// constexpr 方式（C++11 -- 必须是单一 return 表达式）
constexpr long long fibCE(int n) {
    return (n <= 0) ? 0 : (n == 1) ? 1 : fibCE(n-1) + fibCE(n-2);
}

// ============================================================
// 3. constexpr 变量（C++11）
// ============================================================

constexpr double PI = 3.14159265358979323846;
constexpr int MAX_SIZE = 1024;

// constexpr 可以用于计算常量
constexpr double circleArea(double r) {
    return PI * r * r;
}

// ============================================================
// 4. constexpr 类和构造函数（C++11）
// ============================================================

class Point {
    double x_, y_;
public:
    // constexpr 构造函数：允许在编译期创建对象
    constexpr Point(double x, double y) : x_(x), y_(y) {}

    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }

    // constexpr 成员函数
    constexpr double distanceFromOriginSquared() const {
        return x_ * x_ + y_ * y_;
    }
};

// constexpr 函数操作 constexpr 对象
constexpr Point midpoint(const Point& a, const Point& b) {
    return Point((a.x() + b.x()) / 2.0, (a.y() + b.y()) / 2.0);
}

// ============================================================
// 5. constexpr vs TMP 的优缺点对比
// ============================================================
//
// constexpr 的优点：
//   1. 语法简单，像普通函数一样
//   2. 可以在编译期和运行期使用（TMP 只能编译期）
//   3. 错误信息更友好
//   4. 支持浮点数运算（TMP 不能）
//
// TMP 的优点：
//   1. 可以操作类型（不只是值）
//   2. 可以做模式匹配（特化）
//   3. 在没有 constexpr 的旧代码中使用
//
// 最佳实践：
//   - 纯数值计算 -> 用 constexpr
//   - 类型计算/类型变换 -> 用 TMP
//   - 两者经常结合使用

// ============================================================
// 6. 结合使用的例子：编译期查找表
// ============================================================

// 使用 constexpr 生成编译期查找表
template <int N>
struct SquareTable {
    int values[N];

    constexpr SquareTable() : values() {
        // 注意：C++11 的 constexpr 构造函数只能用初始化列表
        // 这里的循环赋值实际上需要 C++14
        // 在 C++11 中我们用递归模板来实现
    }
};

// C++11 兼容的编译期数组构建：使用模板递归
template <int... Values>
struct IntList {};

// 递归生成 0^2, 1^2, ..., (N-1)^2
template <int N, int... Acc>
struct MakeSquareList {
    typedef typename MakeSquareList<N - 1, (N - 1) * (N - 1), Acc...>::type type;
};

template <int... Acc>
struct MakeSquareList<0, Acc...> {
    typedef IntList<Acc...> type;
};

// 打印 IntList
template <int First, int... Rest>
void printIntList(IntList<First, Rest...>) {
    std::cout << First;
    int dummy[] = {0, (std::cout << ", " << Rest, 0)...};
    (void)dummy;
    std::cout << std::endl;
}

void printIntList(IntList<>) {
    std::cout << "(空)" << std::endl;
}

int main() {
    // ============================================================
    // 1. 阶乘对比
    // ============================================================
    std::cout << "=== 阶乘: TMP vs constexpr ===" << std::endl;
    std::cout << "TMP:       5! = " << FactorialTMP<5>::value << std::endl;
    std::cout << "constexpr: 5! = " << factorialCE(5) << std::endl;
    std::cout << "TMP:      10! = " << FactorialTMP<10>::value << std::endl;
    std::cout << "constexpr:10! = " << factorialCE(10) << std::endl;

    // constexpr 直接可以运行时使用
    int n = 7;
    std::cout << "constexpr 运行期: " << n << "! = " << factorialCE(n) << std::endl;

    // TMP 则必须用编译期常量：
    // FactorialTMP<n>::value; // 错误！n 不是编译期常量

    // ============================================================
    // 2. 斐波那契对比
    // ============================================================
    std::cout << "\n=== 斐波那契: TMP vs constexpr ===" << std::endl;
    std::cout << "TMP:       F(20) = " << FibTMP<20>::value << std::endl;
    std::cout << "constexpr: F(20) = " << fibCE(20) << std::endl;

    // ============================================================
    // 3. constexpr 变量
    // ============================================================
    std::cout << "\n=== constexpr 变量和函数 ===" << std::endl;
    std::cout << "PI = " << PI << std::endl;
    std::cout << "MAX_SIZE = " << MAX_SIZE << std::endl;

    // 编译期计算圆面积
    constexpr double area = circleArea(5.0);
    std::cout << "半径5的圆面积 = " << area << std::endl;

    // 这是 TMP 做不到的！（TMP 不支持浮点运算）

    // ============================================================
    // 4. constexpr 类
    // ============================================================
    std::cout << "\n=== constexpr 类 ===" << std::endl;

    constexpr Point p1(3.0, 4.0);
    constexpr Point p2(1.0, 2.0);
    constexpr Point mid = midpoint(p1, p2);

    std::cout << "P1 = (" << p1.x() << ", " << p1.y() << ")" << std::endl;
    std::cout << "P2 = (" << p2.x() << ", " << p2.y() << ")" << std::endl;
    std::cout << "中点 = (" << mid.x() << ", " << mid.y() << ")" << std::endl;
    std::cout << "P1 到原点距离^2 = " << p1.distanceFromOriginSquared() << std::endl;

    // 编译期验证
    static_assert(factorialCE(5) == 120, "5! == 120");
    static_assert(fibCE(10) == 55, "F(10) == 55");

    // ============================================================
    // 5. 编译期查找表
    // ============================================================
    std::cout << "\n=== 编译期平方表 ===" << std::endl;
    std::cout << "0^2 到 9^2: ";
    printIntList(typename MakeSquareList<10>::type{});

    return 0;
}
