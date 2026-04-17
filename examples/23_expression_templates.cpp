// 23_expression_templates.cpp
// 演示：表达式模板（Expression Templates）
// 这是模板元编程最强大的应用之一，用于优化数学表达式的计算

#include <iostream>
#include <vector>
#include <cassert>

// ============================================================
// 1. 问题：朴素的向量加法效率低下
// ============================================================
// 考虑表达式 v4 = v1 + v2 + v3
// 朴素实现会：
//   1. 创建临时对象 temp1 = v1 + v2（分配内存、逐元素相加）
//   2. 创建临时对象 temp2 = temp1 + v3（再次分配内存、逐元素相加）
//   3. 赋值 v4 = temp2
// 这产生了 2 个临时对象和 3 次遍历！
//
// 表达式模板可以将其优化为：
//   对 v4 的每个元素 i：v4[i] = v1[i] + v2[i] + v3[i]
// 只需 1 次遍历，0 个临时对象！

// ============================================================
// 2. 朴素实现（对比用）
// ============================================================

class NaiveVec {
    std::vector<double> data;
public:
    NaiveVec(int n, double val = 0.0) : data(n, val) {}
    NaiveVec(std::initializer_list<double> il) : data(il) {}

    int size() const { return static_cast<int>(data.size()); }
    double operator[](int i) const { return data[i]; }
    double& operator[](int i) { return data[i]; }

    // 朴素加法：创建临时对象
    NaiveVec operator+(const NaiveVec& other) const {
        NaiveVec result(size());
        for (int i = 0; i < size(); ++i) {
            result[i] = data[i] + other[i];
        }
        return result;  // 临时对象！
    }

    void print(const std::string& label) const {
        std::cout << label << ": [";
        for (int i = 0; i < size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
};

// ============================================================
// 3. 表达式模板实现
// ============================================================

// 前向声明
template <typename E>
class VecExpression;

// 向量加法表达式（不实际计算，只记录操作）
template <typename E1, typename E2>
class VecSum {
    const E1& lhs;
    const E2& rhs;
public:
    VecSum(const E1& l, const E2& r) : lhs(l), rhs(r) {}

    // 延迟计算：只在访问元素时才真正计算
    double operator[](int i) const {
        return lhs[i] + rhs[i];
    }

    int size() const { return lhs.size(); }
};

// 向量乘以标量的表达式
template <typename E>
class VecScalarMul {
    const E& vec;
    double scalar;
public:
    VecScalarMul(const E& v, double s) : vec(v), scalar(s) {}

    double operator[](int i) const {
        return vec[i] * scalar;
    }

    int size() const { return vec.size(); }
};

// 实际的向量类
class Vec {
    std::vector<double> data;
public:
    Vec(int n, double val = 0.0) : data(n, val) {}
    Vec(std::initializer_list<double> il) : data(il) {}

    // 从任意表达式构造（这是关键！）
    template <typename E>
    Vec(const E& expr) : data(expr.size()) {
        for (int i = 0; i < static_cast<int>(data.size()); ++i) {
            data[i] = expr[i];  // 在这里才真正计算
        }
    }

    // 从任意表达式赋值
    template <typename E>
    Vec& operator=(const E& expr) {
        data.resize(expr.size());
        for (int i = 0; i < static_cast<int>(data.size()); ++i) {
            data[i] = expr[i];  // 在这里才真正计算
        }
        return *this;
    }

    int size() const { return static_cast<int>(data.size()); }
    double operator[](int i) const { return data[i]; }
    double& operator[](int i) { return data[i]; }

    void print(const std::string& label) const {
        std::cout << label << ": [";
        for (int i = 0; i < size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
};

// 操作符重载：Vec + Vec -> VecSum
VecSum<Vec, Vec> operator+(const Vec& a, const Vec& b) {
    return VecSum<Vec, Vec>(a, b);
}

// Vec + VecSum -> VecSum（支持链式加法）
template <typename E>
VecSum<Vec, E> operator+(const Vec& a, const E& b) {
    return VecSum<Vec, E>(a, b);
}

template <typename E>
VecSum<E, Vec> operator+(const E& a, const Vec& b) {
    return VecSum<E, Vec>(a, b);
}

// VecSum + VecSum
template <typename E1, typename E2, typename E3, typename E4>
VecSum<VecSum<E1, E2>, VecSum<E3, E4>>
operator+(const VecSum<E1, E2>& a, const VecSum<E3, E4>& b) {
    return VecSum<VecSum<E1, E2>, VecSum<E3, E4>>(a, b);
}

// 标量乘法
VecScalarMul<Vec> operator*(const Vec& v, double s) {
    return VecScalarMul<Vec>(v, s);
}

VecScalarMul<Vec> operator*(double s, const Vec& v) {
    return VecScalarMul<Vec>(v, s);
}

int main() {
    // ============================================================
    // 朴素实现
    // ============================================================
    std::cout << "=== 朴素向量加法 ===" << std::endl;
    NaiveVec nv1 = {1.0, 2.0, 3.0};
    NaiveVec nv2 = {4.0, 5.0, 6.0};
    NaiveVec nv3 = {7.0, 8.0, 9.0};

    // nv1 + nv2 创建临时对象，(nv1+nv2) + nv3 再创建临时对象
    NaiveVec nv4 = nv1 + nv2 + nv3;
    nv4.print("nv1+nv2+nv3");

    // ============================================================
    // 表达式模板实现
    // ============================================================
    std::cout << "\n=== 表达式模板向量加法 ===" << std::endl;
    Vec v1 = {1.0, 2.0, 3.0};
    Vec v2 = {4.0, 5.0, 6.0};
    Vec v3 = {7.0, 8.0, 9.0};

    // v1 + v2 不创建临时 Vec，而是返回 VecSum 表达式对象
    // (v1+v2) + v3 返回 VecSum<VecSum<Vec,Vec>, Vec>
    // 赋值时才一次性计算所有元素
    Vec v4 = v1 + v2 + v3;
    v4.print("v1+v2+v3");

    // ============================================================
    // 标量乘法 + 加法
    // ============================================================
    std::cout << "\n=== 标量乘法 ===" << std::endl;
    Vec v5 = v1 * 2.0;
    v5.print("v1 * 2.0");

    Vec v6 = 3.0 * v2;
    v6.print("3.0 * v2");

    // ============================================================
    // 验证结果正确性
    // ============================================================
    std::cout << "\n=== 验证 ===" << std::endl;
    for (int i = 0; i < v4.size(); ++i) {
        double expected = v1[i] + v2[i] + v3[i];
        std::cout << "v4[" << i << "] = " << v4[i]
                  << " (期望: " << expected << ") "
                  << (v4[i] == expected ? "OK" : "FAIL") << std::endl;
    }

    for (int i = 0; i < v5.size(); ++i) {
        double expected = v1[i] * 2.0;
        std::cout << "v5[" << i << "] = " << v5[i]
                  << " (期望: " << expected << ") "
                  << (v5[i] == expected ? "OK" : "FAIL") << std::endl;
    }

    // ============================================================
    // 表达式模板的优势说明
    // ============================================================
    std::cout << "\n=== 总结 ===" << std::endl;
    std::cout << "朴素方式 v1+v2+v3:" << std::endl;
    std::cout << "  - 2 个临时对象" << std::endl;
    std::cout << "  - 3 次遍历" << std::endl;
    std::cout << "  - 2 次内存分配" << std::endl;
    std::cout << "表达式模板 v1+v2+v3:" << std::endl;
    std::cout << "  - 0 个临时向量对象" << std::endl;
    std::cout << "  - 1 次遍历" << std::endl;
    std::cout << "  - 1 次内存分配（目标向量）" << std::endl;

    return 0;
}
