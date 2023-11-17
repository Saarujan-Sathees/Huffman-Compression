# Huffman-Compression
A binary implementation of the Huffman Tree Encoding algorithm, as a data stream. This only works with text containing pure ASCII text.

- Allows data from Strings and files to be encoded with EncodeStream, which writes the Huffman-encoded version of the
  given data to the given output file
- Allows encoded files to be decoded with DecodeStream, which can be used in a similar fashion as ifstream from C++


Files
- main.cpp: the C++ code containing the EncodeStream and DecodeStream class
- input.txt: an example of a file that can be encoded
- test.bin: the encoded version of the provided input.txt file
- output.txt: the decoded version of the provided test.bin file, showing that it matches the original input.txt file
