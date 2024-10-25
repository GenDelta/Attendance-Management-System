#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>  // for formatting
#include <limits>   // for numeric_limits
using namespace std;

const int MAX_STUDENTS = 100;
const int MAX_COURSES = 10;

// Function to split a string
void splitString(const string &str, char delimiter, string tokens[], int &tokenCount) {
    string token;
    stringstream ss(str);
    tokenCount = 0;
    while (getline(ss, token, delimiter)) {
        tokens[tokenCount++] = token;
    }
}

// Base class for Person
class Person {
protected:
    string name;
    int prn;
public:
    Person() : name(""), prn(0) {}
    Person(string n, int p) : name(n), prn(p) {}
    virtual void display() {
        cout << "Name: " << name << ", PRN: " << prn << endl;
    }
    string getName() const { return name; } // Getter for name
    int getPrn() const { return prn; } // Getter for prn
};

// Course class
class Course {
public:
    string courseName;
    int totalHours;
    int attendedHours;

    Course() : courseName(""), totalHours(0), attendedHours(0) {} // Default constructor

    Course(string name, int hours) : courseName(name), totalHours(hours), attendedHours(0) {}

    double getAttendancePercentage() const { // Marked as const
        if (totalHours == 0) return 0;
        return (attendedHours / (double)totalHours) * 100;
    }

    void display() const { // Marked as const
        cout << "Course Name: " << courseName << ", Total Hours: " << totalHours << ", Attended Hours: " << attendedHours << endl;
    }
};

// Derived class for Student, inheriting from Person
class Student : public Person {
public:
    Course courses[MAX_COURSES];  // Courses the student is enrolled in
    int courseCount;

    Student() : Person(), courseCount(0) {}
    Student(string n, int p) : Person(n, p), courseCount(0) {}

    // Adding a course for the student
    void addCourse(Course course) {
        if (courseCount < MAX_COURSES) {
            courses[courseCount++] = course;
        }
    }

    // Overriding markAttendance (example of polymorphism)
    virtual void markAttendance(string courseName, bool present) {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i].courseName == courseName) {
                if (present) {
                    courses[i].attendedHours++;
                    if (courses[i].attendedHours > courses[i].totalHours)
                        courses[i].attendedHours = courses[i].totalHours;
                    cout << "Marked attendance for " << name << " in " << courseName << endl;
                } else {
                    cout << "Attendance not marked for " << name << " in " << courseName << endl;
                }
                return;
            }
        }
        cout << "Course not found." << endl;
    }

    // Viewing attendance in formatted form
    void viewAttendance() const { // Marked as const
        cout << setw(15) << left << "Course Name" << setw(15) << "Attended" << setw(15) << "Total" << setw(10) << "Percentage" << endl;
        cout << "-------------------------------------------------------------" << endl;
        for (int i = 0; i < courseCount; ++i) {
            cout << setw(15) << left << courses[i].courseName
                 << setw(15) << courses[i].attendedHours
                 << setw(15) << courses[i].totalHours
                 << setw(10) << fixed << setprecision(2) << courses[i].getAttendancePercentage() << "%" << endl;
        }
    }
};

// Derived class for Teacher, inheriting from Person
class Teacher : public Person {
public:
    Teacher(string n, int p) : Person(n, p) {}

    // Overriding display function for teachers
    void display() override {
        cout << "Teacher: " << name << ", PRN: " << prn << endl;
    }

    // Display attendance for all students in each course
    void displayAttendanceForAll(const Student students[], int studentCount, const string courseNames[], int courseCount) const {
        for (int i = 0; i < courseCount; ++i) {
            cout << "\nAttendance for course: " << courseNames[i] << endl;
            cout << setw(15) << left << "Student Name" << setw(10) << "PRN" << setw(15) << "Attended" << setw(15) << "Total" << setw(10) << "Percentage" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            for (int j = 0; j < studentCount; ++j) {
                for (int k = 0; k < students[j].courseCount; ++k) {
                    if (students[j].courses[k].courseName == courseNames[i]) {
                        const Course& course = students[j].courses[k];
                        cout << setw(15) << left << students[j].getName()
                             << setw(10) << students[j].getPrn()
                             << setw(15) << course.attendedHours
                             << setw(15) << course.totalHours
                             << setw(10) << fixed << setprecision(2) << course.getAttendancePercentage() << "%" << endl;
                    }
                }
            }
        }
    }
};

// Helper function to write data to CSV file
void writeCSV(const Student students[], int studentCount, const string courseNames[], int courseCount, const string &filename) {
    ofstream file(filename);
    if (file.is_open()) {
        // Write headers
        file << "Name,PRN";
        for (int i = 0; i < courseCount; ++i) {
            file << "," << courseNames[i] << "_TotalHours," << courseNames[i] << "_Attended," << courseNames[i] << "_Percentage";
        }
        file << endl;

        // Write student data
        for (int i = 0; i < studentCount; ++i) {
            file << students[i].getName() << "," << students[i].getPrn();
            for (int j = 0; j < courseCount; ++j) {
                bool courseFound = false;
                for (int k = 0; k < students[i].courseCount; ++k) {
                    if (students[i].courses[k].courseName == courseNames[j]) {
                        file << "," << students[i].courses[k].totalHours
                             << "," << students[i].courses[k].attendedHours
                             << "," << fixed << setprecision(2) << students[i].courses[k].getAttendancePercentage();
                        courseFound = true;
                        break;
                    }
                }
                if (!courseFound) {
                    file << ",0,0,0.00"; // Default values if course not found
                }
            }
            file << endl;
        }
        file.close();
        cout << "Data saved to " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

// Helper function to read data from CSV file
void readCSV(Student students[], int &studentCount, string courseNames[], int &courseCount, const string &filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        bool firstLine = true;
        studentCount = 0;
        while (getline(file, line)) {
            if (firstLine) {
                // Read course names from the first line (header)
                string headers[MAX_COURSES * 3];
                int headerCount;
                splitString(line, ',', headers, headerCount);
                courseCount = 0;
                for (int i = 2; i < headerCount; i += 3) {
                    courseNames[courseCount++] = headers[i].substr(0, headers[i].find("_TotalHours"));
                }
                firstLine = false;
            } else {
                // Read student data
                string tokens[MAX_COURSES * 3 + 2];
                int tokenCount;
                splitString(line, ',', tokens, tokenCount);
                string studentName = tokens[0];
                int prn = stoi(tokens[1]);
                Student student(studentName, prn);

                // Add courses with total and attended hours
                for (int i = 2, j = 0; i < tokenCount; i += 3, ++j) {
                    int totalHours = stoi(tokens[i]);
                    int attendedHours = stoi(tokens[i + 1]);
                    Course course(courseNames[j], totalHours);
                    course.attendedHours = attendedHours;
                    student.addCourse(course);
                }
                students[studentCount++] = student;
            }
        }
        file.close();
        cout << "Data loaded from " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

// Function for teacher to mark attendance
void markAttendance(Student students[], int studentCount, const string courseNames[], int courseCount) {
    cout << "Enter course name: ";
    string courseName;
    cin >> courseName;

    // Check if the course name is valid
    bool courseExists = false;
    for (int i = 0; i < courseCount; ++i) {
        if (courseNames[i] == courseName) {
            courseExists = true;
            break;
        }
    }

    if (!courseExists) {
        cout << "Course not found." << endl;
        return;
    }

    cout << "Marking attendance for course: " << courseName << endl;
    for (int i = 0; i < studentCount; ++i) {
        char response;
        cout << "Is " << students[i].getName() << " present for " << courseName << "? (Enter 'P' for present, 'A' for absent): ";
        while (!(cin >> response) || (response != 'P' && response != 'A')) { // Ensure valid input
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input. Please enter 'P' for present or 'A' for absent: ";
        }
        students[i].markAttendance(courseName, response == 'P');
    }
}

// Student portal for viewing attendance
void studentPortal(const Student students[], int studentCount) {
    cout << "Enter student PRN: ";
    int prn;
    while (!(cin >> prn)) { // Ensure valid input
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        cout << "Invalid input. Please enter a valid PRN: ";
    }

    for (int i = 0; i < studentCount; ++i) {
        if (students[i].getPrn() == prn) { // Use getter method
            students[i].viewAttendance(); // Call const method
            return;
        }
    }
    cout << "Student not found." << endl;
}

int main() {
    Student students[MAX_STUDENTS];
    string courseNames[MAX_COURSES];
    int studentCount = 0;
    int courseCount = 0;
    string filename = "Attendance_File.csv";

    // Load previous data from CSV
    readCSV(students, studentCount, courseNames, courseCount, filename);

    // If no previous data, setup students and courses
    if (studentCount == 0) {
        cout << "Enter number of students: ";
        int numStudents;
        while (!(cin >> numStudents) || numStudents > MAX_STUDENTS) { // Ensure valid input
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input. Please enter a valid number of students (max " << MAX_STUDENTS << "): ";
        }

        for (int i = 0; i < numStudents; ++i) {
            string name;
            int prn;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            cout << "Enter name of student " << (i + 1) << ": ";
            getline(cin, name);
            cout << "Enter PRN of student " << (i + 1) << ": ";
            while (!(cin >> prn)) { // Ensure valid input
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter a valid PRN: ";
            }
            students[studentCount++] = Student(name, prn);
        }

        cout << "Enter number of courses: ";
        int numCourses;
        while (!(cin >> numCourses) || numCourses > MAX_COURSES) { // Ensure valid input
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input. Please enter a valid number of courses (max " << MAX_COURSES << "): ";
        }

        for (int i = 0; i < numCourses; ++i) {
            string courseName;
            int credits;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            cout << "Enter course name and credits for course " << (i + 1) << ": ";
            getline(cin, courseName);
            while (!(cin >> credits)) { // Ensure valid input
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter valid credits: ";
            }
            courseNames[courseCount++] = courseName;
            for (int j = 0; j < studentCount; ++j) {
                students[j].addCourse(Course(courseName, credits * 15)); 
            }
        }

        // Save initial data to CSV
        writeCSV(students, studentCount, courseNames, courseCount, filename);
    }

    while (true) {
        cout << "\n1. Mark Attendance\n2. Student Portal\n3. Display Attendance for All Students\n4. Exit\nChoose an option: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            markAttendance(students, studentCount, courseNames, courseCount);
            writeCSV(students, studentCount, courseNames, courseCount, filename); // Save data after marking attendance
        } else if (choice == 2) {
            studentPortal(students, studentCount);
        } else if (choice == 3) {
            Teacher teacher("Admin", 0);
            teacher.displayAttendanceForAll(students, studentCount, courseNames, courseCount);
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}