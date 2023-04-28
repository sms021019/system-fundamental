#include "hw7.h"

int compare_mats(char a, char b){
    if(a > b){
        return 1;
    }else{
        return 0;
    }
}

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    if(root == NULL){
        bst_sf* temp = malloc(sizeof(bst_sf));
        temp -> left_child = NULL;
        temp -> right_child = NULL;
        temp -> mat = mat;
        return temp;
    }
    
    if(mat -> name < root -> mat -> name){
        root -> left_child = insert_bst_sf(mat , root ->left_child);
    }else if(mat -> name > root -> mat -> name){
        root -> right_child = insert_bst_sf(mat , root ->right_child);
    }

    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if(root == NULL){
        return NULL;
    }
    if(root -> mat -> name == name){
        return root -> mat;
    }else if(name < root -> mat -> name){
        return find_bst_sf(name, root -> left_child);
    }else if(name > root -> mat -> name){
        return find_bst_sf(name, root -> right_child);
    }else{
        return NULL;
    }
}

void free_bst_sf(bst_sf *root) {

    if(root != NULL){
        
        if(root -> left_child != NULL){
            free_bst_sf(root -> left_child);
            
        }
        if(root -> right_child != NULL){
            free_bst_sf(root -> right_child);

        }
        free(root -> mat);
        free(root);
    }


    // if(root -> mat != NULL){
    //     free(root -> mat);
    // }
    // if(root -> left_child != NULL){
    //     free(root -> left_child);
    // }
    // if(root -> right_child != NULL){
    //     free(root -> right_child);
    // }
    // free(root);
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    matrix_sf* temp = malloc(sizeof(matrix_sf) + (mat1 -> num_rows) * (mat1 -> num_cols) * sizeof(int));
    int arrayLength = (mat1 -> num_cols) * (mat1 -> num_rows);
    for(int p = 0; p < (arrayLength); p++){
        temp-> values[p] = mat1->values[p] + mat2->values[p];
    }
    temp -> name = '0';
    temp -> num_rows = mat1 -> num_rows;
    temp -> num_cols = mat1 -> num_cols;
    return temp;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    matrix_sf *result = malloc(sizeof(matrix_sf) + (mat1 -> num_rows) * (mat2 -> num_cols) * sizeof(int));
    result -> name = '0';
    result -> num_rows = mat1 -> num_rows;
    result -> num_cols = mat2 -> num_cols;
    for(unsigned int i = 0; i < (result -> num_rows) * (result -> num_cols); i++){
        result -> values[i] = 0;
    }
    int p = 0;

    for(unsigned int i = 0; i < mat1->num_rows; i++){
        for(unsigned int j = 0; j < mat2->num_cols; j++){
            for(unsigned int k = 0; k < mat1 -> num_cols; k++){
                result -> values[p] += mat1->values[i*(mat1 -> num_cols)+k] * mat2 -> values[k*(mat2 -> num_cols) + j];
            }
            p++;
        }
    }


   return result;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    matrix_sf* result = malloc(sizeof(matrix_sf)+(mat -> num_rows) * (mat -> num_cols) * sizeof(int));
    result -> name = mat -> name;
    result -> num_cols = mat -> num_rows;
    result -> num_rows = mat -> num_cols;
    // int value_of_mat1[] = mat -> values;
    // int arrayLength = sizeof(value_of_mat1) / sizeof(int);
    // result -> values[arrayLength];
    for(unsigned int i = 0; i < (result -> num_rows) * (result -> num_cols); i++){
        result -> values[i] = 0;
    }
    int beforeTrans[mat -> num_rows][mat -> num_cols];
    int trans[result -> num_rows][result -> num_cols];
    int p = 0;
    for(unsigned int i = 0; i < (mat -> num_rows); i++){
        for(unsigned int j = 0; j < (mat -> num_cols); j++){{
            beforeTrans[i][j] = mat -> values[p++];
        }}
    }

    p = 0;

    for(unsigned int i = 0; i < (result -> num_rows); i++){
        for(unsigned int j = 0; j < (result -> num_cols); j++){{
            trans[i][j] = beforeTrans[j][i];
        }}
    }

    p=0;
    for(unsigned int i = 0; i < (result -> num_rows); i++){
        for(unsigned int j = 0; j < (result -> num_cols); j++){{
            result -> values[p++] = trans[i][j];
        }}
    }
    return result;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    int numArr[MAX_LINE_LEN];
    int n, count = 0;
    unsigned int num_rows;
    unsigned int num_cols;
    // char* p = malloc(strlen(expr) + 1);

    // if(p != NULL){
    //     strcpy(p, expr);
    // }
    n = 0;
    while (*expr) { // While there are more characters to process...
        if ( isdigit(*expr) || ( (*expr=='-'||*expr=='+') && isdigit(*(expr+1)) )) {
            // Found a number
            long val = strtol(expr, &expr, 10); // Read number ***** WARNING EXISTING *****
            if(n == 0){
                num_rows = val; // store row number
                // printf("num_rows: %d\n", num_rows);
                n++;
            }else if(n == 1){
                num_cols = val; // store col number
                // printf("num_cols: %d\n", num_cols);
                n++;
            }else{
                numArr[count++] = val; //store into the array
            }
        } else {
            // Otherwise, move on to the next character.
            expr++;
        }
    }

    matrix_sf* temp = malloc(sizeof(matrix_sf) + num_rows*num_cols*sizeof(int));
    temp -> name = name;
    temp -> num_rows = num_rows;
    temp -> num_cols = num_cols;

    for(n = 0; n < count; n++){
        // printf("%dth index in value array: %d\n", n, numArr[n]); // and print it.
        temp -> values[n] = numArr[n];
    }
    
    return temp;

}

int precedence(char operator)
{
    switch (operator){
        case '+':
            return 1;
        case '*':
            return 2;
        case '\'':
            return 3;
        default:
            return -1;
    }
}
int isOperator(char ch)
{
    return (ch == '+' || ch == '*' || ch == '\'');
}

char* infix2postfix_sf(char *infix) {
    int i, j;
    int len = strlen(infix);
    char* postfix = (char*)malloc(sizeof(char) * (len + 2));
    char stack[MAX_LINE_LEN];
    int top = -1;

    for(i = 0, j = 0; i < len; i++){
        if(infix[i] == ' ' || infix[i] == '\t') continue;
    
        if(isalnum(infix[i])) {
            postfix[j++] = infix[i];
        }

        else if(infix[i] == '('){
            stack[++top] = infix[i];
        }
        else if(infix[i] == ')'){
            while(top > -1 && stack[top] != '(')
                postfix[j++] = stack[top--];
            if(top > -1 && stack[top] != '(')
                return "Invalid Expression";
            else
                top--;
        }

        else if(isOperator(infix[i])){
            while(top > -1 && precedence(stack[top]) >= precedence(infix[i]))
                postfix[j++] = stack[top--];
            stack[++top] = infix[i];
        }

    }

    while(top > -1){
        if(stack[top] == '('){
            return "Invalid Expression";
        }
        postfix[j++] = stack[top--];
    }
    postfix[j] = '\0';
    return postfix;
}

void printMat(matrix_sf * stack[], int top){
    int i = 0;
    printf("\nSTACK LIST:\n");
    while(i <= top){
        printf("- %dth stack -\n", i);
        printf("NAME: %c\n", stack[i] -> name);
        printf("# of rows: %d\n", stack[i] -> num_rows);
        printf("# of cols: %d\n\n", stack[i] -> num_cols);
        i++;
    }
    // printf("Top of Stack\nName of matrix: %c\n",mat -> name);
    // printf("Num_rows of matrix: %d\n", mat -> num_rows);
    // printf("Num_cols of matrix: %d\n", mat -> num_cols);
    
}

// matrix_sf* deepcpy_mat(matrix_sf *source , matrix_sf *dest){

// }

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char* prefix = infix2postfix_sf(expr);
    matrix_sf* stack[MAX_LINE_LEN];
    long unsigned int i;
    int top = -1;
    for(i = 0; i < strlen(prefix); i++){
        if(isOperator(prefix[i])){
            if(prefix[i] == '\''){
                // printf("transpose start\n");
                matrix_sf *temp1 = stack[top--];
                matrix_sf *temp = transpose_mat_sf(temp1);
                if(temp1 -> name == '0'){
                    free(temp1);
                }
                temp -> name = '0';
                stack[++top] = temp;
                // printMat(stack, top);
            }else if(prefix[i] == '+'){
                // printf("addition start\n");

                matrix_sf *temp1 = stack[top--];
                matrix_sf *temp2 = stack[top--];
                matrix_sf *temp = add_mats_sf(temp2, temp1);
                if(temp1 -> name == '0'){
                    free(temp1);
                }
                if(temp2 -> name == '0'){
                    free(temp2);
                }
                
                stack[++top] = temp;
                
            }else if(prefix[i] == '*'){
                matrix_sf *temp1 = stack[top--];
                matrix_sf *temp2 = stack[top--];
                matrix_sf *temp = mult_mats_sf(temp2, temp1);
                if(temp1 -> name == '0'){
                    free(temp1);
                }
                if(temp2 -> name == '0'){
                    free(temp2);
                }
                stack[++top] = temp;
            }

        }else if(isalnum(prefix[i])){
            if(find_bst_sf(prefix[i],root) != NULL){
                stack[++top] = find_bst_sf(prefix[i],root);
                
            }
        }
    }
    
    matrix_sf* result = copy_matrix(stack[top] -> num_rows, stack[top] -> num_cols, stack[top] -> values);
    free(stack[top]);
    result -> name = name;
    free(prefix);
    
    // for(int j = 0; j < top; j++){
    //     if(stack[j] -> name == '0'){
    //         free(stack[j]);
    //     }
    // }
    // printf("- result matrix -\n");
    // printf("NAME: %c\n", result -> name);
    // printf("# of rows: %d\n", result -> num_rows);
    // printf("# of cols: %d\n\n", result -> num_cols);
    // if(result -> name == 'R'){
    //     for(int j = 0; j < 12; j++){
    //         printf("%d\n", result -> values[j]);
    //     }
    // }
    
    return result;
}

matrix_sf *execute_script_sf(char *filename) {
    char *str = NULL;
    FILE *file = fopen(filename, "r"); 
    size_t max_line_size = MAX_LINE_LEN;
    bst_sf *tree = NULL;
    // matrix_sf *result = malloc(sizeof(matrix_sf));
    matrix_sf *result;
    while(getline(&str, &max_line_size, file) != -1){
        char * copy = malloc(max_line_size);
        strcpy(copy, str);
        // printf("current line: %s\n", copy);
        if(strstr(copy, "[") != NULL){
            // printf("Creating new matrix\n");
            char * name;
            char * expr;
            name = strtok(str, "=");
            expr = strtok(NULL, "=");
            // printf("Expression of new matrix: %s\n", expr);
            matrix_sf *temp_mat = create_matrix_sf(*name, expr);
            tree = insert_bst_sf(temp_mat, tree); // What do i write for the second argument of insert_bst_sf, which is the pointer of bst_sf? 
            result = temp_mat;
        }else{
            // printf("Operating new matrix\n");
            char * name;
            char * expr;
            name = strtok(str, "=");
            expr = strtok(NULL, "=");
            matrix_sf * temp_mat = evaluate_expr_sf(*name, expr, tree);
            tree = insert_bst_sf(temp_mat, tree);
            result = temp_mat;
        }
        
        free(copy);
    }
    
    free(str);
    fclose(file);
    matrix_sf * cpy = malloc(sizeof(matrix_sf) + (result -> num_cols) * (result -> num_rows) * sizeof(int));
    cpy -> name = result -> name;
    cpy -> num_rows = result -> num_rows;
    cpy -> num_cols = result -> num_cols;
    
    for(int unsigned i = 0; i < (cpy -> num_cols * cpy -> num_rows); i++){
        cpy -> values[i] = result -> values[i];
    }
    free_bst_sf(tree);
    return cpy;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
