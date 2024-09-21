#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <numeric>
using namespace std;

int main(int argc, char** argv) {
    // Read the input file
    FancyInputStream inStream(argv[1]);
    FancyOutputStream outStream(argv[2]);
    if (inStream.filesize() == 0) {
        return 0;
    }

    // Read the optimized header
    int nonZeroCount = inStream.read_int();
    vector<int> freqTable(256, 0);
    for (int i = 0; i < nonZeroCount; i++) {
        int symbol = inStream.read_byte();
        int freq = inStream.read_int();
        freqTable[symbol] = freq;
    }

    // Rebuild HCT
    HCTree* tree = new HCTree();
    tree->build(freqTable);

    // Write the output file
    int total = accumulate(freqTable.begin(), freqTable.end(), 0);
    unsigned char decode;

    for (int i = 0; i < total; i++) {
        decode = tree->decode(inStream);
        outStream.write_byte(decode);
    }
    outStream.flush();
    delete tree;
    return 0;
}
