# Using Sparse Matrix for data compression

Author: [Andrew Gyakobo](https://github.com/Gyakobo)

This project aims utilize a sparse matrix as form of matrix or image value compression by basically implementing a special kind of data structure where it basically omits one continuously recurring value ultimately saving space only for "important" variables. 

## Introduction

Giving a sample matrix of numbers:

 0 | 0 | 0 | 0 | 9 | 0 |
---|---|---|---|---|---|
 0 | 0 | 0 | 0 | 0 | 0 |
 4 | 0 | 0 | 8 | 0 | 0 |
 0 | 0 | 2 | 0 | 4 | 0 |

We immediately notice the overwhelming issue with this matrix that it has too many 0's which don't necessarilly need to use up space hence they could just be left out. 

The Sparse Matrix's compression rate fully depends on the rate of the recurrent value is being omitted. 

##  Review of the Sparse Matrix Data Structure

1. As the program scans the [txt file](https://github.com/Gyakobo/sparse_matrix/matrix.txt) row by row each non-omitted number is subsequently stored in a sample "node" in a struct called `Element`. Each essential value is then saved in the `value` integer in struct `Element`. The code snippet is as follows:

```c
struct Element {
    int value;
    int row;
    int column;
    struct Element * rowElement; // Pointer to next row Element
    struct Element * colElement; // Pointer to next column Element
};
```
2. Afterwards, after the `Element` object is created, alongside it two more objects are created as well. Here this is where we'd have to utilize pointers and a little bit of our imagination. Using `malloc` we allocate space for (basically create) two `Header` structs, each of which would be pointing to both the aforementioned created element and the next `Header` element. If you haven't noticed already we're creating a table of sorts with specific column and row objects pointing to said element objects.

```c
struct Header {
    int index;
    struct Header * header;
    struct Element * element;
};
```

3. The entire structure can then be saved. 

```c
struct Matrix {
    struct Header * headRowHeader;
    struct Header * headColHeader;

    int rows;
    int cols;
    int fillValue;
};
```

