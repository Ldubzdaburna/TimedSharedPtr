#include <iostream>
#include <thread>
#include "TimeSharedPtr.h"

using namespace std;
using Clock = std::chrono::steady_clock;

struct Node {
    int data;
    Node(int d) : data(d) {}
};

int main() {
    TimedSharedPtr<Node> myNode(new Node(7), 100);
    TimedSharedPtr<Node> myOtherNode = myNode;

    this_thread::sleep_until(Clock::now() + chrono::milliseconds(50));
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;
    cout << "myNode.use_count(): " << myNode.use_count() << endl;
    cout << "myOtherNode.use_count(): " << myOtherNode.use_count() << endl;

    this_thread::sleep_until(Clock::now() + chrono::milliseconds(25));
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;

    this_thread::sleep_until(Clock::now() + chrono::milliseconds(75));
    cout << "The ptr should have expired: " << endl;
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;
    cout << "-----------" << endl;

    TimedSharedPtr<int> p(new int(42), 500);
    cout << p.get() << endl;
    cout << "p.use_count(): " << p.use_count() << endl;
    TimedSharedPtr<int> q = p;
    cout << "p.use_count(): " << p.use_count() << endl;
    cout << "q.use_count(): " << q.use_count() << endl;

    return 0;
}
