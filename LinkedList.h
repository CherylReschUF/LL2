#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
using std::cout;
using std::endl;
using std::vector;
using std::runtime_error;
using std::out_of_range;

template <typename T>
class LinkedList
{
public:
    struct Node
    {
        T data;
        Node* next;
        Node* prev;

        Node()
        {
            data = {};
            next = prev = nullptr;
        }
    };

    /*==== Behaviors ====*/
    void PrintForward() const;
    void PrintReverse() const;
    void PrintForwardRecursive(const Node* node) const;
    void PrintReverseRecursive(const Node* node) const;

    /*==== Accessors ====*/
    unsigned int NodeCount() const;
    void FindAll(vector<Node*>& outData, const T& value) const;
    const Node* Find(const T& data) const;
    Node* Find(const T& data);
    const Node* GetNode(unsigned int index) const;
    Node* GetNode(unsigned int index);
    Node* Head();
    const Node* Head() const;
    Node* Tail();
    const Node* Tail() const;

    /*==== Insertion ====*/
    void AddHead(const T& data);
    void AddTail(const T& data);
    void AddNodesHead(const T* data, unsigned int count);
    void AddNodesTail(const T* data, unsigned int count);
    void InsertAfter(Node* node, const T& data);
    void InsertBefore(Node* node, const T& data);
    void InsertAt(const T& data, unsigned int index);

    /*==== Removal ====*/
    bool RemoveHead();
    bool RemoveTail();
    unsigned int Remove(const T& data);
    bool RemoveAt(unsigned int index);
    void Clear();

    /*==== Operators ====*/
    const T& operator[](unsigned int index) const;
    T& operator[](unsigned int index);
    bool operator==(const LinkedList<T>& rhs) const;
    LinkedList<T>& operator=(const LinkedList<T>& rhs);

    /*==== Construction / Destruction ====*/
    LinkedList();
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();

private:
    // Helpers
    void Set(const LinkedList<T>& l);
    void NodeRemoved();

    Node* head_;
    Node* tail_;
    unsigned int nodeCount_;
};

template <typename T>
LinkedList<T> & LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    Set(rhs);
    return *this;
}
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &list)
{
    Set(list);
}

template <typename T>
void LinkedList<T>::Set(const LinkedList<T> &l)
{
    this->nodeCount_ = l.nodeCount_;
    if (nodeCount_ == 0)
    {
        head_ = tail_ = nullptr;
        return;
    }

    // Create the first node.
    head_ = new Node;
    head_->data = l.head_->data;
    head_->prev = nullptr;

    Node* lCurr = l.head_->next;
    Node* prev = head_;
    while (lCurr)
    {
        Node* newNode = new Node;
        newNode->data = lCurr->data;
        newNode->prev = prev;
        newNode->next = nullptr;

        prev->next = newNode;
        prev = newNode;
        lCurr = lCurr->next;
    }
    // No more nodes, must be at the end.
    tail_ = prev;
}

template <typename T>
void LinkedList<T>::NodeRemoved()
{
    nodeCount_--;
    if (nodeCount_ == 0)
        head_ = tail_ = nullptr;
}

template <typename T>
LinkedList<T>::LinkedList()
{
    head_ = tail_ = nullptr;
    nodeCount_ = 0;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    Clear();
}
template <typename T>
void LinkedList<T>::AddHead(const T& data)
{
    // Create a new node.
    Node* newNode = new Node;

    // Set its data
    newNode->data = data;

    // The new node (which will eventually be the first node)
    // should have the current head as its next
    newNode->next = head_;

    if (head_ != nullptr)
        head_->prev = newNode;

    // Reassign the head, to the new node
    head_ = newNode;

    // One and only node...
    if (tail_ == nullptr)
        tail_ = head_;

    nodeCount_++;
}

template <typename T>
void LinkedList<T>::AddTail(const T& data)
{
    Node* newNode = new Node;
    newNode->data = data;

    newNode->prev = tail_;
    if (tail_ != nullptr)
        tail_->next = newNode;

    tail_ = newNode;

    if (head_ == nullptr)
        head_ = tail_;

    nodeCount_++;
}

template <typename T>
void LinkedList<T>::PrintForward() const
{
    Node* current = head_;

    while (current)
    {
        cout << current->data << endl;
        current = current->next;
    }
}

template <typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const
{
    if (node != nullptr)
    {
        cout << node->data << endl;
        PrintForwardRecursive(node->next);
    }
}

template <typename T>
void LinkedList<T>::PrintReverse() const
{
    Node* current = tail_;

    while (current)
    {
        cout << current->data << endl;
        current = current->prev;
    }
}

template <typename T>
void LinkedList<T>::PrintReverseRecursive(const Node*  node) const
{
    if (node != nullptr)
    {
        cout << node->data << endl;
        PrintReverseRecursive(node->prev);
    }
}

template <typename T>
unsigned int LinkedList<T>::NodeCount() const
{
    return nodeCount_;
}

template <typename T>
void LinkedList<T>::InsertAt(const T& data, unsigned int index)
{
    if (index < 0 || index > nodeCount_)
        throw out_of_range("Error! Index out of range!");

    if (index == 0)
    {
        AddHead(data);
        return;
    }
    else if (index == nodeCount_)
    {
        AddTail(data);
        return;
    }

    Node* current = head_;
    Node* previous = nullptr;
    for (unsigned int i = 0; i < index; i++)
    {
        previous = current;
        current = current->next;
    }

    Node* newNode = new Node;
    newNode->data = data;

    // Update surrounding nodes
    previous->next = newNode;
    current->prev = newNode;

    // Set newNode's pointers
    newNode->next = current;
    newNode->prev = previous;
    nodeCount_++;
}

template <typename T>
bool LinkedList<T>::RemoveHead()
{
    if (head_ != nullptr)
    {
        Node* deleteMe = head_;
        head_ = head_->next;

        if (head_ != nullptr)
            head_->prev = nullptr;

        delete deleteMe;
        NodeRemoved();

        return true;
    }

    return false;
}

template <typename T>
bool LinkedList<T>::RemoveTail()
{
    if (tail_ != nullptr)
    {
        Node* deleteMe = tail_;
        tail_ = tail_->prev;
        if (tail_ != nullptr)
            tail_->next = nullptr;

        delete deleteMe;
        NodeRemoved();

        return true;
    }
    return false;
}

template <typename T>
unsigned int LinkedList<T>::Remove(const T& data)
{
    unsigned int removed = 0;
    Node* current = head_;
    Node* previous = nullptr;

    while (current)
    {
        if (current->data == data)
        {
            removed++;
            if (current == head_)
            {
                current = current->next;
                RemoveHead();
                continue;
            }
            else if (current == tail_)
            {
                current = nullptr;
                RemoveTail();
                continue;
            }
            else
            {
                // Get rid of current node
                Node* deleteMe = current;

                previous->next = current->next;

                if (current->next != nullptr)
                    current->next->prev = previous;

                current = current->next;
                delete deleteMe;
                NodeRemoved();
                continue;
            }
        }

        previous = current;
        current = current->next;
    }

    return removed;
}

template <typename T>
bool LinkedList<T>::RemoveAt(unsigned int index)
{
    if (index >= nodeCount_)
        return false;

    if (index == 0)
    {
        RemoveHead();
        return true;
    }
    else if (index == nodeCount_ - 1)
    {
        RemoveTail();
        return true;
    }

    Node* current = head_;
    Node* previous = nullptr;
    for (unsigned int i = 0; i < index; i++)
    {
        previous = current;
        current = current->next;
    }

    previous->next = current->next;
    if (current->next != nullptr)
        current->next->prev = previous;

    delete current;
    NodeRemoved();

    return true;
}

template <typename T>
void LinkedList<T>::Clear()
{
    Node* current = head_;
    while (current)
    {
        head_ = head_->next;
        delete current;
        current = head_;
    }
    nodeCount_ = 0;
    head_ = tail_ = nullptr;
}

template <typename T>
void LinkedList<T>::FindAll(vector<Node*> &outData, const T& value) const
{
    Node* current = head_;

    while (current)
    {
        if (current->data == value)
            outData.push_back(current);
        current = current->next;
    }
}

template <typename T>
const T& LinkedList<T>::operator[](unsigned int index) const
{
    if (index >= nodeCount_)
        throw out_of_range("Error! Index out of range!");

    Node* curr = head_;
    for (unsigned int i = 0; i < index; i++)
        curr = curr->next;

    return curr->data;
}

template <typename T>
T& LinkedList<T>::operator[](unsigned int index)
{
    if (index >= nodeCount_)
        throw out_of_range("Error! Index out of range!");

    Node* curr = head_;
    for (unsigned int i = 0; i < index; i++)
        curr = curr->next;
    return curr->data;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) const
{
    if (index >= nodeCount_)
        throw out_of_range("Error! Index out of range!");

    Node* curr = head_;
    for (unsigned int i = 0; i < index; i++)
        curr = curr->next;
    return curr;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index)
{
    if (index >= nodeCount_)
        throw out_of_range("Error! Index out of range!");

    Node* curr = head_;
    for (unsigned int i = 0; i < index; i++)
        curr = curr->next;
    return curr;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T> &rhs) const
{
    if (nodeCount_ != rhs.nodeCount_)
        return false;

    Node* myCurr = head_;
    Node* rhsCurr = rhs.head_;

    while (myCurr && rhsCurr)
    {
        if (myCurr->data != rhsCurr->data)
            return false;

        myCurr = myCurr->next;
        rhsCurr = rhsCurr->next;
    }
    return true;
}

template <typename T>
void LinkedList<T>::AddNodesHead(const T *data, unsigned int count)
{
    for (unsigned int i = count-1; i >= 0 && i < count; i--)
        AddHead(data[i]);
}

template <typename T>
void LinkedList<T>::AddNodesTail(const T *data, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
        AddTail(data[i]);
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data)
{
    // Need to create a test case for inserting after the last node (they'll need nullptr checks);
    if (node == tail_)
    {
        AddTail(data);
        return;
    }

    Node* curr = head_;
    while (curr)
    {
        if (curr == node)
        {
            Node* newNode = new Node;
            newNode->data = data;

            newNode->prev = curr;
            newNode->next = curr->next;

            curr->next->prev = newNode;
            curr->next = newNode;
            return;
        }

        curr = curr->next;
    }
}

template <typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data)
{
    // Need to create a test case for inserting after the last node (they'll need nullptr checks);
    if (node == head_)
    {
        AddHead(data);
        return;
    }

    Node* curr = head_;
    while (curr)
    {
        if (curr == node)
        {
            Node* newNode = new Node;
            newNode->data = data;

            newNode->next = curr;
            newNode->prev = curr->prev;

            curr->prev->next = newNode;
            curr->prev = newNode;
            return;
        }

        curr = curr->next;
    }
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) const
{
    Node* curr = head_;

    while (curr)
    {
        if (curr->data == data)
            return curr;

        curr = curr->next;
    }

    return nullptr;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data)
{
    Node* curr = head_;

    while (curr)
    {
        if (curr->data == data)
            return curr;

        curr = curr->next;
    }

    return nullptr;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head()
{
    return head_;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const
{
    return head_;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail()
{
    return tail_;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const
{
    return tail_;
}