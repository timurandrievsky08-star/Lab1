#include <cassert>
#include <iostream>

namespace {
constexpr int QUIZ_MIN = 0;
constexpr int QUIZ_MAX = 20;
constexpr int PRACTICE_MIN = 0;
constexpr int PRACTICE_MAX = 30;
constexpr int EXAM_MIN = 0;
constexpr int EXAM_MAX = 50;
constexpr int RECORD_LIMIT = 30;
constexpr int SEARCH_THRESHOLD = 50;

int calculateTotal(int quiz, int practice, int exam) {
    return quiz + practice + exam;
}

const char* categoryFor(int m) {
    if (m < 50) {
        return "needs additional work";
    }
    if (m <= 69) {
        return "average score";
    }
    if (m <= 84) {
        return "good score";
    }
    return "perfect score";
}

bool isSearchMatch(int m) {
    return m >= SEARCH_THRESHOLD;
}

bool readValidRecord(int recordNumber, int& quiz, int& practice, int& exam) {
    while (true) {
        std::cout << "Record " << recordNumber
                  << " (quiz 0..20, practice 0..30, exam 0..50): ";

        if (!(std::cin >> quiz >> practice >> exam)) {
            if (std::cin.eof()) {
                std::cerr << "Incomplete input: EOF reached.\n";
            } else {
                std::cerr << "Format error: please enter three integers. "
                             "Program terminated without summary.\n";
            }
            return false;
        }

        if (quiz < QUIZ_MIN || quiz > QUIZ_MAX ||
            practice < PRACTICE_MIN || practice > PRACTICE_MAX ||
            exam < EXAM_MIN || exam > EXAM_MAX) {
            std::cerr << "Range error. Please repeat record " << recordNumber
                      << " (quiz 0..20, practice 0..30, exam 0..50).\n";
            continue;
        }
        return true;
    }
}
}

int main() {
    assert(calculateTotal(16, 24, 40) == 80);
    assert(calculateTotal(0, 0, 0) == 0);
    assert(calculateTotal(20, 30, 50) == 100);
    assert(isSearchMatch(50));
    assert(!isSearchMatch(49));

    int recordCount = 0;
    while (true) {
        std::cout << "Number of records N (1..30): ";
        if (!(std::cin >> recordCount)) {
            if (std::cin.eof()) {
                std::cerr << "Incomplete input for N. No summary generated.\n";
            } else {
                std::cerr << "Format error for N. Program terminated.\n";
            }
            return 1;
        }
        if (recordCount >= 1 && recordCount <= RECORD_LIMIT) {
            break;
        }
        std::cerr << "N must be between 1 and 30. Please enter N again.\n";
    }

    int sum = 0;
    int minimum = 0;
    int maximum = 0;
    int minimumRecord = 0;
    int maximumRecord = 0;
    int matchCount = 0;
    int firstMatch = 0;

    for (int acceptedRecord = 1; acceptedRecord <= recordCount; ++acceptedRecord) {
        int quiz = 0;
        int practice = 0;
        int exam = 0;
        if (!readValidRecord(acceptedRecord, quiz, practice, exam)) {
            return 1;
        }

        const int m = calculateTotal(quiz, practice, exam);
        const bool matches = isSearchMatch(m);

        if (acceptedRecord == 1) {
            sum = m;
            minimum = m;
            maximum = m;
            minimumRecord = acceptedRecord;
            maximumRecord = acceptedRecord;
        } else {
            sum += m;
            if (m < minimum) {
                minimum = m;
                minimumRecord = acceptedRecord;
            }
            if (m > maximum) {
                maximum = m;
                maximumRecord = acceptedRecord;
            }
        }

        if (matches) {
            ++matchCount;
            if (firstMatch == 0) {
                firstMatch = acceptedRecord;
            }
        }

        std::cout << "  m=" << m << " pts; category: "
                  << categoryFor(m) << "; search: "
                  << (matches ? "yes" : "no") << '\n';
    }

    const double average = static_cast<double>(sum) / recordCount;
    std::cout << "\nSUMMARY AFTER " << recordCount << " ACCEPTED RECORDS\n"
              << "Sum: " << sum << " pts\n"
              << "Average: " << average << " pts\n"
              << "Minimum: " << minimum << " pts (record " << minimumRecord << ")\n"
              << "Maximum: " << maximum << " pts (record " << maximumRecord << ")\n"
              << "Match count: " << matchCount << '\n'
              << "First match: ";
    if (firstMatch == 0) {
        std::cout << "none\n";
    } else {
        std::cout << firstMatch << '\n';
    }
    return 0;
}