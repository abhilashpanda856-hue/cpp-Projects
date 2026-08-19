#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>


class IColumn {
public:
    virtual ~IColumn() = default;
    virtual void print() const = 0;
};

template <typename T>
class ColumnCell : public IColumn {
private:
    T data;
public:
    explicit ColumnCell(T val) : data(val) {}
    void print() const override { std::cout << data; }
    T getValue() const { return data; }
};


class Row {
private:
    IColumn** cells;       
    size_t columnCount;

public:
    explicit Row(size_t colCount) : columnCount(colCount) {
        cells = new IColumn*[columnCount];
        for (size_t i = 0; i < columnCount; ++i) {
            cells[i] = nullptr; 
        }
    }

    ~Row() {
        for (size_t i = 0; i < columnCount; ++i) {
            delete cells[i]; 
        }
        delete[] cells; 
    }

    template <typename T>
    void setCell(size_t index, T value) {
        if (index >= columnCount) {
            throw std::out_of_range("Column index out of bounds");
        }
        delete cells[index]; 
        cells[index] = new ColumnCell<T>(value);
    }

    void printRow() const {
        for (size_t i = 0; i < columnCount; ++i) {
            if (cells[i]) {
                cells[i]->print();
            } else {
                std::cout << "NULL";
            }
            std::cout << "\t| ";
        }
        std::cout << "\n";
    }
};


class Table {
private:
    std::string tableName;
    std::vector<std::string> schemaNames;
    Row** rows;            
    size_t capacity;
    size_t rowCount;

    void expandCapacity() {
        size_t newCapacity = (capacity == 0) ? 2 : capacity * 2;
        Row** newRows = new Row*[newCapacity];
        for (size_t i = 0; i < rowCount; ++i) {
            newRows[i] = rows[i];
        }
        delete[] rows; 
        rows = newRows;
        capacity = newCapacity;
    }

public:
    Table(const std::string& name, const std::vector<std::string>& columns) 
        : tableName(name), schemaNames(columns), capacity(0), rowCount(0), rows(nullptr) {}

    ~Table() {
        for (size_t i = 0; i < rowCount; ++i) {
            delete rows[i]; 
        }
        delete[] rows;      
    }

    Row* createRow() {
        if (rowCount == capacity) {
            expandCapacity();
        }
        Row* newRow = new Row(schemaNames.size());
        rows[rowCount++] = newRow;
        return newRow;
    }

    void display() const {
        std::cout << "\n=== TABLE: " << tableName << " ===\n";
        for (const auto& col : schemaNames) {
            std::cout << col << "\t| ";
        }
        std::cout << "\n--------------------------------------------------------\n";
        for (size_t i = 0; i < rowCount; ++i) {
            rows[i]->printRow();
        }
        std::cout << "--------------------------------------------------------\n";
    }
};


int main() {
    // Define the updated Student Schema
    std::vector<std::string> studentColumns = {"Reg Number", "Username", "CGPA"};
    Table studentsTable("Students", studentColumns);

    std::cout << "Welcome to the Interactive In-Memory Database Engine!\n";
    
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        long long regNumber; // 64-bit int to prevent overflow with 10-digit numbers
        std::string username;
        double cgpa;

        // 2. Take user input for each column
        std::cout << "\n--- Enter New Student Record ---\n";
        
        std::cout << "Enter Registration Number (e.g., 2502081032): ";
        std::cin >> regNumber;

        std::cout << "Enter Username (no spaces): ";
        std::cin >> username;

        std::cout << "Enter CGPA (decimal): ";
        std::cin >> cgpa;

        Row* newRow = studentsTable.createRow();
        newRow->setCell<long long>(0, regNumber);  // Safely stores the massive 10-digit number
        newRow->setCell<std::string>(1, username); // Stores standard string
        newRow->setCell<double>(2, cgpa);          // Stores standard double

        std::cout << "Record added successfully!\n";

        // Ask if they want to loop again
        std::cout << "Do you want to add another student? (y/n): ";
        std::cin >> choice;
    }

    std::cout << "\nFetching all records...";
    studentsTable.display();

    return 0;
}