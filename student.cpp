#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;
// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================
class Student;
class Academic;
class Fee;
class User;
class Admin;
// ============================================================================
// CONSTANTS AND ENUMS
// ============================================================================
enum Role { ADMIN = 1, STAFF = 2, STUDENT = 3 };
enum GradePoint { A_PLUS = 10, A = 9, B_PLUS = 8, B = 7, C_PLUS = 6, C = 5, D = 4, F = 0 };
const string DATA_FILE = "students.dat";
const string USER_FILE = "users.dat";
const string BACKUP_DIR = "backups/";
// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================
// Clear input buffer
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
// Get current timestamp
string getTimestamp() {
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}
// Validate email format
bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    return atPos != string::npos && dotPos != string::npos && atPos < dotPos;
}
// Validate phone number
bool isValidPhone(const string& phone) {
    if(phone.length() != 10) return false;
    for(char c : phone) {
        if(!isdigit(c)) return false;
    }
    return true;
}
// Convert grade to grade point
float gradeToPoint(const string& grade) {
    if(grade == "A+") return 10.0;
    if(grade == "A") return 9.0;
    if(grade == "B+") return 8.0;
    if(grade == "B") return 7.0;
    if(grade == "C+") return 6.0;
    if(grade == "C") return 5.0;
    if(grade == "D") return 4.0;
    return 0.0; // F grade
}
// Display separator line
void displayLine(char c = '=', int length = 60) {
    cout << string(length, c) << endl;
}
// Display header
void displayHeader(const string& title) {
    displayLine();
    cout << "  " << title << endl;
    displayLine();
}
// ============================================================================
// CLASS: Grade - Represents a subject grade
// ============================================================================
class Grade {
private:
    string subjectCode;
    string subjectName;
    int credits;
    string grade;
    float gradePoint;
    int semester;
public:
    // Constructor
    Grade(string code = "", string name = "", int cred = 0, 
          string gr = "F", int sem = 1) {
        subjectCode = code;
        subjectName = name;
        credits = cred;
        grade = gr;
        gradePoint = gradeToPoint(gr);
        semester = sem;
    }
    // Getters
    string getSubjectCode() const { return subjectCode; }
    string getSubjectName() const { return subjectName; }
    int getCredits() const { return credits; }
    string getGrade() const { return grade; }
    float getGradePoint() const { return gradePoint; }
    int getSemester() const { return semester; }
    // Setters
    void setGrade(const string& gr) {
        grade = gr;
        gradePoint = gradeToPoint(gr);
    }
    // Display grade info
    void display() const {
        cout << left << setw(10) << subjectCode 
             << setw(25) << subjectName
             << setw(8) << credits
             << setw(8) << grade
             << setw(8) << fixed << setprecision(1) << gradePoint
             << endl;
    }
    // File I/O
    void writeToFile(ofstream& out) const {
        size_t len;
        len = subjectCode.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(subjectCode.c_str(), len);
        
        len = subjectName.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(subjectName.c_str(), len);
        
        out.write(reinterpret_cast<const char*>(&credits), sizeof(credits));
        
        len = grade.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(grade.c_str(), len);
        
        out.write(reinterpret_cast<const char*>(&gradePoint), sizeof(gradePoint));
        out.write(reinterpret_cast<const char*>(&semester), sizeof(semester));
    }
    void readFromFile(ifstream& in) {
        size_t len;
        char* buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        subjectCode = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        subjectName = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&credits), sizeof(credits));
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        grade = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&gradePoint), sizeof(gradePoint));
        in.read(reinterpret_cast<char*>(&semester), sizeof(semester));
    }
};
// ============================================================================
// CLASS: Payment - Represents a fee payment
// ============================================================================
class Payment {
private:
    int paymentId;
    float amount;
    string date;
    string method; // Cash, Card, Online
public:
    // Constructor
    Payment(int id = 0, float amt = 0, string dt = "", string mtd = "Cash") {
        paymentId = id;
        amount = amt;
        date = dt.empty() ? getTimestamp() : dt;
        method = mtd;
    }
    // Getters
    int getPaymentId() const { return paymentId; }
    float getAmount() const { return amount; }
    string getDate() const { return date; }
    string getMethod() const { return method; }
    // Display payment
    void display() const {
        cout << left << setw(10) << paymentId
             << setw(15) << fixed << setprecision(2) << amount
             << setw(25) << date
             << setw(10) << method
             << endl;
    }
    // File I/O
    void writeToFile(ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&paymentId), sizeof(paymentId));
        out.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
        
        size_t len = date.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(date.c_str(), len);
        
        len = method.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(method.c_str(), len);
    }
    void readFromFile(ifstream& in) {
        in.read(reinterpret_cast<char*>(&paymentId), sizeof(paymentId));
        in.read(reinterpret_cast<char*>(&amount), sizeof(amount));
        
        size_t len;
        char* buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        date = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        method = buffer;
        delete[] buffer;
    }
};
// ============================================================================
// CLASS: Fee - Manages student fees
// ============================================================================
class Fee {
private:
    int studentId;
    float totalAmount;
    float paidAmount;
    string dueDate;
    vector<Payment> payments;
    static int nextPaymentId;
public:
    // Constructor
    Fee(int sId = 0, float total = 0, string due = "") {
        studentId = sId;
        totalAmount = total;
        paidAmount = 0;
        dueDate = due;
    }
    // Getters
    int getStudentId() const { return studentId; }
    float getTotalAmount() const { return totalAmount; }
    float getPaidAmount() const { return paidAmount; }
    float getBalance() const { return totalAmount - paidAmount; }
    string getDueDate() const { return dueDate; }
    string getStatus() const {
        if(paidAmount >= totalAmount) return "PAID";
        if(paidAmount > 0) return "PARTIAL";
        return "PENDING";
    }
    // Setters
    void setTotalAmount(float amt) { totalAmount = amt; }
    void setDueDate(const string& date) { dueDate = date; }
    // Make a payment
    bool makePayment(float amount, const string& method) {
        if(amount <= 0 || amount > getBalance()) {
            cout << "Invalid payment amount!" << endl;
            return false;
        }
        
        Payment p(nextPaymentId++, amount, getTimestamp(), method);
        payments.push_back(p);
        paidAmount += amount;
        
        cout << "Payment of Rs. " << fixed << setprecision(2) << amount 
             << " recorded successfully!" << endl;
        return true;
    }
    // Display fee summary
    void displaySummary() const {
        displayHeader("FEE SUMMARY");
        cout << "Total Amount   : Rs. " << fixed << setprecision(2) << totalAmount << endl;
        cout << "Paid Amount    : Rs. " << paidAmount << endl;
        cout << "Balance        : Rs. " << getBalance() << endl;
        cout << "Due Date       : " << dueDate << endl;
        cout << "Status         : " << getStatus() << endl;
        displayLine('-');
    }
    // Display payment history
    void displayPaymentHistory() const {
        displayHeader("PAYMENT HISTORY");
        if(payments.empty()) {
            cout << "No payments recorded yet." << endl;
            return;
        }
        
        cout << left << setw(10) << "ID" 
             << setw(15) << "Amount"
             << setw(25) << "Date"
             << setw(10) << "Method" << endl;
        displayLine('-');
        
        for(const auto& p : payments) {
            p.display();
        }
    }
    // File I/O
    void writeToFile(ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&studentId), sizeof(studentId));
        out.write(reinterpret_cast<const char*>(&totalAmount), sizeof(totalAmount));
        out.write(reinterpret_cast<const char*>(&paidAmount), sizeof(paidAmount));
        
        size_t len = dueDate.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(dueDate.c_str(), len);
        
        size_t paymentCount = payments.size();
        out.write(reinterpret_cast<const char*>(&paymentCount), sizeof(paymentCount));
        for(const auto& p : payments) {
            p.writeToFile(out);
        }
    }
    void readFromFile(ifstream& in) {
        in.read(reinterpret_cast<char*>(&studentId), sizeof(studentId));
        in.read(reinterpret_cast<char*>(&totalAmount), sizeof(totalAmount));
        in.read(reinterpret_cast<char*>(&paidAmount), sizeof(paidAmount));
        
        size_t len;
        char* buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        dueDate = buffer;
        delete[] buffer;
        
        size_t paymentCount;
        in.read(reinterpret_cast<char*>(&paymentCount), sizeof(paymentCount));
        payments.clear();
        for(size_t i = 0; i < paymentCount; i++) {
            Payment p;
            p.readFromFile(in);
            payments.push_back(p);
            if(p.getPaymentId() >= nextPaymentId) {
                nextPaymentId = p.getPaymentId() + 1;
            }
        }
    }
};
int Fee::nextPaymentId = 1;
// ============================================================================
// CLASS: Academic - Manages academic records
// ============================================================================
class Academic {
private:
    int studentId;
    vector<Grade> grades;
    int currentSemester;
public:
    // Constructor
    Academic(int sId = 0) {
        studentId = sId;
        currentSemester = 1;
    }
    // Getters
    int getStudentId() const { return studentId; }
    int getCurrentSemester() const { return currentSemester; }
    const vector<Grade>& getGrades() const { return grades; }
    // Setters
    void setCurrentSemester(int sem) { currentSemester = sem; }
    // Add a grade
    void addGrade(const Grade& g) {
        grades.push_back(g);
    }
    // Calculate GPA for a specific semester
    float calculateSemesterGPA(int semester) const {
        float totalPoints = 0;
        int totalCredits = 0;
        
        for(const auto& g : grades) {
            if(g.getSemester() == semester) {
                totalPoints += g.getGradePoint() * g.getCredits();
                totalCredits += g.getCredits();
            }
        }
        
        return totalCredits > 0 ? totalPoints / totalCredits : 0.0;
    }
    // Calculate CGPA (Cumulative GPA)
    float calculateCGPA() const {
        float totalPoints = 0;
        int totalCredits = 0;
        
        for(const auto& g : grades) {
            totalPoints += g.getGradePoint() * g.getCredits();
            totalCredits += g.getCredits();
        }
        
        return totalCredits > 0 ? totalPoints / totalCredits : 0.0;
    }
    // Get total credits earned
    int getTotalCredits() const {
        int total = 0;
        for(const auto& g : grades) {
            if(g.getGradePoint() >= 4.0) { // Passing grade
                total += g.getCredits();
            }
        }
        return total;
    }
    // Display grades for a semester
    void displaySemesterGrades(int semester) const {
        cout << "\n--- Semester " << semester << " Grades ---" << endl;
        cout << left << setw(10) << "Code" 
             << setw(25) << "Subject"
             << setw(8) << "Credits"
             << setw(8) << "Grade"
             << setw(8) << "Points" << endl;
        displayLine('-');
        
        bool found = false;
        for(const auto& g : grades) {
            if(g.getSemester() == semester) {
                g.display();
                found = true;
            }
        }
        
        if(!found) {
            cout << "No grades recorded for this semester." << endl;
        } else {
            displayLine('-');
            cout << "Semester GPA: " << fixed << setprecision(2) 
                 << calculateSemesterGPA(semester) << endl;
        }
    }
    // Display all academic records
    void displayAllRecords() const {
        displayHeader("ACADEMIC TRANSCRIPT");
        
        // Find max semester
        int maxSem = 0;
        for(const auto& g : grades) {
            if(g.getSemester() > maxSem) maxSem = g.getSemester();
        }
        
        for(int sem = 1; sem <= maxSem; sem++) {
            displaySemesterGrades(sem);
        }
        
        displayLine('=');
        cout << "\nCGPA: " << fixed << setprecision(2) << calculateCGPA() << endl;
        cout << "Total Credits Earned: " << getTotalCredits() << endl;
    }
    // File I/O
    void writeToFile(ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&studentId), sizeof(studentId));
        out.write(reinterpret_cast<const char*>(&currentSemester), sizeof(currentSemester));
        
        size_t gradeCount = grades.size();
        out.write(reinterpret_cast<const char*>(&gradeCount), sizeof(gradeCount));
        for(const auto& g : grades) {
            g.writeToFile(out);
        }
    }
    void readFromFile(ifstream& in) {
        in.read(reinterpret_cast<char*>(&studentId), sizeof(studentId));
        in.read(reinterpret_cast<char*>(&currentSemester), sizeof(currentSemester));
        
        size_t gradeCount;
        in.read(reinterpret_cast<char*>(&gradeCount), sizeof(gradeCount));
        grades.clear();
        for(size_t i = 0; i < gradeCount; i++) {
            Grade g;
            g.readFromFile(in);
            grades.push_back(g);
        }
    }
};
// ============================================================================
// CLASS: Student - Core student entity
// ============================================================================
class Student {
private:
    int studentId;
    string name;
    string email;
    string phone;
    string address;
    string dateOfBirth;
    char gender;
    string enrollmentDate;
    string department;
    Academic academic;
    Fee fee;
    static int nextId;
public:
    // Default Constructor
    Student() {
        studentId = 0;
        name = "";
        email = "";
        phone = "";
        address = "";
        dateOfBirth = "";
        gender = 'M';
        enrollmentDate = "";
        department = "";
    }
    // Parameterized Constructor
    Student(string n, string e, string p, string addr, 
            string dob, char g, string dept) {
        studentId = nextId++;
        name = n;
        email = e;
        phone = p;
        address = addr;
        dateOfBirth = dob;
        gender = g;
        enrollmentDate = getTimestamp();
        department = dept;
        academic = Academic(studentId);
        fee = Fee(studentId, 50000.0, "2024-12-31"); // Default fee
    }
    // Getters
    int getStudentId() const { return studentId; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }
    string getDateOfBirth() const { return dateOfBirth; }
    char getGender() const { return gender; }
    string getEnrollmentDate() const { return enrollmentDate; }
    string getDepartment() const { return department; }
    Academic& getAcademic() { return academic; }
    Fee& getFee() { return fee; }
    const Academic& getAcademic() const { return academic; }
    const Fee& getFee() const { return fee; }
    // Setters with validation
    bool setName(const string& n) {
        if(n.empty() || n.length() > 100) return false;
        name = n;
        return true;
    }
    bool setEmail(const string& e) {
        if(!isValidEmail(e)) return false;
        email = e;
        return true;
    }
    bool setPhone(const string& p) {
        if(!isValidPhone(p)) return false;
        phone = p;
        return true;
    }
    void setAddress(const string& addr) { address = addr; }
    void setDepartment(const string& dept) { department = dept; }
    // Display student info
    void displayBasicInfo() const {
        cout << left << setw(8) << studentId
             << setw(25) << name
             << setw(15) << department
             << setw(15) << phone
             << setw(25) << email
             << endl;
    }
    void displayFullInfo() const {
        displayHeader("STUDENT DETAILS");
        cout << "Student ID     : " << studentId << endl;
        cout << "Name           : " << name << endl;
        cout << "Email          : " << email << endl;
        cout << "Phone          : " << phone << endl;
        cout << "Address        : " << address << endl;
        cout << "Date of Birth  : " << dateOfBirth << endl;
        cout << "Gender         : " << (gender == 'M' ? "Male" : gender == 'F' ? "Female" : "Other") << endl;
        cout << "Department     : " << department << endl;
        cout << "Enrolled On    : " << enrollmentDate << endl;
        cout << "Current CGPA   : " << fixed << setprecision(2) << academic.calculateCGPA() << endl;
        cout << "Fee Status     : " << fee.getStatus() << endl;
        displayLine();
    }
    // Generate student report
    void generateReport() const {
        displayLine('*');
        cout << "           STUDENT PERFORMANCE REPORT" << endl;
        displayLine('*');
        
        cout << "\nStudent: " << name << " (ID: " << studentId << ")" << endl;
        cout << "Department: " << department << endl;
        cout << "Generated: " << getTimestamp() << endl;
        
        academic.displayAllRecords();
        
        cout << "\n";
        fee.displaySummary();
        
        displayLine('*');
    }
    // File I/O
    void writeToFile(ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&studentId), sizeof(studentId));
        
        size_t len;
        len = name.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(name.c_str(), len);
        
        len = email.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(email.c_str(), len);
        
        len = phone.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(phone.c_str(), len);
        
        len = address.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(address.c_str(), len);
        
        len = dateOfBirth.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(dateOfBirth.c_str(), len);
        
        out.write(reinterpret_cast<const char*>(&gender), sizeof(gender));
        
        len = enrollmentDate.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(enrollmentDate.c_str(), len);
        
        len = department.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(department.c_str(), len);
        
        academic.writeToFile(out);
        fee.writeToFile(out);
    }
    void readFromFile(ifstream& in) {
        in.read(reinterpret_cast<char*>(&studentId), sizeof(studentId));
        if(studentId >= nextId) nextId = studentId + 1;
        
        size_t len;
        char* buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        name = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        email = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        phone = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        address = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        dateOfBirth = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&gender), sizeof(gender));
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        enrollmentDate = buffer;
        delete[] buffer;
        
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        in.read(buffer, len);
        buffer[len] = '\0';
        department = buffer;
        delete[] buffer;
        
        academic.readFromFile(in);
        fee.readFromFile(in);
    }
};
int Student::nextId = 1001;
// ============================================================================
// CLASS: User - System user for authentication
// ============================================================================
class User {
private:
    string username;
    string password;
    Role role;
    string createdAt;
public:
    User(string uname = "", string pwd = "", Role r = STUDENT) {
        username = uname;
        password = pwd;
        role = r;
        createdAt = getTimestamp();
    }
    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    Role getRole() const { return role; }
    string getRoleName() const {
        switch(role) {
            case ADMIN: return "Administrator";
            case STAFF: return "Staff";
            case STUDENT: return "Student";
            default: return "Unknown";
        }
    }
    // Validate password
    bool validatePassword(const string& pwd) const {
        return password == pwd;
    }
    // Display user info
    void display() const {
        cout << left << setw(20) << username
             << setw(15) << getRoleName()
             << setw(25) << createdAt
             << endl;
    }
};
// ============================================================================
// CLASS: StudentManagementSystem - Main system controller
// ============================================================================
class StudentManagementSystem {
private:
    vector<Student> students;
    vector<User> users;
    User* currentUser;
    bool isRunning;
    // Initialize default users
    void initializeUsers() {
        users.push_back(User("admin", "admin123", ADMIN));
        users.push_back(User("staff", "staff123", STAFF));
        users.push_back(User("student", "student123", STUDENT));
    }
    // Load data from file
    void loadData() {
        ifstream file(DATA_FILE, ios::binary);
        if(!file) {
            cout << "No existing data found. Starting fresh." << endl;
            return;
        }
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        students.clear();
        for(size_t i = 0; i < count; i++) {
            Student s;
            s.readFromFile(file);
            students.push_back(s);
        }
        file.close();
        cout << "Loaded " << students.size() << " student records." << endl;
    }
    // Save data to file
    void saveData() {
        ofstream file(DATA_FILE, ios::binary | ios::trunc);
        if(!file) {
            cout << "Error: Could not save data!" << endl;
            return;
        }
        size_t count = students.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        for(const auto& s : students) {
            s.writeToFile(file);
        }
        file.close();
        cout << "Data saved successfully." << endl;
    }
    // Find student by ID
    Student* findStudent(int id) {
        for(auto& s : students) {
            if(s.getStudentId() == id) return &s;
        }
        return nullptr;
    }
    // Check access permission
    bool hasAccess(const string& action) {
        if(!currentUser) return false;
        
        Role role = currentUser->getRole();
        
        if(role == ADMIN) return true; // Admin has full access
        
        if(role == STAFF) {
            // Staff can't delete users or access system settings
            if(action == "DELETE_USER" || action == "SYSTEM_SETTINGS") 
                return false;
            return true;
        }
        
        if(role == STUDENT) {
            // Students can only view their own data
            if(action == "VIEW_OWN" || action == "VIEW_GRADES" || action == "VIEW_FEE")
                return true;
            return false;
        }
        
        return false;
    }
public:
    // Constructor
    StudentManagementSystem() {
        currentUser = nullptr;
        isRunning = true;
        initializeUsers();
    }
    // Destructor
    ~StudentManagementSystem() {
        saveData();
        cout << "System shutdown complete. Goodbye!" << endl;
    }
    // Authentication
    bool login() {
        displayLine('*');
        cout << "       ACADEMIA CONNECT" << endl;
        cout << "   Student Management System" << endl;
        displayLine('*');
        
        string username, password;
        int roleChoice;
        
        cout << "\nUsername: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        for(auto& u : users) {
            if(u.getUsername() == username && u.validatePassword(password)) {
                currentUser = &u;
                cout << "\nLogin successful! Welcome, " << username << endl;
                cout << "Role: " << u.getRoleName() << endl;
                return true;
            }
        }
        
        cout << "\nInvalid credentials!" << endl;
        return false;
    }
    // Display main menu
    void displayMainMenu() {
        displayLine();
        cout << "           MAIN MENU" << endl;
        displayLine();
        cout << "  1. Student Management" << endl;
        cout << "  2. Academic Tracking" << endl;
        cout << "  3. Fee Management" << endl;
        cout << "  4. Reports & Analytics" << endl;
        cout << "  5. Settings & System" << endl;
        cout << "  6. Logout" << endl;
        cout << "  0. Exit System" << endl;
        displayLine();
        cout << "Enter choice: ";
    }
    // Student Management Menu
    void studentManagementMenu() {
        int choice;
        do {
            displayHeader("STUDENT MANAGEMENT");
            cout << "  1. Add New Student" << endl;
            cout << "  2. View All Students" << endl;
            cout << "  3. Search Student" << endl;
            cout << "  4. Update Student" << endl;
            cout << "  5. Delete Student" << endl;
            cout << "  0. Back to Main Menu" << endl;
            displayLine();
            cout << "Enter choice: ";
            cin >> choice;
            clearInput();
            switch(choice) {
                case 1: addStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: searchStudent(); break;
                case 4: updateStudent(); break;
                case 5: deleteStudent(); break;
                case 0: break;
                default: cout << "Invalid choice!" << endl;
            }
        } while(choice != 0);
    }
    // Add new student
    void addStudent() {
        if(!hasAccess("ADD_STUDENT")) {
            cout << "Access denied!" << endl;
            return;
        }
        displayHeader("ADD NEW STUDENT");
        
        string name, email, phone, address, dob, dept;
        char gender;
        
        cout << "Enter Name: ";
        getline(cin, name);
        
        cout << "Enter Email: ";
        getline(cin, email);
        if(!isValidEmail(email)) {
            cout << "Invalid email format!" << endl;
            return;
        }
        
        cout << "Enter Phone (10 digits): ";
        getline(cin, phone);
        if(!isValidPhone(phone)) {
            cout << "Invalid phone number!" << endl;
            return;
        }
        
        cout << "Enter Address: ";
        getline(cin, address);
        
        cout << "Enter Date of Birth (YYYY-MM-DD): ";
        getline(cin, dob);
        
        cout << "Enter Gender (M/F/O): ";
        cin >> gender;
        clearInput();
        
        cout << "Enter Department: ";
        getline(cin, dept);
        
        Student s(name, email, phone, address, dob, toupper(gender), dept);
        students.push_back(s);
        
        cout << "\nStudent added successfully!" << endl;
        cout << "Assigned ID: " << s.getStudentId() << endl;
        
        saveData();
    }
    // View all students
    void viewAllStudents() {
        displayHeader("ALL STUDENTS");
        
        if(students.empty()) {
            cout << "No students found." << endl;
            return;
        }
        
        cout << left << setw(8) << "ID"
             << setw(25) << "Name"
             << setw(15) << "Department"
             << setw(15) << "Phone"
             << setw(25) << "Email"
             << endl;
        displayLine('-');
        
        for(const auto& s : students) {
            s.displayBasicInfo();
        }
        
        cout << "\nTotal Students: " << students.size() << endl;
    }
    // Search student
    void searchStudent() {
        displayHeader("SEARCH STUDENT");
        
        int id;
        cout << "Enter Student ID: ";
        cin >> id;
        clearInput();
        
        Student* s = findStudent(id);
        if(s) {
            s->displayFullInfo();
        } else {
            cout << "Student not found!" << endl;
        }
    }
    // Update student
    void updateStudent() {
        if(!hasAccess("UPDATE_STUDENT")) {
            cout << "Access denied!" << endl;
            return;
        }
        displayHeader("UPDATE STUDENT");
        
        int id;
        cout << "Enter Student ID: ";
        cin >> id;
        clearInput();
        
        Student* s = findStudent(id);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        s->displayFullInfo();
        
        cout << "\nWhat would you like to update?" << endl;
        cout << "1. Name" << endl;
        cout << "2. Email" << endl;
        cout << "3. Phone" << endl;
        cout << "4. Address" << endl;
        cout << "5. Department" << endl;
        cout << "0. Cancel" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        clearInput();
        
        string newValue;
        switch(choice) {
            case 1:
                cout << "Enter new name: ";
                getline(cin, newValue);
                s->setName(newValue);
                break;
            case 2:
                cout << "Enter new email: ";
                getline(cin, newValue);
                if(!s->setEmail(newValue)) {
                    cout << "Invalid email!" << endl;
                    return;
                }
                break;
            case 3:
                cout << "Enter new phone: ";
                getline(cin, newValue);
                if(!s->setPhone(newValue)) {
                    cout << "Invalid phone!" << endl;
                    return;
                }
                break;
            case 4:
                cout << "Enter new address: ";
                getline(cin, newValue);
                s->setAddress(newValue);
                break;
            case 5:
                cout << "Enter new department: ";
                getline(cin, newValue);
                s->setDepartment(newValue);
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice!" << endl;
                return;
        }
        
        cout << "Student updated successfully!" << endl;
        saveData();
    }
    // Delete student
    void deleteStudent() {
        if(!hasAccess("DELETE_STUDENT")) {
            cout << "Access denied!" << endl;
            return;
        }
        displayHeader("DELETE STUDENT");
        
        int id;
        cout << "Enter Student ID: ";
        cin >> id;
        clearInput();
        
        for(auto it = students.begin(); it != students.end(); ++it) {
            if(it->getStudentId() == id) {
                cout << "Are you sure you want to delete " << it->getName() << "? (y/n): ";
                char confirm;
                cin >> confirm;
                clearInput();
                
                if(tolower(confirm) == 'y') {
                    students.erase(it);
                    cout << "Student deleted successfully!" << endl;
                    saveData();
                } else {
                    cout << "Deletion cancelled." << endl;
                }
                return;
            }
        }
        
        cout << "Student not found!" << endl;
    }
    // Academic Tracking Menu
    void academicTrackingMenu() {
        int choice;
        do {
            displayHeader("ACADEMIC TRACKING");
            cout << "  1. Add Grades" << endl;
            cout << "  2. View Student Grades" << endl;
            cout << "  3. Calculate GPA" << endl;
            cout << "  4. Calculate CGPA" << endl;
            cout << "  5. View Academic Transcript" << endl;
            cout << "  0. Back to Main Menu" << endl;
            displayLine();
            cout << "Enter choice: ";
            cin >> choice;
            clearInput();
            switch(choice) {
                case 1: addGrades(); break;
                case 2: viewStudentGrades(); break;
                case 3: calculateGPA(); break;
                case 4: calculateCGPA(); break;
                case 5: viewTranscript(); break;
                case 0: break;
                default: cout << "Invalid choice!" << endl;
            }
        } while(choice != 0);
    }
    // Add grades
    void addGrades() {
        if(!hasAccess("ADD_GRADES")) {
            cout << "Access denied!" << endl;
            return;
        }
        displayHeader("ADD GRADES");
        
        int studentId, semester;
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        cout << "Enter Semester: ";
        cin >> semester;
        clearInput();
        
        char addMore = 'y';
        while(tolower(addMore) == 'y') {
            string code, name, grade;
            int credits;
            
            cout << "\nSubject Code: ";
            getline(cin, code);
            
            cout << "Subject Name: ";
            getline(cin, name);
            
            cout << "Credits: ";
            cin >> credits;
            clearInput();
            
            cout << "Grade (A+/A/B+/B/C+/C/D/F): ";
            getline(cin, grade);
            
            Grade g(code, name, credits, grade, semester);
            s->getAcademic().addGrade(g);
            
            cout << "Grade added successfully!" << endl;
            
            cout << "\nAdd another subject? (y/n): ";
            cin >> addMore;
            clearInput();
        }
        
        saveData();
    }
    // View student grades
    void viewStudentGrades() {
        int studentId, semester;
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        cout << "Enter Semester (0 for all): ";
        cin >> semester;
        clearInput();
        
        if(semester == 0) {
            s->getAcademic().displayAllRecords();
        } else {
            s->getAcademic().displaySemesterGrades(semester);
        }
    }
    // Calculate GPA
    void calculateGPA() {
        int studentId, semester;
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        cout << "Enter Semester: ";
        cin >> semester;
        clearInput();
        
        float gpa = s->getAcademic().calculateSemesterGPA(semester);
        cout << "\nSemester " << semester << " GPA: " << fixed << setprecision(2) << gpa << endl;
    }
    // Calculate CGPA
    void calculateCGPA() {
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        clearInput();
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        float cgpa = s->getAcademic().calculateCGPA();
        cout << "\nCGPA: " << fixed << setprecision(2) << cgpa << endl;
        cout << "Total Credits Earned: " << s->getAcademic().getTotalCredits() << endl;
    }
    // View transcript
    void viewTranscript() {
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        clearInput();
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        s->getAcademic().displayAllRecords();
    }
    // Fee Management Menu
    void feeManagementMenu() {
        int choice;
        do {
            displayHeader("FEE MANAGEMENT");
            cout << "  1. View Fee Summary" << endl;
            cout << "  2. Make Payment" << endl;
            cout << "  3. View Payment History" << endl;
            cout << "  4. Update Fee Amount" << endl;
            cout << "  5. Generate Fee Statement" << endl;
            cout << "  0. Back to Main Menu" << endl;
            displayLine();
            cout << "Enter choice: ";
            cin >> choice;
            clearInput();
            switch(choice) {
                case 1: viewFeeSummary(); break;
                case 2: makePayment(); break;
                case 3: viewPaymentHistory(); break;
                case 4: updateFeeAmount(); break;
                case 5: generateFeeStatement(); break;
                case 0: break;
                default: cout << "Invalid choice!" << endl;
            }
        } while(choice != 0);
    }
    // View fee summary
    void viewFeeSummary() {
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        clearInput();
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        cout << "\nStudent: " << s->getName() << endl;
        s->getFee().displaySummary();
    }
    // Make payment
    void makePayment() {
        if(!hasAccess("MAKE_PAYMENT")) {
            cout << "Access denied!" << endl;
            return;
        }
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        s->getFee().displaySummary();
        
        float amount;
        cout << "Enter payment amount: Rs. ";
        cin >> amount;
        clearInput();
        
        cout << "Payment method (Cash/Card/Online): ";
        string method;
        getline(cin, method);
        
        if(s->getFee().makePayment(amount, method)) {
            saveData();
        }
    }
    // View payment history
    void viewPaymentHistory() {
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        clearInput();
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        cout << "\nStudent: " << s->getName() << endl;
        s->getFee().displayPaymentHistory();
    }
    // Update fee amount
    void updateFeeAmount() {
        if(!hasAccess("UPDATE_FEE")) {
            cout << "Access denied!" << endl;
            return;
        }
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        float newAmount;
        cout << "Enter new total fee amount: Rs. ";
        cin >> newAmount;
        clearInput();
        
        s->getFee().setTotalAmount(newAmount);
        cout << "Fee updated successfully!" << endl;
        saveData();
    }
    // Generate fee statement
    void generateFeeStatement() {
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        clearInput();
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        displayLine('*');
        cout << "          FEE STATEMENT" << endl;
        displayLine('*');
        cout << "\nStudent ID: " << s->getStudentId() << endl;
        cout << "Name: " << s->getName() << endl;
        cout << "Department: " << s->getDepartment() << endl;
        cout << "Generated: " << getTimestamp() << endl;
        
        s->getFee().displaySummary();
        s->getFee().displayPaymentHistory();
        
        displayLine('*');
    }
    // Reports Menu
    void reportsMenu() {
        int choice;
        do {
            displayHeader("REPORTS & ANALYTICS");
            cout << "  1. Individual Student Report" << endl;
            cout << "  2. Class Performance Summary" << endl;
            cout << "  3. Fee Collection Report" << endl;
            cout << "  4. Department-wise Statistics" << endl;
            cout << "  0. Back to Main Menu" << endl;
            displayLine();
            cout << "Enter choice: ";
            cin >> choice;
            clearInput();
            switch(choice) {
                case 1: individualStudentReport(); break;
                case 2: classPerformanceSummary(); break;
                case 3: feeCollectionReport(); break;
                case 4: departmentStatistics(); break;
                case 0: break;
                default: cout << "Invalid choice!" << endl;
            }
        } while(choice != 0);
    }
    // Individual student report
    void individualStudentReport() {
        int studentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        clearInput();
        
        Student* s = findStudent(studentId);
        if(!s) {
            cout << "Student not found!" << endl;
            return;
        }
        
        s->generateReport();
    }
    // Class performance summary
    void classPerformanceSummary() {
        displayHeader("CLASS PERFORMANCE SUMMARY");
        
        if(students.empty()) {
            cout << "No students found." << endl;
            return;
        }
        
        float totalCGPA = 0;
        int count = 0;
        float highestCGPA = 0;
        float lowestCGPA = 10;
        string topStudent, bottomStudent;
        
        for(const auto& s : students) {
            float cgpa = s.getAcademic().calculateCGPA();
            totalCGPA += cgpa;
            count++;
            
            if(cgpa > highestCGPA) {
                highestCGPA = cgpa;
                topStudent = s.getName();
            }
            if(cgpa < lowestCGPA && cgpa > 0) {
                lowestCGPA = cgpa;
                bottomStudent = s.getName();
            }
        }
        
        cout << "Total Students: " << count << endl;
        cout << "Average CGPA: " << fixed << setprecision(2) << (totalCGPA / count) << endl;
        cout << "\nTop Performer: " << topStudent << " (CGPA: " << highestCGPA << ")" << endl;
        cout << "Needs Improvement: " << bottomStudent << " (CGPA: " << lowestCGPA << ")" << endl;
    }
    // Fee collection report
    void feeCollectionReport() {
        displayHeader("FEE COLLECTION REPORT");
        
        if(students.empty()) {
            cout << "No students found." << endl;
            return;
        }
        
        float totalDue = 0, totalCollected = 0, totalPending = 0;
        int paidCount = 0, partialCount = 0, pendingCount = 0;
        
        for(const auto& s : students) {
            const Fee& f = s.getFee();
            totalDue += f.getTotalAmount();
            totalCollected += f.getPaidAmount();
            totalPending += f.getBalance();
            
            string status = f.getStatus();
            if(status == "PAID") paidCount++;
            else if(status == "PARTIAL") partialCount++;
            else pendingCount++;
        }
        
        cout << "Total Fee Due      : Rs. " << fixed << setprecision(2) << totalDue << endl;
        cout << "Total Collected    : Rs. " << totalCollected << endl;
        cout << "Total Pending      : Rs. " << totalPending << endl;
        cout << "Collection Rate    : " << (totalCollected/totalDue*100) << "%" << endl;
        cout << "\nPayment Status:" << endl;
        cout << "  Fully Paid   : " << paidCount << " students" << endl;
        cout << "  Partial Paid : " << partialCount << " students" << endl;
        cout << "  Pending      : " << pendingCount << " students" << endl;
    }
    // Department statistics
    void departmentStatistics() {
        displayHeader("DEPARTMENT-WISE STATISTICS");
        
        if(students.empty()) {
            cout << "No students found." << endl;
            return;
        }
        
        // Collect department data
        vector<string> departments;
        for(const auto& s : students) {
            string dept = s.getDepartment();
            if(find(departments.begin(), departments.end(), dept) == departments.end()) {
                departments.push_back(dept);
            }
        }
        
        cout << left << setw(20) << "Department"
             << setw(10) << "Students"
             << setw(12) << "Avg CGPA"
             << setw(15) << "Fee Collected"
             << endl;
        displayLine('-');
        
        for(const auto& dept : departments) {
            int count = 0;
            float totalCGPA = 0;
            float totalCollected = 0;
            
            for(const auto& s : students) {
                if(s.getDepartment() == dept) {
                    count++;
                    totalCGPA += s.getAcademic().calculateCGPA();
                    totalCollected += s.getFee().getPaidAmount();
                }
            }
            
            cout << left << setw(20) << dept
                 << setw(10) << count
                 << setw(12) << fixed << setprecision(2) << (totalCGPA/count)
                 << "Rs. " << totalCollected
                 << endl;
        }
    }
    // Settings Menu
    void settingsMenu() {
        int choice;
        do {
            displayHeader("SETTINGS & SYSTEM");
            cout << "  1. View All Users" << endl;
            cout << "  2. Add New User" << endl;
            cout << "  3. Create Backup" << endl;
            cout << "  4. System Information" << endl;
            cout << "  0. Back to Main Menu" << endl;
            displayLine();
            cout << "Enter choice: ";
            cin >> choice;
            clearInput();
            switch(choice) {
                case 1: viewAllUsers(); break;
                case 2: addNewUser(); break;
                case 3: createBackup(); break;
                case 4: systemInfo(); break;
                case 0: break;
                default: cout << "Invalid choice!" << endl;
            }
        } while(choice != 0);
    }
    // View all users
    void viewAllUsers() {
        if(!hasAccess("VIEW_USERS")) {
            cout << "Access denied!" << endl;
            return;
        }
        displayHeader("SYSTEM USERS");
        cout << left << setw(20) << "Username"
             << setw(15) << "Role"
             << setw(25) << "Created"
             << endl;
        displayLine('-');
        
        for(const auto& u : users) {
            u.display();
        }
    }
    // Add new user
    void addNewUser() {
        if(!hasAccess("ADD_USER")) {
            cout << "Access denied!" << endl;
            return;
        }
        displayHeader("ADD NEW USER");
        
        string username, password;
        int roleChoice;
        
        cout << "Username: ";
        cin >> username;
        
        cout << "Password: ";
        cin >> password;
        
        cout << "Role (1=Admin, 2=Staff, 3=Student): ";
        cin >> roleChoice;
        clearInput();
        
        Role role = static_cast<Role>(roleChoice);
        users.push_back(User(username, password, role));
        
        cout << "User added successfully!" << endl;
    }
    // Create backup
    void createBackup() {
        string timestamp = getTimestamp();
        // Remove spaces and colons for filename
        for(char& c : timestamp) {
            if(c == ' ' || c == ':') c = '_';
        }
        
        string backupFile = "backup_" + timestamp + ".dat";
        
        ifstream src(DATA_FILE, ios::binary);
        if(!src) {
            cout << "No data to backup!" << endl;
            return;
        }
        
        ofstream dst(backupFile, ios::binary);
        dst << src.rdbuf();
        
        src.close();
        dst.close();
        
        cout << "Backup created: " << backupFile << endl;
    }
    // System info
    void systemInfo() {
        displayHeader("SYSTEM INFORMATION");
        cout << "System Name      : Academia Connect" << endl;
        cout << "Version          : 1.0.0" << endl;
        cout << "Total Students   : " << students.size() << endl;
        cout << "Total Users      : " << users.size() << endl;
        cout << "Data File        : " << DATA_FILE << endl;
        cout << "Current User     : " << (currentUser ? currentUser->getUsername() : "None") << endl;
        cout << "Current Time     : " << getTimestamp() << endl;
    }
    // Run the system
    void run() {
        while(isRunning) {
            if(!login()) {
                cout << "Try again? (y/n): ";
                char retry;
                cin >> retry;
                clearInput();
                if(tolower(retry) != 'y') {
                    isRunning = false;
                }
                continue;
            }
            
            loadData();
            
            int choice;
            bool loggedIn = true;
            
            while(loggedIn && isRunning) {
                displayMainMenu();
                cin >> choice;
                clearInput();
                
                switch(choice) {
                    case 1: studentManagementMenu(); break;
                    case 2: academicTrackingMenu(); break;
                    case 3: feeManagementMenu(); break;
                    case 4: reportsMenu(); break;
                    case 5: settingsMenu(); break;
                    case 6: 
                        cout << "Logging out..." << endl;
                        currentUser = nullptr;
                        loggedIn = false;
                        break;
                    case 0:
                        cout << "Exiting system..." << endl;
                        isRunning = false;
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            }
        }
    }
};
// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    cout << "\n";
    cout << "============================================================" << endl;
    cout << "           ACADEMIA CONNECT" << endl;
    cout << "       Student Management System v1.0" << endl;
    cout << "============================================================" << endl;
    cout << "\n";
    
    StudentManagementSystem sms;
    sms.run();
    
    return 0;
}
