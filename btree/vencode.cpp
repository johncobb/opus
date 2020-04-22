#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

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

struct NFile {
    stack<Node*> vdata;
    Node* data;
    Node* stock_alpha;
    Node* stock_beta;
    int year;
    /* todo: used to calculate radix */
    // uint64_t vrad_alpha;
    // uint64_t vrad_beta;
    // uint64_t vrad_gamma;
};


void nleft(uint16_t* map, int* bitPos) {
    (*bitPos)++;
}

void nright(uint16_t* map, int* bitPos) {
    *map |= (1 << (*bitPos));
    (*bitPos)++;
}
bool ncheck(int32_t data, int bitPos) {
    return ((data) & (1<<(bitPos)));
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

/*
 * example pulled from geeks for geeks
 * link: https://www.geeksforgeeks.org/check-whether-the-two-binary-search-trees-are-identical-or-not/
 */
void g4g_example() {
    Node* root1 = new_node(5);
    Node* root2 = new_node(5);
    root1->left = new_node(3);
    root1->right = new_node(8);
    root1->left->left = new_node(2);
    root1->left->right = new_node(4);

    root2->left = new_node(3);
    root2->right = new_node(8);
    root2->left->left = new_node(2);
    root2->left->right = new_node(4);

    if (identical(root1, root2)) {
        cout << "both bst(s) are identical" << endl;
    } else {
        cout << "both bst(s) are not identical" << endl;
    }
}

string map = "0123456780ABCDEFGHJKLMNPRSTUVWXYZ";

char encc(int code) {
    return char(code + '0');
}
char decc(int code) {
    return char(code - '0');
}

int log_offset_id = 0;



/* perform inorder traversal */
void inorder(Node* root) {

    if (root == NULL)
        return;

    inorder(root->left);
    cout << "root-data: " << encc(root->data) << endl;
    inorder(root->right);
}

void load_stack(stack<int> *buffer, string data) {
    for (int i = 0; i< data.length(); i++) {
        buffer->push(data[i]);
    }    
}

Node* build_tree(stack<int> *data) {

    if (data->empty()) {
        return NULL;
    }

    Node* root = new_node(decc(data->top()));
    
    data->pop();
    
    root->left = build_tree(data);
    // cout << "left: " << endl;
    root->right = build_tree(data);
    // cout << "right: " << endl;
    
    return root;
}

/* map is used to determine order tree took to build the record
 * since we have a determinant lenght of  the record we're encoding
 * we can store the left and right node branches in an integer.
 * this will help high level analysis of records without decoding the 
 * entire tree when performing comparisons
 */
void run_example_map() {
    uint16_t map = 0;
    int max = 4;

    for (int i=0; i<max; i++) {
        if (i % 2 == 0) {
            nright(&map, &i);
        } else {
            nleft(&map, &i);
        }
        cout << ncheck(map, i);
    }

    cout << "map: ";
    for (int i=0; i<max; i++) {
        cout << ncheck(map, i);
    }
    
}
void run_example_buildtree() {
    string vdata1 = "JTHKD5BH0D2170008";
    stack<int> buffer1;
    load_stack(&buffer1, vdata1);
    Node* root1 = build_tree(&buffer1);
    inorder(root1);
}

void run_example() {

    /*
     * JTHKD5BH0D2170008
     * JTHKD5BH8D2169687
     */

    NFile nf;


    string vdata1 = "JTHKD5BH0D2170008";
    string vdata2 = "JTHKD5BH8D2169687";

    stack<int> buffer1;
    stack<int> buffer2;
    load_stack(&buffer1, vdata1);
    load_stack(&buffer2, vdata2);

    Node* root1 = build_tree(&buffer1);
    cout << "root: " << endl;
    Node* root2 = build_tree(&buffer2);

    nf.vdata.push(root1);
    nf.vdata.push(root2);

    cout << "node count: " << nf.vdata.size() << endl;
    
    // if(identical(root1, root2)) {
    //     cout << "identical" << endl;
    // } else {
    //     cout << "different" << endl;
    // }
    Node* x = nf.vdata.top();
    nf.vdata.pop();
    Node* y = nf.vdata.top();
    if(identical(x, y)) {
        cout << "identical" << endl;
    } else {
        cout << "different" << endl;
    }    
}

/*
 * JTHKD5BH0D2170008
 * JTHKD5BH8D2169687
 * JTH
 *        J
 *       / \
 *      H   T
 */

int main() {

    // run_example();
    // run_example_buildtree();
    run_example_map();
    return 0;
    


    fstream fio;
    string line;

    fio.open("output");
    fio.seekg(0, ios::beg);
    cout << "start of file" << endl;
    uint8_t counter = 0;
    size_t len = 0;

    while(fio) {
        if (counter++ > 10) {
            break;
        }
        
        getline(fio, line);
        len = line.length();

        cout << line << " " << len << endl;
    }

    fio.close();
    cout << "end of file" << endl;

}