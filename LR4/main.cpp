#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Record {
    std::string name;
    int pages;
};

using Collection = std::vector<Record>;

const std::string FILE_NAME = "variant01_books.txt";
const int MIN_PAGES = 1;
const int MAX_PAGES = 50;
const std::size_t MAX_NAME_BYTES = 60;
const std::size_t MAX_RECORDS = 30;

std::string trimSpaces(const std::string& text) {
    const std::size_t first = text.find_first_not_of(' ');
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(' ');
    return text.substr(first, last - first + 1);
}

bool parseInteger(const std::string& text, int& value) {
    std::istringstream input(text);
    int parsed = 0;
    if (!(input >> parsed)) {
        return false;
    }
    input >> std::ws;
    if (!input.eof()) {
        return false;
    }
    value = parsed;
    return true;
}

bool validName(const std::string& name) {
    return !name.empty() && name.size() <= MAX_NAME_BYTES &&
           name.find('\n') == std::string::npos &&
           name.find('\r') == std::string::npos;
}

bool validRecord(const Record& record) {
    return validName(record.name) && record.name == trimSpaces(record.name) &&
           record.pages >= MIN_PAGES && record.pages <= MAX_PAGES;
}

bool readLine(const std::string& prompt, std::string& line) {
    std::cout << prompt;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nInput ended. Program is closing without automatic save.\n";
        return false;
    }
    if (std::cin.eof()) {
        std::cout << "\nInput ended. Program is closing without automatic save.\n";
        return false;
    }
    return true;
}

bool readNewRecord(Record& record) {
    std::string name;
    if (!readLine("Book title: ", name)) {
        return false;
    }
    name = trimSpaces(name);
    if (!validName(name)) {
        std::cout << "Name must contain 1..60 bytes after trimming ordinary edge spaces.\n";
        return false;
    }

    std::string numberLine;
    if (!readLine("Number of pages (1..50): ", numberLine)) {
        return false;
    }
    int pages = 0;
    if (!parseInteger(numberLine, pages) || pages < MIN_PAGES || pages > MAX_PAGES) {
        std::cout << "Enter one whole number from 1 to 50. No record was changed.\n";
        return false;
    }

    record = {name, pages};
    return true;
}

void printCollection(const Collection& records) {
    if (records.empty()) {
        std::cout << "Collection is empty.\n";
        return;
    }
    for (std::size_t i = 0; i < records.size(); ++i) {
        std::cout << (i + 1) << ". " << records[i].name
                  << " - pages: " << records[i].pages << '\n';
    }
}

void addRecord(Collection& records) {
    if (records.size() >= MAX_RECORDS) {
        std::cout << "The collection already has 30 records.\n";
        return;
    }
    Record candidate;
    if (readNewRecord(candidate)) {
        records.push_back(candidate);
        std::cout << "Record added.\n";
    }
}

void searchRecords(const Collection& records) {
    std::string query;
    if (!readLine("Substring to find (case-sensitive): ", query)) {
        return;
    }
    query = trimSpaces(query);
    if (query.empty()) {
        std::cout << "The search query cannot be empty.\n";
        return;
    }

    bool found = false;
    for (std::size_t i = 0; i < records.size(); ++i) {
        if (records[i].name.find(query) != std::string::npos) {
            std::cout << (i + 1) << ". " << records[i].name
                      << " - pages: " << records[i].pages << '\n';
            found = true;
        }
    }
    if (!found) {
        std::cout << "No matches.\n";
    }
}

void editRecord(Collection& records) {
    if (records.empty()) {
        std::cout << "Collection is empty.\n";
        return;
    }
    std::string numberLine;
    if (!readLine("Record number (1.." + std::to_string(records.size()) + "): ", numberLine)) {
        return;
    }
    int number = 0;
    if (!parseInteger(numberLine, number) || number < 1 ||
        static_cast<std::size_t>(number) > records.size()) {
        std::cout << "No such record number. No record was changed.\n";
        return;
    }

    Record candidate;
    if (!readNewRecord(candidate)) {
        return;
    }
    records[static_cast<std::size_t>(number - 1)] = candidate;
    std::cout << "Record " << number << " updated.\n";
}

bool readFileInteger(const std::string& line, int& value) {
    return parseInteger(line, value);
}

bool loadRecords(Collection& records) {
    std::ifstream input(FILE_NAME);
    if (!input) {
        std::cout << "Cannot open " << FILE_NAME << ". The collection is unchanged.\n";
        return false;
    }

    std::string line;
    if (!std::getline(input, line)) {
        std::cout << "File is empty or unreadable. The collection is unchanged.\n";
        return false;
    }
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    int count = 0;
    if (!readFileInteger(line, count) || count < 0 ||
        static_cast<std::size_t>(count) > MAX_RECORDS) {
        std::cout << "Invalid record count. The collection is unchanged.\n";
        return false;
    }

    Collection candidate;
    for (int i = 0; i < count; ++i) {
        std::string nameLine;
        std::string pagesLine;
        if (!std::getline(input, nameLine) || !std::getline(input, pagesLine)) {
            std::cout << "File is incomplete. The collection is unchanged.\n";
            return false;
        }
        if (!nameLine.empty() && nameLine.back() == '\r') {
            nameLine.pop_back();
        }
        if (!pagesLine.empty() && pagesLine.back() == '\r') {
            pagesLine.pop_back();
        }
        nameLine = trimSpaces(nameLine);
        int pages = 0;
        Record record{nameLine, 0};
        if (!validName(record.name) || !readFileInteger(pagesLine, pages) ||
            pages < MIN_PAGES || pages > MAX_PAGES) {
            std::cout << "Invalid record in file. The collection is unchanged.\n";
            return false;
        }
        record.pages = pages;
        candidate.push_back(record);
    }

    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            std::cout << "Unexpected extra content in file. The collection is unchanged.\n";
            return false;
        }
    }
    if (input.bad() || !input.eof()) {
        std::cout << "Read error. The collection is unchanged.\n";
        return false;
    }

    records = candidate;
    std::cout << "Loaded " << records.size() << " record(s).\n";
    return true;
}

bool saveRecords(const Collection& records) {
    if (records.size() > MAX_RECORDS) {
        std::cout << "Too many records; file was not opened for overwrite.\n";
        return false;
    }
    for (const Record& record : records) {
        if (!validRecord(record)) {
            std::cout << "A record is invalid; file was not opened for overwrite.\n";
            return false;
        }
    }

    std::ofstream output(FILE_NAME);
    if (!output) {
        std::cout << "Cannot open " << FILE_NAME << " for writing.\n";
        return false;
    }
    output << records.size() << '\n';
    for (const Record& record : records) {
        output << record.name << '\n' << record.pages << '\n';
    }
    output.close();
    if (!output) {
        std::cout << "Write failed. The file may be incomplete; check it before reuse.\n";
        return false;
    }
    std::cout << "Saved " << records.size() << " record(s) to " << FILE_NAME << ".\n";
    return true;
}

void printMenu() {
    std::cout << "\n=== Bookshelf collection ===\n"
              << "0 - Exit\n1 - Add\n2 - View\n3 - Search all matches\n"
              << "4 - Edit one record\n5 - Load\n6 - Save\n";
}

int main() {
    Collection records;
    bool running = true;
    while (running) {
        printMenu();
        std::string commandLine;
        if (!readLine("Choice: ", commandLine)) {
            break;
        }
        int command = -1;
        if (!parseInteger(commandLine, command) || command < 0 || command > 6) {
            std::cout << "Choose a menu number from 0 to 6.\n";
            continue;
        }

        switch (command) {
        case 0:
            running = false;
            break;
        case 1:
            addRecord(records);
            break;
        case 2:
            printCollection(records);
            break;
        case 3:
            searchRecords(records);
            break;
        case 4:
            editRecord(records);
            break;
        case 5:
            loadRecords(records);
            break;
        case 6:
            saveRecords(records);
            break;
        }
        if (std::cin.eof()) {
            running = false;
        }
    }
    std::cout << "Goodbye. Unsaved changes are not saved automatically.\n";
    return 0;
}
