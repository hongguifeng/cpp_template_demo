// 21_tag_dispatch.cpp
// 演示：标签分发（Tag Dispatch）—— 一种基于类型特征选择实现的技术

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <type_traits>
#include <iterator>

// ============================================================
// 1. 标签分发的基本思想
// ============================================================
// 问题：我们想根据迭代器的类别（随机访问、双向、前向等）选择不同的算法实现
// 做法：使用迭代器的标签类型（tag type）来重载函数

// advance_impl 的不同实现：

// 随机访问迭代器：直接 += n（O(1)）
template <typename Iterator>
void myAdvance_impl(Iterator& it, int n, std::random_access_iterator_tag) {
    std::cout << "  [随机访问: 直接 += " << n << "]" << std::endl;
    it += n;
}

// 双向迭代器：逐步移动（O(n)），支持前后移动
template <typename Iterator>
void myAdvance_impl(Iterator& it, int n, std::bidirectional_iterator_tag) {
    std::cout << "  [双向迭代器: 逐步移动 " << n << " 步]" << std::endl;
    if (n >= 0) {
        while (n--) ++it;
    } else {
        while (n++) --it;
    }
}

// 前向迭代器：只能向前（O(n)）
template <typename Iterator>
void myAdvance_impl(Iterator& it, int n, std::input_iterator_tag) {
    std::cout << "  [输入迭代器: 只能向前 " << n << " 步]" << std::endl;
    while (n-- > 0) ++it;
}

// 对外接口：根据迭代器类别自动分发
template <typename Iterator>
void myAdvance(Iterator& it, int n) {
    // iterator_traits 获取迭代器的类别标签
    // 创建一个标签对象，编译器根据标签类型选择正确的重载
    myAdvance_impl(it, n,
                   typename std::iterator_traits<Iterator>::iterator_category{});
}

// ============================================================
// 2. 自定义标签分发：根据类型特征选择序列化方式
// ============================================================

// 定义标签类型
struct ArithmeticTag {};
struct StringTag {};
struct ContainerTag {};
struct OtherTag {};

// 类型到标签的映射
template <typename T, typename = void>
struct SerializeTraits {
    typedef OtherTag tag;
};

// 算术类型
template <typename T>
struct SerializeTraits<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    typedef ArithmeticTag tag;
};

// string
template <>
struct SerializeTraits<std::string> {
    typedef StringTag tag;
};

// 有 begin/end 的容器（但不是 string）
template <typename T>
struct has_begin_end {
private:
    template <typename U>
    static auto test(int) -> decltype(
        std::declval<U>().begin(),
        std::declval<U>().end(),
        std::true_type{});
    template <typename>
    static std::false_type test(...);
public:
    static const bool value = decltype(test<T>(0))::value;
};

template <typename T>
struct SerializeTraits<T, typename std::enable_if<
    has_begin_end<T>::value && !std::is_same<T, std::string>::value
>::type> {
    typedef ContainerTag tag;
};

// 实现：根据标签分发
template <typename T>
void serialize_impl(const T& value, ArithmeticTag) {
    std::cout << "数值: " << value << std::endl;
}

template <typename T>
void serialize_impl(const T& value, StringTag) {
    std::cout << "字符串: \"" << value << "\"" << std::endl;
}

template <typename T>
void serialize_impl(const T& container, ContainerTag) {
    std::cout << "容器: [";
    bool first = true;
    for (const auto& elem : container) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]" << std::endl;
}

template <typename T>
void serialize_impl(const T&, OtherTag) {
    std::cout << "未知类型" << std::endl;
}

// 统一入口
template <typename T>
void serialize(const T& value) {
    serialize_impl(value, typename SerializeTraits<T>::tag{});
}

// ============================================================
// 3. 标签分发 vs enable_if 的比较
// ============================================================

// enable_if 方式（之前用过）
template <typename T>
typename std::enable_if<std::is_integral<T>::value>::type
process_enableif(T value) {
    std::cout << "  enable_if: 整数 " << value << std::endl;
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value>::type
process_enableif(T value) {
    std::cout << "  enable_if: 浮点 " << value << std::endl;
}

// 标签分发方式（同样的功能）
struct IntegralTag {};
struct FloatingTag {};

template <typename T>
struct ProcessTag {
    typedef typename std::conditional<
        std::is_integral<T>::value,
        IntegralTag,
        FloatingTag
    >::type type;
};

template <typename T>
void process_tag_impl(T value, IntegralTag) {
    std::cout << "  tag dispatch: 整数 " << value << std::endl;
}

template <typename T>
void process_tag_impl(T value, FloatingTag) {
    std::cout << "  tag dispatch: 浮点 " << value << std::endl;
}

template <typename T>
void process_tag(T value) {
    process_tag_impl(value, typename ProcessTag<T>::type{});
}

int main() {
    // ============================================================
    // 1. 迭代器标签分发
    // ============================================================
    std::cout << "=== 迭代器标签分发 ===" << std::endl;

    // vector 有随机访问迭代器
    std::vector<int> vec = {10, 20, 30, 40, 50};
    auto vecIt = vec.begin();
    myAdvance(vecIt, 3);
    std::cout << "  vec advance(3): " << *vecIt << std::endl;

    // list 有双向迭代器
    std::list<int> lst = {10, 20, 30, 40, 50};
    auto lstIt = lst.begin();
    myAdvance(lstIt, 3);
    std::cout << "  list advance(3): " << *lstIt << std::endl;

    // ============================================================
    // 2. 自定义序列化标签分发
    // ============================================================
    std::cout << "\n=== 序列化标签分发 ===" << std::endl;

    serialize(42);
    serialize(3.14);
    serialize(std::string("hello world"));
    serialize(std::vector<int>{1, 2, 3, 4, 5});
    serialize(std::list<std::string>{"a", "b", "c"});

    // ============================================================
    // 3. enable_if vs tag dispatch
    // ============================================================
    std::cout << "\n=== enable_if vs tag dispatch ===" << std::endl;

    std::cout << "enable_if:" << std::endl;
    process_enableif(42);
    process_enableif(3.14);

    std::cout << "tag dispatch:" << std::endl;
    process_tag(42);
    process_tag(3.14);

    // 两种方式产生相同的结果
    // 标签分发的优点：
    // 1. 错误信息更友好
    // 2. 更容易扩展新的分支
    // 3. 代码结构更清晰

    return 0;
}
