#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define CHAR_COUNT 128
#define HALF_CHAR_COUNT 64
typedef unsigned long long ulong;

class BigInt {
    public:
    ulong left, right;

    public:
    BigInt() {
        left = 0;
        right = 0;
    }

    BigInt(ulong num) {
        left = 0;
        right = num;
    }

    BigInt(ulong left, ulong right) {
        this -> left = left;
        this -> right = right;
    }

    BigInt(const BigInt& bInt) {
        left = bInt.left;
        right = bInt.right;
    }

    #pragma region Bitwise Operators
    BigInt operator << (ulong num) const {
        BigInt res(left, right);

        //If the shift is bigger than 64 (half of 128), then perform a leftward shift instead
        if (num > HALF_CHAR_COUNT) {
            num %= HALF_CHAR_COUNT;
            res.left <<= num;
            res.left |= res.right << num;
            res.right = 0;
        } else {
            res.left <<= num;
            res.left |= res.right >> (HALF_CHAR_COUNT - num);
            res.right <<= num;
        }

        return res;
    }

    BigInt operator >> (ulong num) const {
        BigInt res(left, right);

        if (num > HALF_CHAR_COUNT) {
            num %= HALF_CHAR_COUNT;
            res.right >>= num;
            res.right |= res.left >> num;
            res.left = 0;
        } else {
            res.right >>= num;
            res.right |= res.left << (HALF_CHAR_COUNT - num);
            res.left >>= num;
        }

        return res;
    }

    BigInt operator | (ulong num) const {
        return BigInt(left, right | num);
    }

    BigInt operator | (BigInt num) const {
        return BigInt(left | num.left, right | num.right);
    }

    BigInt operator & (BigInt num) const {
        return BigInt(left & num.left, right & num.right);
    }

    void operator <<= (ulong num) {
        if (num > HALF_CHAR_COUNT) {
            num %= HALF_CHAR_COUNT;
            left <<= num;
            left |= right << num;
            right = 0;
        } else {
            left <<= num;
            left |= right >> (HALF_CHAR_COUNT - num);
            right <<= num;
        }
    }

    void operator >>= (ulong num) {
        if (num > HALF_CHAR_COUNT) {
            num %= HALF_CHAR_COUNT;
            right >>= num;
            right |= left >> num;
            left = 0;
        } else {
            right >>= num;
            right |= left << (HALF_CHAR_COUNT - num);
            left >>= num;
        }
    }
    
    void operator |= (ulong num) {
        right |= num;
    }

    void operator |= (BigInt num) {
        left |= num.left;
        right |= num.right;
    }

    void operator &= (BigInt num) {
        left &= num.left;
        right &= num.right;
    }

    #pragma endregion

    bool operator == (ulong num) {
        return right == num && left == 0;
    }

    bool operator != (ulong num) {
        return left != 0 || right != num;
    }

    explicit operator ulong() {
        return left > 0 ? -1 : right;
    }

    char bitCount() {
        BigInt temp(left, right);
        char res = 0;
        
        if (temp.left == 0) { //Only test the right side because the left isn't significant as it's 0
            while (temp.right != 0) {
                temp.right >>= 1;
                ++res;
            }
        } else { //Ignore the right side as the left side is more significant
            res = HALF_CHAR_COUNT;
            while (temp.left != 0) {
                temp.left >>= 1;
                ++res;
            }
        }

        return res;
    }

    std::string toBinary() {
        ulong l = left, r = right;

        std::string res = r % 2 == 0 ? "0" : "1";

        while (r > 1) {
            r /= 2;
            res = (char) (r % 2 + '0') + res;
        }

        for (int i = HALF_CHAR_COUNT - res.size(); i > 0; --i) {
            res = "0" + res;
        }

        res = (l % 2 == 0 ? "0" : "1") + res;
        
        while (l > 1) {
            l /= 2;
            res = (char) (l % 2 + '0') + res;
        }

        for (int i = CHAR_COUNT - res.size(); i > 0; --i) {
            res = "0" + res;
        }

        return res;
    }

};

struct Node {
    Node* left, *right;
    int frequency, number;

    Node() {
        left = nullptr;
        right = nullptr;
        number = 0;
        frequency = 0;
    }

    Node(Node* l, Node* r) {
        left = l;
        right = r;
        frequency = left -> frequency + right -> frequency;
    }

    Node(int num) {
        number = num;
        left = nullptr;
        right = nullptr;
    }

    Node(int num, Node* l, Node* r) {
        number = num;
        frequency = l -> frequency + r -> frequency;
        left = l;
        right = r;
    }

    /*static void printInOrder(Node* head) {
        if (head == nullptr) return;
        
        printInOrder(head -> left);
        std::cout << '"' << head -> values << "\", ";
        printInOrder(head -> right);
    }

    static void printPreOrder(Node* head) {
        if (head == nullptr) return;

        std::cout << '"' << head -> values << "\", ";
        printPreOrder(head -> left);
        printPreOrder(head -> right);
    }

    static void printBinary(Node* head, std::string bin = "") {
        if (head -> left == nullptr && head -> right == nullptr) {
            std::cout << (char) head -> number << " - " << bin << '\n';
            return;
        }

        printBinary(head -> left, bin + "0");
        printBinary(head -> right, bin + "1");
    }*/

};

std::vector<Node*> calcFrequency(const std::string& str) {
    std::vector<Node*> freq(CHAR_COUNT), shortened;
    int index;

    for (int i = 0; i < freq.size(); ++i) {
        freq[i] = new Node();
    }

    for (int i = 0; i < str.length(); ++i) {
        index = (int) str[i];

        if (index < 0 || index >= CHAR_COUNT) {
            std::cout << index << "\n";
            std::cout << str.substr(0, i) << "\n";
            throw std::invalid_argument("Cannot contain non-ASCII characters!");  
        }  

        ++(freq[index] -> frequency);
        if (freq[index] -> frequency == 1) {
            freq[index] -> number = index;
            shortened.push_back(freq[index]);
        }
    }

    std::sort(shortened.begin(), shortened.end(), [](Node* a, Node* b) { 
        return a -> frequency > b -> frequency;
    });

    return shortened;
}

void insertNode(Node* n, std::vector<Node*> &freq) {
    for (int i = 0; i < freq.size(); ++i) {
        if (n -> frequency >= freq[i] -> frequency) {
            freq.emplace(freq.begin() + i, n);
            return;
        }
    }

    freq.emplace_back(n);
}

Node* buildTree(std::vector<Node*> freq) {
    if (freq.size() == 1) return freq[0];

    const int a = freq.size() - 1, b = freq.size() - 2;
    Node* head = new Node(-1, freq[a], freq[b]);

    freq.resize(b);
    
    insertNode(head, freq);
    return buildTree(freq);
}

Node* buildTree(const std::string& str) {
    return buildTree(calcFrequency(str));
}


struct Pair {
    BigInt code;
    unsigned char size;

    Pair(BigInt c) {
        code = c;
        size = c.bitCount();
    }

    Pair(BigInt c, unsigned char s) {
        code = c;
        size = s;
    }

};

class HuffmanStream {
    protected:
    BigInt stream;
    Node* encodingTree;
    unsigned char streamSize;
    static const unsigned char SUB_SHIFT = CHAR_COUNT - 8; //Amount to shift by to get the first byte of a BigInt

    public:
    virtual void close() = 0;
};

class EncodeStream : public HuffmanStream {
    protected:
    std::vector<Pair*> encTable;
    std::string fileName, encodedContents;

    void initTable(Node* curr, Pair bin = Pair(BigInt(0), 0)) {
        if (curr -> left == nullptr && curr -> right == nullptr) {
            encTable[curr -> number] = new Pair(bin.code, bin.size); //Setting the binary equivalent of the character at that index
            return;
        }

        bin.code <<= 1; //Shifting the code leftwards by 1 bit
        ++bin.size;
        initTable(curr -> left, bin); //Traversing the left node; leaving the last bit as 0 to indicate it's on the left
        bin.code |= 1;
        initTable(curr -> right, bin); //Traversing the right node; setting the last bit to 1
    }

    void savePostOrder(std::string& res, Node* curr, Pair &bitStream) {
        if (curr == nullptr) return;
        
        //Adding left and right node results first; post-order traversal
        savePostOrder(res, curr -> left, bitStream);
        savePostOrder(res, curr -> right, bitStream);

        if (curr -> left == nullptr && curr -> right == nullptr) { //If its a leaf node
            bitStream.size += 9; //Adding 9 to the size: 1 since it's a leaf, and 8 for the ASCII code for the character
            bitStream.code |= BigInt(curr -> number + 0x100) << (CHAR_COUNT - bitStream.size); //Shifting new char and setting bits
        } else { //If its an internal node
            ++bitStream.size; //Adding 1 to the size, since a 0 shows that it is an internal node
        }
        
        while (bitStream.size >= 8) {
            res += static_cast<ulong>(bitStream.code >> SUB_SHIFT); //Taking the first 8 bits and adding char value
            bitStream.code <<= 8; //Shifting the written bits out of the number
            bitStream.size -= 8; //Subtracting 1 byte from the stream size
        }
    }

    void writeTree() {
        Pair bitStream(0, 0);
        std::string res = "";

        savePostOrder(res, encodingTree, bitStream);

        //Prepending the resulting tree, the remaining bits (padded with 0's) and a NULL character to encodedContents;
        encodedContents = (char) 0 + encodedContents;
        encodedContents = res + (char) static_cast<ulong>(bitStream.code >> SUB_SHIFT) + encodedContents;
    }

    public:
    EncodeStream(const std::string& inputFile) {
        stream = BigInt(0);
        streamSize = 0;
        encodingTree = nullptr;
        fileName = "";
        encodedContents = "";
        encTable = std::vector<Pair*>(CHAR_COUNT, nullptr);
    }

    EncodeStream(const std::string& outputFile, const std::string& inputFile) {
        stream = BigInt(0);
        streamSize = 0;
        fileName = outputFile;
        encodedContents = "";
        encTable = std::vector<Pair*>(CHAR_COUNT, nullptr);

        std::string fileContent = "";
        FILE* input = fopen(inputFile.c_str(), "r"); //Opening the input file in normal read-mode, so a Huffman Tree can be built
        int c;
        while (true) {
            c = fgetc(input);
            if (c == -1) break;

            fileContent += (char) c; //Saving the file content to generate a tree later
        }

        fclose(input); //Closing the file
        encodingTree = buildTree(fileContent);
        initTable(encodingTree);

        /*std::cout << encTable.size() << "\n";
        for (int i = 0; i < encTable.size(); ++i) {
            if (encTable[i] != nullptr)
                std::cout << (char) i << ": " << (encTable[i] -> code << (CHAR_COUNT - encTable[i] -> size)).toBinary().substr(0, encTable[i] -> size) << "\n";
        }*/
    }

    EncodeStream(const std::string& outputFile, Node* encodeTree) {
        stream = BigInt(0);
        streamSize = 0;
        encodingTree = encodeTree;
        fileName = outputFile;
        encodedContents = "";
        encTable = std::vector<Pair*>(CHAR_COUNT, nullptr);
        initTable(encodingTree);
    }

    void write(const char& character) {
        if (encTable[character] == nullptr) {
            std::cout << character << '\n';
        }

        streamSize += encTable[character] -> size; //Increasing the stream size by the new character size
        stream |= (encTable[character] -> code) << (CHAR_COUNT - streamSize); //Shifting new char to right pos and setting their bits
        
        while (streamSize >= 8) { //8 since 1 byte (the min amount you can write to a file) is 8 bits
            encodedContents += static_cast<ulong>(stream >> SUB_SHIFT); //Taking the first 8 bits and adding char value
            stream <<= 8; //Shifting the non-written bits to the front of the number
            streamSize -= 8; //Subtracting 1 byte from the stream size in bits
        }
    }

    void write(const std::string& str) {
        for (char c : str) {
            write(c);
        }
    }

    void close() override {
        writeTree();
        //Adding & flushing remaining bits that haven't been written yet to a byte-size piece (character) to the encoded contents
        encodedContents = (char) static_cast<ulong>(stream >> SUB_SHIFT) + encodedContents; 
        encodedContents = (char) (streamSize + '0') + encodedContents; //Prepending the # of padded zeroes at EOF

        FILE* output = fopen(fileName.c_str(), "wb"); //Opening the output file in binary mode
        fwrite(encodedContents.c_str(), sizeof(char), encodedContents.size(), output); //Writing the encoded contents to the file

        //Emptying encoded contents to not waste memory; using more memory when encoding since decoding will be performed more 
        encodedContents = ""; 
        stream = BigInt(0);
        fclose(output); //Closing the file
    }

};

class DecodeStream : public HuffmanStream {
    protected:
    FILE* input;
    Node* lastNode;
    char endSize;
    BigInt endChar;
    const BigInt BIT_FLIP = BigInt(~0, ~0); //Filled with 1's 

    void rebuildNode(Node* curr, char c, BigInt bin, int binSize) {
        char direction = (char) static_cast<ulong>(bin >> (--binSize));
        bin &= BIT_FLIP >> (CHAR_COUNT - binSize);

        if (direction == 0) { //Left
            if (binSize > 0) {
                if (curr -> left == nullptr)
                    curr -> left = new Node(-1); //Initializing with -1 so less checks have to be performed later
                
                rebuildNode(curr -> left, c, bin, binSize);
            } else if (curr -> left == nullptr) { //If next node is null, create a new Node there
                curr -> left = new Node(c);
            } else if (curr -> left != nullptr) { //If node is at right position but something is already there
                curr -> left -> number = c;
            } 
        } else { //Right
            if (binSize > 0) {
                if (curr -> right == nullptr)
                    curr -> right = new Node(-1); //Initializing with -1 so less checks have to be performed later
                
                rebuildNode(curr -> right, c, bin, binSize);
            } else if (curr -> right == nullptr) { //If at the end of the tree
                curr -> right = new Node(c);
            } else if (curr -> right != nullptr) { //If node is at right position but something is already there
                curr -> right -> number = c;
            }
        }
    }

    Node* rebuildTree() {
        std::vector<Node*> stack(0);
        Node* temp;
        BigInt bitStream = BigInt(fgetc(input)) << SUB_SHIFT;
        char size = 7; //Storing input in character, and setting size to 7 since 1 bit is read
        bool bit = static_cast<ulong>(bitStream >> 127), fileEnded = false;
        bitStream <<= 1; //Shifting the unread bits to the front

        while (true) {
            while (size < 9 && !fileEnded) { //If there isn't enough bits for an ASCII code, read more from the file
                int temp = fgetc(input);
                if (temp == -1) {
                    fileEnded = true;
                } else {
                    size += 8;
                    bitStream |= BigInt(temp) << (CHAR_COUNT - size);
                }
            }

            if (bit) { //If the read bit is a 1
                stack.emplace_back(new Node(static_cast<ulong>(bitStream >> SUB_SHIFT)));
                bitStream <<= 8; //Shifting the read character out of stream
                size -= 8;
            } else {
                if (stack.size() == 1) {
                    if (size > 0) //Unreading the extra character that was read 
                        ungetc(static_cast<ulong>(bitStream >> (CHAR_COUNT - size)) & 0xFF, input); 
                    
                    return stack[0];
                }

                temp = new Node(-1, stack[stack.size() - 2], stack[stack.size() - 1]);
                stack.pop_back();
                stack.pop_back();

                stack.emplace_back(temp);
            }

            bit = static_cast<ulong>(bitStream >> 127); //Getting the first bit of the character (8 bits)
            bitStream <<= 1; //Shifting the unread bits to the front
            --size; //Decrementing the size of the character
        }

        return nullptr;
    }

    void readHeader() {
        if (input == nullptr) return;

        endSize = fgetc(input) - '0';
        endChar = BigInt(fgetc(input)) << SUB_SHIFT;
        encodingTree = rebuildTree();
    }

    char getCharacter(Node* curr) {
        if (curr -> number != -1) { //If at a leaf node
            lastNode = encodingTree;
            return curr -> number;
        } else if (streamSize == 0) { //If the code has been finished, but a character hasn't been reached yet, save node and return -1
            lastNode = curr;
            return -1;
        }

        --streamSize; //Decrementing the size of the stream since the first bit was removed
        bool isLeft = (stream >> 127) == 0;
        stream <<= 1;
        return getCharacter(isLeft ? curr -> left : curr -> right);
    }

    char readEndSequence() {
        streamSize += endSize; //Adding the amount of bits in the end sequence to the stream size
        stream = endChar; //Setting the stream to the end sequence
        endSize = 0; //Setting endSize to 0 so read() knows that the end sequence(s) have been added to the stream

        char decoded = getCharacter(lastNode);
        if (decoded == -1) //If a character hasn't been decoded, return EOF
            return EOF;
        else 
            return decoded; 
    }

    public:
    DecodeStream() {
        input = nullptr;
        streamSize = 0;
        stream = BigInt(0);
        encodingTree = new Node(new Node(), new Node());
    }

    DecodeStream(const std::string& inputFile) {
        input = fopen(inputFile.c_str(), "rb");
        streamSize = 0;
        stream = BigInt(0);
        readHeader();
        lastNode = encodingTree;
    }

    bool eof() {
        int temp = fgetc(input);
        ungetc(temp, input);
        return temp == -1 && streamSize == 0;
    }

    char read() {
        if (streamSize == 0) { //Only reading if the stream has enough space
            int encoded = fgetc(input);

            if (encoded == EOF) { //If reached end of file
                if (endSize != 0) //If there is still character(s) to return; this only runs once,
                    return readEndSequence(); //since the stream size won't be 0 if there were more characters in endChar
                
                return EOF;
            }
            
            streamSize += 8; //Increasing the stream size by the new character size
            stream |= BigInt(encoded) << (CHAR_COUNT - streamSize); //Shifting new char to the right place and setting their bits
        } 

        char decoded = getCharacter(lastNode);

        if (decoded == -1) //If a character hasn't been decoded, keep on reading the stream until a character has been decoded
            return read(); 
        
        return decoded;
    }

    void close() override {
        fclose(input);
    }

};


Node* rebuildTree(const std::string bin) {
    std::vector<Node*> stack(0);
    Node* temp;

    for (int i = 0; i < bin.length(); ++i) {
        if (bin[i] == '1') {
            stack.emplace_back(new Node((int) bin[++i]));
        } else if (bin[i] == '0') {
            if (stack.size() == 1) return stack[0];

            temp = new Node(-1, stack[stack.size() - 2], stack[stack.size() - 1]);
            stack.pop_back();
            stack.pop_back();

            stack.emplace_back(temp);
        }
    }

    return stack.size() == 1 ? stack[0] : nullptr;
}

int main() {
    auto output = EncodeStream("test.bin", "input.txt"); //Generate the tree from the input file
    FILE* inputFile = fopen("input.txt", "r");
    int c;
    while (true) {
        c = fgetc(inputFile);
        if (c == -1) break;

        output.write((char) c);
    }

    fclose(inputFile); //Closing the file
    output.close(); //Closing the encoding stream
    
    auto input = DecodeStream("test.bin"); //Opening the encoded file
    FILE* outputFile = fopen("output.txt", "w");
    while (!input.eof()) {
        fputc(input.read(), outputFile);
    }

    fclose(outputFile); //Closing the output file
    input.close(); //Closing the decoding stream
    std::cout << "Success!\n";
}