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

uint64_t vradenc(string data, int base);
uint64_t vraddec(uint64_t vrad, int base, string data);

/* http://eecs.wsu.edu/~ee314/handouts/numsys.pdf */
uint64_t vradenc(string data, int base) {
    uint64_t vrad = 0;

    for (int i=0; i<data.length(); i++) {
        int exp = (data.length()-1) - i;

        // char c = decc(data[i]);
        // vrad = vrad + c * pow(base, exp); // replace: bitshif
        // cout << "[" << encc(c) << "]" << base << "^" << exp << "=" << vrad << endl;

        /* easier to read */
        char c = data[i];
        vrad = vrad + c * pow(base, exp); // replace: bitshif
        cout << "[" << c << "]" << base << "^" << exp << "=" << vrad << endl;        

    }

    cout << "total: " << vrad << endl;
    vraddec(vrad, base, data);
    return vrad;
}

uint64_t vraddec(uint64_t vrad, int base, string data) {

    cout << "vraddec: " << endl;
    /* ['J']33^2=80586
     * 33^2=1089
     * 80586/1089 = 74 = 'J'
     */

    
    for (int i=0; i<data.length(); i++) {
        int exp = (data.length()-1) - i;

        // char c = decc(data[i]);
        uint64_t vradp = pow(base, exp);

        // vrad = vrad + c * pow(base, exp); // replace: bitshif
        char c = (vrad/vradp); // replace: bitshif
        // cout << "vrad: " << vrad << " vradp: " << vradp << endl;
        // cout << "vrad_p: " << vrad_p << "[" << encc(c) << "]" << base << "^" << exp << "=" << vrad << endl;
        cout << "[" << c << "] " << base << "^" << exp << "=" << vradp << endl;



    }

    cout << "total: " << vrad << endl;
    return vrad;
}

/*
 * our example decodes components of a vin
 * wmi: 0...2
 * vds: 3...7
 * ser: 7...16
 */
void run_example_vradenc() {
    int base = 33;
    string data = "JTH";
    vradenc(data, base);
    // data = "KD5BH";
    // vradenc(data, base);
    // data = "0D2170008";
    // vradenc(data, base);

}

int main() {
    cout << "hello radix" << endl;


    run_example_vradenc();



}