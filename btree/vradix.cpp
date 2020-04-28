#include <iostream>
#include <math.h>

using namespace std;

char encc(int code) {
    return char(code + '0');
}
char decc(int code) {
    return char(code - '0');
}

string vmap = "0123456789ABCDEFGHJKLMNPRSTUVWXYZ";

int vmapindexof(char c) {
    return vmap.find(c);
}

char vmapcharat(int index) {
    return vmap[index];
}

void vmapencode(string src, string* dest) {
    for (int i=0; i<src.length(); i++) {
        dest->push_back(vmapindexof(src[i]));
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
uint64_t vradenc(string data, int base) {
    uint64_t vrad = 0;
    cout << "vradenc: ";
    vmapdecode(data);
    cout << " base: " << base << endl;

    for (int i=0; i<data.length(); i++) {

        int exp = (data.length()-1) - i;

        /* get the index of the character at i(th) position */
        int mval = data[i];
        char mvalc = vmapcharat(mval);

        // cout << "mval: " << mval << " mvalc: " << mvalc << endl;

        vrad = vrad + mval * pow(base, exp); // replace: bitshif
        // cout << "[" << mvalc << "]" << base << "^" << exp << "=" << vrad << endl;    

        cout << "[" << i << "]";
        cout << "[" << mval << "]";     
        cout << "[" << mvalc << "]";   
        cout << base << "^" << exp << "=" << vrad << endl;         

    }

    // cout << "total: " << vrad << endl;
    vraddec(vrad, base, data.length());
    return vrad;
}

uint64_t vraddec(uint64_t vrad, int base, int len) {

    cout << "vraddec: " << vrad << endl;
    
    for (int i=0; i<len; i++) {
        int vmapi = 0;
        int vdelta = 0;
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

void run_example_encdec_base10() {
    int base = 10;
    string buffer = "843";
    string data;

    vmapencode(buffer, &data);
    vradenc(data, base);    
}

void run_example_encdec_base33() {
    int base = 33;
    string buffer = "JTH";
    string data;

    vmapencode(buffer, &data);
    vradenc(data, base);    
}
/*
 * our example decodes components of a vin
 * wmi: 0...2
 * vds: 3...7
 * ser: 7...16
 */
void run_example_vradenc() {

    int base = 33;
    string buffer = "JTHKD5BH0D2170008";
    string data;
    /* perform limited charset encoding */
    vmapencode(buffer, &data);
    vradenc(data, base);

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

int main() {

    // run_example_encdec_base10();
    //run_example_encdec_base33();

    run_example_vradenc();



}