#include <iostream>
#include <fstream>
#include <stack>
#include <ctime>
#include <map>

using namespace std;

clock_t clock_time;
clock_t clock_start;
clock_t clock_end;

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

struct FlagKey {
    std::map<int, Node*> flags;
};

struct FlagResult {
    int token;
    Node* node;
};

struct Flag {
    stack<int> tokens;
    stack<Node*> nodes;
    stack<FlagResult> results;

};

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

/* Record branch moving left on map */
void nleft(uint16_t* map, int* bitPos) {
    (*bitPos)++;
}

/* Record branch moving right on map */
void nright(uint16_t* map, int* bitPos) {
    *map |= (1 << (*bitPos));
    (*bitPos)++;
}

/* Determine direction taken for specific node */
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

// string map = "0123456780ABCDEFGHJKLMNPRSTUVWXYZ";

char encc(int code) {
    return char(code + '0');
}
char decc(int code) {
    return char(code - '0');
}

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

/*
 * This example shows the algorithm ability to flag a specific element 
 * from the tree. Example: flag the i(th) element in the tree while
 * encoding. Allowing the algorithm to start processing anywhere in the tree
 * we set a flag. (start i(th) position and work forward)
 */
// volatile int flag_counter = 0;
Node* build_tree_flag(stack<int> *data, int flag, Node** ptr_flag) {

    static int flag_counter = 0; /* static to preserve value */

    if (data->empty()) {
        return NULL;
    }

    Node* root = new_node(decc(data->top()));
    flag_counter++;
    
    if (flag_counter == flag) {
        cout << "flag: " << flag << " flag_counter: " << flag_counter << " data: " << encc(root->data) << endl;
        (*ptr_flag) = root;
    }
    
    data->pop();
    
    root->left = build_tree_flag(data, flag, ptr_flag);
    // cout << "left: " << endl;
    root->right = build_tree_flag(data, flag, ptr_flag);
    // cout << "right: " << endl;
    
    return root;
}



// Node* build_tree_flagx(stack<int> *data, Flag *flag) {
//     /* used to track nodes of tree */
//     static int flag_counter = 0; /* static to preserve value */
    
//     if (data->empty()) {
//         return NULL;
//     }

//     Node* root = new_node(decc(data->top()));

//     flag_counter++;
    
//     if (flag != NULL) {
//         if (flag->tokens.empty() == false) {
//             if (flag->tokens.top() == flag_counter) {
//                 // cout << "flag: " << flag->tokens.top() << " flag_counter: " << flag_counter << " data: " << encc(root->data) << endl;
//                 /* store the token and node in the result */
//                 FlagResult fr = {flag->tokens.top(), root};

//                 /* queue the FlagResult into the results stack */
//                 flag->results.push(fr);

//                 /* move to next token in stack */
//                 flag->tokens.pop();      
//             }
//         }
//     }

//     /* move to next character in stack */
//     data->pop();
//     root->left = build_tree_flagx(data, flag);
//     // cout << "left: " << endl;
//     root->right = build_tree_flagx(data, flag);
//     // cout << "right: " << endl;
    
//     return root;
// }

Node* build_tree_flagx(stack<int> *data, Flag *flag, FlagKey *fkey) {
    /* used to track nodes of tree */
    static int flag_counter = 0; /* static to preserve value */
    
    if (data->empty()) {
        return NULL;
    }

    Node* root = new_node(decc(data->top()));

    flag_counter++;
    
    if (fkey != NULL) {
        /* check to see if we have a flag for this node level */
        map<int, Node*>::iterator itr = fkey->flags.find(flag_counter);
        
        if (itr != fkey->flags.end()) {
            /* store the node */
            fkey->flags[flag_counter] = root;
        }
    }

    if (flag != NULL) {
        if (flag->tokens.empty() == false) {
            if (flag->tokens.top() == flag_counter) {
                // cout << "flag: " << flag->tokens.top() << " flag_counter: " << flag_counter << " data: " << encc(root->data) << endl;
                /* store the token and node in the result */
                FlagResult fr = {flag->tokens.top(), root};

                /* queue the FlagResult into the results stack */
                flag->results.push(fr);

                /* move to next token in stack */
                flag->tokens.pop();      
            }
        }
    }

    /* move to next character in stack */
    data->pop();
    root->left = build_tree_flagx(data, flag, fkey);
    // cout << "left: " << endl;
    root->right = build_tree_flagx(data, flag, fkey);
    // cout << "right: " << endl;
    
    return root;
}

/* 
 * Map is used to determine branch direction to build the record.
 * Since we have a determinant lenght of the record we're encoding
 * we can store the left and right node branches in an integer.
 * This will help high level analysis of records without decoding the 
 * entire tree when performing comparisons. Possible branch prediction.
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

void run_example_buildtree_flag() {

    Node* flag = NULL;
    string vdata1 = "JTHKD5BH0D2170008";
    stack<int> buffer1;
    load_stack(&buffer1, vdata1);
    Node* root1 = build_tree_flag(&buffer1, 8, &flag);
    inorder(root1);

    if (flag != NULL)
        cout << "flag: " << encc(flag->data) << endl;
}

void run_example_buildtree_flagx() {

    // Node* flag = NULL;
    string vdata1 = "JTHKD5BH0D2170008";
    stack<int> buffer1;
    load_stack(&buffer1, vdata1);



    /* used to designate which index values we are
     * interested in flagging.
     */
    int vdata_index1 = 8;
    int vdata_index2 = 6;

    FlagKey fkey;

    fkey.flags.insert(std::pair<int, Node*> (vdata_index1, NULL));
    fkey.flags.insert(std::pair<int, Node*> (vdata_index2, NULL));        


    /*
     * Setup the flags. Currently the order of the flags matter
     * due to the nature of the stack. Explore vector or some other
     * option that would allow ordinal dequeueing.
     */
    Flag flag;

    flag.tokens.push(vdata_index1);
    flag.tokens.push(vdata_index2);

    /* build the btree */
    Node* root1 = build_tree_flagx(&buffer1, &flag, &fkey);
    /* log the order */
    inorder(root1);

    /* process the results */
    while(flag.results.empty() == false) {

        FlagResult result = flag.results.top();
        Node* node = result.node;

        cout << "node[:" << result.token << "] data: " << encc(node->data) << endl;
        flag.results.pop();
    }

    cout << "fkey-flags[6]: " << encc(fkey.flags[vdata_index1]->data) << endl;
    inorder(fkey.flags[vdata_index1]);
    cout << "fkey-flags[8]: " << encc(fkey.flags[vdata_index2]->data) << endl;
    inorder(fkey.flags[vdata_index2]);

    cout << endl;
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

void run_std_map() {
    std::map<int, Node*> mflag;

    mflag[6] = new_node(decc('A'));
    mflag[8] = new_node(decc('B'));
    // mflag.insert(std::pair<int, Node*> (6, new_node('A')));
    // mflag.insert(std::pair<int, Node*> (8, new_node('B')));

    Node* node_key6 = mflag[6];
    cout << "node[6]: " << encc(node_key6->data) << endl;
    
    Node* node_key8 = mflag[8];
    cout << "node[8]: " << encc(node_key8->data) << endl;

    Node* node_keynull = mflag[0];
    if (node_keynull == NULL) {
        cout << "null" << endl;
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
    // run_std_map();
    // return 0;
    // run_example();
    // run_example_buildtree();
    // run_example_map();
    // run_example_buildtree_flag();
    run_example_buildtree_flagx();
    return 0;
    
    clock_start = clock();

    fstream fio;
    string line;

    fio.open("output");
    fio.seekg(0, ios::beg);
    cout << "start of file" << endl;
    uint32_t counter = 0;
    size_t len = 0;

    stack<Node*> vdata;
    
    while(fio) {
        counter++;

        getline(fio, line);

        // stack<int> buffer1;
        // load_stack(&buffer1, line);
        // Node* root1 = build_tree(&buffer1);
        // vdata.push(root1);
        len = line.length();

        // cout << line << " " << len << endl;
    }

    cout << "stack depth: " << vdata.size() << endl;

    fio.close();
    clock_end = clock();

double time_taken = double(clock_end - clock_start) / double(CLOCKS_PER_SEC); 
    cout << "total duration : " << fixed  
    << time_taken << setprecision(5); 
    cout << " sec " << endl; 

}