#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

void reverse_string(char *str, int len);
void print_words_and_lengths(char *buff, int len, int str_len);


int search_and_replace(char *buff, int len, int str_len, char *search_word, char *replace_word);


//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


//here user_string is the address of the string that user entered
int setup_buff(char *buff, char *user_str, int len) {
    char *src = user_str;
    char *internal_buff = buff;

    int i = 0;             
    bool in_whitespace = false;  

    while (*src != '\0') {
        if (i >= len) {
            return -1;
        }

        if (*src == ' ' || *src == '\t') {
            if (!in_whitespace) {
                *internal_buff = ' ';
                internal_buff++;
                i++;
                in_whitespace = true; 
            }
        } else {
            *internal_buff = *src;
            internal_buff++;
            i++;
            in_whitespace = false; 
        }

        src++;  
    }

    while (i < len) {
        *internal_buff = '.';
        internal_buff++;
        i++;
    }

    return i;
}



void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    if (buff == NULL || str_len <= 0 || len <= 0) {
        return -1;
    }

    int word_count = 0;
    bool in_word = false;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] == ' ' || buff[i] == '\t' || buff[i] == '\n') {
            if (in_word) {
                in_word = false;
            }
        } else if (buff[i] == '\0') {
            break;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }
    }

    return word_count;
}

void reverse_string(char *str, int len) {
    char *start = str;
    char *end = str + len - 1;

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    for (int i = len; i < BUFFER_SZ; i++) {
        str[i] = '.';
    }
}


void print_words_and_lengths(char *buff, int len, int str_len) {
    if (buff == NULL || len <= 0 || str_len <= 0) {
        printf("Error printing words, invalid input.\n");
        exit(3);
    }

    int word_start = -1;
    int word_count = 0;

    printf("Word Print\n----------\n");
    for (int i = 0; i <= str_len; i++) {
        if ((buff[i] == ' ' || i == str_len) && word_start != -1) {
            printf("%d. ", ++word_count);

            int word_end = i;
            if (i == str_len) { 
                while (word_end > word_start && buff[word_end - 1] == '.') {
                    word_end--;
                }
            }

            for (int j = word_start; j < word_end; j++) {
                putchar(buff[j]);
            }

            printf(" (%d)\n", word_end - word_start);
            word_start = -1;
        } else if (buff[i] != ' ' && word_start == -1) {
            word_start = i;
        }
    }
}


int search_and_replace(char *buff, int len, int str_len, char *search_word, char *replace_word) {
    if (buff == NULL || len <= 0 || str_len <= 0 || search_word == NULL || replace_word == NULL) {
        return -1; 
    }

    int search_len = 0, replace_len = 0;

    while (*(search_word + search_len) != '\0') search_len++;
    while (*(replace_word + replace_len) != '\0') replace_len++;

    if (search_len == 0 || replace_len == 0) {
        return -2; 
    }

    char *current = buff; 
    int offset = 0;

    while (offset <= str_len - search_len) {
        int match = 1;

        for (int i = 0; i < search_len; i++) {
            if (*(current + i) != *(search_word + i)) {
                match = 0;
                break;
            }
        }

        if (match) {
            if (str_len - search_len + replace_len > len) {
                return -3; 
            }

            if (replace_len != search_len) {
                if (replace_len > search_len) {
                    for (int i = str_len; i >= offset + search_len; i--) {
                        *(buff + i + (replace_len - search_len)) = *(buff + i);
                    }
                } else {
                    for (int i = offset + search_len; i <= str_len; i++) {
                        *(buff + i - (search_len - replace_len)) = *(buff + i);
                    }
                }
            }

            for (int i = 0; i < replace_len; i++) {
                *(current + i) = *(replace_word + i);
            }

            str_len = str_len - search_len + replace_len;

            offset += replace_len;
            current = buff + offset;
        } else {
            offset++;
            current++;
        }
    }

    for (int i = str_len; i < len; i++) {
        *(buff + i) = '.';
    }

    return str_len; 
}




//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //This is safe because it first ensures that argc is at least 2,
    //meaning argv[1] exists. It then checks if argv[1] starts with a hyphen ('-'),
    //preventing access to an invalid or non-existent argument
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //This checks if there are fewer than 3 arguments passed,
    //ensuring the program has enough input to proceed, otherwese, it calls usage() and exits
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    // this is the the argument string
    input_string = argv[2]; //capture the user input string


    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = (char*)malloc(sizeof(char)*BUFFER_SZ);

    if (buff == NULL){
        printf("Error allocating buffer, rc = %d", rc);
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                
                //Did free the memory just in case
                free(buff);
                
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        
        case 'r':
            reverse_string(buff, user_str_len);
            break;

        case 'w':
            print_words_and_lengths(buff, BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error printing words, rc = %d\n", rc);
                free(buff);
                exit(2);
            }
            break;
        
        case 'x':
            if (argc != 5) {
                printf("Error: Missing search and replace arguments.\n");
                usage(argv[0]);
                free(buff);
                exit(1);
            }

            char *search_word = argv[3];
            char *replace_word = argv[4];

            if (strlen(search_word) == 0 || strlen(replace_word) == 0) {
                printf("Error: Search or replace word cannot be empty.\n");
                free(buff);
                exit(3);
            }

            rc = search_and_replace(buff, BUFFER_SZ, user_str_len, search_word, replace_word);
            if (rc < 0) {
                printf("Not Implemented!\n");
                free(buff);
                exit(3);
            }
            break;
       
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            free(buff);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          Providing both the pointer and the length is good practice because it makes the function more flexible and reusable in different contexts
//          While the buffer may have a fixed size in main(), passing the length allows the function to work with buffers of varying sizes or dynamically allocated memory
//          It also ensures the function can handle cases where the actual content in the buffer might not fill the entire allocated space, preventing potential buffer overflows or misuse of memory