#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

class Tokenizer {
private:
    struct Node {
        std::string word;
        int token_id;
        Node* next;
        // Constructor for quick node creation
        Node(std::string w, int id, Node* n) : word(std::move(w)), token_id(id), next(n) {}
    };

    static const int TABLE_SIZE = 10007; // Prime number for better distribution
    Node* hashTable[TABLE_SIZE];
    int current_token_id;

    //  hash function
    unsigned long hashFunction(const std::string& str) {
        unsigned long hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + c; 
        }
        return hash % TABLE_SIZE;
    }

    void toLowercase(std::string& str) {
        for (char& c : str) {
            c = std::tolower(c);
        }
    }

public:
    Tokenizer() : current_token_id(1) {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            hashTable[i] = nullptr;
        }
    }

    // Dynamic Destructor prevents memory leaks
    ~Tokenizer() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = hashTable[i];
            while (current != nullptr) {
                Node* nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }
    }

    int getOrCreateToken(const std::string& word) {
        unsigned long index = hashFunction(word);
        
        // Traversal: Check if word exists
        Node* current = hashTable[index];
        while (current != nullptr) {
            if (current->word == word) {
                return current->token_id;
            }
            current = current->next;
        }
        
        // Insertion: Create new node and push to head of the chain
        hashTable[index] = new Node(word, current_token_id++, hashTable[index]);
        return hashTable[index]->token_id;
    }

    void tokenizeText(std::string text) {
        std::cout << "Raw Text: " << text << "\n\nML Token Stream: [ ";
        
        for (char& c : text) {
            if (c == ',' || c == '.' || c == '-' || c == '\n') c = ' ';
        }
        
        std::stringstream ss(text);
        std::string token;
        
        while (ss >> token) {
            toLowercase(token);
            std::cout << getOrCreateToken(token) << " ";
        }
        std::cout << "]\n";
    }

    int getVocabSize() const {
        return current_token_id - 1;
    }
};

int main() {
    Tokenizer mlTokenizer;
    
    std::string training_data = "Data structures are essential for AI. Data structures power modern machine learning.";
    
    mlTokenizer.tokenizeText(training_data);
    
    std::cout << "\nVocabulary Size: " << mlTokenizer.getVocabSize() << " unique tokens\n";
    
    return 0;
}