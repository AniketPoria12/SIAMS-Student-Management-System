
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"

typedef struct Student {
    int id;
    char name[50];
    float fees;
    float paid;
    int course;
    int result;
    struct Student *next;
} Student;

Student *head = NULL;

// Course and Result names
const char *courses[] = {"Not Allocated", "DS", "AI", "ML", "Web"};
const char *results[] = {"Not Declared", "Excellent", "Good", "Fail"};

// Input functions
void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readName(char name[]) {
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';
}

int readInt() {
    int n;
    while (scanf("%d", &n) != 1) {
        printf("Enter a valid number: ");
        clearInput();
    }
    clearInput();
    return n;
}

float readFloat() {
    float n;
    while (scanf("%f", &n) != 1) {
        printf("Enter a valid amount: ");
        clearInput();
    }
    clearInput();
    return n;
}

// Save student records permanently
void saveData() {
    FILE *fp = fopen(FILE_NAME, "wb");
    Student *temp = head;

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    while (temp != NULL) {
        fwrite(temp, sizeof(Student), 1, fp);
        temp = temp->next;
    }

    fclose(fp);
}

// Load records from file
void loadData() {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;

    if (fp == NULL)
        return;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        Student *newNode = malloc(sizeof(Student));

        if (newNode == NULL)
            break;

        *newNode = s;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
        } else {
            Student *temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newNode;
        }
    }

    fclose(fp);
}

// Find student by ID
Student *findStudent(int id) {
    Student *temp = head;

    while (temp != NULL) {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    }

    return NULL;
}

// Admin login
int login() {
    char username[30], password[30];

    printf("\n===== ADMIN LOGIN =====\n");
    printf("User ID: ");
    scanf("%29s", username);

    printf("Password: ");
    scanf("%29s", password);
    clearInput();

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "admin123") == 0) {
        printf("\nLogin Successful!\n");
        return 1;
    }

    printf("\nInvalid User ID or Password!\n");
    return 0;
}

// Student registration
void addStudent() {
    Student *newNode = malloc(sizeof(Student));

    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\nEnter Student ID: ");
    newNode->id = readInt();

    if (findStudent(newNode->id) != NULL) {
        printf("Student ID already exists!\n");
        free(newNode);
        return;
    }

    printf("Enter Student Name: ");
    readName(newNode->name);

    printf("Enter Total Fees: ");
    newNode->fees = readFloat();

    if (newNode->fees <= 0) {
        printf("Invalid fee amount!\n");
        free(newNode);
        return;
    }

    newNode->paid = 0;
    newNode->course = 0;
    newNode->result = 0;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        Student *temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    saveData();
    printf("\nStudent Registered Successfully!\n");
}

// Display all students
void displayStudents() {
    Student *temp = head;

    if (temp == NULL) {
        printf("\nNo Student Records Found!\n");
        return;
    }

    printf("\n========== STUDENT RECORDS ==========\n");

    while (temp != NULL) {
        printf("\nID: %d", temp->id);
        printf("\nName: %s", temp->name);
        printf("\nTotal Fees: %.2f", temp->fees);
        printf("\nPaid Fees: %.2f", temp->paid);
        printf("\nCourse: %s", courses[temp->course]);
        printf("\nResult: %s\n", results[temp->result]);

        temp = temp->next;
    }
}

// Search student
void searchStudent() {
    int id;

    printf("\nEnter Student ID: ");
    id = readInt();

    Student *s = findStudent(id);

    if (s == NULL) {
        printf("Student Not Found!\n");
        return;
    }

    printf("\nID: %d", s->id);
    printf("\nName: %s", s->name);
    printf("\nFees: %.2f", s->fees);
    printf("\nPaid: %.2f", s->paid);
    printf("\nCourse: %s", courses[s->course]);
    printf("\nResult: %s\n", results[s->result]);
}

// Update student name
void updateStudent() {
    int id;

    printf("\nEnter Student ID: ");
    id = readInt();

    Student *s = findStudent(id);

    if (s == NULL) {
        printf("Student Not Found!\n");
        return;
    }

    printf("Enter New Name: ");
    readName(s->name);

    saveData();
    printf("Record Updated Successfully!\n");
}

// Delete student
void deleteStudent() {
    int id;

    printf("\nEnter Student ID: ");
    id = readInt();

    Student *temp = head, *prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Student Not Found!\n");
        return;
    }

    if (prev == NULL)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    saveData();

    printf("Student Deleted Successfully!\n");
}

// Fee payment and course allocation
void feePayment() {
    int id, choice;

    printf("\nEnter Student ID: ");
    id = readInt();

    Student *s = findStudent(id);

    if (s == NULL) {
        printf("Student Not Found!\n");
        return;
    }

    if (s->paid >= s->fees) {
        printf("Fees Already Paid!\n");
    } else {
        float amount;

        printf("Total Fees: %.2f\n", s->fees);
        printf("Already Paid: %.2f\n", s->paid);
        printf("Remaining: %.2f\n", s->fees - s->paid);

        printf("Enter Payment Amount: ");
        amount = readFloat();

        if (amount <= 0 || amount > s->fees - s->paid) {
            printf("Invalid Payment Amount!\n");
            return;
        }

        s->paid += amount;
    }

    if (s->paid >= s->fees && s->course == 0) {
        printf("\nSelect Course:\n");
        printf("1. DS\n2. AI\n3. ML\n4. Web\n");
        printf("Enter Choice: ");

        choice = readInt();

        if (choice < 1 || choice > 4) {
            printf("Invalid Course Choice!\n");
            return;
        }

        s->course = choice;
        printf("Course Allocated Successfully!\n");
    }

    saveData();
    printf("Payment Updated Successfully!\n");
}

// Result declaration
void declareResult() {
    int id, choice;

    printf("\nEnter Student ID: ");
    id = readInt();

    Student *s = findStudent(id);

    if (s == NULL) {
        printf("Student Not Found!\n");
        return;
    }

    if (s->course == 0) {
        printf("Course Not Allocated Yet!\n");
        return;
    }

    printf("\nSelect Result:\n");
    printf("1. Excellent\n");
    printf("2. Good\n");
    printf("3. Fail\n");

    printf("Enter Choice: ");
    choice = readInt();

    if (choice < 1 || choice > 3) {
        printf("Invalid Choice!\n");
        return;
    }

    s->result = choice;
    saveData();

    printf("Result Declared Successfully!\n");
}

// Course-wise report
void courseReport() {
    int choice, count = 0;

    printf("\n1.DS  2.AI  3.ML  4.Web\n");
    printf("Select Course: ");
    choice = readInt();

    if (choice < 1 || choice > 4) {
        printf("Invalid Choice!\n");
        return;
    }

    Student *temp = head;

    printf("\nStudents in %s:\n", courses[choice]);

    while (temp != NULL) {
        if (temp->course == choice) {
            printf("ID: %d | Name: %s | Result: %s\n",
                   temp->id, temp->name, results[temp->result]);
            count++;
        }
        temp = temp->next;
    }

    printf("Total Students: %d\n", count);
}

// Main menu
void menu() {
    int choice;

    do {
        printf("\n\n===== SIAMS MAIN MENU =====\n");
        printf("1. Register Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Fee Payment & Course Allocation\n");
        printf("7. Declare Result\n");
        printf("8. Course-wise Report\n");
        printf("9. Logout\n");
        printf("10. Exit\n");

        printf("\nEnter Choice: ");
        choice = readInt();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: feePayment(); break;
            case 7: declareResult(); break;
            case 8: courseReport(); break;
            case 9:
                printf("Logged Out Successfully!\n");
                return;
            case 10:
                printf("Thank You!\n");
                exit(0);
            default:
                printf("Invalid Choice!\n");
        }

    } while (1);
}

// Main function
int main() {
    loadData();

    printf("====================================\n");
    printf(" STUDENT INFORMATION MANAGEMENT\n");
    printf("====================================\n");

    while (1) {
        if (login()) {
            menu();
        }
    }

    return 0;
}
