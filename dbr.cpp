#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>

// Function to count the frequency of each character in the input string
std::vector<int> countCharacters(const std::string &inputStr) {
    std::vector<int> count(256, 0); // Initialize a vector to store the count of each ASCII character
    for (int i = 0; i < inputStr.size(); i++) {
        count[static_cast<int>(inputStr[i]) + 128]++; // Increment the count of the corresponding character
    }
    return count; // Return the count vector
}

// Function to generate forward mapping and prefix for encoding
void getForwardReverseMapping(const std::vector<int> &count, std::string &outputPrefix, std::unordered_map<char, char> &forwardMapping) {
    std::vector<std::pair<int, char>> freqPairs; // Vector to store character frequencies
    for (int i = 0; i < 256; i++) {
        freqPairs.push_back(std::make_pair(count[i], static_cast<char>(i - 128))); // Create pairs of frequency and character
    }
    std::sort(freqPairs.rbegin(), freqPairs.rend()); // Sort the pairs based on frequency in descending order
    for (int i = 0; i < 256; i++) {
        outputPrefix += freqPairs[i].second; // Append characters to the output prefix
        forwardMapping[freqPairs[i].second] = static_cast<char>(i); // Create forward mapping
    }
}

// Function to encode the input string using dynamic byte remapping
void dynamicByteRemap(const std::string &input, std::string &output) {
    std::vector<int> count = countCharacters(input); // Count the frequency of characters in the input string
    std::string outputPrefix = ""; // Initialize the output prefix
    std::unordered_map<char, char> mapping; // Initialize the mapping
    getForwardReverseMapping(count, outputPrefix, mapping); // Generate forward mapping and prefix
    for (char c : input) {
        output += mapping[c]; // Encode the input string using the mapping
    }
    output = outputPrefix + output; // Prepend the prefix to the encoded string
}

// Function to decode the encoded string using dynamic byte remapping
std::string decodeDBR(const std::string &encoding) {
    std::string decodedString = ""; // Initialize the decoded string
    std::unordered_map<char, char> reverseMap; // Initialize the reverse mapping
    for (int i = 0; i < 256; ++i) {
        reverseMap[static_cast<char>(i)] = encoding[i]; // Create reverse mapping
    }
    for (size_t i = 256; i < encoding.size(); i++) {
        decodedString += reverseMap[encoding[i]]; // Decode the encoded string using reverse mapping
    }
    return decodedString; // Return the decoded string
}

// Function to encode data from a file
void encodeFromFile(const std::string &inputFileName, const std::string &outputFileName) {
    std::ifstream inputFile(inputFileName); // Open input file stream
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file '" << inputFileName << "'" << std::endl;
        return;
    }

    std::string inputString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>()); // Read input file into string
    inputFile.close(); // Close input file stream
    std::string outputString; // Initialize output string
    dynamicByteRemap(inputString, outputString); // Encode the input string

    std::ofstream outputFile(outputFileName); // Open output file stream
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file '" << outputFileName << "'" << std::endl;
        return;
    }

    outputFile << outputString; // Write encoded string to output file
    outputFile.close(); // Close output file stream

    std::cout << "Encoding successful. Output written to '" << outputFileName << "'" << std::endl;
}

// Function to decode data from a file
void decodeFromFile(const std::string &inputFileName, const std::string &outputFileName) {
    std::ifstream inputFile(inputFileName); // Open input file stream
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file '" << inputFileName << "'" << std::endl;
        return;
    }

    std::string inputString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>()); // Read input file into string
    inputFile.close(); // Close input file stream

    std::string decodedString = decodeDBR(inputString); // Decode the input string

    std::ofstream outputFile(outputFileName); // Open output file stream
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file '" << outputFileName << "'" << std::endl;
        return;
    }

    outputFile << decodedString; // Write decoded string to output file
    outputFile.close(); // Close output file stream

    std::cout << "Decoding successful. Output written to '" << outputFileName << "'" << std::endl;
}

// Main function
int main() {
    int choice;
    std::cout << ">>>>>Welcome to Dynamic Byte Remapping (DBR) Encoder/Decoder!" << std::endl;
    std::cout << ">>>>>Press 0 for encoding, Press 1 for decoding: ";
    std::cin >> choice;

    if (choice == 0) {
        std::string inputFileName, outputFileName;
        std::cout << ">>  Enter the input file name: ";
        std::cin >> inputFileName;
        std::cout << ">>  Enter the output file name: ";
        std::cin >> outputFileName;
        encodeFromFile(inputFileName, outputFileName); // Encode input file
    } else if (choice == 1) {
        std::string inputFileName, outputFileName;
        std::cout << ">>  Enter the input file name (encoded): ";
        std::cin >> inputFileName;
        std::cout << ">>  Enter the output file name (decoded): ";
        std::cin >> outputFileName;
        decodeFromFile(inputFileName, outputFileName); // Decode input file
    } else {
        std::cout << "Invalid choice." << std::endl;
    }

    return 0;
}
