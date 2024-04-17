#pragma once

#include <cinttypes> // std::size_t
#include <memory>    // std::unique_ptr (dla chętnych)
#include <set>


#define UNIMPLEMENTED_OBJECT_LIST_CONSTRUCTOR_SIZE_EMPTY_METHODS
#define UNIMPLEMENTED_PUSH_FRONT
#define UNIMPLEMENTED_POP_FRONT
#define UNIMPLEMENTED_FRONT
#define UNIMPLEMENTED_CLEAR
#define UNIMPLEMENTED_REVERSE // rozmowa kwalifikacyjna
#define UNIMPLEMENTED_ITERATOR_METHODS_AT_LEAST_EMPTY
#define UNIMPLEMENTED_ITERATORS
#define UNIMPLEMENTED_ERASE_AFTER
#define UNIMPLEMENTED_ERASE


class Object;


class ObjectList
{
    struct Node;

public:
    using value_type = Object;

    ObjectList();
    ~ObjectList();

    void clear();

    void reverse() noexcept;

    value_type& front();


    struct iterator
    {
        // TODO: ...
    };

    iterator begin();
    iterator end()
    {
        return {};
    }

    /**
     * @brief push_front
     * @param newObject
     * Wpierw sprawdzamy czy obiekt już jest w liście
     */
    void push_front(value_type* newObject);

    void pop_front();

    void erase_after(iterator position);

    /// tego std::forward_list nie ma
    void erase(iterator position);

    /// dla chętnych, tego testy nie sprawdzają:
    ObjectList reverse_copy() const;

    /// dla chętnych, tego testy nie sprawdzają:
    void sort() noexcept;

protected:
    // TODO: ...
};
