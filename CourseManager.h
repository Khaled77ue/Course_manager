
#ifndef WET_1_COURSEMANAGER_H
#define WET_1_COURSEMANAGER_H
#include "AVLBinaryTree.h"
#include <assert.h>
#include <memory>



template <class T>
class  Vector {
public:
    //Vector(unsigned int size) : length(size) {};
    Vector(unsigned int size, const T& initial = T()) : length(size) {
        vec = new T[size];
        for (unsigned int i = 0; i < size; i++)
            vec[i] = initial;
    }
    Vector(const Vector<T>& v) {
        length = v.length;
        vec = new T[length];
        for (unsigned int i = 0; i < length; i++)
            vec[i] = v.vec[i];
    }
    ~Vector() {
        delete[] vec;
    }

    unsigned int size() const {
        return length;
    }
    T& operator[](unsigned int index) {
        return vec[index];
    }

private:
    unsigned int length;
    T* vec;
};
///////////////////////////////////////////////////////////////////////////
class Empty {};


void inOrderAndDestroy(AVL_TREE<int, AVL_TREE<int, Empty>>::AVLNode<int, AVL_TREE<int, Empty>>* root);////////////
void DestroyRecursive_(AVL_TREE<int, Empty>::AVLNode<int, Empty>* node);



class DoubleDirectionNode {
public:
    explicit DoubleDirectionNode(unsigned int x = 0) :sum(x), CoursesSameSumTree(new AVL_TREE<int, AVL_TREE<int, Empty>>()),
                                                      next(nullptr), previous(nullptr) {};
    ~DoubleDirectionNode() {
        delete CoursesSameSumTree;


    }
    unsigned int sum;
    AVL_TREE<int, AVL_TREE<int, Empty>>* CoursesSameSumTree;
    DoubleDirectionNode* next;
    DoubleDirectionNode* previous;


};

class LectursArray {
public:
    explicit LectursArray(const int amountOfClasses) :Classes(amountOfClasses), size(amountOfClasses) {};
    ~LectursArray() = default;

    DoubleDirectionNode*& operator[](const unsigned int x) {
        return Classes[x];
    }
    int Size()const {
        return size;
    }
    Vector<DoubleDirectionNode*> Classes;

private:
    unsigned int size;
};

class ClassesCount {
public:
    ClassesCount() :start(nullptr), end1(nullptr) {
        DoubleDirectionNode* tmp = new DoubleDirectionNode(0);

        end1 = tmp;
        start = tmp;

    }
    ~ClassesCount() {
        while (start != end1) {
            DoubleDirectionNode* tmp = start;
            start = start->next;
            delete tmp;

        }


        if (start == end1 && start != nullptr && end1 != nullptr) {
            delete  end1;

            start = nullptr;
            end1 = nullptr;
        }
    }
    void addNode(DoubleDirectionNode* current, unsigned int sum) {
        DoubleDirectionNode* tmp = new DoubleDirectionNode(sum);
        if (current->previous == nullptr) {
            tmp->next = current;
            tmp->previous = nullptr;
            current->previous = tmp;
            start = tmp;
        }
        else {
            DoubleDirectionNode* prev = current->previous;
            tmp->next = current;
            current->previous = tmp;
            prev->next = tmp;
            tmp->previous = prev;
        }
    }
    void removeNode(DoubleDirectionNode* current) {
        if (current->sum == 0) {
            return;
        }
        if (current->previous == nullptr) {
            start = start->next;
            delete current;
            return;
        }
        current->next->previous = current->previous;
        current->previous->next = current->next;
        delete current;
    }


    DoubleDirectionNode* getStart() {
        return start;
    }
    DoubleDirectionNode* getEnd() {
        return end1;
    }
private:

    DoubleDirectionNode* start;
    DoubleDirectionNode* end1; // it's for sum=0 it's not going change
};

class CoursesManager {
public:
    CoursesManager() :numOfClasses(0) {};
    ~CoursesManager() = default;

    void addCourse(const int CourseID, const int numOfClasses) {
        LectursArray* tmp = new LectursArray(numOfClasses);
        for (int i = 0; i < numOfClasses; ++i) {
            tmp->operator[](i) = ClassesCount1.getEnd();
        }
        CoursesTree.putNodeInMyTree(0,CourseID, tmp,0);

        AVL_TREE<int, Empty>* artist_zero = new AVL_TREE<int, Empty>();

        for (int i = 0; i < numOfClasses; ++i) {
            //Empty* it = new Empty();
            artist_zero->putNodeInMyTree(0,i,nullptr,0);

        }

        ClassesCount1.getEnd()->CoursesSameSumTree->putNodeInMyTree(0,CourseID, artist_zero,0);
        this->numOfClasses = (this->numOfClasses) + numOfClasses;
        return;
    }
    int getnum() {
        return numOfClasses;
    }

    void  RemoveCourse_M(int CourseID) {
        LectursArray* res = CoursesTree.FindInTree(CourseID,0);
        for (int i = 0; i < res->Size(); i++) {
            //auto *tex = res->operator[](i)->ArtistsSameSumTree->Find(artistID);
            //tex->cleara(tex->m_pRoot);
            res->operator[](i)->CoursesSameSumTree->RemoveInTree(CourseID,0);
        }
        this->numOfClasses = (this->numOfClasses) -(res->Classes.size());

        //delete res;
        CoursesTree.RemoveInTree(CourseID,0);

        return;
    }

    LectursArray* accessCourseData(int artistID) {
        return CoursesTree.FindInTree(artistID,0);
    }

    ClassesCount& accessClassCount() {
        return ClassesCount1;
    }

private:
    int numOfClasses;
    AVL_TREE<int, LectursArray> CoursesTree;
    ClassesCount ClassesCount1;
};

/////////////////////   stack    //////////////////////////////
template <class T>
class Stack {
public:
    T* data;
    int size;
    int nextIndex;
    //public:
    explicit Stack(int size = 0);
    ~Stack();
    void push(T& t);
    T& pop();
    bool isEmpty() const;
};

template <class T>
Stack<T>::Stack(int size) :
        data(new T[size]), size(size), nextIndex(0) {
}

template <class T>
Stack<T>::~Stack() {
    delete[] data;
}

template <class T>
void Stack<T>::push(T& t) {
    assert(nextIndex < size);
    data[nextIndex++] = t;
}

template <class T>
T& Stack<T>::pop() {
    assert(nextIndex > 0);
    return data[nextIndex--];
}
template <class T>
bool Stack<T>::isEmpty()const {
    return nextIndex == 0;
}
///////////////////////////////////////////////////////////////////////////


void inOrderAndDestroy(AVL_TREE<int,AVL_TREE<int,Empty>>::AVLNode<int, AVL_TREE<int, Empty>>* root) {
    if (root == nullptr) {
        return;
    }
    inOrderAndDestroy(root->left_son);
    inOrderAndDestroy(root->right_son);
    DestroyRecursive_(root->m_Data->Root);
}

void DestroyRecursive_(AVL_TREE<int, Empty>::AVLNode<int, Empty>* node) {
    if (node != nullptr)
    {
        DestroyRecursive_(node->left_son);
        DestroyRecursive_(node->right_son);
        delete node;
    }
    return;
}







#endif //WET_1_COURSEMANAGER_H
