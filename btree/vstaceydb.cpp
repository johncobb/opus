#include <iostream>
#include <fstream>
#include <stack>
#include <ctime>
#include <math.h>
#include <map>

using namespace std;

clock_t clock_time;
clock_t clock_start;
clock_t clock_end;

struct ISO3779_VDS {
    uint64_t radix;
    string data;
};

struct ISO3779_WMI {
    uint64_t radix;
    ISO3779_VDS vds;
    string data; 
};

struct ISO3779_Node {
    ISO3779_WMI wmi;
    ISO3779_Node* left;
    ISO3779_Node* right;
};

uint64_t vradenc(string data, int base);
uint64_t vraddec(uint64_t vrad, int base, int len);

struct ISO3779_Node* new_node(string data) {

    ISO3779_Node* node = (ISO3779_Node*) malloc(sizeof(ISO3779_Node));
    node->wmi.radix = vradenc(data.substr(0,3), 33);
    node->wmi.vds.radix = vradenc(data.substr(3,5), 33);

    node->left = NULL;
    node->right = NULL;

    return node;
};

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

string map = "0123456780ABCDEFGHJKLMNPRSTUVWXYZ";

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



Node* build_tree_flagx(stack<int> *data, Flag *flag) {
    /* used to track nodes of tree */
    static int flag_counter = 0; /* static to preserve value */
    
    if (data->empty()) {
        return NULL;
    }

    Node* root = new_node(decc(data->top()));

    flag_counter++;
    
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
    root->left = build_tree_flagx(data, flag);
    // cout << "left: " << endl;
    root->right = build_tree_flagx(data, flag);
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

    /*
     * Setup the flags. Currently the order of the flags matter
     * due to the nature of the stack. Explore vector or some other
     * option that would allow ordinal dequeueing.
     */
    Flag flag;

    flag.tokens.push(vdata_index1);
    flag.tokens.push(vdata_index2);

    /* build the btree */
    Node* root1 = build_tree_flagx(&buffer1, &flag);
    /* log the order */
    inorder(root1);

    /* process the results */
    while(flag.results.empty() == false) {

        FlagResult result = flag.results.top();
        Node* node = result.node;

        cout << "node[:" << result.token << "] data: " << encc(node->data) << endl;
        flag.results.pop();
    }

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

string vmap = "*0123456789ABCDEFGHJKLMNPRSTUVWXYZ*";
int vmapindexof(char c) {
    return vmap.find(c);
}

char vmapcharat(int index) {
    return vmap[index];
}

void vmapencode(string src, string* dest) {
    for (int i=0; i<src.length(); i++) {
        // dest->push_back(vmapindexof(src[i]));
        dest->push_back(vmapindexof(toupper(src[i])));
    }
}

void vmapdecode(string data) {
    for (int i=0; i<data.length(); i++) {
        // cout << "vmap[" << i << "] " << vmapcharat(data[i]) << endl;
        cout << vmapcharat(data[i]);
    }
}

uint64_t vradenc(string data, int base) {
    uint64_t vrad = 0;
    uint64_t vradshxp = 0; /* used to track bitshift exponent */
    int exp = 0;

    cout << "vradenc: ";
    vmapdecode(data);
    cout << " base: " << base << endl;

    for (int i=0; i<data.length(); i++) {

        exp = (data.length()-1) - i;

        /* get the index of the character at i(th) position */
        int mval = data[i];
        char mvalc = vmapcharat(mval);

        vrad += (mval * pow(base, exp)); // replace: bitshif

        cout << "[" << i << "]";
        cout << "[" << mval << "]";     
        cout << "[" << mvalc << "]";   
        cout << base << "^" << exp << "=" << vrad << endl;

    }

    return vrad;
}

uint64_t vraddec(uint64_t vrad, int base, int len) {
    int exp = 0;
    cout << "vraddec: " << vrad << endl;
    
    for (int i=0; i<len; i++) {
        int vmapi = 0;
        uint64_t vdelta = 0;

        exp = (len-1) -i;


        uint64_t vradp = pow(base, exp);
        uint64_t vmod = vrad % vradp; // replace: bitshif

        cout << "[" << i << "]";
        
        if (vmod > 0) {
            vdelta = vrad - vmod;
            vmapi = vdelta/vradp;
            cout << "[" << vmapi << "]";
            cout << "[" << vmapcharat(vmapi) << "]";
            vrad = vmod;
        }
        else {
            vdelta = vrad;
            vmapi = vrad;
            cout << "[" << vmapi << "]";
            cout << "[" << vmapcharat(vmapi) << "]";
        }
        cout << base << "^" << exp << "=" << vdelta;
        cout << endl;
    }

    return vrad;
}



void run_example_vdecoder() {

    int base = 34;

//     struct ISO3779_Node {
//     ISO3779_WMI wmi;
//     uint16_t year;
//     uint16_t timestamp;
// };

    ISO3779_Node node;
    /* ascii buffer we want to radix encode */
    string vbuffer = "ZHWUC2ZX";
    string vdata;


    /* encode buffer to vdata */
    vmapencode(vbuffer, &vdata);

    // node.year = 2019;
    node.wmi.radix = vradenc(vdata.substr(0,3), base);
    node.wmi.vds.radix = vradenc(vdata.substr(3,5), base);


    vraddec(node.wmi.radix, base, 3);
    vraddec(node.wmi.vds.radix, base, 5);

    // cout << "node wmi: " << node.wmi.radix << " vds: " << node.wmi.vds.radix << endl;
    // cout << "node wmi:" << node.wmi.data << "vds: " << node.wmi.vds.data << endl;

    // stack<int> buffer;
    // load_stack(&buffer, vdata);
    // Node* root1 = build_tree(&buffer);
    // inorder(root1);
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
    run_example_vdecoder();
    return 0;
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