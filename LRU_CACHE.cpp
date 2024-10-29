#include <bits/stdc++.h>
using namespace std;

// Implementing an LRU(Least Recently Used) Cache using Doubly linked-list and Hash map
class Node {
public:
    Node *prev, *next;
    pair<int, int> data;

    Node() {
        this->prev = NULL;
        this->next = NULL;
        this->data = {0, 0};
    }
};

class LRU_Cache {
private:
    unordered_map<int, Node*> cache; // used to search key in O(1) time
    Node *head, *tail;
    int size, capacity;

public:
    LRU_Cache() {
        // Initialize head and tail nodes
        head = new Node();
        tail = new Node();
        
        // create the structure for the Doubly linked-list
        head->next = tail;
        tail->prev = head;
        
        this->size = 0;
        this->capacity = 0;
    }

    void setCapacity(int cap) {
        this->capacity = cap;
    }

    int getCapacity() {
        return this->capacity;
    }

    int get(int key) {
        // get the value of LRU key from LRU cache
        if (cache.find(key) != cache.end()) {
            // update the order of the cache to achieve LRU behavior
            Node* curr = cache[key];
            int result = curr->data.second;

            // remove curr from its current position
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;

            // insert curr next to head
            Node* nexHead = head->next;
            head->next = curr;
            curr->prev = head;
            curr->next = nexHead;
            nexHead->prev = curr;

            // return the value of the key
            return result;
        }
        // key not found
        else return -1;
    }

    void put(int key, int value) {
        // Check if the key already exists
        if (cache.find(key) != cache.end()) {
            // Update the value and reorder
            Node* existingNode = cache[key];
            existingNode->data.second = value;
            
            // Move the node to the front (Most Recently Used position)
            get(key);
            return;
        }

        // push a new entry to the LRU cache -> maintaining the order of the LRU key
        if (this->size == this->capacity) {
            // if the size of cache is full then remove the Least Recently Used entry
            // LRU entry is just before the tail
            Node* lru = tail->prev;

            // remove the LRU node from the map and list
            cache.erase(lru->data.first);   // v.imp step
            lru->prev->next = tail;
            tail->prev = lru->prev;

            // deallocate memory of lru
            delete lru;
            // reduce the size
            size--;
        }

        // create a new node
        Node* newNode = new Node();
        newNode->data = {key, value};

        // put new entry next to the head -> because it's the Most recently used entry.
        Node* nexHead = head->next;
        head->next = newNode;
        newNode->prev = head;
        newNode->next = nexHead;
        nexHead->prev = newNode;

        // push the same entry into hashmap and increase the size value
        size++;
        cache[key] = newNode;
    }

    void printCache(){
        Node* temp = this->head->next;
        while(temp != this->tail){
            cout<<"("<<temp->data.first<<"->"<<temp->data.second<<"), ";
            temp = temp->next;
        }
        cout<<endl;
        return;
    }
};

int main() {
    LRU_Cache lru;

    int cap = 0;
    cout << "Enter the capacity of LRU cache: ";
    cin >> cap;
    lru.setCapacity(cap);

    lru.put(1, 4);
    lru.put(2, 6);
    cout << lru.get(1) << endl;
    lru.printCache();
    lru.put(6, 12);
    lru.put(3, 2);
    lru.printCache();
    lru.put(12, 111);
    lru.printCache();
    cout << lru.get(6) << endl;
    lru.put(7, 7);
    lru.printCache();
    cout << lru.get(1) << endl;

    return 0;
}
