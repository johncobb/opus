#include <iostream>
#include <math.h>

using namespace std;

struct Vradix{
    uint64_t wmi;
    uint64_t vds;
    uint64_t ser;
};

char encc(int code) {
    return char(code + '0');
}
char decc(int code) {
    return char(code - '0');
}

string vmap = "*0123456789ABCDEFGHJKLMNPRSTUVWXYZ";

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

uint64_t vradenc(string data, int base);
uint64_t vraddec(uint64_t vrad, int base, int len);



/* http://eecs.wsu.edu/~ee314/handouts/numsys.pdf */
/* https://www.cs.colostate.edu/~cs270/.Spring12/Notes/NumberSystems */
uint64_t vradenc(string data, int base) {
    uint64_t vrad = 0;
    uint64_t vradshxp = 0; /* used to track bitshift exponent */

    uint64_t sheval = 33;

    cout << "vradenc: ";
    vmapdecode(data);
    cout << " base: " << base << endl;

    for (int i=0; i<data.length(); i++) {

        int exp = (data.length()-1) - i;

        /* get the index of the character at i(th) position */
        int mval = data[i];
        char mvalc = vmapcharat(mval);

        // cout << "mval: " << mval << " mvalc: " << mvalc << endl;

        vrad += (mval * pow(base, exp)); // replace: bitshif
        // vradshxp += (mval * (sheval <<= exp)); // todo: research

        cout << "[" << i << "]";
        cout << "[" << mval << "]";     
        cout << "[" << mvalc << "]";   
        cout << base << "^" << exp << "=" << vrad << endl;
        // cout << base << "^" << exp << "=" << vrad << ":" << vradshxp << endl;   

    }

    // cout << "total: " << vrad << endl;
    // vraddec(vrad, base, data.length());
    return vrad;
}

uint64_t vraddec(uint64_t vrad, int base, int len) {

    cout << "vraddec: " << vrad << endl;
    
    for (int i=0; i<len; i++) {
        int vmapi = 0;
        uint64_t vdelta = 0;
        int exp = (len-1) - i;

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

/* base 10 encodes sequence of numbers 0...9 */
void run_example_encdec_base10() {
    int base = 10;
    string buffer = "843";
    string data;

    vmapencode(buffer, &data);
    vradenc(data, base);    
}

/* base 33 can encodes sequence of alphanumeric 0...9 A...Z (excluding IOQ) */
void run_example_encdec_base33(string buffer) {

    int base = 33;
    string data;

    vmapencode(buffer, &data);
    uint64_t vrad = vradenc(data, base);
    cout << "enc result: " << vrad << endl;
    vraddec(vrad, base, buffer.length());
}


void run_example_vradenc() {

    int base = 33;
    string buffer = "JTH";
    string data;
    /* perform limited charset encoding */
    vmapencode(buffer, &data);
    vradenc(data, base);
}

/* 
 * used to illustrate optimizing exponents using bit shifting
 * up to 23x more efficient
 */
void run_example_exp() {
    uint64_t eval = 8;

    cout << eval << "^n using pow." << endl;
    cout << eval << "^0=" << pow(eval,0) << endl;
    cout << eval << "^1=" << pow(eval,1) << endl;
    cout << eval << "^2=" << pow(eval,2) << endl;

    cout << eval << "^n using bitshift." << endl;

    uint64_t sheval = 8;
    for (int i=0; i<3; i++) {
        cout << eval << "^" << i << "=" << (sheval <<= i) << endl;
    }


}

/*
 * example to exercise encoding buffer to base 33 charset
 */
void run_vmap_example() {
    /* what we want to encode */
    string buffer = "JTH";
    /* where we store encoding */ 
    string data;            

    /* perform limited charset encoding */
    vmapencode(buffer,  &data);
    /* log what was encoded */
    vmapdecode(data);

}

void run_example_vencode() {
    /* 
     * JTHKD5BH0D2170008
     * JTH
     * KD5BH0
     * D2170008
     */
    
    Vradix vradix = {0,0,0};
    int base = 33;
    string data_x;
    string data_y;
    string data_z;

    string buffer_x = "JTH";
    string buffer_y = "KD5BH";
    string buffer_z = "0d2170008";

    vmapencode(buffer_x, &data_x);
    vradix.wmi = vradenc(data_x, base);

    // vmapencode(buffer_y, &data_y);
    // vradix.vds = vradenc(data_y, base); 

    // vmapencode(buffer_z, &data_z);
    // vradix.ser = vradenc(data_z, base);

    vraddec(vradix.wmi, base, data_x.length());
    // vraddec(vradix.vds, base, data_y.length());
    // vraddec(vradix.ser, base, data_z.length());

}

int main() {
    // run_example_exp();
    // return 0;
    // run_example_vencode();
    // run_example_encdec_base10();
    // run_example_encdec_base33("HACK3D");
    

    run_example_vradenc();



}