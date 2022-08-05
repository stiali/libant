#ifndef LIBANT_INCLUDE_LIBANT_UTILS_NONCOPYABLE_H_
#define LIBANT_INCLUDE_LIBANT_UTILS_NONCOPYABLE_H_

namespace ant {

class Noncopyable {
protected:
    Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

#define NONCOPYABLE(class_)         \
    class_(const class_&) = delete; \
    class_& operator=(const class_&) = delete

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_UTILS_NONCOPYABLE_H_
