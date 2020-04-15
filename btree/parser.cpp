#include <iostream>
#include <fstream>

using namespace std;

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* new_node(int data) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
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
    struct Node* root1 = new_node(5);
    struct Node* root2 = new_node(5);
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


/* log_offset_idx used to cleanly format log output */
void tree_from_string(Node* node, string data, int index, int len, int log_offset_idx) {

    /* check to see if we completed the string */
    if ((index+1) > len)  {
        cout << endl;
        return;
    }

    // Node* root = NULL;    
    /*
     * develop pattern where we look at current node being NULL
     * if 
     */

    if (node == NULL) {
        node = new_node(decc(data[index]));

        if (index == 0) {
            cout << "[" << log_offset_idx << "] root:" << encc(node->data) << endl;
        } else {
            cout << "[" << log_offset_idx << "] lf:" << encc(node->data) << endl;
        }
        index++;
        log_offset_idx++;
        
        tree_from_string(node, data, index, len, log_offset_idx);
    } else {
        char buffer = decc(data[index]);

        /* get the char value of the node data */
        // char node_data = encc(node->data);

        if (buffer > node->data) {
            node->right = new_node(buffer);
            cout << "[" << log_offset_idx << "] lf-rt:" << encc(node->right->data) << endl;
            index++;
            log_offset_idx++;
            tree_from_string(node->left, data, index, len, log_offset_idx);
        } else {
            node->left = new_node(buffer);
            cout << "[" << log_offset_idx << "] lf-lt:" << encc(node->left->data) << endl;
            index++;
            log_offset_idx++;
            tree_from_string(node->right, data, index, len, log_offset_idx);     
        }
        
    }

    return;

}

/*
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
    Node* root_1 = NULL;
    Node* root_2 = NULL;

    cout << "JTHKD5BH0D2170007" << endl;
    tree_from_string(root_1, "JTHKD5BH0D2170007", 0, 17, 0);
    // cout << "JTHKD5BH0D2170008" << endl;
    // tree_from_string(root_2, "JTHKD5BH0D2170008", 0, 17, 0);


    if (identical(root_1, root_2)) {
        cout << "both bst(s) are identical" << endl;
    } else {
        cout << "both bst(s) are not identical" << endl;
    }
 
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