// 09_full_specialization.cpp
// 演示：模板的完全特化（Full Specialization）

#include <iostream>
#include <string>
#include <cstring>

// ============================================================
// 1. 类模板的完全特化
// ============================================================

// 主模板（Primary Template）：通用版本
template <typename T>
class TypeInfo {
public:
    static std::string name() { return "unknown"; }
    static int size() { return sizeof(T); }
    static bool isPointer() { return false; }
};

// 针对 int 的完全特化
// template <> 表示"没有模板参数"，所有参数都已确定
template <>
class TypeInfo<int> {
public:
    static std::string name() { return "int"; }
    static int size() { return sizeof(int); }
    static bool isPointer() { return false; }
};

// 针对 double 的完全特化
template <>
class TypeInfo<double> {
public:
    static std::string name() { return "double"; }
    static int size() { return sizeof(double); }
    static bool isPointer() { return false; }
};

// 针对 std::string 的完全特化
template <>
class TypeInfo<std::string> {
public:
    static std::string name() { return "std::string"; }
    static int size() { return sizeof(std::string); }
    static bool isPointer() { return false; }
};

// 针对 bool 的完全特化
template <>
class TypeInfo<bool> {
public:
    static std::string name() { return "bool"; }
    static int size() { return sizeof(bool); }
    static bool isPointer() { return false; }
};

// ============================================================
// 2. 函数模板的完全特化
// ============================================================

// 主模板：通用比较函数
template <typename T>
bool isEqual(const T& a, const T& b) {
    std::cout << "  [通用版本]" << std::endl;
    return a == b;
}

// 针对 double 的特化：浮点数不能直接用 == 比较
template <>
bool isEqual<double>(const double& a, const double& b) {
    std::cout << "  [double 特化版本 - epsilon比较]" << std::endl;
    const double epsilon = 1e-9;
    return (a - b < epsilon) && (b - a < epsilon);
}

// 针对 const char* 的特化：需要用 strcmp 比较内容
template <>
bool isEqual<const char*>(const char* const& a, const char* const& b) {
    std::cout << "  [const char* 特化版本 - strcmp]" << std::endl;
    return std::strcmp(a, b) == 0;
}

// ============================================================
// 3. 特化用于优化：针对 bool 的位压缩存储
// ============================================================

// 主模板
template <typename T>
class SimpleVector {
private:
    T* data;
    int sz;
    int cap;

public:
    SimpleVector() : data(nullptr), sz(0), cap(0) {}

    ~SimpleVector() { delete[] data; }

    void push_back(const T& val) {
        if (sz >= cap) {
            int newCap = (cap == 0) ? 4 : cap * 2;
            T* newData = new T[newCap];
            for (int i = 0; i < sz; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            cap = newCap;
        }
        data[sz++] = val;
    }

    const T& operator[](int i) const { return data[i]; }
    int size() const { return sz; }

    void print() const {
        std::cout << "[";
        for (int i = 0; i < sz; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "] (每个元素 " << sizeof(T) << " 字节)" << std::endl;
    }
};

// 针对 bool 的完全特化：用位存储来节省空间
// 注意：这只是一个教学演示，真实的 std::vector<bool> 特化更复杂
template <>
class SimpleVector<bool> {
private:
    unsigned char* data;  // 每个 byte 存 8 个 bool
    int sz;
    int cap;  // 以 bit 为单位的容量

public:
    SimpleVector() : data(nullptr), sz(0), cap(0) {}
    ~SimpleVector() { delete[] data; }

    void push_back(bool val) {
        if (sz >= cap) {
            int newCap = (cap == 0) ? 8 : cap * 2;
            int newBytes = (newCap + 7) / 8;
            int oldBytes = (cap + 7) / 8;
            unsigned char* newData = new unsigned char[newBytes]();
            for (int i = 0; i < oldBytes; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            cap = newCap;
        }
        int byteIdx = sz / 8;
        int bitIdx = sz % 8;
        if (val) {
            data[byteIdx] |= (1 << bitIdx);
        } else {
            data[byteIdx] &= ~(1 << bitIdx);
        }
        sz++;
    }

    bool operator[](int i) const {
        int byteIdx = i / 8;
        int bitIdx = i % 8;
        return (data[byteIdx] >> bitIdx) & 1;
    }

    int size() const { return sz; }

    void print() const {
        int bytes = (sz + 7) / 8;
        std::cout << "[";
        for (int i = 0; i < sz; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << ((*this)[i] ? "true" : "false");
        }
        std::cout << "] (共 " << bytes << " 字节存储 " << sz << " 个 bool)" << std::endl;
    }
};

int main() {
    // ============================================================
    // 1. 类模板特化
    // ============================================================
    std::cout << "=== TypeInfo 特化 ===" << std::endl;
    std::cout << "int:    name=" << TypeInfo<int>::name()
              << ", size=" << TypeInfo<int>::size() << std::endl;
    std::cout << "double: name=" << TypeInfo<double>::name()
              << ", size=" << TypeInfo<double>::size() << std::endl;
    std::cout << "string: name=" << TypeInfo<std::string>::name()
              << ", size=" << TypeInfo<std::string>::size() << std::endl;
    std::cout << "bool:   name=" << TypeInfo<bool>::name()
              << ", size=" << TypeInfo<bool>::size() << std::endl;
    std::cout << "float:  name=" << TypeInfo<float>::name()
              << " (使用主模板)" << std::endl;

    // ============================================================
    // 2. 函数模板特化
    // ============================================================
    std::cout << "\n=== isEqual 特化 ===" << std::endl;

    std::cout << "isEqual(1, 1): " << isEqual(1, 1) << std::endl;

    // double 使用 epsilon 比较
    double a = 0.1 + 0.2;
    double b = 0.3;
    std::cout << "isEqual(0.1+0.2, 0.3): " << isEqual(a, b) << std::endl;
    std::cout << "直接 == 比较: " << (a == b) << " (可能为 false!)" << std::endl;

    // const char* 使用 strcmp
    const char* s1 = "hello";
    const char* s2 = "hello";
    std::cout << "isEqual(\"hello\", \"hello\"): " << isEqual(s1, s2) << std::endl;

    // ============================================================
    // 3. 优化特化：bool 位压缩
    // ============================================================
    std::cout << "\n=== SimpleVector<bool> 位压缩 ===" << std::endl;

    SimpleVector<int> intVec;
    for (int i = 0; i < 8; ++i) intVec.push_back(i);
    std::cout << "int版本: ";
    intVec.print();

    SimpleVector<bool> boolVec;
    for (int i = 0; i < 8; ++i) boolVec.push_back(i % 2 == 0);
    std::cout << "bool版本: ";
    boolVec.print();

    return 0;
}
