#include <iostream>

using namespace std;
int main() 
    {
        int quiz;
        int practice;
        int exam;

        std::cout << "Enter your quiz score: ";
        std::cin >> quiz;
            if (quiz < 0 || quiz > 20) {
            std::cout << "Invalid quiz score. Please enter a score between 0 and 20." << std::endl;
            return 1;
        }
        std::cout << "Enter your practice score: ";
        std::cin >> practice;
            if (practice < 0 || practice > 30) {
            std::cout << "Invalid practice score. Please enter a score between 0 and 30." << std::endl;
            return 1;
        }
        std::cout << "Enter your exam score: ";
        std::cin >> exam;
            if (exam < 0 || exam > 50) {
            std::cout << "Invalid exam score. Please enter a score between 0 and 50." << std::endl;
            return 1;
      }
    int total = quiz + practice + exam;
    std::cout << "total=" << total << std::endl;
    if (total < 50) {
        std::cout << "needs additional work" << std::endl;
    } else if (total <= 69) {
        std::cout << "average score" << std::endl;
    } else if (total <= 84) {
        std::cout << "good score" << std::endl;
    } else {
        std::cout << "perfect score" << std::endl;
    }
    return 0;
}   