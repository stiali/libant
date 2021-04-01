#ifndef LIBANT_SINGLETON_SINGLETON_H_
#define LIBANT_SINGLETON_SINGLETON_H_

namespace ant {

/**
 * A singleton implementation which guarantees the object to be created before main() begins.
 * Usage: Singleton<T>::Instance().
 *
 * @tparam T must be no-throw default constructable and no-throw destructible
 */
template<typename T>
struct Singleton {
public:
    using ObjectType = T;

    // If, at any point (in user code), Singleton<T>::instance() is called,
    // then the following function is instantiated.
    static ObjectType& Instance()
    {
        // This is the object that we return a reference to.
        // It is guaranteed to be created before main() begins because of the next line.
        static ObjectType obj;
        // The following line does nothing else than force the instantiation of Singleton<T>::ObjectCreator,
        // whose constructor is called before main() begins.
        obj_.DoNothing();
        return obj;
    }

private:
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    const Singleton& operator=(const Singleton&) = delete;

private:
    struct ObjectCreator {
        // This constructor does nothing more than ensure that instance() is called before main() begins,
        // thus creating the static T object before multithreading race issues can come up.
        ObjectCreator()
        {
            Singleton<T>::Instance();
        }

        inline void DoNothing() const
        {
        }
    };

private:
    static ObjectCreator obj_;
};

template<typename T>
typename Singleton<T>::ObjectCreator Singleton<T>::obj_;

} // namespace ant

#endif // LIBANT_SINGLETON_SINGLETON_H_