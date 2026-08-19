#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <iomanip>
#include <sstream> // to dynamically measure text lengths


class IColumn {
public:
    virtual ~IColumn() = default;
    virtual void print(int width) const = 0; 
    virtual int getPrintWidth() const = 0; //  to measure data length
};

template <typename T>
class ColumnCell : public IColumn {
private:
    T data;
public:
    explicit ColumnCell(T val) : data(val) {}
    
    void print(int width) const override { 
        std::cout << std::left << std::setw(width) << data; 
    }
    
    // Calculates exactly how many characters this specific data takes up
    int getPrintWidth() const override {
        std::ostringstream oss;
        oss << data;
        return oss.str().length();
    }
    
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

    int getCellWidth(size_t index) const {
        if (cells[index]) return cells[index]->getPrintWidth();
        return 4; // Length of the word "NULL"
    }

    void printRow(const std::vector<int>& colWidths) const {
        for (size_t i = 0; i < columnCount; ++i) {
            if (cells[i]) {
                cells[i]->print(colWidths[i]);
            } else {
                std::cout << std::left << std::setw(colWidths[i]) << "NULL";
            }
            std::cout << " | ";
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
        //  Dynamically calculate the maximum width for each column
        std::vector<int> widths(schemaNames.size());
        for (size_t c = 0; c < schemaNames.size(); ++c) {
            widths[c] = schemaNames[c].length(); // Start with the header length
            for (size_t r = 0; r < rowCount; ++r) {
                int cellWidth = rows[r]->getCellWidth(c);
                if (cellWidth > widths[c]) {
                    widths[c] = cellWidth; // Expand margin if the data is longer
                }
            }
            widths[c] += 2; // Adds 2 spaces of padding for breathing room
        }

        //  Print Headers
        std::cout << "\n=== TABLE: " << tableName << " ===\n";
        int totalWidth = 0;
        for (size_t c = 0; c < schemaNames.size(); ++c) {
            std::cout << std::left << std::setw(widths[c]) << schemaNames[c] << " | ";
            totalWidth += widths[c] + 3; // +3 accounts for the " | " divider
        }
        
        //  Print dividing line exactly matching total table width
        std::cout << "\n" << std::string(totalWidth, '-') << "\n";
        
        //  Print Rows using dynamic margins
        for (size_t i = 0; i < rowCount; ++i) {
            rows[i]->printRow(widths);
        }
        std::cout << std::string(totalWidth, '-') << "\n";
    }
};


int main() {
    std::vector<std::string> studentColumns = {"Reg Number", "Username", "CGPA"};
    Table studentsTable("Students", studentColumns);

    std::cout << "Welcome to the Interactive In-Memory Database Engine!\n";
    
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        long long regNumber = 0; 
        std::string username;
        double cgpa;

        std::cout << "\n--- Enter New Student Record ---\n";
        
        // STRICT 10-DIGIT VALIDATION LOOP
        while (true) {
            std::cout << "Enter Registration Number (exactly 10 digits): ";
            std::cin >> regNumber;

            // Check if input failed OR if the length of the number is not exactly 10
            if (std::cin.fail() || std::to_string(regNumber).length() != 10) {
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Dump bad input
                std::cout << "Error: Invalid input! Registration number MUST be exactly 10 digits.\n\n";
            } else {
                break; // Input is valid, break out of loop
            }
        }

        std::cout << "Enter Username (spaces allowed): ";
        std::getline(std::cin >> std::ws, username);

        std::cout << "Enter CGPA (decimal): ";
        std::cin >> cgpa;

        Row* newRow = studentsTable.createRow();
        newRow->setCell<long long>(0, regNumber);  
        newRow->setCell<std::string>(1, username); 
        newRow->setCell<double>(2, cgpa);          

        std::cout << "Record added successfully!\n";

        std::cout << "Do you want to add another student? (y/n): ";
        std::cin >> choice;
    }

    std::cout << "\nFetching all records...\n";
    studentsTable.display();

    return 0;
}