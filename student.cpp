#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

// -------------------- Student Class --------------------
class Student {
    int id;
    string name;
    int age;
    string course;
    float marks;

public:
    Student() {}
    Student(int id, string name, int age, string course, float marks) {
        this->id = id;
        this->name = name;
        this->age = age;
        this->course = course;
        this->marks = marks;
    }

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getCourse() const { return course; }
    float getMarks() const { return marks; }

    // Setters
    void setName(string n) { name = n; }
    void setAge(int a) { age = a; }
    void setCourse(string c) { course = c; }
    void setMarks(float m) { marks = m; }

    void displayDetails() const {
        cout << "ID: " << id << " | Name: " << name 
             << " | Age: " << age << " | Course: " << course 
             << " | Marks: " << marks << endl;
    }
};

// -------------------- StudentManager Class --------------------
class StudentManager {
    map<int, Student> students; // Using map for fast lookup

public:
    void addStudent(Student s) {
        students[s.getId()] = s;
        cout << "✅ Student added successfully!\n";
    }

    void removeStudentById(int id) {
        if(students.erase(id))
            cout << "🗑️ Student removed successfully!\n";
        else
            cout << "❌ Student not found!\n";
    }
    void removeStudentByName(string name) {
    bool found = false;
    for(auto it = students.begin(); it != students.end(); ) {
        if(it->second.getName() == name) {
            it = students.erase(it); // erase returns next iterator
            found = true;
            cout << "🗑️ Student removed successfully!\n";
        } else {
            ++it;
        }
    }
    if(!found) cout << "❌ Student not found!\n";
}

    // Search by ID
void searchStudentById(int id) {
    if(students.find(id) != students.end()) {
        cout << "🔍 Student found:\n";
        students[id].displayDetails();
    } else {
        cout << "❌ Student not found!\n";
    }
}

// Search by Name
void searchStudentByName(string name) {
    bool found = false;
    for(auto &pair : students) {
        if(pair.second.getName() == name) {
            cout << "🔍 Student found:\n";
            pair.second.displayDetails();
            found = true;
        }
    }
    if(!found) cout << "❌ Student not found!\n";
}

// Search by Course (Branch)
void searchStudentByCourse(string course) {
    bool found = false;
    for(auto &pair : students) {
        if(pair.second.getCourse() == course) {
            cout << "🔍 Student found:\n";
            pair.second.displayDetails();
            found = true;
        }
    }
    if(!found) cout << "❌ Student not found!\n";
}

    void updateStudent(int id) {
        if(students.find(id) != students.end()) {
            string name, course;
            int age;
            float marks;
            cout << "Enter new name: "; cin >> name;
            cout << "Enter new age: "; cin >> age;
            cout << "Enter new course: "; cin >> course;
            cout << "Enter new marks: "; cin >> marks;

            students[id].setName(name);
            students[id].setAge(age);
            students[id].setCourse(course);
            students[id].setMarks(marks);

            cout << "✏️ Student updated successfully!\n";
        } else {
            cout << "❌ Student not found!\n";
        }
    }
    void updateStudentByName(string name) {
    bool found = false;
    for(auto &pair : students) {
        if(pair.second.getName() == name) {
            string newName, course;
            int age;
            float marks;
            cout << "Enter new name: "; cin >> newName;
            cout << "Enter new age: "; cin >> age;
            cout << "Enter new course: "; cin >> course;
            cout << "Enter new marks: "; cin >> marks;

            pair.second.setName(newName);
            pair.second.setAge(age);
            pair.second.setCourse(course);
            pair.second.setMarks(marks);

            cout << "✏️ Student updated successfully!\n";
            found = true;
        }
    }
    if(!found) cout << "❌ Student not found!\n";
}
    

    void displayAllStudents() {
        if(students.empty()) {
            cout << "⚠️ No students available!\n";
            return;
        }
        cout << "\n📋 All Students:\n";
        for(auto &pair : students) {
            pair.second.displayDetails();
        }
    }

    void sortStudentsByMarks() {
        if(students.empty()) {
            cout << "⚠️ No students available!\n";
            return;
        }
        vector<Student> v;
        for(auto &pair : students) v.push_back(pair.second);

        sort(v.begin(), v.end(), [](Student &a, Student &b) {
            return a.getMarks() > b.getMarks(); // Descending order
        });

        cout << "\n🏆 Students Sorted by Marks:\n";
        for(auto &s : v) s.displayDetails();
    }
};

// -------------------- Main Function --------------------
int main() {
    StudentManager sm;
    int choice;

    do {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Remove Student\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Display All Students\n";
        cout << "6. Sort Students by Marks\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: {
                int id, age;
                string name, course;
                float marks;
                cout << "Enter ID: "; cin >> id;
                cout << "Enter Name: "; cin >> name;
                cout << "Enter Age: "; cin >> age;
                cout << "Enter Course: "; cin >> course;
                cout << "Enter Marks: "; cin >> marks;
                sm.addStudent(Student(id, name, age, course, marks));
                break;
            }
              case 2: {
    int removeChoice;
    cout << "\nRemove By:\n";
    cout << "1. ID\n";
    cout << "2. Name\n";
    cout << "Enter choice: ";
    cin >> removeChoice;

    if(removeChoice == 1) {
        int id;
        cout << "Enter ID to remove: "; cin >> id;
        sm.removeStudentById(id);
    } else if(removeChoice == 2) {
        string name;
        cout << "Enter Name to remove: "; cin >> name;
        sm.removeStudentByName(name);
    } else {
        cout << "❌ Invalid choice!\n";
    }
    break;
}

              case 3: {
                 int searchChoice;
                 cout << "\nSearch By:\n";
                 cout << "1. ID\n";
                cout << "2. Name\n";
                 cout << "3. Course\n";
                cout << "Enter choice: ";
                 cin >> searchChoice;

    if(searchChoice == 1) {
        int id;
        cout << "Enter ID: "; cin >> id;
        sm.searchStudentById(id);
    } else if(searchChoice == 2) {
        string name;
        cout << "Enter Name: "; cin >> name;
        sm.searchStudentByName(name);
    } else if(searchChoice == 3) {
        string course;
        cout << "Enter Course: "; cin >> course;
        sm.searchStudentByCourse(course);
    } else {
        cout << "❌ Invalid choice!\n";
    }
    break;
}
            
            case 4: {
    int updateChoice;
    cout << "\nUpdate By:\n";
    cout << "1. ID\n";
    cout << "2. Name\n";
    cout << "Enter choice: ";
    cin >> updateChoice;

    if(updateChoice == 1) {
        int id;
        cout << "Enter ID to update: "; cin >> id;
        sm.updateStudent(id);
    } else if(updateChoice == 2) {
        string name;
        cout << "Enter Name to update: "; cin >> name;
        sm.updateStudentByName(name);
    } else {
        cout << "❌ Invalid choice!\n";
    }
    break;
}

            case 5:
                sm.displayAllStudents();
                break;
            case 6:
                sm.sortStudentsByMarks();
                break;
            case 7:
                cout << "👋 Exiting... Goodbye!\n";
                break;
            default:
                cout << "❌ Invalid choice!\n";
        }
    } while(choice != 7);

    return 0;
}
