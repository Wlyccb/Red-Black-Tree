
#include "RBTree.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Wrong input format" << endl;
    }
    
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Can' open file" << endl;
    }
    //setup the tree
    RBTree rbtree;
    //insert nodes
    int n;
    input >> n;
    while (n--) {
        int id, m;
        input >> id >> m;
        rbtree.insert(id, m);
    }
    //start to excute commands
    string cmd;
    while (true) {
        cin >> cmd;
        int id, m, id2;
        if (cmd == "increase") {
            cin >> id >> m;
            rbtree.Increase(id, m);
        } else if (cmd == "reduce") {
            cin >> id >> m;
            rbtree.Reduce(id, m);
        } else if (cmd == "inrange") {
            cin >> id >> id2;
            rbtree.InRange(id, id2);
        } else if (cmd == "count") {
            cin >> id;
            rbtree.Count(id);
        } else if (cmd == "next") {
            cin >> id;
            rbtree.Next(id);
        } else if (cmd == "previous") {
            cin >> id;
            rbtree.Previous(id);
        } else if (cmd == "quit") {
            break;
        } else {
            cerr << "Unsupported command" << endl;
        }
    }
    
    
}


