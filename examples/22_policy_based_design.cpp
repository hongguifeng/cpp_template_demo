// 22_policy_based_design.cpp
// 演示：基于策略的设计（Policy-based Design）
// 使用模板参数来注入不同的行为策略

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

// ============================================================
// 1. 基本概念：日志策略
// ============================================================

// 策略类：定义不同的日志方式

// 策略 1：输出到控制台
struct ConsoleLogPolicy {
    static void log(const std::string& message) {
        std::cout << "[Console] " << message << std::endl;
    }
};

// 策略 2：不输出任何内容（用于发布版）
struct NullLogPolicy {
    static void log(const std::string&) {
        // 什么都不做
    }
};

// 策略 3：带时间戳的日志
struct TimestampLogPolicy {
    static void log(const std::string& message) {
        std::cout << "[2026-04-17 12:00:00] " << message << std::endl;
    }
};

// 使用策略的类
template <typename LogPolicy = ConsoleLogPolicy>
class Application {
public:
    void doWork() {
        LogPolicy::log("开始工作...");
        // 实际工作...
        LogPolicy::log("工作完成!");
    }

    void processData(int data) {
        LogPolicy::log("处理数据: " + std::to_string(data));
    }
};

// ============================================================
// 2. 多策略组合：线程安全策略 + 存储策略
// ============================================================

// 线程安全策略

// 无锁策略（单线程环境）
struct NoLockPolicy {
    struct Lock {
        Lock(NoLockPolicy&) {}  // 空操作
    };
};

// 互斥锁策略（多线程环境）
struct MutexPolicy {
    std::mutex mtx;
    struct Lock {
        std::unique_lock<std::mutex> guard;
        Lock(MutexPolicy& policy) : guard(policy.mtx) {}
    };
};

// 存储策略

// 动态数组存储
template <typename T>
struct DynamicStoragePolicy {
    std::vector<T> storage;

    void add(const T& val) { storage.push_back(val); }
    T get(int index) const { return storage.at(index); }
    int size() const { return static_cast<int>(storage.size()); }
    void clear() { storage.clear(); }
};

// 固定大小数组存储
template <typename T, int MaxSize = 100>
struct FixedStoragePolicy {
    T storage[MaxSize];
    int count = 0;

    void add(const T& val) {
        if (count < MaxSize) {
            storage[count++] = val;
        }
    }
    T get(int index) const { return storage[index]; }
    int size() const { return count; }
    void clear() { count = 0; }
};

// 组合多个策略的容器
template <typename T,
          template <typename> class StoragePolicy = DynamicStoragePolicy,
          typename ThreadPolicy = NoLockPolicy>
class SmartContainer : private StoragePolicy<T>, private ThreadPolicy {
public:
    void add(const T& val) {
        typename ThreadPolicy::Lock lock(*this);
        StoragePolicy<T>::add(val);
    }

    T get(int index) const {
        return StoragePolicy<T>::get(index);
    }

    int size() const {
        return StoragePolicy<T>::size();
    }

    void print(const std::string& label) const {
        std::cout << label << ": [";
        for (int i = 0; i < size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << get(i);
        }
        std::cout << "]" << std::endl;
    }
};

// ============================================================
// 3. 排序策略
// ============================================================

// 冒泡排序策略
struct BubbleSortPolicy {
    template <typename Iterator>
    static void sort(Iterator begin, Iterator end) {
        std::cout << "  [使用冒泡排序]" << std::endl;
        for (auto i = begin; i != end; ++i) {
            for (auto j = begin; j != end; ++j) {
                auto next = j;
                ++next;
                if (next != end && *next < *j) {
                    auto temp = *j;
                    *j = *next;
                    *next = temp;
                }
            }
        }
    }
};

// 选择排序策略
struct SelectionSortPolicy {
    template <typename Iterator>
    static void sort(Iterator begin, Iterator end) {
        std::cout << "  [使用选择排序]" << std::endl;
        for (auto i = begin; i != end; ++i) {
            auto minIt = i;
            for (auto j = i; j != end; ++j) {
                if (*j < *minIt) {
                    minIt = j;
                }
            }
            if (minIt != i) {
                auto temp = *i;
                *i = *minIt;
                *minIt = temp;
            }
        }
    }
};

// 使用排序策略的容器
template <typename T, typename SortPolicy = BubbleSortPolicy>
class SortableVector {
    std::vector<T> data;

public:
    void add(const T& val) { data.push_back(val); }

    void sort() {
        SortPolicy::sort(data.begin(), data.end());
    }

    void print(const std::string& label) const {
        std::cout << label << ": [";
        for (size_t i = 0; i < data.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
};

// ============================================================
// 4. 验证策略（输入验证）
// ============================================================

// 不验证
struct NoValidation {
    static bool validate(int) { return true; }
    static bool validate(const std::string&) { return true; }
};

// 范围验证
struct RangeValidation {
    static bool validate(int value) {
        bool ok = (value >= 0 && value <= 100);
        if (!ok) std::cout << "  [验证失败: " << value << " 不在 0-100 范围内]" << std::endl;
        return ok;
    }
    static bool validate(const std::string& value) {
        bool ok = !value.empty();
        if (!ok) std::cout << "  [验证失败: 字符串为空]" << std::endl;
        return ok;
    }
};

template <typename T, typename ValidationPolicy = NoValidation>
class ValidatedValue {
    T value;
    bool valid;

public:
    ValidatedValue(const T& v) : value(v), valid(ValidationPolicy::validate(v)) {}

    bool isValid() const { return valid; }
    T getValue() const { return value; }

    void print() const {
        std::cout << "  值: " << value << ", 有效: " << (valid ? "是" : "否") << std::endl;
    }
};

int main() {
    // ============================================================
    // 1. 日志策略
    // ============================================================
    std::cout << "=== 日志策略 ===" << std::endl;

    std::cout << "--- Console 策略 ---" << std::endl;
    Application<ConsoleLogPolicy> app1;
    app1.doWork();

    std::cout << "--- Null 策略（无输出）---" << std::endl;
    Application<NullLogPolicy> app2;
    app2.doWork();
    std::cout << "(什么都没有输出)" << std::endl;

    std::cout << "--- Timestamp 策略 ---" << std::endl;
    Application<TimestampLogPolicy> app3;
    app3.doWork();

    // ============================================================
    // 2. 多策略组合
    // ============================================================
    std::cout << "\n=== 多策略组合容器 ===" << std::endl;

    // 动态存储 + 无锁（单线程，性能最好）
    SmartContainer<int> c1;
    c1.add(1); c1.add(2); c1.add(3);
    c1.print("动态+无锁");

    // 动态存储 + 互斥锁（多线程安全）
    SmartContainer<int, DynamicStoragePolicy, MutexPolicy> c2;
    c2.add(10); c2.add(20); c2.add(30);
    c2.print("动态+互斥锁");

    // ============================================================
    // 3. 排序策略
    // ============================================================
    std::cout << "\n=== 排序策略 ===" << std::endl;

    SortableVector<int, BubbleSortPolicy> sv1;
    sv1.add(5); sv1.add(2); sv1.add(8); sv1.add(1); sv1.add(3);
    sv1.print("冒泡排序前");
    sv1.sort();
    sv1.print("冒泡排序后");

    SortableVector<int, SelectionSortPolicy> sv2;
    sv2.add(5); sv2.add(2); sv2.add(8); sv2.add(1); sv2.add(3);
    sv2.print("选择排序前");
    sv2.sort();
    sv2.print("选择排序后");

    // ============================================================
    // 4. 验证策略
    // ============================================================
    std::cout << "\n=== 验证策略 ===" << std::endl;

    std::cout << "--- 无验证 ---" << std::endl;
    ValidatedValue<int, NoValidation> v1(999);
    v1.print();

    std::cout << "--- 范围验证 ---" << std::endl;
    ValidatedValue<int, RangeValidation> v2(50);
    v2.print();

    ValidatedValue<int, RangeValidation> v3(150);
    v3.print();

    ValidatedValue<std::string, RangeValidation> v4("");
    v4.print();

    ValidatedValue<std::string, RangeValidation> v5("hello");
    v5.print();

    return 0;
}
