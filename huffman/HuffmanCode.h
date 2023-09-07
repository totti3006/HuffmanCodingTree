#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include "HuffmanTree.h"
#include "../include/PriorityQueue.h"
#include <unordered_map>
#include <vector>
#include <algorithm>


typedef HuffTree<string>::HuffNode HNode;

class HuffmanCode {
private:
    HNode **weightNodes;
    int size; // size of weightNodes
    unordered_map<string, string> huffmanCode; // string of char, string of bits
    HuffTree<string> huffTree;

public:
    HuffmanCode() {}
    HuffmanCode(string input) {
        this->generateWeigthNodes(input);
        this->buildHuffTree(HuffmanCode::minHeapLeafNodeComparator);
        this->generateHuffCode();
    }
    ~HuffmanCode() {}

    void generateWeigthNodes(string input) {
        vector<HNode*> v;

        for (int i = 1; i < 256; i++) {
            int ch = (char) i;
            int freq = count(input.begin(), input.end(), ch);
            if (freq > 0) {
                char c = (char) i;
                string s(1, c);
                v.push_back(new HNode(s, freq));
            }
        }

        this->size = v.size();
        this->weightNodes = new HNode*[v.size()];
        for (unsigned int i = 0; i < v.size(); i++) {
            this->weightNodes[i] = v[i];
        }
    }

    void buildHuffTree(int (*comparator)(HNode*& , HNode*&)) {
        PriorityQueue<HNode*> pq(comparator);
        for (int i = 0; i < this->size; i++) pq.push(this->weightNodes[i]);
        
        HNode *temp1, *temp2, *temp3 = nullptr;
        if (pq.size() == 1) {
            temp1 = pq.pop();
            temp3 = new HNode(temp1->getKey(), temp1->getWeigth(), nullptr, temp1);
        }

        while (pq.size() > 1) {
            temp1 = pq.pop(); 
            temp2 = pq.pop(); 
            string s = temp1->getKey() + temp2->getKey();
            temp3 = new HNode(s, temp1->getWeigth() + temp2->getWeigth(), temp1, temp2);
            pq.push(temp3); 
        }        

        this->huffTree.setRoot(temp3);
    }

    void printPriorityQueue(int (*comparator)(HNode*& , HNode*&)) {
        PriorityQueue<HNode*> pq(comparator);
        for (int i = 0; i < this->size; i++) {
            cout << this->weightNodes[i]->getKey() << ": " << this->weightNodes[i]->getWeigth() << endl;
            pq.push(this->weightNodes[i]);
        }
        // pq.println(leaf2str);

        cout << "[";
        while (pq.size() > 0) {
            HNode *temp = pq.pop();
            cout << temp << ", ";
        }
        cout << "]\n";
    }

    void printPQver2(int (*comparator)(HNode*& , HNode*&), PriorityQueue<HNode*> pq) {
        cout << "[";
        while (pq.size() > 0) {
            HNode *temp = pq.pop();
            cout << temp << ", ";
        }
        cout << "]\n";
    }

    void generateHuffCode() {
        this->genHuffCode(this->huffTree.getRoot(), "");
    }

    void process(string input) {
        this->generateWeigthNodes(input);
        this->buildHuffTree(HuffmanCode::minHeapLeafNodeComparator);
        this->generateHuffCode();
    }

    string encode(string input) {
        string encoded = "";

        for (int i = 0; i < input.size(); i++) {
            encoded.append(this->huffmanCode.at(input.substr(i, 1)));
        }

        return encoded;
    }

    string decode(string input) {
        HNode *root = this->huffTree.getRoot();
        if (root == nullptr) return "";

        string decoded;
        HNode *temp = root;
        for (int i = 0; i < input.size(); i++) {
            if (temp == nullptr) break;

            if (input.at(i) == '0') {
                temp = temp->getLeft();
            } else {
                temp = temp->getRight();
            }
            
            if (temp->isLeaf()) {
                decoded.append(temp->getKey());
                temp = root;
            }
        }

        return decoded;
    }

    void printHuffCode() {
        for (auto& x: this->huffmanCode) {
            cout << x.first << ": " << x.second << endl;
        }
    }

    void printInOrderHuffTree() {
        this->huffTree.printInOrder();
    }

    void printWeightNodes() {
        for (int i = 0; i < this->size; i++) {
            cout << this->weightNodes[i]->getKey() << ": " << this->weightNodes[i]->getWeigth() << endl;
        }
    }

    static int minHeapLeafNodeComparator(HNode* &a, HNode* &b){
        if (a->getWeigth() < b->getWeigth()) return -1;
        else if(a->getWeigth() > b->getWeigth()) return +1;
        else {
            if (a->getKey().length() == b->getKey().length()) {
                if (a->getKey().at(0) - b->getKey().at(0) < 0) return -1;
                else return +1;
            } else if (a->getKey().length() < b->getKey().length()) {
                return -1;
            } else if (a->getKey().length() > b->getKey().length()) {
                return +1;
            }
            return 0;
        }
    }

private:
    void genHuffCode(HNode *root, string str) {
        if (root == nullptr) return;

        if (!root->getLeft() && !root->getRight()) {
            this->huffmanCode[root->getKey()] = str;
        }

        genHuffCode(root->getLeft(), str + "0");
        genHuffCode(root->getRight(), str + "1");
    }
};

#endif