#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ###################### Student ######################

// Contains date info
typedef struct Date
{
    int day;
    int month;
    int year;
} Date;

// Contains student info
typedef struct StudentInfo
{
    int id;
    char name[25];
    Date birthDate;
    int score;
} StudentInfo;

// Fills student info from input stream
void fillStudent(StudentInfo *student)
{
    printf("Enter ID: ");
    scanf("%d", &student->id);

    // Flush input
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        /* ignore */;

    printf("Enter Name: ");
    fgets(student->name, sizeof(student->name), stdin);
    student->name[strcspn(student->name, "\n")] = '\0';

    printf("Enter Birth Date (Day Month Year): ");
    scanf("%d", &student->birthDate.day);
    scanf("%d", &student->birthDate.month);
    scanf("%d", &student->birthDate.year);

    printf("Enter Score: ");
    scanf("%d", &student->score);
    printf("\n");
}

// Fills student info with random info
void fillRandomStudent(StudentInfo *student)
{
    typedef const char *str;
    static const str firstNames[] = {"Adham", "Ali", "Ibrahim"};
    static const str lastNames[] = {"Mohamed", "Medhat", "Nader"};

    str firstName = firstNames[rand() % 3];
    size_t firstNameLength = strlen(firstName);
    str lastName = lastNames[rand() % 3];
    size_t lastNameLength = strlen(lastName);

    memcpy(student->name, firstName, firstNameLength * sizeof(char));
    student->name[firstNameLength] = ' ';
    memcpy(student->name + firstNameLength + 1, lastName, lastNameLength * sizeof(char));
    student->name[firstNameLength + lastNameLength + 1] = '\0';
    student->id = (rand() % 999999) + 1;
    student->birthDate.day = (rand() % 30) + 1;
    student->birthDate.month = (rand() % 12) + 1;
    student->birthDate.year = (rand() % 30) + 1980;
    student->score = (rand() % 50) + 50;
}

// Prints student info to output stream
void printStudent(const StudentInfo *student)
{
    printf("%d: %s %d/%d/%d %d%%\n\n", student->id, student->name,
           student->birthDate.day, student->birthDate.month,
           student->birthDate.year, student->score);
}

// Prints students table header
void printStudentTableHeader()
{
    printf("--------------------------------------------------------------\n");
    printf("|     ID |           Name           |  Birth Date  |   Score |\n");
    printf("--------------------------------------------------------------\n");
}

// Prints student in tabular form
void printStudentTable(const StudentInfo *student)
{
    // Format birth date
    char birthDate[13];
    sprintf(birthDate, "%d/%d/%d", student->birthDate.day,
            student->birthDate.month, student->birthDate.year);

    // Print student in tabular form
    printf("| %6d | %-24s | %-12s | %6d%% |\n",
           student->id, student->name,
           birthDate, student->score);
}

// Prints student in tabular form
void printStudentTableFooter()
{
    printf("--------------------------------------------------------------\n\n");
}

// #################### Linked List ####################

// Represents linked list node
typedef struct StudentListNode StudentListNode;
struct StudentListNode
{
    StudentInfo student;   // Node data (student info)
    StudentListNode *next; // Points to next node or end (null ptr)
};

// Contains linked list info
typedef struct StudentList
{
    StudentListNode *head; // Points to first node or end (null ptr)
    StudentListNode *tail; // Points to last node or end (null ptr)
    size_t size;           // List size (nodes count)
} StudentList;

// Initialize list
void initList(StudentList *students)
{
    // Empty list
    students->head = NULL;
    students->tail = NULL;
    students->size = 0;
}

// Adds student info to list (from head)
void insertFirstListStudent(StudentList *students, const StudentInfo *student)
{
    // Create new head which points to old head [which can be end (null ptr)]
    StudentListNode *newHead = (StudentListNode *)malloc(sizeof(StudentListNode));
    newHead->student = *student;
    newHead->next = students->head;

    // Check if list is empty
    if (students->size == 0)
    {
        // List is empty; Adjust tail to point to newly created head
        students->tail = newHead;
    }

    // Point list head to newly created head
    students->head = newHead;
    students->size++;
}

// Adds student info to list (from tail)
void insertLastListStudent(StudentList *students, const StudentInfo *student)
{
    // Create new tail which points to end (null ptr)
    StudentListNode *newTail = (StudentListNode *)malloc(sizeof(StudentListNode));
    newTail->student = *student;
    newTail->next = NULL;

    // Check if list is empty
    if (students->size == 0)
    {
        // List is empty; Adjust head to point to newly created tail
        students->head = newTail;
    }
    else
    {
        // List is not empty; Point old tail to newly created tail
        students->tail->next = newTail;
    }

    // Point list tail to newly created tail
    students->tail = newTail;
    students->size++;
}

// Adds student info to list at nth position (from head)
void insertNthListStudent(StudentList *students, unsigned int index, const StudentInfo *student)
{
    // Check if index is out of bounds
    if (index > students->size)
        return;

    // Check insert from head
    if (index == 0)
    {
        insertFirstListStudent(students, student);
        return;
    }

    // Check insert from tail
    if (index == students->size)
    {
        insertLastListStudent(students, student);
        return;
    }

    // !! Size can't equal zero (head isn't NULL nor tail)

    // Get second node
    size_t i = 1;
    StudentListNode *prev = students->head;
    StudentListNode *current = students->head->next;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Check index and count
        if (i == index)
        {
            // Reached index; insert node

            // Create new node which points to current node
            StudentListNode *newNode = (StudentListNode *)malloc(sizeof(StudentListNode));
            newNode->student = *student;
            newNode->next = current;

            // Point prev node to newly created node
            prev->next = newNode;
            students->size++;
            return;
        }

        // Get next node
        i++;
        prev = current;
        current = current->next;
    }
}

// Prints all students in list
void printListStudents(const StudentList *students)
{
    // Print Title
    printf("[Students List]\n");

    // Check if list is empty
    if (students->size == 0)
    {
        // List is empty
        printf("List is empty!\n\n");
        return;
    }

    // Print table header
    printStudentTableHeader();

    // Get first node
    StudentListNode *current = students->head;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Print student
        printStudentTable(&current->student);

        // Get next node
        current = current->next;
    }

    // Print table footer
    printStudentTableFooter();
}

void destroyList(StudentList *students)
{
    // Get first node
    StudentListNode *temp = NULL;
    StudentListNode *current = students->head;

    // Reset list
    students->head = NULL;
    students->tail = NULL;
    students->size = 0;

    // Iterate over list until end (null ptr)
    while (current != NULL)
    {
        // Get next node
        temp = current;
        current = current->next;

        // Delete node
        free(temp);
    }
}

int main()
{
    // ############ Welcome Text ############
    printf("############### Welcome ###############\n\n");

    // Get number of students
    int N;
    printf("Enter N (number of students): ");
    scanf("%d", &N);
    printf("\n");

    // ########## Linked List Demo ##########
    printf("########## Linked List Demo ###########\n\n");

    // Create student list
    StudentList studentList;
    initList(&studentList);

    // Add N random students to list
    for (size_t i = 0; i < N; i++)
    {
        StudentInfo listStudent;
        fillRandomStudent(&listStudent);
        insertFirstListStudent(&studentList, &listStudent);
    }

    // Print list students
    printListStudents(&studentList);

    // Insert student at first demo
    printf("[Insert First Demo]\n");
    StudentInfo firstListStudent;
    fillStudent(&firstListStudent);
    insertFirstListStudent(&studentList, &firstListStudent);

    // Print list students
    printListStudents(&studentList);

    // Insert student at last demo
    printf("[Insert Last Demo]\n");
    StudentInfo lastListStudent;
    fillStudent(&lastListStudent);
    insertLastListStudent(&studentList, &lastListStudent);

    // Print list students
    printListStudents(&studentList);

    // Insert student at middle demo
    printf("[Insert Middle Demo]\n");
    StudentInfo middleListStudent;
    fillStudent(&middleListStudent);
    insertNthListStudent(&studentList, studentList.size / 2, &middleListStudent);

    // Print list students
    printListStudents(&studentList);

    // Destroy list
    destroyList(&studentList);

    return 0;
}