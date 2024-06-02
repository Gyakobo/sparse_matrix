# Using a Sparse Matrix for lossless data compression

![image](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![image](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![image](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![image](https://img.shields.io/badge/windows%20terminal-4D4D4D?style=for-the-badge&logo=windows%20terminal&logoColor=white)

Author: [Andrew Gyakobo](https://github.com/Gyakobo)

This project aims utilize a sparse matrix as form of matrix or image value compression by basically implementing a special kind of data structure where it omits one continuously recurring value ultimately saving space only for "important" variables. 

>[!NOTE]
>Just a small personal note and a small gag per say, this program is written in clean C and has no errors or warnings. The program was run and precompiled into a `.exe` with the following bash command:

```bash
$ sudo gcc -ansi -Wpedantic -Wextra -Wall main.c -o exe
```

## Introduction

Giving a sample matrix of numbers:

 0 | 0 | 0 | 8 | 0 | 0 | 0 | 3 | 0 | 0 | 0 |
---|---|---|---|---|---|---|---|---|---|---|
 0 | 0 | 0 | 0 | 0 | 3 | 0 | 0 | 0 | 0 | 8 |
 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 0 | 0 | 7 | 0 | 0 | 0 | 0 | 5 | 0 | 0 | 0 |
 0 | 0 | 0 | 8 | 0 | 0 | 0 | 0 | 0 | 8 | 0 |

We immediately notice the overwhelming issue with this matrix that it has too many 0's which don't necessarilly need to use up space hence they could just be left out. 

The Sparse Matrix's compression rate fully depends on the rate of the recurrent value is being omitted. 

##  Review of the Sparse Matrix Data Structure

1. As the program scans the [txt file](https://github.com/Gyakobo/sparse_matrix/blob/master/matrix.txt) row by row each non-omitted number is subsequently stored in a sample "node" in a struct called `Element`. Each essential value is then saved in the `value` integer in struct `Element`. The code snippet is as follows:

```c
struct Element {
    int value;
    int row;
    int column;
    struct Element * rowElement; // Pointer to next row Element
    struct Element * colElement; // Pointer to next column Element
};
```
2. Afterwards, after the `struct Element` object is created, alongside it two more concomitant objects are created as well. Here this is where we'd have to utilize pointers and a little bit of our imagination. Using `void* malloc` we allocate space for (basically create) two `struct Header` structs, each of which would be pointing to both the aforementioned created element and the next `struct Header` element. If you haven't noticed already we're creating a table of sorts with specific column and row objects pointing to said element objects. The `struct Header` objects would also connect to each other using once again pointers `struct Header * header;`

```c
struct Header {
    int index;
    struct Header * header;
    struct Element * element;
};
```

3. The entire structure can then be saved by just having two pointers to the column and row headers: 
```c
struct Header * headRowHeader;
struct Header * headColHeader;
```

The headers in their place would contain the said element structure. We could also add other elements to the `struct Matrix` like number of rows, columns, and the arbitrary recurrent value that would be omitted.

```c
struct Matrix {
    struct Header * headRowHeader;
    struct Header * headColHeader;

    int rows;
    int cols;
    int fillValue;
};
```
## Diagram of a sparse matrix

To hearken back to the matrix back in our introduction: 
 
 0 | 0 | 0 | 8 | 0 | 0 | 0 | 3 | 0 | 0 | 0 |
---|---|---|---|---|---|---|---|---|---|---|
 0 | 0 | 0 | 0 | 0 | 3 | 0 | 0 | 0 | 0 | 8 |
 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 0 | 0 | 7 | 0 | 0 | 0 | 0 | 5 | 0 | 0 | 0 |
 0 | 0 | 0 | 8 | 0 | 0 | 0 | 0 | 0 | 8 | 0 |

This is a visual representation of how our data structure would ultimately look like: 

<img src="./assets/sparse_matrix_img.jpg" style="float: left; margin-bottom: 1rem;">

>[!Note] 
>Assuming the image per say gets bigger or is expanded in its size, then hypothetically the size of the sparse matrix should remain unscaved as only the gaps(numbers equal to zero) between the relevant values(in our case any number not equal to zero) would be prone to change. In simpler terms, if the image is shrunk or expanded the overall information weight remains the same.

## Compression Rates and Analysis

When it comes to compression rates let's test the program out on the following data:

 1 | 1 |
---|---|
 1 | 1 |

`main.c` would then output the following snippet:

```bash
index: 1
val: 1
val: 1

index: 2
val: 1
val: 1

==========================

index: 1
val: 1
val: 1

index: 2
val: 1
val: 1


Size of sparse mtrx: 96
Size of simpler mtrx: 8
```

>[!NOTE]
>The first two indices above the division line indicates the first two columns, whilst the second two indices - the first two rows.

As we can see from the displayed sizes of the sparse matrix and just a normal array the array is uses `16 bytes` and the sparse matrix utilizes `96 bytes`. You might immediately mention that the sparse matrix data structure is not efficient with this example and you'd be definitely right. Let's however take another example into consideration. Below is practically the same data table but skewed to the right:

 0 | 0 | 1 | 1 |
---|---|---|---|
 0 | 0 | 1 | 1 |

In this case we get the following sizes. As you can see the sparse matrix remains unscaved in its size `96 bytes`, however, the array only increments with every new number. Current array size: `32 bytes`. 

```bash
Size of sparse mtrx: 96
Size of simpler mtrx: 32
```

Such behaviour would be encountered repeatedly until the size of the said examined data structure would surpass the array itself.

Here are a list of examples proving the previous point:

 0 | 0 | 0 | 0 | 1 | 1 |
---|---|---|---|---|---|
 0 | 0 | 0 | 0 | 1 | 1 |

```bash
Size of sparse mtrx: 96
Size of simpler mtrx: 48
```

## License
MIT
