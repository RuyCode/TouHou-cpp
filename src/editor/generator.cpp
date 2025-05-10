#include <functional>

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>

namespace Generator {
template <typename T, typename = void>
struct has_set_value : std::false_type
{
};

template <typename T>
struct has_set_value<T, std::void_t<decltype(std::declval<T>().setValue(std::declval<int>()))>>
    : std::true_type
{
};

template <typename T, typename F>
typename std::enable_if<has_set_value<T>::value, std::function<void()>>::type
UpdateInspector(T *object, F getter)
{
    return [object, getter]() {
        object->blockSignals(true);
        object->setValue(getter());
        object->blockSignals(false);
    };
}
} // namespace Generator