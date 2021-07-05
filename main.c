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

// #################### Dynamic Array ####################

// Contains dynamic array info
typedef struct StudentArray
{
    StudentInfo *array; // Pointer to first student (contiguous)
    size_t size;        // Array size
} StudentArray;

// Initialize array
void initArray(StudentArray *students)
{
    // Empty array
    students->array = NULL;
    students->size = 0;
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

int main()
{
    // ############ Welcome Text ############
    printf("############### Welcome ###############\n\n");

    // Get number of students
    int N;
    printf("Enter N (number of students): ");
    scanf("%d", &N);
    printf("\n");

    // ########## Dynamic Array Demo ##########
    printf("########## Dynamic Array Demo ###########\n\n");

    // Create student array
    StudentArray studentArray;
    initArray(&studentArray);

    // Add N random students to array
    for (size_t i = 0; i < N; i++)
    {
        StudentInfo arrayStudent;
        fillRandomStudent(&arrayStudent);
        insertFirstArrayStudent(&studentArray, &arrayStudent);
    }

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

    return 0;
}
