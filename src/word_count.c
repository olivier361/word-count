#word_count.c
#written by Olivier Gervais-Gougeon

#include "word_count.h" 

int
real_main(int argc, char *argv[]) {
    

    //Prints out message if not enough arguments present.
    //i.e no --infile and/or no filename.
    if (argc < 3) {
        printf("program: missing '--infile <filename> [--sort] [--print-words]'\n");
        return 0;
    }
    //We could also check for too many arguments or we can
    //just ignore them without letting the user know they provided
    //extra unecessarry arguments.

    //Declare variables related to command line parameters:
    //int arg_infile = 0;
    char *arg_filename = "";
    int arg_sort = 0;
    int arg_print = 0;

    //Loop through arguments to find which supported arguments were given:
    for (int i=0; i<argc; i++) {
        //if --infile argument found, set respective variable:
        if (strcmp(argv[i], "--infile") == 0) {
            //arg_infile = 1;

            //checks that next argument after --infile exists and saves it as filename.
            //else prints error message and stops program:
            if (i<(argc-1)){
                arg_filename = argv[i+1];
            }
            else {
                printf("program: missing '--infile <filename> [--sort] [--print-words]'\n");
                return 0;
            }
        }
        //if --sort found, set var:
        else if (strcmp(argv[i], "--sort") == 0) {
            arg_sort = 1;
        }
        //if --print-words found, set var:        
        else if (strcmp(argv[i], "--print-words") == 0) {
            arg_print = 1;
        }
        
    }

    //DEBUG: Prints state of found arguments:
    //printf("--infile: %d, --sort: %d, --print-words: %d, filename: %s", arg_infile, arg_sort, arg_print, arg_filename);

    // check that file is valid then pass all parameters to read_file()
    //TODO
    char file_buffer[MAX_FILESIZE] = {'\0'}; //modified for make test.
    size_t file_len = 0;
    int word_count = 0;
    struct word_t words[MAX_WORD_CNT] = {};
    int count[MAX_WORD_LEN] = {0};
    int sort_count_index[MAX_WORD_LEN] = {0};

    //Initialize words length to 0 to avoid errors:
    for(int i=0; i<MAX_WORD_CNT; i++) {
        words[i].len = 0;
    }
 
    //Read file and returns file length in char:
    file_len = read_file(arg_filename, file_buffer);

    if (file_len == 0) {
        //Debug:
        //printf("file_len=0. Stoping program\n");

        return 0;
    }

    if (file_len > MAX_FILESIZE) {
        printf("WARNING: file of size %ld is larger than tested limit!\n", file_len);
    }

    //TODO: Implement word_t bucket for this to work:
    word_count = tokenize_string(file_buffer, file_len, words);
    
    //DEBUG: Print word count:
    //printf("Words found: %d\n", word_count);

    if (word_count > MAX_WORD_CNT) {
        printf("WARNING: %d words found in file. This is larger than tested limit!\n", word_count);
    }

    //Debug: print words[4].word & .len:
    //printf("In real_main: 5th word is: %s of length %d\n", words[4].word, words[4].len);

    //TODO: This function will create a count array like wanted
    //for final results with indices being lenght of words and
    //integer at index equal to number of occurences.
    build_count(words, count);
    print_histogram(count, sort_count_index, words, arg_sort, arg_print);

    return 0;
}


#ifndef TEST
int
main(int argc, char *argv[]) {
    return real_main(argc, argv);
}
#endif
