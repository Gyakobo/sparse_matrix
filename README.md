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

## The Sparse Matrix Data Structure

Each




