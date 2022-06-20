#include "btree.h"

// TREE FUNCTIONS ---------------------------------------------------------------

/*Retrives page from file pointer*/
page_t *readPageFromFile(FILE *p_file){
    //Aloca espaço para carregar página
    page_t *page = malloc(sizeof(page_t));
    //Lê dados da página do arquivo
    fread(&page->lPageRRN, sizeof(long), 1, p_file);
    for(int i=0; i<101; i++){
        fread(&page->records[i], sizeof(record_t), 1, p_file);
    }
    for(int i=0; i<102; i++){
        fread(&page->lChilds[i], sizeof(long), 1, p_file);
    }
    fread(&page->sNumberOfKeys, sizeof(short), 1, p_file);
    fread(&page->isLeaf, sizeof(bool), 1, p_file);
    return page;
}


/*Writes page into file in certain rrn position*/
void writePageIntoFile(long lRRN, page_t *page, FILE *p_file){
    //Verifica se está tudo ok com os dados
    //Encontra local para escrita baseado no RRN
    fseek(p_file, lRRN, SEEK_SET);
    //Escreve dados
    fwrite(&page->lPageRRN, sizeof(long), 1, p_file);
    for(int i=0; i<101; i++){
        fwrite(&page->records[i].iKey, sizeof(int), 1, p_file);
        fwrite(&page->records[i].lRRN, sizeof(long), 1, p_file);
    }
    for(int i=0; i<102; i++){
        fwrite(&page->lChilds[i], sizeof(long), 1, p_file);
    }
    fwrite(&page->sNumberOfKeys, sizeof(short), 1, p_file);
    fwrite(&page->isLeaf, sizeof(bool), 1, p_file);
    //Atualiza valor de espaço livre na página
    //Dica: você pode criar uma função que só lida com a escrita dos dados e chamar aqui
}


void writeNewPageIntoFile(page_t *newPage, FILE *p_file){
    addTo_nPages(p_file);
    int iNumberOfPages = getNumberOfPages(p_file);
    newPage->lPageRRN = RRN + (iNumberOfPages*sizeof(page_t));
    writePageIntoFile(newPage->lPageRRN, newPage, p_file);
}


/*Get page by rrn*/
page_t *getPage(long lRRN, FILE *p_file){
    //Recupera uma página baseado no RRN
    fseek(p_file, lRRN, SEEK_SET);
    //Procura e carrega seus dados
    return  (readPageFromFile(p_file));
}


/*Get root RRN from header*/
long getTreeHeader(FILE *p_file){
    //Carrega o cabeçalho da árvore, que está no início do arquivo
    fseek(p_file, 0, SEEK_SET);
    long lHeaderRRN;
    fread(&lHeaderRRN, sizeof(long), 1, p_file);
    return lHeaderRRN;
}

/*Writes root RRN in header*/
void writeTreeHeader(FILE *p_file, long rootRRN, int iNumberOfPages){
    //Calcula espaço livre e escreve no cabeçalho da árvore, junto com o RRN do nó raíz
    fseek(p_file, 0, SEEK_SET);
    fwrite(&rootRRN, sizeof(long), 1, p_file);
    fwrite(&iNumberOfPages, sizeof(int), 1, p_file);
}


// pega o numero de paginas do cabeçalho
int getNumberOfPages(FILE *p_file){
    int iNumberOfPages;
    fseek(p_file, sizeof(long), SEEK_SET);
    fread(&iNumberOfPages, sizeof(int), 1, p_file);
    return iNumberOfPages;
}


// adiciona 1 ao numero de paginas do cabeçalho
void addTo_nPages(FILE *p_file){
    int iNumberOfPages = getNumberOfPages(p_file);
    iNumberOfPages++;
    fseek(p_file, sizeof(long), SEEK_SET);
    fwrite(&iNumberOfPages, sizeof(int), 1, p_file);
}


// cria árvore
page_t *createTree(FILE *p_file){
    //Aloca espaço pra raiz
    page_t *page = malloc(sizeof(page_t));
    page = new_Page(page);
    //Inicializa os valores
    //Escreve a raiz no cabeçalho
    //escreve a pagina raiz no arquivo
    writePageIntoFile(RRN, page, p_file);
    writeTreeHeader(p_file, RRN, 0);
    return page;
}


// Pega a raiz, se não existi cria ela
page_t *getOrCreateRoot(FILE *p_file){
    page_t *page = malloc(sizeof(page_t));
    //Verifica se a árvore já existe ou precisa criar uma nova
    if(p_file == NULL){
        //Se a árvore não existir, cria ela
        page = createTree(p_file);
        return page;
    }
    //Se existir, só pega o RRN da raiz no cabeçalho e carrega sua página
    else{
        long rootRRN = getTreeHeader(p_file);
        return (getPage(rootRRN, p_file));
    }
    //Pode ser adaptada pra inserção e busca sem precisar de 2 funções
}


/*Insert some key on page*/
page_t *searchPositionOnPageAndInsert(page_t *page, promotedKey *newKey){
    //Encontra a posicao para inserir a chave
    if(page->sNumberOfKeys==0){
    //se estiver vazia insere na primeira pos.
        page->records[0].iKey = newKey->iKey;
        page->records[0].lRRN = newKey->lRRN;
        page->lChilds[0] = newKey->lChilds[0];
        page->lChilds[1] = newKey->lChilds[1];
    }
    else if(newKey->iKey < page->records[0].iKey){
    // se a chave é menor que a da primeira pos.
        //move chaves e filhos
        for(int j=99; j>=0; j--){
            page->records[j+1].iKey = page->records[j].iKey;
            page->records[j+1].lRRN = page->records[j].lRRN;
            page->lChilds[j+2] = page->lChilds[j+1];
        }
        //insere na primeira pos
        page->records[0].iKey = newKey->iKey;
        page->records[0].lRRN = newKey->lRRN;
        page->lChilds[1] = newKey->lChilds[1];
    }
    else if(newKey->iKey < page->records[page->sNumberOfKeys-1].iKey){
    //senão encontra onde colocar
        int i = 0;
        while(i<page->sNumberOfKeys-1){
            if((newKey->iKey > page->records[i].iKey) && (newKey->iKey < page->records[i+1].iKey)){
                //move as chaves para colocar a newKey
                for(int j=page->sNumberOfKeys-1; j>i; j--){
                    page->records[j+1].iKey = page->records[j].iKey;
                    page->records[j+1].lRRN = page->records[j].lRRN;
                    if(j<=page->sNumberOfKeys-1){
                        page->lChilds[j+2] = page->lChilds[j+1];
                    }
                }
                page->records[i+1].iKey = newKey->iKey;
                page->records[i+1].lRRN = newKey->lRRN;
                page->lChilds[i+2] = newKey->lChilds[1];
                break;
            }
            i++;
        }
    }
    else{
    // senão insere na ultima pos
        page->records[page->sNumberOfKeys].iKey = newKey->iKey;
        page->records[page->sNumberOfKeys].lRRN = newKey->lRRN;
        page->lChilds[page->sNumberOfKeys+1] = newKey->lChilds[1];
    }
    //Se não existir espaço, precisa criar uma nova página (usem uma função para criar)
    //Salvar dados da nova chave na página
    page->sNumberOfKeys++;
    return page;
}
/*
    If page size is odd the return is biggest slice
*/


// Splita página cheia
page_t *splitAndCreateNewNode(page_t *page){
    //Encontra a posição do meio das chaves
    //Cria espaço pra nova página
    page_t *newPage = malloc(sizeof(page_t));
    newPage = new_Page(newPage);
    record_t recordClean;
    recordClean = newRecord(recordClean);
    //Copia metade das chaves pra página nova
    for(int i=51; i<101; i++){
        int j=0;
        newPage->records[j].iKey = page->records[i].iKey;
        newPage->records[j].lRRN = page->records[i].lRRN;
        page->records[i].iKey = recordClean.iKey;
        page->records[i].lRRN = recordClean.lRRN;
        j++;
    }
    //Atualiza os filhos de ambas as páginas
    for(int i=51; i<102; i++){
        int j=0;
        newPage->lChilds[j] = page->lChilds[i];
        j++;
        page->lChilds[i] = -1;
    }
    //Limpa elas do nó antigo
    //Atualiza o número de chaves de ambas
    page->sNumberOfKeys = 51;
    newPage->sNumberOfKeys = 50;
    newPage->isLeaf = page->isLeaf;
    return newPage;
}


/*Extract a promoted key from page*/
promotedKey *extractpromotedKey(page_t *page){
    //Aloca espaço pra chave
    promotedKey *extractedKey = malloc(sizeof(promotedKey));
    extractedKey = new_PromoKey(extractedKey);
    //Tira ela da página
    extractedKey->iKey = page->records[50].iKey;
    extractedKey->lRRN = page->records[50].lRRN;
    extractedKey->lChilds[0] = page->lPageRRN;
    //Atualiza dados da página (filho, número de chaves, etc)
    record_t recordClean;
    recordClean = newRecord(recordClean);
    page->records[50].iKey = recordClean.iKey;
    page->records[50].lRRN = recordClean.lRRN;
    page->lChilds[50] = page->lChilds[51];
    page->lChilds[51] = -1;
    page->sNumberOfKeys--;
    return extractedKey;
}


/*Used if promotions reaches root*/
page_t *createNodeWithPromotedKey(promotedKey *promoKey){
    //Se promoção cria estrutura para nova raiz,
    //Aloca espaço para ela
    page_t *newPage = malloc(sizeof(page_t));
    newPage = new_Page(newPage);
    newPage->records[0].iKey = promoKey->iKey;
    newPage->records[0].lRRN = promoKey->lRRN;
    newPage->lChilds[0] = promoKey->lChilds[0];
    newPage->lChilds[1] = promoKey->lChilds[1];
    newPage->isLeaf = false;
    return newPage;
    //Salva dados da chave promovida na nova raiz
    //atualiza os filhos deste novo nó/página
    //Pode reusar função que cria página nova e adicionar somente especificidades
}


// Setta uma pagina como raiz
void setNodeAsRoot(page_t *page, FILE *p_file){
    //Escreve página nova e atualiza o cabeçalho para conter ela como raiz
    fseek(p_file, 0, SEEK_SET);
    fwrite(&page->lPageRRN, sizeof(long), 1, p_file);
    //Deveria ser chamada junto com criação de novo nó quando promoção cria uma nova raiz
}


/*Recursive insertion*/
promotedKey *_bTreeInsert(page_t *node, promotedKey *key, FILE *p_file){
    // checa se a chave é inválida
    if(key->iKey==0){
        printf("ERRO: chave invalida\n");
        return key;
    }
    //Se nó a ser inserido a chave é folha, tenta inserir
    if(node->isLeaf){
        return insertAndReturnsPromotedKeyIfNeeded(node, key, p_file);
        // se tiver promoção retorna a chave promovida, senão retorna chave limpa
    }
    page_t *childNode = malloc(sizeof(page_t));
    //Se não for nó folha, procura qual sub-árvore seguir para inserir numa folha
    if(key->iKey < node->records[0].iKey){
    //tenta inserir na primeira subarvore
        childNode = getPage(node->lChilds[0], p_file);
        key = _bTreeInsert(childNode, key, p_file);
        // checa se a chave inserida é limpa ou promovida
        if(key->iKey != 0){
            // se promvida insere no nó autal
            // se essa inserção causar uma promoção a chave promovida vai ser retornada
            return insertAndReturnsPromotedKeyIfNeeded(node, key, p_file);
        }
        else{
            // senão só retorna a chave vazia
            return key;
        }
    }
    for(int i=0; i < node->sNumberOfKeys; i++){
    //tenta inserir numa subarvore do meio
        if((key->iKey<node->records[i].iKey) && (key->iKey>node->records[i+1].iKey)){
            childNode = getPage(node->lChilds[i+1], p_file);
            key = _bTreeInsert(childNode, key, p_file);
            // checa se a chave inserida é limpa ou promovida
            if(key->iKey != 0){
                // se promvida insere no nó autal
                // se essa inserção causar uma promoção a chave promovida vai ser retornada
                return insertAndReturnsPromotedKeyIfNeeded(node, key, p_file);
            }
            else{            
                // senão só retorna a chave vazia
                return key;        
            }
        }
    }
    // senão insere na ultima subarvore
    childNode = getPage(node->lChilds[node->sNumberOfKeys+1], p_file);
    key = _bTreeInsert(childNode, key, p_file);
    // checa se a chave inserida é limpa ou promovida
    if(key->iKey != 0){                   
        // se promvida insere no nó autal
        // se essa inserção causar uma promoção a chave promovida vai ser retornada
        return insertAndReturnsPromotedKeyIfNeeded(node, key, p_file);
    }
    else{
        // senão só retorna a chave vazia
        return key;
    }
}

  
// função auxiliar para inserir
promotedKey *insertAndReturnsPromotedKeyIfNeeded(page_t *page, promotedKey *key, FILE *p_file){
    searchPositionOnPageAndInsert(page, key);
    // se a pagina estiver cheia, divide e retorna chave promovida
    if(page->sNumberOfKeys==101){
        page_t *newPage = malloc(sizeof(page_t));
        newPage = splitAndCreateNewNode(page);
        key = extractpromotedKey(page);
        writePageIntoFile(page->lPageRRN, page, p_file);
        writeNewPageIntoFile(newPage, p_file);
        key->lChilds[1] = newPage->lPageRRN;
        return key;
    }
    // senão retorna chave vazia
    else{
        writePageIntoFile(page->lPageRRN, page, p_file);
        promotedKey *clearKey = malloc(sizeof(promotedKey));
        clearKey = new_PromoKey(clearKey);
        return clearKey;
    }
}


// Função mais abstrata de inserção
void bTreeInsert(PrimaryIndex inputKey, page_t *root, FILE *p_file){
    //Prepara os dados da nova chave
    promotedKey *newKey = malloc(sizeof(promotedKey));
    newKey = new_PromoKey(newKey);
    newKey->iKey = inputKey.iIndexKey;
    newKey->lRRN = inputKey.lIndexRRN;
    //Tenta inserir recursivamente
    newKey = _bTreeInsert(root, newKey, p_file);
    //Se tiver chave promovida no final da recursão, é que existe nova raiz
    if(newKey->iKey != 0){
        //Chama as funções pra criar nova raiz e atualizar o cabeçalho
        page_t *newPage = malloc(sizeof(page_t));
        newPage = createNodeWithPromotedKey(newKey);
        writeNewPageIntoFile(newPage, p_file);
        setNodeAsRoot(newPage, p_file);
    }
    //senao foi inserida na raiz
}


// Busca recursiva
long bTreeSelect(page_t *node, int iKey, FILE *p_file){
    //Procura no nó atual se a chave existe
    for(int i=0; i<node->sNumberOfKeys; i++){
        if(iKey==node->records[i].iKey){
            return node->records[i].lRRN; 
        }
        //Se não existir, tenta procurar no filho adequado, recursivamente
        if((!node->isLeaf) && (iKey<node->records[i].iKey) && (iKey>node->records[i+1].iKey)){
            node = getPage(node->lChilds[i+1], p_file);
            return bTreeSelect(node, iKey, p_file);
        }
        if((!node->isLeaf) && (iKey<node->records[0].iKey)){
            node = getPage(node->lChilds[0], p_file);
            return bTreeSelect(node, iKey, p_file);
        }
        if((!node->isLeaf) && (iKey>node->records[node->sNumberOfKeys].iKey)){
            node = getPage(node->lChilds[node->sNumberOfKeys], p_file);
            return bTreeSelect(node, iKey, p_file);
        }
    }    
    return -1;
    //Se encontrar a chave, retorna RRN dela
    //Se não encontrar (chegar num nó folha e não estiver lá), retorna -1
} /*Returns rrn if key exist else return -1*/


// Função abstrata de busca
record_t binSearch_Btree (FILE *p_file, int iKey) {
    record_t record;
    record.iKey = iKey;
    long rootRRN = getTreeHeader(p_file);
    page_t *pRoot = getPage(rootRRN, p_file);
    record.lRRN = bTreeSelect(pRoot, iKey, p_file);
    return record;
}


// Inicializa valores para uma pagina limpa
page_t *new_Page(page_t *page){
    for (size_t i = 0; i < 101; i++)
    {
        record_t record;
        page->records[i] = newRecord(record);
    }
    for (size_t i = 0; i < 102; i++)
    {
        page->lChilds[i] = -1;
    }
    page->sNumberOfKeys = 0;
    page->lPageRRN = RRN;
    page->isLeaf = true;
    return page;
}


// Inicializa valores para record limpo
record_t newRecord(record_t record){
    record.iKey = 0;
    record.lRRN = -1;
    return record;
}


// Inicializa valores para chave limpa
promotedKey *new_PromoKey(promotedKey *key){
    key->iKey = 0;
    key->lRRN = -1;
    key->lChilds[0] = -1;
    key->lChilds[1] = -1;
    return key;
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
    student.iKey = atoi(currentInput);

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

    fread(&student.iKey, sizeof(int), 1, p_file);
    fread(&student.sName, 50, 1, p_file);
    fread(&student.sSurname, 50, 1, p_file);
    fread(&student.sCourse, 50, 1, p_file);
    fread(&student.fGrade, sizeof(float), 1, p_file);

    return student;
}

void binWrite_Student (student_t student, FILE *p_file){

    fwrite(&student.iKey, sizeof(int), 1, p_file);
    fwrite(&student.sName, 50, 1, p_file);
    fwrite(&student.sSurname, 50, 1, p_file);
    fwrite(&student.sCourse, 50, 1, p_file);
    fwrite(&student.fGrade, sizeof(float), 1, p_file);

}

void print_Student (student_t student){
    printf("-------------------------------\n");
    printf("nUSP: %d\n", student.iKey);
    printf("Nome: %s\n", student.sName);
    printf("Sobrenome: %s\n", student.sSurname);
    printf("Curso: %s\n", student.sCourse);
    printf("Nota: %.2f\n", student.fGrade);
    printf("-------------------------------\n");
}

student_t getStudentByRRN(FILE *p_file, long lRRN) {
    student_t student;
    fseek(p_file, lRRN, SEEK_SET);
    student=binRead_Student(p_file);
    return student;
}


// RECORD FUNCTIOS  ---------------------------------------------------------------

record_t binRead_Record (FILE *p_file) {

    record_t record;

    fread(&record.iKey, sizeof(int), 1, p_file);
    fread(&record.lRRN, sizeof(int), 1, p_file);

    return record;
}

void binWrite_Record (record_t record, FILE *p_file){

    fwrite(&record.iKey, sizeof(int), 1, p_file);
    fwrite(&record.lRRN, sizeof(int), 1, p_file);
}

// void printRecord (record_t record){
//     printf("-------------------------------\n");
//     printf("nUSP: %d\n", record.iKey);
//     printf("RRN: %d\n", record.lRRN);
//     printf("-------------------------------\n");
// }