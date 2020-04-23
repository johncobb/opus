/* 
 * Similarity Search based upon Edit Distance "Levenshtein Distance"
 * Youtube: https://www.youtube.com/watch?v=MiqoA-yF-0M
 * 
 * Transformation distance
 * dp table
 */

/* Adjacency Matrix - Genetic Algorithm */

/* 
 * Transformation distance
 * DP Table
 */


#include <iostream>
#include <fstream>

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

/*
 * perform inorder traversal
 */
void inorder (Node* root) {
    if (root == NULL) 
        return;

        inorder(root->left);

        cout << root->data << " ";

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

void pre_order(Node* x) {
    if (x == NULL) {
        cout << "node-data: " << encc(x->data);
        pre_order(x->left);
        pre_order(x->right);
    } else {
        cout << "node is null" << endl;
    }
}

/* node is our reference to the node in previous pass */
Node* tree_from_string(Node* node, Node* leaf, string data, int index, int len, int log_offset_idx) {

    /* check to see if we completed the string */
    if ((index+1) > len)  {
        cout << endl;
        cout << "returning NULL" << endl;
        return NULL;
    }
    
    char buffer = decc(data[index]);

    Node* root = new_node(buffer);

    // cout << "buffer: " << encc(buffer) << endl;
    if (node != NULL) {
        cout << "node: " << encc(node->data) << endl;

        if (node->right != NULL) {
            cout << "node-right: " << encc(node->right->data) << " next: " << encc(buffer) << endl;
        }
            

        if (node->left != NULL)
            cout << "node-left: " << encc(node->left->data) << endl;

    }

    /*
    * todo: index is never getting beyond 2 because the callstack is only
    * incrementing to one. each stack would have to be the next seqency index
    * through the stack.
    * recommendation: use a stack to pop values out of the string
    */

    /* if node is NULL we are at the root level and need to handle this 
     * special case.
     */
    if (node == NULL) {

        cout << "[" << log_offset_idx << "] root: " << encc(root->data) << endl;

        root = tree_from_string(root, NULL, data, index + 1, len, log_offset_idx + 1);
        log_offset_id = index;
        
    } else {

        /* if leaf is null we need to determine right/left branch
         * otherwise just return the root variable b/c that's 
         * what the subsequent  call makes.
         */

        // cout << "index: " << index << " log_offset_idx: " << log_offset_idx << endl;
        if (leaf == NULL) {
            // cout << "root: " << encc(node->data) << " buffer: " << encc(buffer) << endl;
            if (buffer >= node->data) {
                node->right = root;
                node->left = tree_from_string(node, node->right, data, index + 1, len, log_offset_idx + 1);
                return node;
            } else {
                node->left = root;
                node->right = tree_from_string(node, node->left, data, index + 1, len, log_offset_idx + 1);
                return node;
            }
            return node;
        } else {
            return root;
        }
    }

    return root;
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

    /*
     * JTHKD5BH0D2170008
     * JTHKD5BH8D2169687
     */

    cout << "starting" << endl;

    cout << "JTHKD5BH0D2170007" << endl;
    Node* root_1 = tree_from_string(NULL, NULL, "JTHKD5BH0D2170007", 0, 17, 0);

 
    return 0;



    string data = "ABCD";
    for (int i=0; i<data.length(); i++) {
        cout << "index: " << i << " data: " << data[i] << endl;
    }
    return 0;
    g4g_example();

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