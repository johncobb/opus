#include <iostream>
#include <fstream>
#include <stack>
#include <deque>
#include <ctime>

using namespace std;

/* references: 
 * https://www.geeksforgeeks.org/binary-tree-set-1-introduction/
 * https://www.geeksforgeeks.org/introduction-of-b-tree-2/
 * 
 */
clock_t clock_time;
clock_t clock_start;
clock_t clock_end;

uint32_t sanity_level = 0;
#define SANE_MAX  20

void sanity() {
    if (sanity_level++ > SANE_MAX)
        exit(0);
}

struct Node {
    int data;
    Node* left;
    Node* right;
};

struct Node* new_node(int data) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node* build_binary_tree(deque<int> *data);

char encc(int code);
char decc(int code);
void load_stack(stack<int> *buffer, string data);
void load_deque(deque<int> * buffer, string data);

void inorder(Node* root);
int identical(Node* root1, Node* root2);


char encc(int code) {
    return char(code + '0');
}
char decc(int code) {
    return char(code - '0');
}

void load_stack(stack<int> *buffer, string data) {
    for (int i = 0; i< data.length(); i++) {
        buffer->push(data[i]);
    }    
}

void load_dequeue(deque<int> *buffer, string data) {
    for(int i=0; i<data.length(); i++) {
        buffer->push_back(data[i]);
    }
}

void inorder(Node* root) {
    if (root == NULL)
        return;

    inorder(root->left);
    cout << "root-data: " << encc(root->data) << endl;
    inorder(root->right);
}

int identical(Node* root1, Node* root2) {
    
    /* are both root nodes empty? */
    if (root1 == NULL && root2 == NULL) 
        return 1;
    /* if any one of the tree is non-empty and other
       is empty, return false
    */
    else if (root1 != NULL && root2 == NULL)
        return 0;
    else if (root1 == NULL && root2 != NULL)
        return 0;
    else {
        /*
         * check if current data of both trees are equal
         * and recursively check for left and right subtrees
         */
        if (root1->data == root2->data 
            && identical(root1->left, root2->left) 
            && identical(root1->right, root2->right))
            return 1;
        else {
            return 0;
        }
    }
}

Node* build_binary_tree(deque<int> *data) {

    if (data->empty()) {
        return NULL;
    }


    Node* root = new_node(decc(data->front()));

    data->pop_front();
    
    root->left = build_binary_tree(data);
    // cout << "left: " << endl;
    root->right = build_binary_tree(data);
    // cout << "right: " << endl;
    
    return root;

}

void run_build_binary_tree() {
    
    cout << "run_build_binary_tree " << endl;

    string vdata1 = "JTHKD5BH0D2170008";
    deque<int> vdeque1;
    load_dequeue(&vdeque1, vdata1);


    string vdata2 = "JTHKD5BH8D2169687";
    deque<int> vdeque2;
    load_dequeue(&vdeque2, vdata2);


    Node* root1 = build_binary_tree(&vdeque1);
    Node* root2 = build_binary_tree(&vdeque2);

    cout << "inorder root1: " << endl;
    inorder(root1);
    cout << "inorder root2: " << endl;
    inorder(root2);

    cout << "identical: " << ((identical(root1, root2) == 1) ? "true" : "false") << endl;
    cout << "end of the line." << endl;
}

int main() {

    run_build_binary_tree();
    return 0;
}