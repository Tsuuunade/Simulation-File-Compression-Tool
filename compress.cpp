#include "HCTree.hpp"
#include "Helper.hpp"
#include <string.h>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
        // Read the input file and count frequencies of symbols
    FancyInputStream inStream(argv[1]);
    FancyOutputStream outStream(argv[2]);
    if (inStream.filesize() == 0) {
        return 0;
    }

    int symbol;
    vector<int> freqTable(256, 0);
    
    while ((symbol = inStream.read_byte()) != -1) {
        freqTable[symbol] += 1;
    }

    // Construct a HCT
    HCTree* tree = new HCTree();
    tree->build(freqTable);
    inStream.reset();

    // Calculate non-zero frequencies
    int nonZeroCount = 0;
    for (int freq : freqTable) {
        if (freq > 0) {
            nonZeroCount++;
        }
    }

    outStream.write_int(nonZeroCount);

    // Write file header
    for (int i = 0; i < 256; i++) {
        if (freqTable[i] > 0) {
            outStream.write_byte(static_cast<unsigned char>(i)); // Write the byte value
            outStream.write_int(freqTable[i]); // Write its frequency
        }
    }
    
    int c;
    while ((c = inStream.read_byte()) != -1) {
        tree->encode(c, outStream);
    }
    outStream.flush();
    delete tree;
    return 0;
}
