#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

const int MAX_STUDENTS = 100;
const int MAX_COURSES = 10;

void splitString(const string &str, char delimiter, string tokens[], int &tokenCount) {
    string token;
    stringstream ss(str);
    tokenCount = 0;
    while (getline(ss, token, delimiter)) {
        tokens[tokenCount++] = token;
    }
}

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
    string getName() const { return name; }
    int getPrn() const { return prn; }

    static bool validateName(const string &name) {
        for (char c : name) {
            if (!isalpha(c) && c != ' ') {
                return false;
            }
        }
        return true;
    }
};

class Course {
public:
    string courseName;
    int totalHours;
    int attendedHours;
    int conductedHours;

    Course() : courseName(""), totalHours(0), attendedHours(0), conductedHours(0) {}

    Course(string name, int hours) : courseName(name), totalHours(hours), attendedHours(0), conductedHours(0) {}

    double getAttendancePercentage() const {
        if (conductedHours == 0) return 0;
        return (attendedHours / (double)conductedHours) * 100;
    }

    double getOverallAttendancePercentage() const {
        if (totalHours == 0) return 0;
        return (attendedHours / (double)totalHours) * 100;
    }

    void display() const {
        cout << "Course Name: " << courseName << ", Total Hours: " << totalHours << ", Attended Hours: " << attendedHours << ", Conducted Hours: " << conductedHours << endl;
    }
};

class Student : public Person {
public:
    Course courses[MAX_COURSES];
    int courseCount;

    Student() : Person(), courseCount(0) {}
    Student(string n, int p) : Person(n, p), courseCount(0) {}

    void addCourse(Course course) {
        if (courseCount < MAX_COURSES) {
            courses[courseCount++] = course;
        }
    }

    virtual void markAttendance(string courseName, bool present) {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i].courseName == courseName) {
                courses[i].conductedHours++;
                if (present) {
                    courses[i].attendedHours++;
                    if (courses[i].attendedHours > courses[i].conductedHours)
                        courses[i].attendedHours = courses[i].conductedHours;
                    cout << "Marked attendance for " << name << " in " << courseName << endl;
                } else {
                    cout << "Attendance not marked for " << name << " in " << courseName << endl;
                }
                return;
            }
        }
        cout << "Course not found." << endl;
    }

    void viewAttendance() const {
        cout << setw(15) << left << "Course Name" << setw(15) << "Attended" << setw(15) << "Total" << setw(15) << "Conducted" << setw(15) << "Current %" << setw(15) << "Overall %" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        for (int i = 0; i < courseCount; ++i) {
            cout << setw(15) << left << courses[i].courseName
                 << setw(15) << courses[i].attendedHours
                 << setw(15) << courses[i].totalHours
                 << setw(15) << courses[i].conductedHours
                 << setw(15) << fixed << setprecision(2) << courses[i].getAttendancePercentage() << "%"
                 << setw(15) << fixed << setprecision(2) << courses[i].getOverallAttendancePercentage() << "%" << endl;
        }
    }

    void deleteStudentInfo() {
        name = "";
        prn = 0;
        courseCount = 0;
    }
};

class Teacher : public Person {
public:
    Teacher(string n, int p) : Person(n, p) {}

    void display() override {
        cout << "Teacher: " << name << ", PRN: " << prn << endl;
    }

    void displayAttendanceForAll(const Student students[], int studentCount, const string courseNames[], int courseCount) const {
        for (int i = 0; i < courseCount; ++i) {
            cout << "\nAttendance for course: " << courseNames[i] << endl;
            cout << setw(15) << left << "Student Name" << setw(10) << "PRN" << setw(15) << "Attended" << setw(15) << "Total" << setw(15) << "Conducted" << setw(15) << "Current %" << setw(15) << "Overall %" << endl;
            cout << "----------------------------------------------------------------------------------------------------" << endl;
            for (int j = 0; j < studentCount; ++j) {
                for (int k = 0; k < students[j].courseCount; ++k) {
                    if (students[j].courses[k].courseName == courseNames[i]) {
                        const Course& course = students[j].courses[k];
                        cout << setw(15) << left << students[j].getName()
                             << setw(10) << students[j].getPrn()
                             << setw(15) << course.attendedHours
                             << setw(15) << course.totalHours
                             << setw(15) << course.conductedHours
                             << setw(15) << fixed << setprecision(2) << course.getAttendancePercentage()
                             << setw(15) << fixed << setprecision(2) << course.getOverallAttendancePercentage() << endl;
                    }
                }
            }
        }
    }
};

void writeCSV(const Student students[], int studentCount, const string courseNames[], int courseCount, const string &filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "Name,PRN";
        for (int i = 0; i < courseCount; ++i) {
            file << "," << courseNames[i] << "_TotalHours," << courseNames[i] << "_Attended," << courseNames[i] << "_Conducted," << courseNames[i] << "_Current%," << courseNames[i] << "_Overall%";
        }
        file << endl;

        for (int i = 0; i < studentCount; ++i) {
            file << students[i].getName() << "," << students[i].getPrn();
            for (int j = 0; j < courseCount; ++j) {
                bool courseFound = false;
                for (int k = 0; k < students[i].courseCount; ++k) {
                    if (students[i].courses[k].courseName == courseNames[j]) {
                        file << "," << students[i].courses[k].totalHours
                             << "," << students[i].courses[k].attendedHours
                             << "," << students[i].courses[k].conductedHours
                             << "," << fixed << setprecision(2) << students[i].courses[k].getAttendancePercentage()
                             << "," << fixed << setprecision(2) << students[i].courses[k].getOverallAttendancePercentage();
                        courseFound = true;
                        break;
                    }
                }
                if (!courseFound) {
                    file << ",0,0,0,0.00,0.00";
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

void readCSV(Student students[], int &studentCount, string courseNames[], int &courseCount, const string &filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        bool firstLine = true;
        studentCount = 0;
        while (getline(file, line)) {
            if (firstLine) {
                string headers[MAX_COURSES * 5];
                int headerCount;
                splitString(line, ',', headers, headerCount);
                courseCount = 0;
                for (int i = 2; i < headerCount; i += 5) {
                    courseNames[courseCount++] = headers[i].substr(0, headers[i].find("_TotalHours"));
                }
                firstLine = false;
            } else {
                string tokens[MAX_COURSES * 5 + 2];
                int tokenCount;
                splitString(line, ',', tokens, tokenCount);
                string studentName = tokens[0];
                int prn = stoi(tokens[1]);
                Student student(studentName, prn);

                for (int i = 2, j = 0; i < tokenCount; i += 5, ++j) {
                    int totalHours = stoi(tokens[i]);
                    int attendedHours = stoi(tokens[i + 1]);
                    int conductedHours = stoi(tokens[i + 2]);
                    Course course(courseNames[j], totalHours);
                    course.attendedHours = attendedHours;
                    course.conductedHours = conductedHours;
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

void markAttendance(Student students[], int studentCount, const string courseNames[], int courseCount) {
    cout << "Enter course name: ";
    string courseName;
    cin >> courseName;

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
        while (!(cin >> response) || (response != 'P' && response != 'A')) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 'P' for present or 'A' for absent: ";
        }
        students[i].markAttendance(courseName, response == 'P');
    }
}

void studentPortal(const Student students[], int studentCount) {
    cout << "Enter student PRN: ";
    int prn;
    while (!(cin >> prn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid PRN: ";
    }

    for (int i = 0; i < studentCount; ++i) {
        if (students[i].getPrn() == prn) {
            students[i].viewAttendance();
            return;
        }
    }
    cout << "Student not found." << endl;
}

void deleteStudent(Student students[], int &studentCount) {
    cout << "Enter student PRN to delete: ";
    int prn;
    while (!(cin >> prn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid PRN: ";
    }

    for (int i = 0; i < studentCount; ++i) {
        if (students[i].getPrn() == prn) {
            students[i].deleteStudentInfo();
            for (int j = i; j < studentCount - 1; ++j) {
                students[j] = students[j + 1];
            }
            studentCount--;
            cout << "Student information deleted." << endl;
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

    readCSV(students, studentCount, courseNames, courseCount, filename);

    if (studentCount == 0) {
        cout << "Enter number of students: ";
        int numStudents;
        while (!(cin >> numStudents) || numStudents > MAX_STUDENTS) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number of students (max " << MAX_STUDENTS << "): ";
        }

        for (int i = 0; i < numStudents; ++i) {
            string name;
            int prn;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter name of student " << (i + 1) << ": ";
            getline(cin, name);
            while (!Person::validateName(name)) {
                cout << "Invalid name. Please enter a name with alphabetic characters only: ";
                getline(cin, name);
            }
            cout << "Enter PRN of student " << (i + 1) << ": ";
            while (!(cin >> prn)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid PRN: ";
            }
            students[studentCount++] = Student(name, prn);
        }

        cout << "Enter number of courses: ";
        int numCourses;
        while (!(cin >> numCourses) || numCourses > MAX_COURSES) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number of courses (max " << MAX_COURSES << "): ";
        }

        for (int i = 0; i < numCourses; ++i) {
            string courseName;
            int credits;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter course name and credits for course " << (i + 1) << ": ";
            getline(cin, courseName);
            while (!(cin >> credits)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter valid credits: ";
            }
            courseNames[courseCount++] = courseName;
            for (int j = 0; j < studentCount; ++j) {
                students[j].addCourse(Course(courseName, credits * 15));
            }
        }

        writeCSV(students, studentCount, courseNames, courseCount, filename);
    }

    while (true) {
        cout << "\n1. Mark Attendance\n2. Student Portal\n3. Display Attendance for All Students\n4. Delete Student Information\n5. Exit\nChoose an option: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            markAttendance(students, studentCount, courseNames, courseCount);
            writeCSV(students, studentCount, courseNames, courseCount, filename);
        } else if (choice == 2) {
            studentPortal(students, studentCount);
        } else if (choice == 3) {
            Teacher teacher("Admin", 0);
            teacher.displayAttendanceForAll(students, studentCount, courseNames, courseCount);
        } else if (choice == 4) {
            deleteStudent(students, studentCount);
            writeCSV(students, studentCount, courseNames, courseCount, filename);
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}