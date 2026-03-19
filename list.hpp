#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>
#include <functional>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        /**
         * add data members and constructors & destructor
         */
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}

        node(const T &value) : prev(nullptr), next(nullptr) {
            data = new T(value);
        }

        ~node() {
            if (data != nullptr) {
                delete data;
                data = nullptr;
            }
        }
    };

protected:
    /**
     * add data members for linked list as protected members
     */
    node *head;
    node *tail;
    size_t listSize;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        if (pos == nullptr) {
            // Insert at end
            if (tail == nullptr) {
                // Empty list
                head = tail = cur;
            } else {
                tail->next = cur;
                cur->prev = tail;
                tail = cur;
            }
        } else {
            cur->next = pos;
            cur->prev = pos->prev;
            if (pos->prev != nullptr) {
                pos->prev->next = cur;
            } else {
                head = cur;
            }
            pos->prev = cur;
        }
        listSize++;
        return cur;
    }

    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (pos == nullptr) return nullptr;

        if (pos->prev != nullptr) {
            pos->prev->next = pos->next;
        } else {
            head = pos->next;
        }

        if (pos->next != nullptr) {
            pos->next->prev = pos->prev;
        } else {
            tail = pos->prev;
        }

        listSize--;
        pos->prev = nullptr;
        pos->next = nullptr;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        node *current;
        const list *container;

        friend class list;
        friend class const_iterator;

    public:
        /**
         * iter++
         */
        iterator operator++(int) {
            iterator temp = *this;
            if (current == nullptr) {
                // end() iterator, can't increment
                throw invalid_iterator();
            }
            current = current->next;
            return temp;
        }

        /**
         * ++iter
         */
        iterator & operator++() {
            if (current == nullptr) {
                // end() iterator, can't increment
                throw invalid_iterator();
            }
            current = current->next;
            return *this;
        }

        /**
         * iter--
         */
        iterator operator--(int) {
            iterator temp = *this;
            if (current == nullptr) {
                // end() iterator, move to tail
                if (container == nullptr || container->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = container->tail;
            } else {
                current = current->prev;
                if (current == nullptr) {
                    // Trying to decrement before begin()
                    throw invalid_iterator();
                }
            }
            return temp;
        }

        /**
         * --iter
         */
        iterator & operator--() {
            if (current == nullptr) {
                // end() iterator, move to tail
                if (container == nullptr || container->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = container->tail;
            } else {
                current = current->prev;
                if (current == nullptr) {
                    // Trying to decrement before begin()
                    throw invalid_iterator();
                }
            }
            return *this;
        }

        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (current == nullptr || current->data == nullptr) {
                throw invalid_iterator();
            }
            return *(current->data);
        }

        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (current == nullptr || current->data == nullptr) {
                throw invalid_iterator();
            }
            return current->data;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return current == rhs.current;
        }

        bool operator==(const const_iterator &rhs) const {
            return current == rhs.current;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return current != rhs.current;
        }

        bool operator!=(const const_iterator &rhs) const {
            return current != rhs.current;
        }

        iterator(node *n = nullptr, const list *c = nullptr) : current(n), container(c) {}
        iterator(const iterator &other) : current(other.current), container(other.container) {}
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *current;
        const list *container;

        friend class list;
        friend class iterator;

    public:
        const_iterator(const node *n = nullptr, const list *c = nullptr) : current(n), container(c) {}
        const_iterator(const const_iterator &other) : current(other.current), container(other.container) {}
        const_iterator(const iterator &other) : current(other.current), container(other.container) {}

        const_iterator operator++(int) {
            const_iterator temp = *this;
            if (current == nullptr) {
                // end() iterator, can't increment
                throw invalid_iterator();
            }
            current = current->next;
            return temp;
        }

        const_iterator & operator++() {
            if (current == nullptr) {
                // end() iterator, can't increment
                throw invalid_iterator();
            }
            current = current->next;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator temp = *this;
            if (current == nullptr) {
                // end() iterator, move to tail
                if (container == nullptr || container->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = container->tail;
            } else {
                current = current->prev;
                if (current == nullptr) {
                    // Trying to decrement before begin()
                    throw invalid_iterator();
                }
            }
            return temp;
        }

        const_iterator & operator--() {
            if (current == nullptr) {
                // end() iterator, move to tail
                if (container == nullptr || container->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = container->tail;
            } else {
                current = current->prev;
                if (current == nullptr) {
                    // Trying to decrement before begin()
                    throw invalid_iterator();
                }
            }
            return *this;
        }

        const T & operator *() const {
            if (current == nullptr || current->data == nullptr) {
                throw invalid_iterator();
            }
            return *(current->data);
        }

        const T * operator ->() const {
            if (current == nullptr || current->data == nullptr) {
                throw invalid_iterator();
            }
            return current->data;
        }

        bool operator==(const iterator &rhs) const {
            return current == rhs.current;
        }

        bool operator==(const const_iterator &rhs) const {
            return current == rhs.current;
        }

        bool operator!=(const iterator &rhs) const {
            return current != rhs.current;
        }

        bool operator!=(const const_iterator &rhs) const {
            return current != rhs.current;
        }
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : head(nullptr), tail(nullptr), listSize(0) {}

    list(const list &other) : head(nullptr), tail(nullptr), listSize(0) {
        node *current = other.head;
        while (current != nullptr) {
            push_back(*(current->data));
            current = current->next;
        }
    }

    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
    }

    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;

        clear();

        node *current = other.head;
        while (current != nullptr) {
            push_back(*(current->data));
            current = current->next;
        }

        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    T & front() {
        if (empty()) throw container_is_empty();
        return *(head->data);
    }

    const T & front() const {
        if (empty()) throw container_is_empty();
        return *(head->data);
    }

    T & back() {
        if (empty()) throw container_is_empty();
        return *(tail->data);
    }

    const T & back() const {
        if (empty()) throw container_is_empty();
        return *(tail->data);
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head, this);
    }

    const_iterator cbegin() const {
        return const_iterator(head, this);
    }

    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(nullptr, this);
    }

    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return listSize == 0;
    }

    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return listSize;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        node *current = head;
        while (current != nullptr) {
            node *next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        listSize = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) throw invalid_iterator();

        node *newNode = new node(value);
        node *posNode = pos.current;

        insert(posNode, newNode);
        return iterator(newNode, this);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) throw container_is_empty();
        if (pos.container != this || pos.current == nullptr) throw invalid_iterator();

        node *posNode = pos.current;
        node *nextNode = posNode->next;

        erase(posNode);
        delete posNode;

        return iterator(nextNode, this);
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *newNode = new node(value);
        insert(nullptr, newNode);
    }

    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();
        node *toDelete = tail;
        erase(tail);
        delete toDelete;
    }

    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *newNode = new node(value);
        insert(head, newNode);
    }

    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();
        node *toDelete = head;
        erase(head);
        delete toDelete;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (listSize <= 1) return;

        // Allocate raw memory for array
        void *rawMemory = operator new[](listSize * sizeof(T));
        T *arr = static_cast<T*>(rawMemory);

        // Copy elements to array using placement new
        size_t i = 0;
        for (iterator it = begin(); it != end(); ++it) {
            new (&arr[i++]) T(*it);
        }

        // Use sjtu::sort
        std::function<bool(const T&, const T&)> cmp = [](const T &a, const T &b) {
            return a < b;
        };
        sjtu::sort(arr, arr + listSize, cmp);

        // Copy sorted array back to list
        i = 0;
        for (iterator it = begin(); it != end(); ++it) {
            *it = arr[i++];
        }

        // Destroy array elements and free memory
        for (size_t j = 0; j < listSize; ++j) {
            arr[j].~T();
        }
        operator delete[](rawMemory);
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;

        node dummy;
        node *current = &dummy;
        node *p1 = head;
        node *p2 = other.head;

        while (p1 != nullptr && p2 != nullptr) {
            if (*(p2->data) < *(p1->data)) {
                // Take from other list
                current->next = p2;
                p2->prev = current;
                p2 = p2->next;
            } else {
                // Take from this list (for equivalent elements, this comes first)
                current->next = p1;
                p1->prev = current;
                p1 = p1->next;
            }
            current = current->next;
        }

        // Attach remaining nodes
        if (p1 != nullptr) {
            current->next = p1;
            p1->prev = current;
            // The tail is the original tail of this list
            tail = this->tail;
        } else if (p2 != nullptr) {
            current->next = p2;
            p2->prev = current;
            // The tail is the original tail of other list
            tail = other.tail;
        } else {
            // Both lists exhausted, current is the tail
            tail = current;
        }

        // Update head and size
        if (dummy.next != nullptr) {
            head = dummy.next;
            head->prev = nullptr;
        }

        listSize += other.listSize;

        // Clear other list
        other.head = other.tail = nullptr;
        other.listSize = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (listSize <= 1) return;

        node *current = head;
        node *temp = nullptr;

        while (current != nullptr) {
            // Swap prev and next pointers
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            // Move to next node (which was prev)
            current = current->prev;
        }

        // Swap head and tail
        temp = head;
        head = tail;
        tail = temp;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (listSize <= 1) return;

        node *current = head;
        while (current != nullptr && current->next != nullptr) {
            if (*(current->data) == *(current->next->data)) {
                node *toDelete = current->next;
                erase(toDelete);
                delete toDelete;
            } else {
                current = current->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
