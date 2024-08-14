#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Custom MinHeap class
class MinHeap {
private:
    vector<pair<int, string>> heap;

    void heapifyUp(int index) {
        int parent = (index - 1) / 2;   // because of zero based indexing
        while (index > 0 && heap[parent].first > heap[index].first) {
            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size && heap[left].first < heap[smallest].first)
                smallest = left;
            if (right < size && heap[right].first < heap[smallest].first)
                smallest = right;

            if (smallest == index)
                break;

            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    void push(pair<int, string> item) {
        heap.push_back(item);
        heapifyUp(heap.size() - 1);
    }

    pair<int, string> top() {
        return heap[0];
    }

    void pop() {
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
    }

    bool empty() {
        return heap.empty();
    }
};

// Custom MaxHeap class
class MaxHeap {
private:
    vector<pair<int, string>> heap;

    void heapifyUp(int index) {
        int parent = (index-1)/2;
        while (index > 0 && heap[parent].first < heap[index].first) {
            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int largest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size && heap[left].first > heap[largest].first)
                largest = left;
            if (right < size && heap[right].first > heap[largest].first)
                largest = right;

            if (largest == index)
                break;

            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    void push(pair<int, string> item) {
        heap.push_back(item);
        heapifyUp(heap.size() - 1);
    }

    pair<int, string> top() {
        return heap[0];
    }

    void pop() {
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
    }

    bool empty() {
        return heap.empty();
    }
};

// Transaction class
class Transaction {
public:
    string from;
    string to;
    int amount;
    
    // Transaction(string f, string t, int a) : from(f), to(t), amount(a) {}
    Transaction(string f, string t, int a){   // constructor
        from=f;
        to=t;
        amount=a;
    }
};

int main() {
    cout << "Cash Flow Minimizer Algorithm\n\n";

    int no_of_transactions, friends;
    cout << "Enter the number of friends: ";
    cin >> friends;
    cout << "Enter the number of transactions: ";
    cin >> no_of_transactions;

    unordered_map<string, int> net;

    cout << "\nEnter the transactions in the format: <payer> <receiver> <amount>\n";
    cout << "For example: Alice Bob 50\n";
    cout << "This means Alice paid 50 to Bob.\n\n";

    for (int i = 0; i < no_of_transactions; i++) {
        string payer, receiver;
        int amount;
        cout << "Transaction " << i+1 << ": ";
        cin >> payer >> receiver >> amount;
        net[payer] -= amount;
        net[receiver] += amount;
    }

    MinHeap minHeap;
    MaxHeap maxHeap;

    for (const auto& entry : net) {
        if (entry.second < 0) {
            minHeap.push({entry.second, entry.first});
        } else if (entry.second > 0) {
            maxHeap.push({entry.second, entry.first});
        }
    }

    vector<Transaction> transactions;

    while (!minHeap.empty() && !maxHeap.empty()) {
        auto debt = minHeap.top();
        auto credit = maxHeap.top();
        minHeap.pop();
        maxHeap.pop();

        int amount = min(-debt.first, credit.first);  // absolute minimum
        transactions.push_back(Transaction(debt.second, credit.second, amount));

        debt.first += amount;
        credit.first -= amount;

        if (debt.first < 0) {
            minHeap.push(debt);
        }
        if (credit.first > 0) {
            maxHeap.push(credit);
        }
    }

    cout << "\nMinimized transactions:\n";
    cout << "Number of transactions: " << transactions.size() << endl;
    for (const auto& t : transactions) {
        cout << t.from << " pays " << t.amount << " to " << t.to << endl;
    }

    return 0;
}

// Cash Flow Minimizer Algorithm

// Enter the number of friends: 3
// Enter the number of transactions: 3

// Enter the transactions in the format: <payer> <receiver> <amount>
// For example: Alice Bob 50
// This means Alice paid 50 to Bob.

// Transaction 1: Rahul Ajay 100
// Transaction 2: Ajay Neha 50
// Transaction 3: Neha Rahul 40

// Minimized transactions:
// Number of transactions: 2
// Rahul pays 50 to Ajay
// Rahul pays 10 to Neha
