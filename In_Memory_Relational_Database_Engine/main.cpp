#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <iomanip>
#include <sstream>


//  TEMPLATES FOR GENERIC DATA COLUMNS

class IColumn {
public:
    virtual ~IColumn() = default;
    virtual void print(int width) const = 0 ; 
    virtual int getPrintWidth() const = 0; 
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
    
    int getPrintWidth() const override {
        std::ostringstream oss;
        oss << data;
        return oss.str().length();
    }
    
    T getValue() const { return data; }
};


//  DYNAMIC MEMORY ALLOCATION FOR ROWS

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
        return 4; 
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


//  TABLE SCHEMA & LIFECYCLE MANAGEMENT

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
        //  Dynamically calculate the max width AND total table width first
        std::vector<int> widths(schemaNames.size());
        int totalWidth = 0; 
        
        for (size_t c = 0; c < schemaNames.size(); ++c) {
            widths[c] = schemaNames[c].length(); 
            for (size_t r = 0; r < rowCount; ++r) {
                int cellWidth = rows[r]->getCellWidth(c);
                if (cellWidth > widths[c]) {
                    widths[c] = cellWidth; 
                }
            }
            widths[c] += 2; // Padding
            totalWidth += widths[c] + 3; // Accounts for the " | " divider
        }

        //  Print Table Name and TOP Line
        std::cout << "\n=== TABLE: " << tableName << " ===\n";
        std::cout << std::string(totalWidth, '-') << "\n";
        
        //  Print Headers
        for (size_t c = 0; c < schemaNames.size(); ++c) {
            std::cout << std::left << std::setw(widths[c]) << schemaNames[c] << " | ";
        }
        
        //  Print MIDDLE Line
        std::cout << "\n" << std::string(totalWidth, '-') << "\n";
        
        //  Print Rows
        for (size_t i = 0; i < rowCount; ++i) {
            rows[i]->printRow(widths);
        }
        
        // 6. Print BOTTOM Line
        std::cout << std::string(totalWidth, '-') << "\n";
    }
};

//  MAIN APPLICATION (INTERACTIVE INPUTS)

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
        
        while (true) {
            std::cout << "Enter Registration Number (exactly 10 digits): ";
            std::cin >> regNumber;

            if (std::cin.fail() || std::to_string(regNumber).length() != 10) {
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                std::cout << "Error: Invalid input! Registration number MUST be exactly 10 digits.\n\n";
            } else {
                break; 
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