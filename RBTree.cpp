#include "RBTree.h"

#include <iostream>
#include <vector>

using namespace std;

RBTree::RBTree()
{
    NullNode = (rb_node_t *)malloc(sizeof(struct rb_node_t));
    //fail to allocate memory
    if (!NullNode)
    {
        cout << "malloc failed!" << endl;
        exit(-1);
    }
    NullNode ->left = NullNode ->right = NullNode ->parent = NULL;
    NullNode ->color = BLACK;
    root = NullNode;
}

RBTree::~RBTree()
{
    //free space of the tree
    __freeRBTree(root);
}

//insert new node into the tree
//key：the ID of new node
void RBTree::insert(int key,int count)
{
    rb_node_t *y = NullNode,*x = root;
    rb_node_t *node = __newNode(key,count);
    //search from the root, and find a place for the new node
    while(x != NullNode)
    {
        y = x;
        if(key < x ->key)
            x = x ->left;
        else
            x = x ->right;
    }
    node ->parent = y;
    
    //if the root is null, insert the node into the root
    if(NullNode == y)
        root = node;
    else{
        //change pointer and insert the node into the right place
        if (key < y ->key)
            y ->left = node;
        else
            y ->right = node;
    }
    
    //fix up the tree
    __insertFixUp(node);
    }

//delete node from the tree
void RBTree::deleteNode(int key)
{
    rb_node_t *node = searchNode(key);
    if (node->key != key)
    {
        cout << "Warnning： the node that has the key value not exist！Delete failed！" << endl;
        return;
    }
    //删除节点的思想：
    //如果被删除节点不具有两个非空孩子，则直接删除这个节点，用其位移子节点替代其位置；
    //如果有两个非空孩子，则用把它的直接后继节点内容复制到它的位置，然后以同样的方式删除这个后继节点
    //注意：节点的后继节点不可能有两个非空孩子，因此此传递过程最多只进行一次。
    rb_node_t *nodeDeleted;//point to the node need to be deleted
    //if the node have one null child, this node is the node need to be deleted
    if(NullNode == node ->left || NullNode == node ->right)
        nodeDeleted = node;
    else
        //otherwise, the node need to be deleted is its successor node
        nodeDeleted = getTreeSuccessor(node);
    
    rb_node_t *x;//x is the child node of deleted node, and x repalce its parents node
    if(nodeDeleted ->left != NullNode)
        x = nodeDeleted ->left;
    else
        x = nodeDeleted ->right;
    
    x ->parent = nodeDeleted ->parent;
    //if the node need to be deleted is the root node
    if(NullNode == nodeDeleted ->parent)
        root = x;//change root
    else if(nodeDeleted == nodeDeleted ->parent ->left)
        nodeDeleted ->parent ->left = x;
    else
        nodeDeleted ->parent ->right= x;
    
    if(nodeDeleted != node)
        node ->key = nodeDeleted ->key;
    
    //if the deleted node is black,the tree need to be fixed.
    if(BLACK == nodeDeleted ->color)
        __deleteNodeFixUp(x);
}

//search node from the tree according to key(ID) and return the pointer to this node
rb_node_t* RBTree::searchNode(int key)
{
    //search from the root, and its parents is a null node
    rb_node_t *node = root,*parent = NullNode;
    //search from top to bottom
    while(node != NullNode)
    {
        parent = node;
        //if the key of node less than key, search the left sub-tree
        if (key < node ->key)
            node = node ->left;
        //if the key of node greater than key, search the left sub-tree
        else if(key > node ->key)
            node = node ->right;
        else
        {
            //if find the node, return the pointer to the node
            return node;
        }
    }
    //if cannot find the node, return the last node searched.
    return parent;
}

//implement the function 'Count'
void RBTree::Count (int key){
    //search the node
    rb_node_t *node = searchNode(key);
    //if the node is in the tree, output its count value
    if (node->key == key){
        cout << node->count <<endl;
    }
    //otherwise, output 0
    else cout<<0<<endl;
    
}

//implement the function 'Incerase'
void RBTree::Increase (int key, int m){
    //search the node from the tree
    rb_node_t *node = searchNode(key);
    //if we can find the node in the tree, incraese its count value by m and output its new count value
    if (node->key == key){
        node->count= node->count + m;
        cout<< node->count << endl;
    }
    //otherwise, insert a new node with ID=key and count=m
    else {
        insert(key, m);
        cout<< m << endl;
    }
}

//implement the function 'Reduce'
void RBTree::Reduce (int key, int m){
    //search the ndoe
    rb_node_t *node = searchNode(key);
    //if we can find the node in the tree, reduce its count value by m
    if (node->key == key){
        node->count= node->count - m;
        //if the count of node is less than 1, delete the node and output 0
        if (node->count < 1) {
            deleteNode(key);
            cout << 0 << endl;
        }
        //otherwise, output its count
        else cout<< node->count << endl;
    }
    //if cannot find the node, output 0
    else {
        cout << 0 << endl;
    }
}

//implement the function 'Next'
void RBTree::Next (int key){
    //search the node
    rb_node_t *node = searchNode(key);
    //if the node is not present and the key we find is greater than key, just output as result
    if (node->key > key && node != NullNode) cout<<node->key << " " <<node->count<<endl;
    else if (node != NullNode){
            rb_node_t *max_node=getMaxNode(root);
            rb_node_t *next=NullNode;
            //if the node have the maximun key, it has no next node, and output '0 0'
            if (node->key == max_node->key) cout<< "0 0" <<endl;
            //if the node's right child is null
            else if (node->right == NullNode){
                //if the node is the left child, its parent node is its next node
                if (node == node->parent->left) {
                    next=node->parent;
                    cout<< next->key <<" "<< next->count <<endl;
                }
                else {
                    next=node->parent;
                    while (next==next->parent->right){
                        next=next->parent;
                    }
                    next=next->parent;
                    cout<< next->key <<" "<< next->count <<endl;
                }
            }
            //otherwise, find the node with minimun key of its right sub-tree
            else {
                next=getMinNode(node->right);
                cout<< next->key <<" "<< next->count<<endl;
            }
        }
    //if no next node, output '0 0'
    else cout << "0 0" << endl;
    
        
}

//same as Next, but this function return the pointer to the next node
rb_node_t* RBTree::Next2 (rb_node_t *node){
    rb_node_t *next=NullNode;
    if (node != NullNode){
        if (node->right == NullNode){
            if (node == node->parent->left) {
                next=node->parent;
            }
            else {
                next=node->parent;
                while (next==next->parent->right){
                    next=next->parent;
                }
                next=next->parent;
            }
        }
        else {
            next=getMinNode(node->right);
        }
    }
    return next;
}

void RBTree::Previous(int key){
    //search the node
    rb_node_t *node = searchNode(key);
    //if the node is not present and the key we find is less than key, just output as result
    if (node->key < key) cout<< node->key << " " <<node->count<<endl;
    else if (node != NullNode){
        rb_node_t *min_node=getMinNode(root);
        rb_node_t *next=NullNode;
        //if the node have the minimum key, it has no previous node, and output '0 0'
        if (node->key == min_node->key) cout<< "0 0" <<endl;
        //if the left child of the node is null
        else if (node->left == NullNode){
            //if the node is the right child, its parent node is its previous node
            if (node == node->parent->right) {
                next=node->parent;
                cout<< next->key <<" "<< next->count <<endl;
            }
            else {
                next=node->parent;
                while (next==next->parent->left){
                    next=next->parent;
                }
                next=next->parent;
                cout<< next->key <<" "<< next->count <<endl;
            }
        }
        //otherwise, find the node with maximun key of its left sub-tree
        else {
            next=getMaxNode(node->left);
            cout<< next->key <<" "<< next->count<<endl;
        }
    }
    //if no previous node, output '0 0'
    else cout << "0 0" << endl;
}

//same as function Previous, but return the pointer to the previous node
rb_node_t* RBTree::Previous2(rb_node_t *node){
    rb_node_t *next=NullNode;
    if (node->left == NullNode){
        if (node == node->parent->right) {
            next=node->parent;
        }
        else {
            next=node->parent;
            while (next==next->parent->left){
                next=next->parent;
            }
            next=next->parent;
        }
    }
    return next;
}





//left rotate
//x is the need to be left rotated
void RBTree::rotateLeft(rb_node_t *x)
{
    rb_node_t *y = x ->right;//set y as the right child of x
    x ->right = y ->left;//make the pointer to x's right child is equal to the pointer to the y's left child
    //if y's right child is not null, we need to change the parent pointer of y's left child
    if (y ->left != NullNode)
        y ->left ->parent = x;
    
    y ->parent = x ->parent;//change y's parent pointer
    if (NullNode == x ->parent)//if x is the root, we need to change root node
        root = y;
    else
    {
        //change the child pointer of x's parent node
        if (x == x ->parent ->left)
            x ->parent ->left = y;
        else
            x ->parent ->right = y;
    }
    
    //set x as y's left child
    y ->left = x;
    x ->parent = y;
}

//right rotate
//x: the node need to be right rotated
void RBTree::rotateRight(rb_node_t *y)
{
    rb_node_t *x = y ->left;
    y ->left = x ->right;
    if(x ->right != NullNode)
        x ->right ->parent = y;
    
    x ->parent = y ->parent;
    if (NullNode == y ->parent)
        root = x;
    else{
        if(y == y ->parent ->left)
            y ->parent ->left = x;
        else
            y ->parent ->right = x;
    }
    x ->right = y;
    y ->parent = x;
}

rb_node_t* RBTree::__newNode(int key,int count)
{
    rb_node_t *node = (rb_node_t *)malloc(sizeof(struct rb_node_t));
    
    //fail to allocate memory
    if (!node)
    {
        cout << "malloc failed!" << endl;
        exit(-1);
    }
    
    node ->key = key;//ID
    node ->count=count;//count value
    node ->color = RED;//the color of new node is red
    node ->left = node ->right = NullNode;
    node ->parent = NullNode;
    
    return node;
}


void RBTree::__insertFixUp(rb_node_t *node)
{
    while(RED == node ->parent->color)//
    {
        //the node's parent is the left child
        if(node ->parent == node ->parent ->parent ->left)
        {
            rb_node_t *y = node ->parent ->parent ->right;//the uncle node of the node
            //case1: the uncle node of the node is a red node
            if (RED == y ->color)
            {
                //change the color of node's parent node and uncle node to black and change the color of node's grandparent to red
                node ->parent ->color = BLACK;
                y ->color = BLACK;
                node ->parent ->parent ->color = RED;
                node = node ->parent ->parent;
            }
            else
            {
                //case2: the uncle node is a black node and the node is the right child
                if (node == node ->parent ->right)
                {
                    //left rotate and change case2 to case3
                    node = node ->parent;
                    rotateLeft(node);
                }
                //case3: uncle node is a black node and the node is the left child
                //change the color of its parent node and grandparent, and right rotate
                node ->parent ->color = BLACK;
                node ->parent ->parent ->color = RED;
                rotateRight(node ->parent ->parent);
            }
        }
        else{
            rb_node_t *y = node ->parent ->parent ->left;
            //case1: the uncle node of the node is a red node
            if (RED == y ->color)
            {
                //change the color of node's parent node and uncle node to black and change the color of node's grandparent to red
                node ->parent ->color = BLACK;
                y ->color = BLACK;
                node ->parent ->parent ->color = RED;
                node = node ->parent ->parent;
            }
            else
            {
                //case2: the uncle node is a black node and the node is the right child
                if (node == node ->parent ->left)
                {
                    //left rotate and change case2 to case3
                    node = node ->parent;
                    rotateRight(node);
                }
                //case3: uncle node is a black node and the node is the left child
                //change the color of its parent node and grandparent, and right rotate
                node ->parent ->color = BLACK;
                node ->parent ->parent ->color = RED;
                rotateLeft(node ->parent ->parent);
            }
        }
    }
    
    root ->color = BLACK;//set the color of root node
}

void RBTree::__deleteNodeFixUp(rb_node_t *node)
{
    while(node != root && node ->color == BLACK)
    {
        //Case 1：the node is the left child of its parents
        if (node == node ->parent ->left)
        {
            rb_node_t *brother = node ->parent ->right;
            //case11: its brother is a red node
            if (RED == brother ->color)
            {
                //change the color of its brother node and parents node, and than do left totate, which change case11 to case12, case13 or case14
                brother ->color = BLACK;
                node ->parent ->color = RED;
                rotateLeft(node ->parent);
                brother = node ->parent ->right;
            }
            //case12: its brother node is a black node, and the two chiilren of brother node are both black node
            if (BLACK == brother ->left ->color && BLACK == brother ->right ->color)
            {
                brother ->color = RED;
                node = node ->parent;
            }
            else{
                //case13: ite brother node is a black node and its brother node's left child is a red node and its brother node's right node is a black node
                if (BLACK == brother ->right ->color)
                {
                    //change the color of node and do right rotate, which change case13 to case14
                    brother ->left ->color = BLACK;
                    brother ->color = RED;
                    rotateRight(brother);
                    brother = node ->parent ->right;
                }
                //case14: its brother node is a black node and its brother node's right ode is a red node
                brother ->color = node ->parent ->color;
                node ->parent ->color = BLACK;
                brother ->right ->color = BLACK;
                rotateLeft(node ->parent);
                node = root;
            }
        }
        //Case 2: the node is the right child of its parents
        else{
            rb_node_t *brother = node ->parent ->left;//兄弟节点
            //case21: its brother is a red node
            if (RED == brother ->color)
            {
                //change the color of its brother node and parents node, and than do left totate, which change case21 to case22, case23 or case24
                brother ->color = BLACK;
                node ->parent ->color = RED;
                rotateRight(node ->parent);
                brother = node ->parent ->left;
            }
            //case22: its brother node is a black node, and the two chiilren of brother node are both black node
            if (BLACK == brother ->left ->color && BLACK == brother ->right ->color)
            {
                brother ->color = RED;
                node = node ->parent;
            }
            else{
                //case23: ite brother node is a black node and its brother node's left child is a red node and its brother node's right node is a black node
                if (BLACK == brother ->left ->color)
                {
                    brother ->right ->color = BLACK;
                    brother ->color = RED;
                    rotateLeft(brother);
                    brother = node ->parent ->left;
                }
                //case24: its brother node is a black node and its brother node's right ode is a red node
                brother ->color = node ->parent ->color;
                node ->parent ->color = BLACK;
                brother ->left ->color = BLACK;
                rotateRight(node ->parent);
                node = root;
            }
        }
    }
    
    node ->color = BLACK;
}

//get the node's successor node
rb_node_t* RBTree::getTreeSuccessor(rb_node_t *node)
{
    rb_node_t *tempNode = node;
    
    //the node's right child is not null node
    if(tempNode ->right != NullNode)
        return getMinNode(tempNode ->right);
    
    rb_node_t *y = tempNode ->parent;
    while(y != NullNode && tempNode == y ->right)
    {
        tempNode = y;
        y = y ->parent;
    }
    
    return y;
}

//get the node in the tree with maninum key(ID)
rb_node_t* RBTree::getMinNode(rb_node_t *t)
{
    rb_node_t *tempNode = t;
    while(tempNode ->left != NullNode)
        tempNode = tempNode ->left;
    
    return tempNode;
}

rb_node_t* RBTree::getMaxNode(rb_node_t *t)
{
    rb_node_t *tempNode = t;
    while(tempNode ->right != NullNode)
        tempNode = tempNode ->right;
    
    return tempNode;
}

//free the space
void RBTree::__freeRBTree(rb_node_t *t)
{
    if (NullNode == t)//tree is null
        return;
    //if the node's left child and right child are both null node, just free the space of the node
    if (NullNode == t ->left && NullNode == t ->right)
    {
        free(t);
        return;
    }
    if (t ->left != NullNode)
        __freeRBTree(t ->left);
    if (t ->right != NullNode)
        __freeRBTree(t ->right);
}

//get total count of sub-tree whose root is node
int RBTree::preOrder(rb_node_t *node){
    int t=0;
    if(node != NullNode)
    {
        t=t+node->count;
        t=t+preOrder(node->left);
        t=t+preOrder(node->right);
    }
    return t;
}

//implement the function 'Inrange'
void RBTree::InRange(int key1,int key2){
    //set total count=0
    int totalcount=0;
    rb_node_t *node = root;
    //find the first node between key1 and key2
    rb_node_t *head = searchNode(key1);
    //find the last node between key1 and key2
    rb_node_t *tail = searchNode(key2);
    int min=getMinNode(root)->key;
    int max=getMaxNode(root)->key;
    //if key1 is greater than maximum ID or key2 is less than minimun ID, there is no node between key1 and key2, total count is equal to 0
    if (key1 > max || key2 < min){
        cout<<totalcount<<endl;
    }
    else {
        //find the first key(ID) between key1 and key2 and set it as key1
        if (head->key > key1) key1=head->key;
        else if (head->key < key1) key1=Next2(head)->key;
        //find the last key(ID) between key1 and key2 and set it as key2
        if (tail->key < key2) key2=tail->key;
        else if (tail->key > key2) key2=Previous2(tail)->key;
            //if key1 is greater than key2, total count is 0
            if (key1>key2) totalcount=0;
            //if key1=key2, total count is the count of this node with ID=key1=key2
            else if (key1==key2) {
                rb_node_t *no=searchNode(key1);
                totalcount=no->count;
            }
            else {
                //find the root of a sub-tree, the node with key1 is on the left sub-tree or the root, and the node with key2 is on the right sub-tree or the root
                while (!(node->key>=key1 && node->key<=key2)){
                    if (node->key<key1) node=node->right;
                    if (node->key>key2) node=node->left;
                }
                rb_node_t *root2=node;
                //add the count of root to the total count
                totalcount=totalcount+node->count;
                //if key2 is not equal to the key of root, search from the root
                if (key2!=root2->key){
                    node=root2;
                    while (node->key!=key2) {
                        //if the key of node is greater than key2, set node as its left child
                        if(node->key>key2){
                            node=node->left;
                            //if the key of node is less than key2, all the counts of the node on its left sub-tree should be add to total count, including the node
                            if (node->key <key2){
                                totalcount=totalcount+node->count;
                                totalcount=totalcount+preOrder(node->left);
                            }
                        }
                        //if the key of node is less than key2, set node as its right child
                        if(node->key<key2){
                            node=node->right;
                            //if the key of node is less than key2, all the counts of the node on its left sub-tree should be add to total count, including the node
                            if (node->key <key2){
                                totalcount=totalcount+node->count;
                                totalcount=totalcount+preOrder(node->left);
                            }
                        }
                    }
                    //find the node with key2 and add the count to the total count
                    totalcount=totalcount+node->count;
                    //all the counts of the node on its left sub-tree should be add to total count
                    totalcount=totalcount+preOrder(node->left);
                }
                //if key1 is not equal to the key of root, search from the root
                if (key1!=root2->key) {
                    node=root2;
                    while (node->key!=key1) {
                        //if the key of node is greater than key1, set node as its left child
                        if(node->key>key1){
                            node=node->left;
                            //if the key of node is greater than key1, all the counts of the node on its right sub-tree should be add to total count, including the node
                            if (node->key >key1){
                                totalcount=totalcount+node->count;
                                totalcount=totalcount+preOrder(node->right);
                            }
                        }
                        //if the key of node is less than key1, set node as its right child
                        if(node->key<key1){
                            node=node->right;
                            //if the key of node is greater than key1, all the counts of the node on its left sub-tree should be add to total count, including the node
                            if (node->key >key1){
                                totalcount=totalcount+node->count;
                                totalcount=totalcount+preOrder(node->right);
                            }
                        }
                    }
                    //find the node with key1 and add the count to the total count
                    totalcount=totalcount+node->count;
                    //all the counts of the node on its right sub-tree should be add to total count
                    totalcount=totalcount+preOrder(node->right);
                }
            }
    }
    cout<<totalcount<<endl;
}
