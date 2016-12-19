#ifndef RBTREE_H
#define RBTREE_H
#include <cstdlib>
#include <vector>
using namespace std;


//define RBTree color
typedef enum color_t{
    RED = 0,
    BLACK = 1
}color_t;

//define RBTree node
typedef struct rb_node_t{
    struct rb_node_t *left,*right,*parent;//left child, right child and parent
    int key;   //ID
    int count;
    color_t color;  //node's color
}rb_node_t;

//class RBTree
class RBTree{
private:
    rb_node_t *root;
    
    rb_node_t *NullNode;// null node

    int preOrder(rb_node_t *node);
    rb_node_t* Next2 (rb_node_t *node);
    rb_node_t* Previous2(rb_node_t *node);
    
    void deleteNode(int key);//delete node
    //rb_node_t *searchNode(int key);//seearch node by key
    rb_node_t *searchNode(int key);//seearch node by key
    
    void rotateLeft(rb_node_t *x);
    void rotateRight(rb_node_t *y);
    
    //some useful function
    rb_node_t* __newNode(int key,int count);//generate new node
    void __insertFixUp(rb_node_t *node);//fix up after inserting node
    void __deleteNodeFixUp(rb_node_t *node);//fix up after deleting node
    rb_node_t *getTreeSuccessor(rb_node_t *node);//get successor node
    rb_node_t *getMinNode(rb_node_t *t);//get node with minimum key
    rb_node_t *getMaxNode(rb_node_t *t);//get node with maximun key
    void __freeRBTree(rb_node_t *t);//free space
    
    
public:
    RBTree();
    ~RBTree();
    
    void insert(int key,int count);//insert new node
    void Count(int key);
    void Increase (int key, int m);
    void Reduce (int key, int m);
    void Next (int key);
    void Previous(int key);
    void InRange(int key1,int key2);
    
};

#endif
