//
// Created by Alis Daniel.
//

#ifndef DLIST_H
#define DLIST_H


/**
 * Data Structure that implement a dual linked list.
 *  the data type, T, need to have:
 *  default c'tor, operator=, copy c'tor
 */
/**
* base Node in the list. so user can access a node in O(1) time.
* @tparam T
*/
template<class T>
class DList;

template<class T>
class Node {
    friend DList<T>;
    Node *next;
    Node *prev;
    void deleteNodeAux()
    {
        this->prev->next = this->next;
        if (this->next != nullptr) {
            this->next->prev = this->prev;
        }
        delete this;
    }
public:
    T data;
};

template<class T>
class DList {
    friend Node<T>;
    Node<T> *head;

public:
    DList() : head(new(Node<T>)) {
        head->next = nullptr;
        head->prev = nullptr;
    }

    DList(const DList& src) = delete;

    ~DList() {
        while (head->next != nullptr) {
            head->next->deleteNodeAux();
        }
        delete head;
    }


    DList& operator= (const DList& src)
    {
        if(this == &src){
            return *this;
        }
        Node<T>* new_head  = new Node<T>;
        new_head->next = nullptr;
        new_head->prev = nullptr;
        const Node<T>* current = src.head->next;
        Node<T>* prev = new_head;
        while(current != nullptr) {
            Node<T> *next = new (std::nothrow) Node<T>;
            if(next == nullptr){
                while(new_head->next != nullptr) {
                    deleteNode(new_head->next);
                }
                delete new_head;
                throw std::bad_alloc();
            }
            next->data = current->data;
            next->next = nullptr;
            next->prev = prev;
            prev->next = next;
            current = current->next;
            prev = next;
        }
        while(this->head->next != nullptr) {
            deleteNode(this->head->next);
        }
        delete this->head;
        this->head = new_head;
        return *this;
    }

    Node<T>* insertFirst(const T& data)
    {
        Node<T>* node = new (Node<T>);
        node->data = data;
        node->next = head->next;
        node->prev = head;
        if(head->next != nullptr){
            head->next->prev = node;
        }
        head->next = node;
        return node;
    }

    void deleteNode(Node<T>* node)
    {
        if (node == nullptr){
            return;
        }
        node->deleteNodeAux();
    }

    /*
     * iterator made to iterate through the data in the list.
     */
    class iterator{
        Node<T>* current;
        friend DList;
        explicit iterator(Node<T>* init) : current(init) {}
    public:
        T& getData() const{
            return current->data;
        }

        Node<T>* operator* ()
        {
            return current;
        }
        const iterator& operator ++ ()
        {
            current = current->next;
            return *this;
        }
        iterator operator++ (int)
        {
            iterator tmp = *this;
            current = current->next;
            return tmp;
        }

        bool operator == (const iterator& it) const
        {
            return (it.current == current);
        }
        bool operator != (const iterator& it) const
        {
            return !(it == *this);
        }

    };

    iterator begin()
    {
        return iterator(head->next);
    }
    iterator end()
    {
        return iterator(nullptr);
    }

    class ConstIterator{
        const Node<T>* current;
        friend DList;
        explicit ConstIterator(const Node<T>* init) : current(init) {}
    public:
        const T& getData() const{
            return current->data;
        }

        const Node<T>* operator* () const
        {
            return current;
        }
        ConstIterator& operator ++ ()
        {
            current = current->next;
            return *this;
        }
        ConstIterator operator++ (int)
        {
            ConstIterator tmp = *this;
            current = current->next;
            return tmp;
        }

        bool operator == (const ConstIterator& it) const
        {
            return (it.current == current);
        }
        bool operator != (const ConstIterator& it) const
        {
            return !(it == *this);
        }

    };

    ConstIterator constBegin() const
    {
        return ConstIterator(head->next);
    }
    ConstIterator constEnd() const
    {
        return ConstIterator(nullptr);
    }

};


#endif //DLIST_H
