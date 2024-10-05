#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 

int main() 
{ 
    int fd1[2];  // Pipe 1: Parent to Child
    int fd2[2];  // Pipe 2: Child to Parent

    char fixed_str1[] = "howard.edu"; 
    char input_str[100]; 
    pid_t p; 

    // Create two pipes
    if (pipe(fd1) == -1 || pipe(fd2) == -1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 

    // Get the initial string input from the user
    printf("Enter a string to concatenate: ");
    scanf("%s", input_str); 
    p = fork(); 

    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
    else if (p > 0) // Parent process
    { 
        // Close unnecessary pipe ends
        close(fd1[0]);  // Close reading end of Pipe 1
        close(fd2[1]);  // Close writing end of Pipe 2

        // Write the input string to the child process
        write(fd1[1], input_str, strlen(input_str) + 1); 
        close(fd1[1]);  // Close writing end of Pipe 1

        // Wait for the child process to finish
        wait(NULL); 

        // Read the concatenated string from the child
        char concat_str[200]; 
        read(fd2[0], concat_str, 200); 
        close(fd2[0]);  // Close reading end of Pipe 2

        // Print the concatenated string from the child
        printf("Concatenated string: %s\n", concat_str);

        // Ask the user for another string to concatenate
        char user_input[100];
        printf("Enter another string to concatenate: ");
        scanf("%s", user_input);

        // Concatenate the new input string with the result from the child
        strcat(concat_str, user_input);

        // Print the final result
        printf("Final concatenated string: %s\n", concat_str);
    } 
    else // Child process
    { 
        // Close unnecessary pipe ends
        close(fd1[1]);  // Close writing end of Pipe 1
        close(fd2[0]);  // Close reading end of Pipe 2

        // Read the input string from the parent process
        char concat_str[200]; 
        read(fd1[0], concat_str, 100); 
        close(fd1[0]);  // Close reading end of Pipe 1

        // Concatenate the fixed string (howard.edu) to the input string
        strcat(concat_str, fixed_str1);

        // Send the concatenated string back to the parent process
        write(fd2[1], concat_str, strlen(concat_str) + 1); 
        close(fd2[1]);  // Close writing end of Pipe 2

        exit(0); 
    } 
    return 0;
}
