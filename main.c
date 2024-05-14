#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Element {
    int value;
    int row;
    int column;
    struct Element * rowElement; /* -> */
    struct Element * colElement; /* \|/ */
};

struct Header {
    int index;
    struct Header * header;
    struct Element * element;
};

struct Matrix {
    struct Header * headRowHeader;
    struct Header * headColHeader;

    int rows;
    int cols;
    int fillValue;
};

void blue_color() {
    printf("\033[0;34m");
}
void white_color() {
    printf("\033[0;37m");
}
void green_color() {
    printf("\033[0;32m");
}
void red_color() {
    printf("\033[1;31m");
}
void yellow_color() {
    printf("\033[1;33m");
}
void reset_color() {
    printf("\033[0m");
}
    
int print_header_ver(struct Header ** header) {
    struct Header * p;
    struct Element * element;
    int size = 0;

    if (!(*header)) {
        red_color();
        printf("Row header is empty\n");
        reset_color();

        return 0; 
    }
    else 
        for (p = (*header); p != NULL; p = p->header) { 
            size += sizeof(p); 
            
            red_color(); 
            printf("index: %d\n", p->index);
 
            yellow_color();
            for (element = p->element; element != NULL; element = element->colElement) {
                printf("val: %d\n", element->value);
                size += sizeof(element); 
            } 
            reset_color(); 
            printf("\n");
 
        }
    
    
    return size;
}

int print_header_hor(struct Header ** header) {
    struct Header * p;
    struct Element * element;
    int size = 0;

    if (!(*header)) {
        red_color();
        printf("Column header is empty\n");
        reset_color();

        return 0; 
    } 
    else  
        for (p = (*header); p != NULL; p = p->header) { 
            size += sizeof(p); 
            
            red_color(); 
            printf("index: %d\n", p->index);
 
            yellow_color();
            for (element = p->element; element != NULL; element = element->rowElement) {
                printf("val: %d\n", element->value);
                size += sizeof(element); 
            } 
            reset_color(); 
            printf("\n");
 
        }

    
    return size;
}

void add_row_header(struct Header **mtrx_column, struct Header * column_header, struct Element * element) {
    struct Header * p;
    struct Header * q;
    struct Element * p_element;
    int header_index = column_header->index;
    
    if (*mtrx_column == NULL) {
        *mtrx_column                = column_header;
        (*mtrx_column)->element     = element;
    }

    else {
        /* Edge cases */ 
        
        /* Prefix */
        if ((*mtrx_column)->index > header_index) {
            column_header->element      = element; /* Added */
            column_header->header       = (*mtrx_column);
            (*mtrx_column)              = column_header; 
        }    
     
        /* Infix - Matching - Postfix */
        else {
            p = (*mtrx_column); 

            while (p) {
                q = p->header;

                /* Infix */
                if (q && p->index < header_index && q->index > header_index) {
                    column_header->element      = element; /* Added */
                    p->header                   = column_header;
                    column_header->header       = q; 
                    break;
                }

                /* Matching */
                else if (p && p->index == header_index) {
                    p_element = p->element; /* ->rowElement */

                    while (p_element->colElement) 
                        p_element = p_element->colElement;

                    p_element->colElement = element;

                    free(column_header);
                    break;
                }

                /* Postfix */
                else if (p && p->header == NULL) {
                    column_header->element  = element;
                    p->header = column_header;
                    break;
                } 

                p = p->header; 
            }
        }
    }
}

/*add_col_header(mtrx.headColHeader, column_header, element);*/
void add_col_header(struct Header **mtrx_column, struct Header * column_header, struct Element * element) {
    struct Header * p;
    struct Header * q;
    struct Element * p_element;
    int header_index = column_header->index;
    
    if (*mtrx_column == NULL) {
        *mtrx_column                = column_header;
        (*mtrx_column)->element     = element;
    }

    else {
        /* Edge cases */ 
        
        /* Prefix */
        if ((*mtrx_column)->index > header_index) {
            column_header->element      = element; /* Added */
            column_header->header       = (*mtrx_column);
            (*mtrx_column)              = column_header; 
        }    
     
        /* Infix - Matching - Postfix */
        else {
            p = (*mtrx_column); 

            while (p) {
                q = p->header;

                /* Infix */
                if (q && p->index < header_index && q->index > header_index) {
                    column_header->element      = element; /* Added */
                    p->header                   = column_header;
                    column_header->header       = q; 
                    break;
                }

                /* Matching */
                else if (p && p->index == header_index) {
                    p_element = p->element; /* ->rowElement */

                    while (p_element->rowElement) 
                        p_element = p_element->rowElement;

                    p_element->rowElement = element;

                    free(column_header);
                    break;
                }

                /* Postfix */
                else if (p && p->header == NULL) {
                    column_header->element  = element;
                    p->header = column_header;
                    break;
                } 

                p = p->header; 
            }
        }
    }
}

void mtrx_print(struct Matrix * mtrx) {
    long int size = 0;
    
    size += print_header_hor(&(mtrx->headColHeader));

    printf("==========================\n\n");

    size += print_header_ver(&(mtrx->headRowHeader));

    printf("\n");

    blue_color();
    printf("Size of sparse mtrx: %ld\n", size);
    reset_color();
}

struct Matrix mtrx_alloc(char * filename, int fillValue) {
    /* Global Var(s) */ 
    FILE *in_file;
    struct Matrix mtrx;
    char ch; /* Used to be 'int' */
    
    int cols = 0;
    int rows = 1;

    /*int mtrx_cols = 0;
    int mtrx_rows = 0;*/

    struct Header * column_header;
    struct Header * row_header;
    struct Element * element;

    in_file = fopen(filename, "r"); /*read only*/

    if (in_file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }


    /* Configure Matrix */
    mtrx.headColHeader = NULL;
    mtrx.headRowHeader = NULL;

    while ((ch = fgetc(in_file)) != EOF) {

        if (isdigit(ch)) ++cols; /* Increment column */

        if (isdigit(ch) && (ch = ch-'0') != fillValue) {

            /* printf("%d: col=%d, row=%d\n", ch, col, row); */

            /* Configure Element */
            element                 = malloc(sizeof(struct Element)); 
            element->value          = ch;
            /*element->column         = cols;
            element->row            = rows;*/
            element->rowElement     = NULL;
            element->colElement     = NULL;

            /* Configure Column */
            column_header           = malloc(sizeof(struct Header));
            column_header->index    = cols;
            column_header->header   = NULL; 
            column_header->element  = NULL;
            
            /* Configure Row */
            row_header              = malloc(sizeof(struct Header));
            row_header->index       = rows;
            row_header->header      = NULL; 
            row_header->element     = NULL;

            add_col_header(&mtrx.headColHeader, column_header, element);
            add_row_header(&mtrx.headRowHeader, row_header, element);
        }

        else if (ch == '\n') {
            ++rows; /* Increment row */
            cols = 0;
        }

    }

    /* Configure Matrix */
    mtrx.rows       = rows;
    mtrx.cols       = cols;
    mtrx.fillValue  = fillValue;
    
    fclose(in_file);

    return mtrx;
}

void mtrx_set(struct Matrix *mtrx, int row, int col, int val) {
    int i;
    int j;

    struct Header *p;
    struct Element *e;

    p = mtrx->headColHeader;
    for (i=0; i<col-1; ++i) p = p->header;

    e = p->element;
    for (j=0; j<row-1; ++j) e = e->rowElement;

    printf("Element value: %d\n", e->value);
}

/*struct Element* mtrx_get_element(struct Matrix * mtrx, int row, int col) {
    struct Header * colHeader   = mtrx->headColHeader;
    struct Element * element;

    while (colHeader->index != col) {
        colHeader   = colHeader->header;
    } 
    element = colHeader->element;
    
    return element;
}*/ 

void mtrx_clear(struct Matrix * mtrx) {
    struct Header * colHeader            = mtrx->headColHeader;

    struct Element * element             = NULL;
    struct Element * prev_element        = NULL;
    
    struct Header * p;
    struct Header * prev_p;

    while (colHeader) {
        element         = colHeader->element;
        prev_element    = element;
        
        /* Go through Elements */
        if (element) {
            while (element) {
                prev_element    = element;
                element         = element->rowElement;
                free(prev_element);
            } 
        }
        else free(prev_element);  
        /* Change Header */ 
        colHeader       = colHeader->header;
    }

    p       = mtrx->headColHeader;
    prev_p  = p;
    while (p) {
        p = p->header;
        free(prev_p);
        prev_p = p; 
    }
    
    p       = mtrx->headRowHeader;
    prev_p  = p;
    while (p) {
        p = p->header;
        free(prev_p);
        prev_p = p; 
    }

    mtrx->headColHeader = NULL;
    mtrx->headRowHeader = NULL;
}


int main() {
    /* int argc, char *argv[] */
    struct Matrix mtrx = mtrx_alloc("matrix.txt", 0);
    
    /*green_color();
    printf("(1, 5) => %d, p: %p\n", mtrx_get_element(&mtrx, 1, 5)->value, (void *)mtrx_get_element(&mtrx, 1, 5));
    printf("(3, 1) => %d, p: %p\n", mtrx_get_element(&mtrx, 3, 1)->value, (void *)mtrx_get_element(&mtrx, 3, 1));
    printf("(3, 4) => %d, p: %p\n", mtrx_get_element(&mtrx, 3, 4)->value, (void *)mtrx_get_element(&mtrx, 3, 4));
    printf("(4, 3) => %d, p: %p\n", mtrx_get_element(&mtrx, 4, 3)->value, (void *)mtrx_get_element(&mtrx, 4, 3));
    printf("(4, 5) => %d, p: %p\n", mtrx_get_element(&mtrx, 4, 5)->value, (void *)mtrx_get_element(&mtrx, 4, 5));
    printf("(3, 3) => %d, p: %p\n", mtrx_get_element(&mtrx, 3, 3)->value, (void *)mtrx_get_element(&mtrx, 3, 3));
    reset_color();*/

    /* Print Mtrx contents */

    green_color();
    mtrx_set(&mtrx, 2, 3, 3);  
    reset_color();

    mtrx_print(&mtrx);
    
    /*blue_color();     
    printf("Size of simpler mtrx: %ld\n", sizeof(arr));
    reset_color();*/
    
    /* Clear Mtrx contents */
    mtrx_clear(&mtrx);
    
    return 0;
}
