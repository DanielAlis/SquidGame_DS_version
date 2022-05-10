//
// Created by Alis Daniel.
//

#ifndef RANKAVL_H
#define RANKAVL_H
#include <iostream>
#include <cassert>




/**
 * Implementation of an AVL tree data structure.
 * this tree won't allow duplicated keys.
 * the tree iterator run through the items without using the stack (O(1) in memory)
 * assuming T type has: comparison operator (<, ==) ,
 * assume T and S has no parameters c'tor.
 * assume T and S has operator = ;
 *
 *
 *
 */

/**
 * a class to inherit from to make changes to the data in the tree
 * @tparam S
 */
template<typename T>
inline void swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}
template<typename T>
inline const T& max (const T& a, const T& b){
    return (a<b ? b : a);
}
template <class T, class S>
class RankAVL {
    struct node {
        node() = default;
        node(T& key,S& data, int height,int info):parent(nullptr),right(nullptr),left(nullptr), key(key), data(data), height(height),rank(1),sumInfo(info),info(info){}
        node* parent;
        node* right;
        node* left;
        T key;
        S data;
        int height;
        int rank;
        int sumInfo;
        int info;
    };
    node* root;
    node* highest;
    node* lowest;
    int size;

    friend int getBF (const node* v)
    {
        int left_height = v->left==nullptr? -1 : v->left->height;
        int right_height = v->right==nullptr? -1 : v->right->height;
        return left_height - right_height;
    }

    int calculateHeight(const node* v) const
    {
        if(v == nullptr || v == root){
            return -1;
        }
        int left_height = v->left == nullptr ? -1 : v->left->height;
        int right_height = v->right == nullptr ?  -1 : v->right->height;
        return 1+max(left_height, right_height);
    }

    void rollLL (node* v)
    {
        node* B = v->left;
        B->parent = v->parent;
        v->left = B->right;
        if (v->parent->right == v){
            v->parent->right = B;
        }
        else{
            assert(v->parent->left == v);
            v->parent->left = B;
        }
        v->parent = B;
        if(B->right != nullptr){
            B->right->parent = v;
        }
        B->right = v;
        v->height = calculateHeight(v);
        B->height = calculateHeight(B);
        updatingRank (v);
        updatingInfo (v);
        updatingRank (B);
        updatingInfo (B);
    }

    void rollLR (node* v)
    {
        node* A = v->left;
        node* B = v->left->right;
        v->left = B->right;
        A->right = B->left;
        if(B->right != nullptr){
            B->right->parent = v;
        }
        if(B->left != nullptr){
            B->left->parent = A;
        }
        B->left = A;
        B->right = v;
        A->parent = B;
        B->parent = v->parent;
        if(v->parent->right == v){
            v->parent->right = B;
        }
        else{
            assert(v->parent->left == v);
            v->parent->left = B;
        }
        v->parent = B;
        v->height = calculateHeight(v);
        A->height = calculateHeight(A);
        B->height = calculateHeight(B);
        updatingRank (v);
        updatingInfo (v);
        updatingRank (A);
        updatingInfo (A);
        updatingRank (B);
        updatingInfo (B);
    }

    void rollRR (node* v)
    {
        node* B = v->right;
        B->parent = v->parent;
        v->right = B->left;
        if (v->parent->right == v){
            v->parent->right = B;
        }
        else{
            assert(v->parent->left == v);
            v->parent->left = B;
        }
        v->parent = B;
        if(B->left != nullptr){
            B->left->parent = v;
        }
        B->left = v;
        v->height = calculateHeight(v);
        B->height = calculateHeight(B);
        updatingRank (v);
        updatingInfo (v);
        updatingRank (B);
        updatingInfo (B);
    }

    void rollRL (node* v)
    {
        node* A = v->right;
        node* B = v->right->left;
        v->right = B->left;
        A->left = B->right;
        if(B->right != nullptr){
            B->right->parent = A;
        }
        if(B->left != nullptr){
            B->left->parent = v;
        }
        B->right = A;
        B->left = v;
        A->parent = B;
        B->parent = v->parent;
        if(v->parent->right == v){
            v->parent->right = B;
        }
        else{
            assert(v->parent->left == v);
            v->parent->left = B;
        }
        v->parent = B;
        v->height = calculateHeight(v);
        A->height = calculateHeight(A);
        B->height = calculateHeight(B);
        updatingRank (v);
        updatingInfo (v);
        updatingRank (A);
        updatingInfo (A);
        updatingRank (B);
        updatingInfo (B);
    }

    void doRoll (node* v)
    {
        int BF = getBF(v);
        switch (BF) { // according to slide 23, lecture 4, in course 234218.
            case 2 :
                if (getBF(v->left) >= 0){
                    rollLL(v);
                    return;
                }
                rollLR(v);
                break;
            case -2 :
                if (getBF(v->right) <=0){
                    rollRR(v);
                    return;
                }
                rollRL(v);
                break;
            default :
                return;
        }
    }

    friend bool canRemoveNode(const node* v)
    {
        if(v == nullptr){
            return false;
        }
        return !(v->right != nullptr && v->left != nullptr);
    }
    /**
     * helper function for remove function.
     * used to remove a single node, which has 0 or 1 child.
     * if it has 2 - it is undefined behaviour.
     */
    void removeSingleNode(node* v)
    {
        if(v == nullptr){
            return;
        }
        if (v->right == nullptr){
            v->parent->right == v ? v->parent->right = v->left : v->parent->left = v->left;
            if(v->left != nullptr){
                v->left->parent = v->parent;
            }
            if(v == highest){
                v->left != nullptr ? highest = v->left : highest = v->parent;
                if(highest == root){
                    highest = nullptr;
                    lowest = nullptr;
                }
            }
            else if (v == lowest){
                lowest = v->parent;
                if(lowest == root){
                    highest = nullptr;
                    lowest = nullptr;
                }
            }
            delete v;
            return;
        }
        assert(v->left == nullptr);
        if(v == lowest){
            lowest = v->right;
            assert(lowest != root);
        }
        v->parent->right == v ? v->parent->right = v->right : v->parent->left = v->right;
        if(v->right != nullptr){
            v->right->parent = v->parent;
        }
        delete v;
    }
    /**
     * helper function, delete the tree with tree_root as the root.
     * can be used to delete subTree in case a memory allocation failed.
     * @param tree_root
     * the root and all it's childs will be deleted
     */
    friend void deleteTree(node* tree_root)
    {
        if(tree_root == nullptr){
            return;
        }
        deleteTree(tree_root->left);
        deleteTree(tree_root->right);
        delete tree_root;
    }

    node* copyTreeAux(const node* v) const {
        if (v == nullptr){
            return nullptr;
        }
        node* new_root = new (node);
        new_root->key = v->key;
        new_root->info = v->info;
        new_root->sumInfo = v->sumInfo;
        new_root->rank = v->rank;
        new_root->data = v->data;
        new_root->parent = nullptr;
        try {
            new_root->right = copyTreeAux(v->right);
        }catch(...){
            delete new_root;
            throw;
        }
        try{
            new_root->left = copyTreeAux(v->left);
        }catch(...){
            deleteTree(v->right);
            delete new_root;
            throw;
        }
        if(new_root->right != nullptr){
            new_root->right->parent = new_root;
        }
        if(new_root->left != nullptr){
            new_root->left->parent = new_root;
        }
        new_root->height = calculateHeight(new_root);
        assert(getBF(new_root) == getBF(v));
        return new_root;

    }

    void updatingRank (node* node_to_update)
    {
        node_to_update->rank = 1 + rankOfChild(node_to_update->left) + rankOfChild(node_to_update->right);
    }
    void updatingInfo (node* node_to_update)
    {
        node_to_update->sumInfo =node_to_update->info + infoOfChild(node_to_update->left) + infoOfChild(node_to_update->right);
    }
    int rankOfChild (const node* child) const
    {
        if (child == NULL)
        {
            return 0;
        }
        else
        {
            return (child->rank);
        }
    }
    int infoOfChild (const node* child) const
    {
        if (child == NULL)
        {
            return 0;
        }
        else
        {
            return (child->sumInfo);
        }
    }


public:
    RankAVL () : root(new (node))
    {
        root->parent = nullptr;
        root->right = nullptr;
        root->left = nullptr;
        root->height = -1;
        highest = nullptr;
        lowest = nullptr;
        size = 0;
    }

    ~RankAVL ()
    {
        deleteTree(root->right);
        delete root;
    }

    RankAVL (const RankAVL& src) : root(new (node))
    {

        root->parent = nullptr;;
        root->left = nullptr;
        root->height = -1;
        root->right = copyTreeAux(src.root->right);
        if(root->right != nullptr){
            root->right->parent = root;
        }
        highest = root->right;
        lowest = root->right;
        size = root->size;
        if(highest == nullptr){
            assert(lowest == nullptr);
            return;
        }
        while(highest->key != src.highest->key){
            highest = highest->right;
        }
        while(lowest->key != src.lowest->key){
            lowest = lowest->left;
        }
    }

    RankAVL& operator= (const RankAVL& src)
    {
#ifndef NDEBUG
        std::cout<<"AVL Tree operator=  called"<<std::endl;
#endif
        if(this == &src){
            return *this;
        }
        node* new_root = new (node);
        try{
            new_root->right = copyTreeAux(src.root->right);
        }catch(...){
            delete new_root;
            throw;
        }
        if(new_root->right != nullptr){
            new_root->right->parent = new_root;
        }
        new_root->parent = nullptr;
        new_root->left = nullptr;
        new_root->height = -1;
        deleteTree(this->root);
        this->root = new_root;
        highest = root->right;
        lowest = root->right;
        size = root->size;
        if(highest == nullptr){
            assert(lowest == nullptr);
            return *this;
        }
        while(highest->key != src.highest->key){
            highest = highest->right;
        }
        while(lowest->key != src.lowest->key){
            lowest = lowest->left;
        }
        return *this;
    }

    S& find (const T& key) const
    {
        node* current = root->right;
        while (current != nullptr){
            if(current->key == key){
                return current->data;
            }
            if(current->key < key){
                current = current->right;
                continue;
            }
            assert(current->key > key);
            current = current->left;
        }
        throw notFound();
    }
    bool Exists (const T& key) const
    {
        node* current = root->right;
        while (current != nullptr){
            if(current->key == key){
                return true;
            }
            if(current->key < key){
                current = current->right;
                continue;
            }
            assert(current->key > key);
            current = current->left;
        }
        return false;
    }

    void insert (const T& key, const S& data,int info)
    {
        node* current = root->right;
        node* parent = root;
        while(current != nullptr){
            if(current->key == key){
                throw alreadyExists();
            }
            if(current->key < key){
                parent = current;
                current = current->right;
                continue;
            }
            assert(current->key > key);
            parent = current;
            current = current->left;
        }
        node* newNode = new (node);
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 0;
        newNode->rank = 1;
        newNode->info = info;
        newNode->sumInfo = info;
        newNode->key = key;
        newNode->data = data;
        newNode->parent = parent;
        if(parent == root){
            root->right = newNode;
            highest = newNode;
            lowest = newNode;
            size++;
            return;
        }
        if (parent->key < key){
            parent->right = newNode;
            if(highest == parent){
                highest = newNode;
            }
        }
        else{
            parent->left = newNode;
            if(lowest == parent){
                lowest = newNode;
            }
        }
        //going all the way up - to update ranks of every node on the search path.
        while (parent != root){
            int new_height = calculateHeight(parent);
            parent->height = new_height;
            updatingRank(parent);
            updatingInfo(parent);
            if(getBF(parent) > 1 || getBF(parent) < -1){
                doRoll(parent);
            }
            parent = parent->parent;
        }
        size++;
        return;
    }
    /**
     * don't remove while iterating, it is undefined behaviour.
     * @param key
     */
    void remove (const T& key)
    {
        node* current = root->right;
        while (current != nullptr){
            if(current->key == key){
                break;
            }
            if(current->key < key){
                current = current->right;
                continue;
            }
            assert(current->key > key);
            current = current->left;
        }
        if(current == nullptr){
            throw notFound();
        }
        node* parent = current->parent;
        if(canRemoveNode(current)){
            removeSingleNode(current);
        }
        else {
            node *swapNode = current->right;
            while (swapNode->left != nullptr) {
                swapNode = swapNode->left;
            }
            current->right->parent = swapNode;
            current->left->parent = swapNode;
            swap(current->left, swapNode->left);
            swap(current->right, swapNode->right);
            swap((current->parent->right == current ? current->parent->right : current->parent->left),
                 (swapNode->parent->right == swapNode ? swapNode->parent->right : swapNode->parent->left));
            swap(current->parent, swapNode->parent);
            parent = current->parent;
            removeSingleNode(current);
        }
        while(parent != root){
            parent->height = calculateHeight(parent);
            updatingRank(parent);
            updatingInfo(parent);
            node* tmp = parent->parent;
            doRoll(parent);
            parent = tmp;
        }
        size--;
    }
    int getSize() const
    {
        return this->size;
    }

    node* makeTree_aux(T* array,S* array2,int* array3, int low, int high){

        if(low > high)
            return nullptr;

        int mid = (high + low) / 2;
        node* left = makeTree_aux(array,array2,array3, low, mid - 1);
        node* right = makeTree_aux(array,array2,array3, mid + 1, high);
        node* node = new RankAVL<T,S>::node(array[mid],array2[mid],0,array3[mid]);

        node -> left = left;
        node -> right = right;

        if(right)
            right -> parent = node;

        if(left)
            left -> parent = node;

        node->height = calculateHeight(node);
        updatingRank(node);
        updatingInfo(node);
        return node;
    }


    void makeTree(RankAVL<T,S>* tree,T* array,S* array2,int* array3, int size){
        tree->size = size;
        tree->root->right = makeTree_aux(array,array2,array3, 0, size-1);

        if(tree->root->right != nullptr)
            tree->root->right->parent = tree->root;

        node* temp = tree->root->right;

        while(temp != nullptr)
        {
            if(temp->left == nullptr)
                tree->lowest = temp;

            temp = temp->left;
        }

        temp = tree->root->right;

        while(temp != nullptr)
        {
            if(temp->right == nullptr)
                tree->highest = temp;

            temp = temp->right;
        }
    }

    const S& rankSelect (int select) const
    {
        assert ( select > 0);
        node* current = root->right;
        while(current != nullptr) {
            if (rankOfChild(current->right) == select - 1 ) {
                return current->data;
            } else if (rankOfChild(current->right) > select - 1)
            {
                current = current->right;
            } else {
                assert (rankOfChild(current->right) < select -1);
                select = select - 1 - rankOfChild(current->right);
                current = current->left;
            }
        }
        throw invalidRank();
    }
    S& rankSelect (int select)
    {
        return const_cast<S&>(const_cast<const RankAVL<T,S>*>(this)->rankSelect(select));
    }
    int rank (const T& key) const
    {
        int r = 0;
        node* current = root->right;
        while (current != nullptr){
            if(current->key == key){
                r = r + rankOfChild(current->left) + 1;
                return r;
            }
            if(current->key < key){
                r = r + rankOfChild(current->left) + 1;
                current = current->right;
                continue;
            }
            assert(current->key > key);
            current = current->left;
        }
        throw invalidRank();
    }
    int biggerSelect (const T& key) const
    {
        int sum = 0;
        node* current = root->right;
        while (current != nullptr){
            if(current->key == key){
                sum += current->info + infoOfChild(current->right);
                return sum;
            }
            if(current->key < key){
                current = current->right;
                continue;
            }
            assert(current->key > key);
            sum += current->info + infoOfChild(current->right);
            current = current->left;
        }
        throw invalidRank();
    }

    class iterator{
        node* current;
        explicit iterator(node* init) : current(init){}
        friend RankAVL;
    public:
        const T& getKey() const
        {
            return current->key;
        }

        S& getData() const
        {
            return current->data;
        }

        S& operator* () const
        {
            return this->getData();
        }

        const iterator& operator++ (){
            if(current->right !=nullptr){
                node* next = current->right;
                while(next != nullptr){
                    current = next;
                    next = next->left;
                }
                return *this;
            }
            else{
                while(current->parent->right == current && current->parent->height != -1){//height -1 only in root
                    current = current->parent;
                }
                current = current->parent;
                return *this;
            }
        }

        const iterator operator++ (int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator == (const iterator& it) const
        {
            return (it.current == current);
        }

        bool operator != (const iterator& it) const
        {
            return !(*this == it);
        }
    };
    class revIterator{
        node* current;
        revIterator(node* init) : current(init){}
        friend RankAVL;
    public:
        S& getData() const{
            return current->data;
        }
        const T& getKey() const{
            return current->key;
        }
        S& operator* () const{
            return getData;
        }
        bool operator == (const revIterator& it) const
        {
            return (it.current == current);
        }
        bool operator != (const revIterator& it) const
        {
            return !(*this == it);
        }

        const revIterator& operator++()
        {
            if(current->left !=nullptr){
                node* next = current->left;
                while(next != nullptr){
                    current = next;
                    next = next->right;
                }
                return *this;
            }
            else{
                while(current->parent->left == current){//no check for root, cause it's only his right child.
                    current = current->parent;
                }
                current = current->parent;
                return *this;
            }
        }
        const revIterator operator++(int)
        {
            revIterator tmp = *this;
            ++(*this);
            return tmp;
        }
    };
    //class const_interator;

    iterator begin() const
    {
        return RankAVL<T,S>::iterator(lowest != nullptr ? lowest : root);
    }

    iterator end() const
    {
        return RankAVL<T,S>::iterator(root);
    }

    revIterator rbegin() const
    {
        return revIterator(highest != nullptr ? highest : root);
    }

    revIterator rend() const
    {
        return revIterator(root);
    }

    bool isEmpty() const
    {
        return root->right == nullptr;
    }

    class RankAVLException : public std::exception {};
    class notFound : public RankAVLException{
    public:
        const char* what () const noexcept override{
            return "RankAVLError: key not found.\n";
        }
    };
    class invalidInput : public RankAVLException{
    public:
        const char* what () const noexcept override{
            return "RankAVLError: invalid input.\n";
        }
    };
    class invalidRank : public RankAVLException{
    public:
        const char* what () const noexcept override{
            return "RankAVLError: invalid rank.\n";
        }
    };
    class alreadyExists : public RankAVLException{
    public:
        const char* what () const noexcept override{
            return "RankAVLError: key already exists.\n";
        }
    };

};

#endif //RANKAVL_H
