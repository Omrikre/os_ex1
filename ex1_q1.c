#define _CRT_SECURE_NO_WARNINGS
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#define NAME_SIZE 10
#define GRADES_SIZE 30
#define LINE_SIZE 100
#define NUM_OF_STUDENTS 256
#define RES_GRADES_FILE_NAME "all_std.txt"

typedef struct student {
	char name[NAME_SIZE];
	int grades[GRADES_SIZE];
	int num_of_grades;
	int avg;
} Student;

void data_to_struct(FILE* read_file);
int combine_students(FILE* all_std, int* pids, int num_of_files);
void report_data_summary(int num_stud);


int main(int argc, char *argv[]) {
	int num_of_students = 0;
	//Student** students = (Student**)malloc(NUM_OF_STUDENTS * sizeof(Student*));
	FILE* all_std = fopen(RES_GRADES_FILE_NAME, "w+");

	// check if all_std.txt file was opened
	if (!all_std) {
		printf("ERROR 0\n");
		printf("Error!");
		exit(1);
	}

	// save file names

	int* pids = (int*)malloc((argc-1) * sizeof(int));

	// open all files
	for (int i = 1; i < argc; i++) {
		int cpid;
		FILE* file_to_read = fopen(argv[i], "r+");
		if (!file_to_read) {
			printf("ERROR 1\n");
			printf("ERROR - opening file %s\n", argv[i]);
			exit(1);
		}
		else { // success opening file
			cpid = fork();
			if (cpid == 0){
				data_to_struct(file_to_read);
				exit(1);
			}
			else
				pids[i-1] = cpid;
		}
		fclose(file_to_read);
	} // close current file

	// wait to all children
	int status;
    pid_t pid;
    while ((pid = wait(&status)) > 0) {
        //printf("Child %d finished with status %d\n", pid, status);
    }

	// copy all temp files to all_std
	num_of_students = combine_students(all_std, pids, (argc - 1));


	fclose(all_std); // close all_std.txt file
	report_data_summary(num_of_students);
	return 0;
}

void report_data_summary(int num_stud) {
	fprintf(stderr, "grade calculation for %d students is done\n", num_stud);
}

int combine_students(FILE* all_std, int* pids, int num_of_files) {
    char c;
    int num_of_students = 0;
    char file_name[LINE_SIZE];

    int x = 10;
    char str[20];
    sprintf(str, "%d.temp", x);
    //printf("number of files: %d\n", num_of_files);

    for (int i = 0; i < num_of_files; i++) {
        //printf("combine file with pros number: %d\n", pids[i]);
        sprintf(file_name, "%d.temp", pids[i]);
        FILE* file_to_read = fopen(file_name, "r");
        if (!file_to_read) {
            printf("ERROR - opening file %d\n", pids[i]);
            exit(1);
        }
        else { // success opening file
            // copy the data
            while ((c = fgetc(file_to_read)) != EOF) {
                fputc(c, all_std);
                if (c == '\n') {
                    num_of_students++;
                }
            }
            fclose(file_to_read);
        }
    }

    return num_of_students;
}





void data_to_struct(FILE* read_file) {
	//Student** students = (Student**)malloc(NUM_OF_STUDENTS * sizeof(Student*));
	char line[LINE_SIZE], name[NAME_SIZE], file_name[LINE_SIZE];
	char* tok;
	double avg = 0;
	int num_of_grades, num_of_students = 0;
	int pid = getpid();

	sprintf(file_name, "%d.temp", pid);
	FILE* temp_file = fopen(file_name, "w");

	// read file line by line
	while (fgets(line, LINE_SIZE, read_file)) {
		num_of_grades = 0;
		//students[num_of_students] = (Student*)malloc(NUM_OF_STUDENTS * sizeof(Student));
		sscanf(line, "%s", name);
		tok = strtok(line, " ");
		tok = strtok(NULL, " ");
		//strcpy(students[num_of_students]->name, name);

		// get grades and calculate avg
		while (tok != NULL) {
			avg += atoi(tok);
			num_of_grades++;
			tok = strtok(NULL, " ");
		}
		//students[num_of_students]->num_of_grades = num_of_grades;
		avg = avg / num_of_grades;
		//students[num_of_students]->avg = avg;

		fprintf(temp_file, "%s %.1f\n", name, avg);
		avg = 0;
		num_of_students++;
	}

	fprintf(stderr, "process: %d file : %s number of students : %d\n", pid, file_name, num_of_students);

	fclose(temp_file);
	return;
}