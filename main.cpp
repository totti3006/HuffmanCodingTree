#include <iostream>
#include <sstream>
#include "huffman/HuffmanCode.h"
#include "huffman/HuffmanTree.h"

using namespace std;

template<class K, class V>
struct Pair{
    K key;
    V value;
    Pair() {}
    Pair(K key, V value){
        this->key = key;
        this->value = value;
    }
    Pair(const Pair& pair){
        this->key = pair.key;
        this->value = pair.value;
    }
    Pair& operator=(const Pair& pair){
        this->key = pair.key;
        this->value = pair.value;
        return *this;
    }

    bool operator>(const Pair& rhs) const {
        return (this->value > rhs.value);
    }

    bool operator<(const Pair& rhs) const {
        return (this->value < rhs.value);
    }

    bool operator==(const Pair& rhs) const {
        return (this->value == rhs.value);
    }
};

template<class K, class V>
ostream &operator<<(ostream &os, const Pair<K,V>& pair){
    os << "Pair("
        << pair.key << ", "
        << pair.value
       << ")";
    return os;
}

typedef HuffTree<string>::HuffNode HNode;

ostream &operator<<(ostream &os, const HNode& node){
    os << "N("
        << node.getKey() << ", "
        << node.getWeigth()
       << ")";
    return os;
}

ostream &operator<<(ostream &os, const HNode* node){
    os << "N("
        << node->getKey() << ", "
        << node->getWeigth()
       << ")";
    return os;
}

string leaf2str(HNode& node){
    stringstream os;
    os  << "("  << node.getKey()
                << ", "
                << node.getWeigth()
        << ")";
    return os.str();
}

string leaf2str(HNode*& node){
    stringstream os;
    os  << "("  << node->getKey()
                << ", "
                << node->getWeigth()
        << ")";
    return os.str();
}

int minHeapComparator(int& a, int& b){
    if (a < b) return -1;
    else if(a > b) return +1;
    else return 0;
}
int maxHeapComparator(int& a, int& b){
    if (a < b) return +1;
    else if(a > b) return -1;
    else return 0;
}

int minHeapLeafNodeComparator(HNode* &a, HNode* &b){
    if (a->getWeigth() < b->getWeigth()) return -1;
    else if(a->getWeigth() > b->getWeigth()) return +1;
    else return 0;
}

int minHeapLeafNodeComparator(HNode &a, HNode &b){
    if (a.getWeigth() < b.getWeigth()) return -1;
    else if(a.getWeigth() > b.getWeigth()) return +1;
    else return 0;
}

int minHeapPairComparator(Pair<int, int>& a, Pair<int, int>& b) {
    if (a.value < b.value) return -1;
    else if (a.value > b.value) return +1;
    else return 0;
}

string pair2Str(Pair<int, int>& pair) {
    stringstream os;
    os << pair;
    return os.str();
}

int main()
{
    HuffmanCode hc;
    string input = "vwgxPPmmIGDQIaxuvbUBsNovOjlfykCdeCStuNCpMFJFRHjqbgj";
    hc.generateWeigthNodes(input);

    // hc.printWeightNodes();
    // hc.printPriorityQueue(HuffmanCode::minHeapLeafNodeComparator);

    hc.buildHuffTree(HuffmanCode::minHeapLeafNodeComparator);

    hc.generateHuffCode();

    string encoded = hc.encode(input);
    cout << encoded << endl;

    cout << hc.decode(encoded) << endl;


    return 0;
}