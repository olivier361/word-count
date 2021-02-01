#word_count.h
#Written by Olivier Gervais-Gougeon

#ifndef A1_H
#define A1_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define FILE_NOT_FOUND  1
#define BAD_ARGS        2
#define MAX_FILESIZE    5000
#define MAX_WORD_CNT    750
#define MAX_WORD_LEN    35 //Supercalifragilisticexpialidocious
#define TRUE 1
#define FALSE 0

/* 
 * Debugging print function that can has the same 
 * format as `printf`.  If `-D DEBUG` is passed to
 * the compiler then the print code is compiled, 
 * otherwise, it's an empty function.
 *
 */
void debug(const char* format, ... );
void debug(const char* format, ... ) {
#ifdef DEBUG 
	va_list args;
	
	fprintf(stdout, "  -->  ");
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	fprintf(stdout, "\n");
	fflush(stdout);
#endif	
}

typedef struct word_t
{
    int len;
    char word[MAX_WORD_LEN];
    
} word_t;

//TODO Change declaration cause some of the given function got changed arguments:
size_t read_file(char* filepath, char* buffer);
int tokenize_string(char* buffer, size_t bytes, word_t* words); //remplaced freq for words.
void print_histogram(int *count, int *sort_index, word_t* buckets, int sort, int print_words); //modified from originally given.
void print_median(int *count, int *sort_index, int size); //Modified from original.
void print_text(word_t *words, int len); //Added by me.

void build_count(word_t *words, int *count); //New function added by me.
int build_freq_index(int *count, int *sort_index); //New function added by me.
int by_freq( const void* a, const void* b);
int by_len( const void* a, const void* b);
int by_alphanum( const void* a, const  void* b);

int
tokenize_string(char *buffer, size_t bytes, word_t* words) {
    /* char* buffer: Input string to tokenize
     * size_t bytes: size of buffer in bytes
     * word_t* bucket: pointer to struct word array that holds the word histogram
     *
     * Function tokenizes the buffer and update the appropriate word_t buckets 
     *
     * Returns: number of buckets 
     */
    
    //Convert file buffer string to tokenized words:
    //Source: https://stackoverflow.com/questions/26597977/split-string-with-multiple-delimiters-using-strtok-in-c
    char *token;
   // char token[MAX_WORD_CNT];
   // struct word_t words[MAX_WORD_CNT]; //Now defined in .c file.

    token = strtok(buffer, " \t\r\n\v\f.,;()");
    int i = 0;

    //DEBUG:
    //printf("strlen(token)=%ld\n", strlen(token));
    //printf("Found words:\n");

    while (token != NULL || i == MAX_WORD_CNT) {
        //DEBUG: Print word at index:
        //printf("%d: %s\n", i, token);
        
        words[i].len = strlen(token);
        strcpy(words[i].word, token); 

        token = strtok(NULL, " \t\r\n\v\f.,;()");
        
        i++;

        //DEBUG:
        //printf("In Loop: token=%s", token);
        //printf("In Loop: token[4]=%d", token[4]);
    } 

    //TODO: Finish function based on documentation.
    
    //DEBUG:
    //printf("token=%s", token);
    //printf("token[4]=%d", token[4]);
    //printf("In .h tokenize: 5th word is: %s of length %d\n", words[4].word, words[4].len);

    return i; //The number of words found in the file.
}

void
print_histogram(int *count, int *sort_index, word_t* bucket, int sort, int print_words)
{
    /* word_t* bucket: pointer to struct word array that holds the word histogram
     * print_words: flag to print words (Part C) or not
     *
     * Function prints out the word histogram 
     */

    //DEBUG:
    //printf("In print_histogram: sort=%d, print=%d\n", sort, print_words);
    
    if (sort != 1) {
        for (int i=0; i< MAX_WORD_LEN ; i++) {
            if (count[i] != 0) {
                if (print_words != 1) {
                    printf("Count[%02d]=%02d;\n", i, count[i]);
                }
                else {
                //TODO: If print_words is true, print them here from
                //a 2d array[word_count][word_itself].
                    printf("Count[%02d]=%02d;", i, count[i]);
                    print_text(bucket, i);
                }
            }
        }
    //TODO: Could possibly add an array of indices in count that actually
    //have a value over zero to help with median idk.
    }
    else {
        int size = 0;
        size = build_freq_index(count, sort_index);

        for (int i=0; i<size; i++) {
            if (print_words != 1) {
                if (count[sort_index[i]] != 0) {
                    printf("Count[%02d]=%02d;\n", sort_index[i], count[sort_index[i]]);
                }
            }
            else {
                if (count[sort_index[i]] != 0) {
                    printf("Count[%02d]=%02d;", sort_index[i], count[sort_index[i]]);
                    print_text(bucket, sort_index[i]);
                }
            }
        }
        if (print_words != 1) {
            print_median(count, sort_index, size);
        }
    }
}

void
print_median(int *count, int *sort_index, int size) {
    /* int *count: pointer to count array.
     * int *sort_index: pointer to array of count indices sorted by word freq.
     * int size: the size of sort_index.
     * 
     * Function prints out the word length median value 
     */
    //TODO: Cleanup this code and remove unnecessary paramater here
    //and in declaration.
    double result = -1.0;
    //double middle = size/2;
    int amount = 0;
    int median_set[MAX_WORD_LEN] = {0};

    //DO NOT USE THIS CODE, it followed the wrong
    //specification for median calculation.
    /*
    if((size%2) == 0) {
        result = ((double)(sort_index[(int)middle] + sort_index[(int)middle - 1]))/2;
    }
    else {
        result = sort_index[(int)middle];
    }
    */
    for (int i=0; i<MAX_WORD_LEN; i++) {
        if (count[i] != 0) {
            median_set[amount] = i;
            amount += 1;
        }
    }

    double middle = (double)amount/2.0;

    if((amount%2) == 0) {
        result = ((double)(median_set[(int)middle] + median_set[(int)middle - 1]))/2;
    }
    else {
        result = median_set[(int)middle];
    }

    //DEBUG:
    //printf("In print_median: size=%d, middle=%lf, result=%lf\n", size, middle, result);
    //printf("In print_median: sort_index[(int)middle]=%d, sort_index[(int)middle-1]=%d\n", sort_index[(int)middle], sort_index[(int)middle - 1]);

    printf("Median word length: %3.1f\n", result);
}

void print_text(word_t *words, int len) {
    /*word_t words: Pointer to array of struct word_t items.
     *int len: length of the words we are looking to print.
     *
     *Function: Looks through words array and adds every
     *non-duplicate word in a temp array to then be formated
     *and printed alongside print_histogram.
     */
    char list[MAX_WORD_CNT][MAX_WORD_LEN] = {'\0'};
    int i = 0;
    int write = 1;
    int size = 0;

    while (words[i].len != 0) {
        if (words[i].len == len) {
            int j = 0;
            while (*list[j] != '\0') {
                if (strcmp(words[i].word, list[j]) == 0) {
                    write = 0;
                    break;
                }
                j++;
            }
            if (write == 1) {
                strcpy(list[size], words[i].word);
                size++;                
            } else {write = 1;}
        }
        i++;
    }

    qsort(list, size, MAX_WORD_LEN, by_alphanum);
     
    printf(" (words:");
    if (size >= 2) {
        for(int i=0; i<size-2; i++) {
            printf(" \"%s\",", list[i]);
        }
        printf(" \"%s\" and \"%s\")\n",list[size-2], list[size-1]);
    }
    else if (size == 1) {
        printf(" \"%s\")\n", list[0]);
    }
}

size_t
read_file(char* filepath, char* buffer) {
    /* char* filepath: path to the input word file (i.e. t01.txt) 
     * char* buffer: buffer to save the contents of the file to 
     *
     * Function that reads the contents of a file into the array buffer
     *
     * Returns: the size of the array in bytes 
     */
    
    //Check if file is empty and other checks refer to input tests.
    FILE *file_ptr = fopen(filepath, "r");
    size_t char_count = 0;

    if (file_ptr == NULL) {
        printf("unable to open '%s' for reading\n", filepath);
    }
    else {
       //TODO: Finalize code here then tokenize the string into words.
       
       //This code could work in a loop if you append to an array:
       // fgets(buffer, MAX_FILESIZE, file_ptr);
       // fgets(buffer, MAX_FILESIZE, file_ptr);
       
        //reads the file and puts characters in the buffer array: 
        char_count = fread(buffer, 1, MAX_FILESIZE, file_ptr);

        //DEBUG: Prints the string read from file and num of char found:
        //printf("String from file: %s\n", buffer);
        //printf("char_count = %ld\n", char_count);
    }
    return char_count;
}


void build_count(word_t *words, int *count) {
    /*word_t *words: Pointer to array of words.
     *int *count: pointer to currently empty array of integers.
     *data will be stored here.
     *
     *Returns: currently nothing
     *
     *Function: Creates a count array which stores the number of
     * occurences of words of index lenght at that index.
     */

    int i =0;
    int cur_word_len = 0;

    while(words[i].len != 0) {
        //TODO
        cur_word_len = words[i].len;
        count[cur_word_len] += 1;         
        i++;
    }


}

int build_freq_index(int *count, int *sort_index) {
    /*int *count: Pointer to count array.
     *int *sort_index: Pointer to sort_count_index.
     *
     *Returns: length of sort_index.
     *
     *Function: Creates an array with the index values of count array
     *where the word frequency is the highest in descending order.
     *In the case of a freq tie, use the lowest count index first.
     */
   
    int cur_max = -1; //largest value found so far in count.
    int cur_i = 0; //index of current max value in count.
    int count_copy[MAX_WORD_LEN] = {0};
    
    //Create a copy of count as well set index values
    //to zero in the sorting process.
    for(int i=0; i<MAX_WORD_LEN; i++) {
        count_copy[i] = count[i];
    }

    for(int i=0; i<MAX_WORD_LEN; i++) {
        for(int j=0; j<MAX_WORD_LEN; j++) {
            if (count_copy[j] > cur_max) {
                cur_max = count_copy[j];
                cur_i = j;
            }
        }
        sort_index[i] = cur_i;
        count_copy[cur_i] = 0;
        if (cur_max == 0) {
            return i; //size of sort_index array.
        }
        cur_max = -1;
    }
    return MAX_WORD_LEN; //If it gets here it should return size 35.
}


/* Functions to be used by `qsort`
 *
 * These functions should sort by these rules:
 *      a left of b   return positive value
 *      a equals b    return 0
 *      b right of a  return negative value
 *
 * In you function, you need to cast your pointer
 * to the correct pointer type:
 *     
 *     const char *ai = (const char*) a;  
 */

int by_freq( const void* a, const void* b)
{
    //Sort by word count/frequency (--sort flag)
    //In the case of a tie, sort by word length

    return 0; //stub
}

int by_len( const void* a, const void* b)
{
    //Sort by word length

    return 0; //stub 
}

int by_alphanum(const void* a, const void* b)
{
    //Sort alphaphetically
    //Remember that upper should come before lowercase
    //Source: https://cboard.cprogramming.com/c-programming/115724-using-qsort-sort-array-strings.html
    //char *const *pa = a;
    //char *const *pb = b;
    //return strcmp(*pa, *pb);

    //const char **pa = (const char **)a;
    //const char **pb = (const char **)b;
    //return strcmp(*pa, *pb);

    const char *pa = a;
    const char *pb = b;
    return strcmp(pa, pb);
}


#endif
