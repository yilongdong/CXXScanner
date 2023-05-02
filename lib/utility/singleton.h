#include <memory>

namespace CXXScanner::utility {
    /*!
     * 懒加载的单例基类模板
     * LazySingleton<T> 中提供了 struct token 的方法来拒绝使用者直接构造一个类
     * 
     * @tparam T
     */
    template<typename T> class LazySingleton {
    public:
        static T &instance();

        LazySingleton(const LazySingleton &) = delete;
        LazySingleton(LazySingleton &&) = delete;
        LazySingleton &operator=(const LazySingleton &) = delete;
        LazySingleton &operator=(LazySingleton &&) = delete;

    protected:
        struct token {};
        LazySingleton() = default;
    };

    template<typename T>
    inline T &LazySingleton<T>::instance() {
        static const std::unique_ptr<T> instance{new T{token{}}};
        return *instance;
    }

    /*!
     * 默认单例是懒加载
     */
    template <typename T> using Singleton = LazySingleton<T>;
}

