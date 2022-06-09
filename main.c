#include "btree.h"

int main (void) {

    FILE  *p_database, *p_btree;


    p_database = fopen("database.bin", "w+");
    p_btree = fopen("btree.bin", "w+");

    if (p_database == NULL){
        return 0;
    }

    if (p_btree == NULL){
        return 0;
    }

    char input[50];
    int iNumberOfStudents = 0;
    int *iNumberOfPages = 0;

    while (strcmp(input, "exit") != 0) {
        scanf("%s", input); // Leitura da próxima operação
        if (strcmp(input, "insert") == 0) { //Insert

            student_t student;
            record_t record;

            student = csvRead_Student (stdin);
            record = binSearch_Btree (p_btree, student.iKey);
            
            if (record.iKey == student.iKey) {
                printf("O registro ja existe!\n");
            } else {
                // Write student into database
                record.iKey = student.iKey;
                record.lRRN = iNumberOfStudents*(sizeof(student_t));
                fseek(p_database, record.lRRN, SEEK_SET);
                binWrite_Student (student, p_database);
                iNumberOfStudents++;
                // Write Record in Btree
                page_t *root = getOrCreateRoot(p_btree);
                PrimaryIndex *primaryKey = malloc(sizeof(PrimaryIndex));
                primaryKey->iIndexKey = record.iKey;
                primaryKey->lIndexRRN = record.lRRN;
                bTreeInsert(primaryKey, root, p_btree, iNumberOfPages);
            }
        }

        if (strcmp(input, "search") == 0) { //Search

            student_t student;
            record_t record;
            int searchKey;

            scanf("%d", &searchKey);
            record = binSearch_Btree (p_btree, searchKey);
            
            if (record.lRRN == -1) {
                printf("Registro nao encontrado!\n");
            } else {
                student = getStudentByRRN(p_database, record.lRRN);
                print_Student(student);
            }
        }

        if (strcmp(input, "update") == 0) { //Update

            student_t student;
            record_t record;

            student = csvRead_Student (stdin);
            record = binSearch_Btree (p_btree, student.iKey);
            
            if (record.lRRN == -1) {
                printf("Registro nao encontrado!\n");
            } else {
                fseek(p_database, record.lRRN, SEEK_SET);
                binWrite_Student (student, p_database);
            }
        }
    }

    fclose(p_database); // Fecha o arquivo binario;
    fclose(p_btree); // Fecha o arquivo binario;

    return 0;
}