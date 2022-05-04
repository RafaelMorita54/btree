#include "btree.h"

// TREE FUNCTIONS ---------------------------------------------------------------

/*Retrives page from file pointer*/
page_t *readPageFromFile(FILE *p_file){
    //Aloca espaço para carregar página
    //Lê dados da página do arquivo
}

/*Writes page into file in certain rrn position*/
Errors writePageIntoFile(long lRRN, page_t *page, FILE *p_file){
    //Verifica se está tudo ok com os dados
    //Encontra local para escrita baseado no RRN
    //Escreve dados
    //Atualiza valor de espaço livre na página
    //Dica: você pode criar uma função que só lida com a escrita dos dados e chamar aqui
}

/*Get page by rrn*/
page_t *getPage(long lRRN, FILE *p_file){
    //Recupera uma página baseado no RRN
    //Procura e carrega seus dados
}

/*Get root RRN from header*/
long getTreeHeader(FILE *p_file){
    //Carrega o cabeçalho da árvore, que está no início do arquivo
}

/*Writes root RRN in header*/
void writeTreeHeader(FILE *p_file, long rootRRN){
    //Calcula espaço livre e escreve no cabeçalho da árvore, junto com o RRN do nó raíz
}

page_t *createTree(FILE *p_file){
    //Aloca espaço pra raiz
    //Inicializa os valores
    //Escreve a raiz no cabeçalho
}

page_t *getOrCreateRoot(FILE *p_file){
    //Verifica se a árvore já existe ou precisa criar uma nova
    //Se a árvore não existir, cria ela
    //Se existir, só pega o RRN da raiz no cabeçalho e carrega sua página
    //Pode ser adaptada pra inserção e busca sem precisar de 2 funções
}

promotedKey *insertIntoNode(page_t *page, promotedKey *newKey, FILE *p_file){
    //Procura local pra inserir nova chave na página
    //Se não couber, splitta ele
    //Escreve dados na página
}

/*Insert some key on page*/
page_t *searchPositionOnPageAndInsert(page_t *page, promotedKey *newKey){
    //Encontra a posição para inserir a chave
    //Se não existir espaço, precisa criar uma nova página (usem uma função para criar)
    //Salvar dados da nova chave na página
}
/*
    If page size is odd the return is biggest slice
*/
page_t *splitAndCreateNewNode(page_t **page){
    //Encontra a posição do meio das chaves
    //Cria espaço pra nova página
    //Copia metade das chaves pra página nova
    //Limpa elas do nó antigo
    //Atualiza os filhos de ambas as páginas
    //Atualiza o número de chaves de ambas
}

/*Extract a promoted key from page*/
promotedKey *extractpromotedKey(page_t *page){
    //Aloca espaço pra chave
    //Tira ela da página
    //Atualiza dados da página (filho, número de chaves, etc)
}

/*Split node and writes into file*/
promotedKey *_split(page_t *page,FILE *p_file, promotedKey *newKey){
    //Divide a página, cria o novo nó (faça numa função auxiliar pois é complexo)
    //Extrai a chave promovida e atualiza os filhos da chave
    //Escreve a página nova e a que foi dividida (com suas atualizações) no arquivo
}


/*Used if promotions reaches root*/
page_t *createNodeWithPromotedKey(promotedKey *promoKey){
    //Se promoção cria estrutura para nova raiz,
    //Aloca espaço para ela
    //Salva dados da chave promovida na nova raiz
    //atualiza os filhos deste novo nó/página
    //Pode reusar função que cria página nova e adicionar somente especificidades
}

Errors setNodeAsRoot(page_t *page, FILE *p_file){
    //Escreve página nova e atualiza o cabeçalho para conter ela como raiz
    //Deveria ser chamada junto com criação de novo nó quando promoção cria uma nova raiz
}

/*Recursive insertion*/
promotedKey *_bTreeInsert(page_t *node, promotedKey *key, FILE *p_file){
    //Se nó a ser inserido a chave é folha, tenta inserir
    //Caso a inserção crie uma promoção, precisa retornar a chave promovida para a recursão
    //Se não for nó folha, procura qual sub-árvore seguir para inserir numa folha
    //Encontrar a posição correta e descer para filho à esquerda se a chave for menor
    //E descer à direita se for maior
    //Chamar a inserção recursiva pro filho escolhido
    //Se a inserção recursiva retornar uma chave promovida, precisa tentar inserir essa chave promovida no nó atual
    //Retornar chave promovida ou um valor NULL se não houve promoção
}

Errors bTreeInsert(PrimaryIndex *newRecord, page_t *root, FILE *p_file){
    //Função mais abstrata de inserção
    //Prepara os dados da nova chave
    //Tenta inserir recursivamente
    //Se tiver chave promovida no final da recursão, é que existe nova raiz
    //Chama as funções pra criar nova raiz e atualizar o cabeçalho
}

/*Returns rrn if key exist else return -1*/
long bTreeSelect(page_t *node, int key, FILE *p_file){
    //Procura no nó atual se a chave existe
    //Se não existir, tenta procurar no filho adequado, recursivamente
    //Se encontrar a chave, retorna RRN dela
    //Se não encontrar (chegar num nó folha e não estiver lá), retorna -1
}

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