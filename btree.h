#ifndef btree
#define btree

// LIBRARIES --------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// CONSTANTS --------------------------------------------------------------------------------------------------------

#define PAGESIZE 4096
#define TREE_HEADER PAGESIZE
#define MAXKEYS 204
#define AUX_FIELDS_SIZE_ON_PAGE (2+1) // number of keys INT and ”isLeaf" BOOL
#define FREE_SPACE_ON_PAGE (PAGESIZE - ((MAXKEYS*4) +(MAXKEYS*8)+((MAXKEYS+1)*8)+3))
#define BUF 200
#define RRN 158
#define DELIMITERCHAR ','

// TYPEDEFS ---------------------------------------------------------------------------------------------------------

typedef struct student_t{
    long lKey; 
    char sName[50];
    char sSurname[50];
    char sCourse[50];
    float fGrade;
} student_t;

typedef struct record_t {
    int lKey = 0;
    long lRRN;
} record_t;

typedef struct page_t {
    record_t *records;
    long *lChilds;
    short sNumberOfKeys;
    bool isLeaf;
} page_t;

typedef struct promotedkey {
    long lKey;
    long lRRN;
    long lChilds[2];
} promotedKey;

// FUNCTIONS ---------------------------------------------------------------------------------------------------------


page_t *getOrCreateRoot (FILE *p_file);
page_t *getRoot (FILE *p_file);
long bTreeSelect (page_t *page, int number, FILE *p_file);
record_t binSearch_Btree (FILE *p_file, long lKey);

int binFileSize (FILE *p_file);
int csvFileSize (FILE *p_file);
char readCurrentInput (FILE *p_file, char *currentInput);

student_t csvRead_Student (FILE *p_file);
student_t binRead_Student (FILE *p_file);
void binWrite_Student (student_t student, FILE *p_file);
void print_Student (student_t student);
student_t getStudentByKey(FILE *p_database, long lKey);

record_t binRead_Record (FILE *p_file);
void binWrite_Record (record_t record, FILE *p_file);
void print_Record (record_t record);

#endif