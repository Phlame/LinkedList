#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BENCHMARK

// ###################### Utility ######################

// Flushes input stream (used for getch, gets)
void flushInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        /* ignore */;
}

const char *timerName; // Timer name (task name)
float startTime;       // Start time of timer
// Start timer
void startTimer(const char *name)
{
    // Save timer name
    timerName = name;

    // Record start time
    startTime = (float)clock() / CLOCKS_PER_SEC;
}

// End timer and prints elapsed time
void endTimer()
{
    // Record end time and calculate delta time
    float endTime = (float)clock() / CLOCKS_PER_SEC;
    float deltaTime = endTime - startTime;

    // Print delta time
    printf("%s: %.2f ms\n", timerName, deltaTime * 1e3);
}

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
    flushInput();

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
    static const str FIRST_NAMES[] = {"Adham", "Ali", "Ibrahim"};
    static const str LAST_NAMES[] = {"Mohamed", "Medhat", "Nader"};

    str firstName = FIRST_NAMES[rand() % 3];
    size_t firstNameLength = strlen(firstName);
    str lastName = LAST_NAMES[rand() % 3];
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

// #################### Dynamic Array ####################

// Contains dynamic array info
typedef struct StudentArray
{
    StudentInfo *array; // Pointer to first student (contiguous)
    size_t size;        // Array size
} StudentArray;

// Initialize array
void initArray(StudentArray *students, size_t initSize)
{
    // Empty array
    students->array = NULL;
    students->size = 0;

    // Check if init size is not zero
    if (initSize != 0)
    {
        // Create array with N elements
        size_t newSize = initSize;
        StudentInfo *temp = (StudentInfo *)malloc(newSize * sizeof(StudentInfo));
        students->array = temp;
        students->size = newSize;
    }
}

// Adds student info to array (from start)
void insertFirstArrayStudent(StudentArray *students, const StudentInfo *student)
{
    // Create new array
    size_t newSize = students->size + 1;
    StudentInfo *temp = (StudentInfo *)malloc(newSize * sizeof(StudentInfo));

    // Check if array is empty
    if (students->size != 0)
    {
        // Copy old data (0:end) --> (1:end+1)
        memcpy(temp + 1, students->array, students->size * sizeof(StudentInfo));

        // Remove old array
        free(students->array);
    }

    // Replace old array
    students->array = temp;

    // Set first element
    students->array[0] = *student;
    students->size = newSize;
}

// Adds student info to array (from end)
void insertLastArrayStudent(StudentArray *students, const StudentInfo *student)
{
    // Create new array
    size_t newSize = students->size + 1;
    StudentInfo *temp = (StudentInfo *)malloc(newSize * sizeof(StudentInfo));

    // Check if array is empty
    if (students->size != 0)
    {
        // Copy old data (0:end) --> (0:end)
        memcpy(temp, students->array, students->size * sizeof(StudentInfo));

        // Remove old array
        free(students->array);
    }

    // Replace old array
    students->array = temp;

    // Set last element
    students->array[students->size] = *student;
    students->size = newSize;
}

// Adds student info to array at nth position (from start)
void insertNthArrayStudent(StudentArray *students, unsigned int index, const StudentInfo *student)
{
    // Check if index is out of bounds
    if (index > students->size)
        return;

    // Check insert from head
    if (index == 0)
    {
        insertFirstArrayStudent(students, student);
        return;
    }

    // Check insert from tail
    if (index == students->size)
    {
        insertLastArrayStudent(students, student);
        return;
    }

    // !! Size can't equal zero (array isn't NULL)

    // Create new array
    size_t newSize = students->size + 1;
    StudentInfo *temp = (StudentInfo *)malloc(newSize * sizeof(StudentInfo));

    // Copy old data (0:index-1) --> (0:index-1)
    memcpy(temp, students->array, index * sizeof(StudentInfo));
    // Copy old data (index:end) --> (index+1:end+1)
    memcpy(temp + index + 1, students->array + index, (students->size - index) * sizeof(StudentInfo));

    // Remove and replace old array
    free(students->array);
    students->array = temp;

    // Set element at index
    students->array[index] = *student;
    students->size = newSize;
}

// Prints all students in array
void printArrayStudents(const StudentArray *students)
{
    // Print Title
    printf("[Students Array]\n");

    // Check if array is empty
    if (students->size == 0)
    {
        // Array is empty
        printf("Array is empty!\n\n");
        return;
    }

    // Print table header
    printStudentTableHeader();

    // Iterate over array
    for (size_t i = 0; i < students->size; i++)
    {
        // Print student
        printStudentTable(&students->array[i]);
    }

    // Print table footer
    printStudentTableFooter();
}

void destroyArray(StudentArray *students)
{
    // Save array
    StudentInfo *temp = students->array;

    // Reset array
    students->array = NULL;
    students->size = 0;

    // Remove array
    free(temp);
}

// #################### Benchmark ###################

// Runs different tests on a list
void benchmarkList(unsigned int N)
{
    StudentInfo listStudent;

    // Create student list
    StudentList studentList;
    initList(&studentList);

    // Add N random students to list
    for (size_t i = 0; i < N; i++)
    {
        fillRandomStudent(&listStudent);
        insertFirstListStudent(&studentList, &listStudent);
    }

    // Memory size benchmark
    unsigned int structSize = sizeof(StudentList) + studentList.size * sizeof(StudentListNode);
    printf("Struct Size: %u Bytes\n", structSize);
    printf("Node Size: %u Bytes\n", sizeof(StudentListNode));

    // Insert student at first benchmark
    startTimer("Insert First");
    fillRandomStudent(&listStudent);
    insertFirstListStudent(&studentList, &listStudent);
    endTimer();

    // Insert student at last benchmark
    startTimer("Insert Last");
    fillRandomStudent(&listStudent);
    insertLastListStudent(&studentList, &listStudent);
    endTimer();

    // Insert student at middle benchmark
    startTimer("Insert Middle");
    fillRandomStudent(&listStudent);
    insertNthListStudent(&studentList, studentList.size / 2, &listStudent);
    endTimer();

    // Destroy list
    destroyList(&studentList);
}

// Runs different tests on an array
void benchmarkArray(unsigned int N)
{
    StudentInfo arrayStudent;

    // Create student array
    StudentArray studentArray;
    initArray(&studentArray, N);

    // Add N random students to array
    for (size_t i = 0; i < N; i++)
        fillRandomStudent(&studentArray.array[i]);

    // Memory size benchmark
    unsigned int structSize = sizeof(StudentArray) + studentArray.size * sizeof(StudentInfo);
    printf("Struct Size: %u Bytes\n", structSize);

    // Insert student at first benchmark
    startTimer("Insert First");
    fillRandomStudent(&arrayStudent);
    insertFirstArrayStudent(&studentArray, &arrayStudent);
    endTimer();

    // Insert student at last benchmark
    startTimer("Insert Last");
    fillRandomStudent(&arrayStudent);
    insertLastArrayStudent(&studentArray, &arrayStudent);
    endTimer();

    // Insert student at middle benchmark
    startTimer("Insert Middle");
    fillRandomStudent(&arrayStudent);
    insertNthArrayStudent(&studentArray, studentArray.size / 2, &arrayStudent);
    endTimer();

    // Destroy array
    destroyArray(&studentArray);
}

int main()
{
    // ############ Welcome Text ############
    printf("############### Welcome ###############\n\n");

    // ############# Benchmark ##############

#ifdef BENCHMARK
    printf("############ List Benchmark ###########\n\n");
    printf("Small: [N = 100K]\n");
    printf("-----------------\n");
    benchmarkList(100000);
    printf("\nMedium: [N = 1M]\n");
    printf("----------------\n");
    benchmarkList(1000000);
    printf("\nLarge: [N = 10M]\n");
    printf("----------------\n");
    benchmarkList(10000000);

    printf("\n############ Array Benchmark ##########\n\n");
    printf("Small: [N = 100K]\n");
    printf("-----------------\n");
    benchmarkArray(100000);
    printf("\nMedium: [N = 1M]\n");
    printf("----------------\n");
    benchmarkArray(1000000);
    printf("\nLarge: [N = 10M]\n");
    printf("----------------\n");
    benchmarkArray(10000000);

    printf("\n");
#endif

    // ########################################

    // Get number of students
    unsigned int N;
    printf("Enter N (number of students): ");
    scanf("%u", &N);
    printf("\n");

    // ########################################

    printf("Press ENTER to start linked list demo...");
    flushInput();
    getchar();

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

    // ########################################

    printf("Press ENTER to start dynamic array demo...");
    flushInput();
    getchar();

    // ########## Dynamic Array Demo ##########
    printf("######### Dynamic Array Demo ##########\n\n");

    // Create student array
    StudentArray studentArray;
    initArray(&studentArray, N);

    // Add N random students to array
    for (size_t i = 0; i < N; i++)
        fillRandomStudent(&studentArray.array[i]);

    // Print array students
    printArrayStudents(&studentArray);

    // Insert student at first demo
    printf("[Insert First Demo]\n");
    StudentInfo firstArrayStudent;
    fillStudent(&firstArrayStudent);
    insertFirstArrayStudent(&studentArray, &firstArrayStudent);

    // Print array students
    printArrayStudents(&studentArray);

    // Insert student at last demo
    printf("[Insert Last Demo]\n");
    StudentInfo lastArrayStudent;
    fillStudent(&lastArrayStudent);
    insertLastArrayStudent(&studentArray, &lastArrayStudent);

    // Print array students
    printArrayStudents(&studentArray);

    // Insert student at middle demo
    printf("[Insert Middle Demo]\n");
    StudentInfo middleArrayStudent;
    fillStudent(&middleArrayStudent);
    insertNthArrayStudent(&studentArray, studentArray.size / 2, &middleArrayStudent);

    // Print array students
    printArrayStudents(&studentArray);

    // Destroy array
    destroyArray(&studentArray);

    // ########################################

    printf("Press ENTER to exit...");
    flushInput();
    getchar();

    return 0;
}