// Hackerrank_Abstract_Classes_Polymorphism.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node {
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
    Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {

protected:
    map<int, Node*> mp; //map the key to the node in the linked list
    int cp = 0;  //capacity
    Node* tail = NULL; // double linked list tail pointer
   
    virtual void set(int, int) = 0; //set function
    virtual int get(int) = 0; //get function
public:
    Node* head = NULL; // double linked list head pointer
};

class LRUCache : public Cache {
    int nodeEntries;

    //private functions
    Node* isKeyInLinkedList(int);
    void CacheHitUpdate(Node*,int,int);
public:    
    void set(int, int);
    int get(int);
    LRUCache(int cap) {
        cp = cap;
        nodeEntries = 0;
        tail = NULL;
        head = NULL;
    }
};

void LRUCache::CacheHitUpdate(Node* tempNode, int key, int val)
{
    //Cache hit
    //for the node thats hit, link the prev and next node together, then re-arrange the cache hit key/value pair to the head
    tempNode->key = key;
    tempNode->value = val;
    //linking prior and next nodes
    if (head != tempNode)
    {
        if (tempNode->prev != NULL)
            tempNode->prev->next = tempNode->next;
        if (tempNode->next != NULL)
            tempNode->next->prev = tempNode->prev;
        //makes temp node the head
        head->prev = tempNode;
        tempNode->next = head;
        head = tempNode;
    }

}


/*
    LRUCache::isKeyInLinkedList(int key)

    If the key exists in the linked list, return the Node*,
    otherwise returns NULL
*/
Node* LRUCache::isKeyInLinkedList(int key)
{
    Node* itr;
    itr = head;

    while (itr != NULL)
    {
        if (itr->key == key) break;
        itr = itr->next;
    }
    return itr;
}

void LRUCache::set(int key, int val)
{
    Node* tempNode;
    Node* getNode;
    if (nodeEntries < cp)
    {
        //Cache is not yet filled

        if (nodeEntries == 0)
        {
            head = new Node(key, val);
            tail = head;
            head->next = NULL;
            head->prev = NULL;
            nodeEntries++;
        }
        else
        {
            getNode = isKeyInLinkedList(key);
            if (getNode == NULL)
            {
                tempNode = new Node(key, val);
                tempNode->next = head;
                tempNode->prev = NULL;
                head->prev = tempNode;
                head = tempNode;
                nodeEntries++;
            }
            else
            {
                getNode->value = val;
                if (getNode != head) // no need to update position if already at head
                {
                    //move node with found key to the front   
                    if (getNode->prev != NULL) // dont attempt to write to a null pointer
                        getNode->prev->next = getNode->next;
                    if (getNode->next != NULL)
                        getNode->next->prev = getNode->prev;
                    getNode->prev = NULL;
                    getNode->next = head;
                    head = getNode;
                }
            }
        }
    }
    else
    {
        //Cache is full
        tempNode = isKeyInLinkedList(key);

        if (tempNode == NULL)
        {
            //Cache miss
            //we don't have to erase or create any nodes, just change the key/value contents of existing nodes

            //puts the new key/value pair into the tail; then re-arrange the node to become the head
            tail->key = key;
            tail->value = val;
            if (head != tail)
            {
                //only update linked list if there is more than 1 element
                tail->next = head;
                tail->prev->next = NULL;
                head->prev = tail;  //tail is about to be the new head             
                head = tail;
                tail = tail->prev; //pre-tail becomes new the tail
                head->prev = NULL;
            }
        }
        else
        {
            CacheHitUpdate(tempNode,key,val);       
        }
    }
}

int LRUCache::get(int key)
{
    Node* tempNode;
    tempNode = isKeyInLinkedList(key);
    if (tempNode == NULL)
    {
        return -1;
    }
    else
    {
        //technically writing the same key/value again, but allows code reuse for setting Cache Hits
        CacheHitUpdate(tempNode,key,tempNode->value);
        return tempNode->value;
    } 
    

    //TODO - implement map container to speed up searching!
}

void PrintKeysInNode(Node* node)
{
    while (node != NULL)
    {
        printf("%d:%d  ", node->key, node->value);
        node = node->next;
    }
    printf("END\n");
}

int main() {
    int n, capacity, i;
    cin >> n >> capacity;
    LRUCache l(capacity);
    for (i = 0; i < n; i++) {
        string command;
        PrintKeysInNode(l.head);
        cin >> command;
        if (command == "get") {
            int key;
            cin >> key;
            cout << l.get(key) << endl;
        }
        else if (command == "set") {
            int key, value;
            cin >> key >> value;
            l.set(key, value);
        }
    }
    return 0;
}
