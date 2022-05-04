#include "btree.c"

int main () {

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

    while (strcmp(input, "exit") != 0) {
        scanf("%s", input); // Leitura da próxima operação

        if (strcmp(input, "insert") == 0) { //Insert

            student_t student;
            record_t record;

            student = csvRead_Student (stdin);
            record = binSearch_Btree (p_btree, student.lKey);
            
            if (record.lKey == student.lKey) {
                printf("O registro ja existe!\n");
            } else {
                // Write Record in Btree
                binWrite_Student (student, p_database);
            }
        };

        if (strcmp(input, "search") == 0) { //Search

            student_t student;
            record_t record;
            long searchKey;

            scanf("%li", &searchKey);
            record = binSearch_Btree (p_btree, searchKey);
            
            if (record.lKey == 0) {
                printf("Registro nao encontrado!\n");
            } else {
                student = getStudentByKey(p_database, record.lKey);
                print_Student(student);
            }
        }

        if (strcmp(input, "update") == 0) { //Update

            student_t student;
            record_t record;

            student = csvRead_Student (stdin);
            record = binSearch_Btree (p_btree, student.lKey);
            
            if (record.lKey == 0) {
                printf("Registro nao encontrado!\n");
            } else {
                binWrite_Student (student, p_database);
            }
        }
    }

    fclose(p_database); // Fecha o arquivo binario;
    fclose(p_btree); // Fecha o arquivo binario;

    return 0;
}