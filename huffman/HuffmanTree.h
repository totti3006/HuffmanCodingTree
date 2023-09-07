#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
using namespace std;

template<typename K>
class HuffTree {
public:
    class HuffNode;
private:
    HuffNode* root;
public:
    HuffTree() {
        this->root = nullptr;
    }
    HuffTree(HuffNode *root) {
        this->root = root;
    }
    ~HuffTree() {
        this->clear();
    }

    class HuffNode {
    private:
        K key;
        int weigth;
        HuffNode *pLeft, *pRight;
        friend class HuffTree<K>;
    public:
        HuffNode() : pLeft(nullptr), pRight(nullptr) {}
        HuffNode(K key, int weigth) : key(key), weigth(weigth), pLeft(nullptr), pRight(nullptr) {}
        HuffNode(K key, int weigth, HuffNode *left, HuffNode *right) : key(key), weigth(weigth), pLeft(left), pRight(right) {}
        ~HuffNode() {}

        K getKey() const {
            return this->key;
        }

        int getWeigth() const {
            return this->weigth;
        }

        HuffNode* getLeft() const {
            return this->pLeft;
        }

        HuffNode* getRight() const {
            return this->pRight;
        }

        void setLeft(HuffNode *node) {
            this->pLeft = node;
        }

        void setRight(HuffNode *node) {
            this->pRight = node;
        }

        void increaseWeight() {
            this->weigth++;
        }

        bool isLeaf() {
            return (this->pLeft == nullptr && this->pRight == nullptr);
        }

        bool operator<(HuffNode rhs) {
            return (this->weigth < rhs.weigth);
        }

        bool operator>(HuffNode rhs) {
            return (this->weigth > rhs.weigth);
        }
    };
    
    void clear() {
        this->deleteNode(this->root);
    }

    void printInOrder() {
        this->inTraversal(this->root);
    }

    void setRoot(HuffNode* root) {
        this->root = root;
    }

    HuffNode* getRoot() const {
        return this->root;
    }

private:
    void deleteNode(HuffNode *node) {
        if (node == nullptr) return;

        this->deleteNode(node->pLeft);
        this->deleteNode(node->pRight);

        delete node;
    }

    void inTraversal(HuffNode *root) {
        if (root == nullptr) return;
        inTraversal(root->pLeft);
        cout << *root << endl;
        inTraversal(root->pRight);
    }
};

#endif