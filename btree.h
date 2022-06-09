#ifndef btree
#define btree

// LIBRARIES --------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// #include <utils.h>
#include <btree.h>

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
    int iKey; 
    char sName[50];
    char sSurname[50];
    char sCourse[50];
    float fGrade;
} student_t;

typedef struct record_t {
    int iKey;
    long lRRN;
} record_t;

typedef struct page_t {
    long lPageRRN;
    record_t records[21];
    long lChilds[22];
    short sNumberOfKeys;
    bool isLeaf;
} page_t;

typedef struct promotedkey {
    int iKey;
    long lRRN;
    long lChilds[2];
} promotedKey;

typedef struct PrimaryIndex {
    int iIndexKey;
    long lIndexRRN;
} PrimaryIndex;

// FUNCTIONS ---------------------------------------------------------------------------------------------------------

page_t *readPageFromFile(FILE *p_file);
void writePageIntoFile(long lRRN, page_t *page, FILE *p_file); 
page_t *getPage(long lRRN, FILE *p_file);
int getNumberOfPages(FILE *p_file);
long getTreeHeader(FILE *p_file);
void writeTreeHeader(FILE *p_file, long rootRRN, int iNumberOfPages);
page_t *createTree(FILE *p_file);
page_t *getOrCreateRoot(FILE *p_file);
//promotedKey *insertIntoNode(page_t *page, promotedKey *newKey, FILE *p_file);
page_t *searchPositionOnPageAndInsert(page_t *page, promotedKey *newKey);
page_t *splitAndCreateNewNode(page_t *page);
promotedKey *extractpromotedKey(page_t *page);
//promotedKey *_split(page_t *page,FILE *p_file, promotedKey *newKey);
page_t *createNodeWithPromotedKey(promotedKey *promoKey);
void setNodeAsRoot(page_t *page, FILE *p_file);
promotedKey *_bTreeInsert(page_t *node, promotedKey *key, FILE *p_file);
void bTreeInsert(PrimaryIndex *newRecord, page_t *root, FILE *p_file, int *iNumberOfPages);
long bTreeSelect(page_t *node, int key, FILE *p_file);
void writeNewPageIntoFile(page_t *newPage, FILE *p_file);
void addTo_nPages(FILE *p_file);
void writePage(page_t page, FILE *p_file, long lRRN);
record_t newRecord(record_t record);
page_t new_Page(page_t page);
promotedKey new_PromoKey(promotedKey promoKey);

record_t binSearch_Btree (FILE *p_file, int iKey);

int binFileSize (FILE *p_file);
int csvFileSize (FILE *p_file);
char readCurrentInput (FILE *p_file, char *currentInput);

student_t csvRead_Student (FILE *p_file);
student_t binRead_Student (FILE *p_file);
void binWrite_Student (student_t student, FILE *p_file);
void print_Student (student_t student);
student_t getStudentByRRN(FILE *p_database, long lRRN);

record_t binRead_Record (FILE *p_file);
void binWrite_Record (record_t record, FILE *p_file);
// void print_Record (record_t record);

#endif