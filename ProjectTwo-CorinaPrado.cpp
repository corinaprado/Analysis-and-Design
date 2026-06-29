#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure used to store course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Hash table class for storing courses
class HashTable {
private:
    static const int TABLE_SIZE = 101;

    // Separate chaining used to handle collisions
    vector<Course> table[TABLE_SIZE];

    // Generate hash value from course number
    int hashFunction(string key) {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % TABLE_SIZE;
    }

public:
    // Insert a course into the hash table
    void insertCourse(Course course) {
        int index = hashFunction(course.courseNumber);
        table[index].push_back(course);
    }

    // Search for a course by course number
    Course* searchCourse(string courseNumber) {
        int index = hashFunction(courseNumber);

        for (Course& course : table[index]) {
            if (course.courseNumber == courseNumber) {
                return &course;
            }
        }
        return nullptr;
    }

    // Return all courses from the hash table
    vector<Course> getAllCourses() {
        vector<Course> courses;
        
        //Collect courses from every bucket
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (Course course : table[i]) {
                courses.push_back(course);
            }
        }
        
        //Sort courses alphanumerically by course number
        sort(courses.begin(), courses.end(), [](Course a, Course b) {
            return a.courseNumber < b.courseNumber;
        });

        return courses;
    }

    //Remove all entries from the hash table
    void clearTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i].clear();
        }
    }
};

//Load course data from CSV file
void loadCourses(string fileName, HashTable& courseTable) {
    ifstream file(fileName);
    //Verify file opened successfully
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    //Clear any existing data before loading new data
    courseTable.clearTable();

    string line;

    //Read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> fields;

        //Split CSV fields by comma
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                fields.push_back(token);
            }
        }

        // Ensure valid course record
        if (fields.size() >= 2) {
            Course course;
            // First field is course number
            course.courseNumber = fields[0];

            // Second field is course title
            course.courseTitle = fields[1];

            // Remaining fields are prerequisites
            for (int i = 2; i < fields.size(); i++) {
                course.prerequisites.push_back(fields[i]);
            }

            // Insert course into hash table
            courseTable.insertCourse(course);
        }
    }

    file.close();
    cout << "Course data loaded successfully." << endl;
}

// Insert course into hash table
void printCourseList(HashTable& courseTable) {
    vector<Course> courses = courseTable.getAllCourses();

    cout << endl;
    cout << "Here is a sample schedule:" << endl;

    for (Course course : courses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

// Display detailed information for one course
void printCourseInformation(HashTable& courseTable) {
    string courseNumber;

    cout << "What course do you want to know about? Enter the course code. ";
    cin >> courseNumber;

    // Convert input to uppercase for consistency
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    // Search for requested course
    Course* course = courseTable.searchCourse(courseNumber);

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;
    cout << "Prerequisites: ";

    // Check if course has prerequisites
    if (course->prerequisites.empty()) {
        cout << "None";
    } else {
        // Display prerequisite information
        for (int i = 0; i < course->prerequisites.size(); i++) {
            Course* prereq = courseTable.searchCourse(course->prerequisites[i]);

            if (prereq != nullptr) {
                cout << prereq->courseNumber << " - " << prereq->courseTitle;
            } else {
                cout << course->prerequisites[i];
            }

            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

int main() {
    HashTable courseTable;
    string fileName;
    int choice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    // Continue displaying menu until user exits
    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        cin >> choice;

         // Validate numeric menu input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter the file name: ";
            cin.ignore();
            getline(cin, fileName);
            loadCourses(fileName, courseTable);
            dataLoaded = true;
            break;

        case 2:
            // Ensure data is loaded first
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            } else {
                printCourseList(courseTable);
            }
            break;

        case 3:
            // Ensure data is loaded first
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            } else {
                printCourseInformation(courseTable);
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            // Handle invalid menu choices
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}