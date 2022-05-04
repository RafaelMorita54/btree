#include "btree.h"

// TREE FUNCTIONS ---------------------------------------------------------------

page_t *getOrCreateRoot (FILE *p_file) {
    page_t *page;
    return page;
};

page_t *getRoot (FILE *p_file) {
    page_t *page;
    return page;
};

long bTreeSelect (page_t *page, int number, FILE *p_file) {
    return 0;
};

record_t binSearch_Btree (FILE *p_file, long lKey) {
    record_t record;
    return record;
}

// SUPPORT FUNCTIONS ---------------------------------------------------------------

int csvFileSize (FILE *p_file){
    if(p_file==NULL){
        printf("\n\nErro ao receber o arquivo.\n\n");
        return 0;
    } else {
        char currentChar;
        int Lines = 0;
        currentChar = fgetc(p_file);
        while (currentChar != EOF){
            currentChar = fgetc(p_file);
            if (currentChar == '\n')
            Lines++;
        }
        fseek(p_file,0,SEEK_SET);
        Lines++;
        return Lines;
    }
}

int binFileSize (FILE *p_file){
    int binSize;
    if(p_file==NULL){
        printf("\n\nErro ao receber o arquivo.\n\n");
        return 0;
    }
    fseek(p_file,0,SEEK_END);
    binSize = ftell(p_file);
    fseek(p_file,0,SEEK_END);
    return binSize;
}

char readCurrentInput(FILE *p_file, char *currentInput){
    

    char currentChar;
    int indexcurrentInput = 0;
    currentChar = fgetc(p_file);
    while((currentChar != EOF) && (currentChar != DELIMITERCHAR) && (currentChar != '\n')){
        currentInput[indexcurrentInput++] = currentChar;
        currentChar = fgetc(p_file);
    }
    currentInput[indexcurrentInput] = '\0';
    return currentChar; 
}

// STUDENT FUNCTIOS  ---------------------------------------------------------------

student_t csvRead_Student (FILE *p_file){
    student_t student;

    char currentInput[80];

    readCurrentInput(p_file, currentInput);
    student.lKey = atoi(currentInput);

    readCurrentInput(p_file, currentInput);
    strcpy(student.sName, currentInput);

    readCurrentInput(p_file, currentInput);
    strcpy(student.sSurname, currentInput);

    readCurrentInput(p_file, currentInput);
    strcpy(student.sCourse, currentInput);

    readCurrentInput(p_file, currentInput);
    student.fGrade = atof(currentInput);

    return student;
}

student_t binRead_Student (FILE *p_file) {

    student_t student;

    fread(&student.lKey, sizeof(int), 1, p_file);
    fread(&student.sName, 50, 1, p_file);
    fread(&student.sSurname, 50, 1, p_file);
    fread(&student.sCourse, 50, 1, p_file);
    fread(&student.fGrade, sizeof(float), 1, p_file);

    return student;
}

void binWrite_Student (student_t student, FILE *p_file){

    fwrite(&student.lKey, sizeof(int), 1, p_file);
    fwrite(&student.sName, 50, 1, p_file);
    fwrite(&student.sSurname, 50, 1, p_file);
    fwrite(&student.sCourse, 50, 1, p_file);
    fwrite(&student.fGrade, sizeof(float), 1, p_file);

}

void print_Student (student_t student){
    printf("-------------------------------\n");
    printf("nUSP: %d\n", student.lKey);
    printf("Nome: %s\n", student.sName);
    printf("Sobrenome: %s\n", student.sSurname);
    printf("Curso: %s\n", student.sCourse);
    printf("Nota: %.2f\n", student.fGrade);
    printf("-------------------------------\n");
}

student_t getStudentByKey(FILE *p_file, long lKey) {
    student_t student;
    fseek(p_file, lKey, SEEK_SET);
    student=binRead_Student(p_file);
    return student;
}


// RECORD FUNCTIOS  ---------------------------------------------------------------

record_t binRead_Record (FILE *p_file) {

    record_t record;

    fread(&record.lKey, sizeof(int), 1, p_file);
    fread(&record.lRRN, sizeof(int), 1, p_file);

    return record;
}

void binWrite_Record (record_t record, FILE *p_file){

    fwrite(&record.lKey, sizeof(int), 1, p_file);
    fwrite(&record.lRRN, sizeof(int), 1, p_file);
}

void printRecord (record_t record){
    printf("-------------------------------\n");
    printf("nUSP: %d\n", record.lKey);
    printf("RRN: %d\n", record.lRRN);
    printf("−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−\n");
}