## Attendance Management System in CPP

This project implements a student attendance management system using object-oriented programming concepts like inheritance, polymorphism and virtual functions in C++. It allows teachers to mark attendance, students to view their attendance records, and persists data to a CSV file.

**Contributors**

* Aayush Joshi
* Ananya Poundrik
* Ankush Dutta
* Anushka Desai

**Features**

* Manage students and their details (name, PRN)
* Manage courses and their details (name, total hours)
* Enroll students in courses
* Mark attendance for students in a specific course (present/absent)
* View individual student attendance records
* Display attendance information for all students in a course (Teacher view)
* Save attendance data to a CSV file
* Load attendance data from a CSV file

**Getting Started**

1. **Prerequisites:**
    * A C++ compiler (e.g., g++ on Linux/macOS/Windows)
    * A basic understanding of C++ and object-oriented programming concepts

2. **Compiling and Running:**
    * Clone this repository or download the project files.
    * Open a terminal in the project directory.
    * Compile the code using a C++ compiler:
        ```
        g++ Attendance_System.cpp
        ```
    * Run the compiled program:
        ```
        ./a.exe
        ```

**Using the System**

The program presents a menu with options for teachers and students.

* **Marking Attendance (Teacher)**
    1. Choose option 1 ("Mark Attendance").
    2. Enter the course name for which you want to mark attendance.
    3. The system will prompt for each student's attendance (present/absent).

* **Viewing Attendance (Student)**
    1. Choose option 2 ("Student Portal").
    2. Enter your student PRN number.
    3. The system will display your attendance records for all courses.

* **Viewing Attendance for All Students (Teacher)**
    1. Choose option 3 ("Display Attendance for All Students").
    2. The system will display a table showing attendance information for all students in each course.

* **Saving and Loading Data**
    * The system automatically saves attendance data to a file named "attendance_1.csv" after marking attendance.
    * Upon startup, the system attempts to load data from the same file.

**Explanation of Code**

The code is well-structured and utilizes object-oriented programming principles. Key classes include:

* `Person`: Base class for students and teachers, storing name and PRN.
* `Course`: Represents a course with name, total hours, and attended hours.
* `Student`: Inherits from `Person`, contains enrolled courses, and methods for adding courses, marking attendance, and viewing attendance.
* `Teacher`: Inherits from `Person`, with methods to mark attendance for all students in a course and display attendance for the entire class.

**Additional Notes**

* The code incorporates error handling and validation for user input.
* File I/O functions (`writeCSV` and `readCSV`) manage data persistence with a CSV file.
* The provided code serves as a basic framework and can be extended with additional features like:
    * Support for different attendance marking options (e.g., late, excused absence)
    * Reports generation for overall attendance statistics
    * Graphical user interface (GUI) for a more interactive experience

We hope this project provides a valuable tool for managing student attendance.