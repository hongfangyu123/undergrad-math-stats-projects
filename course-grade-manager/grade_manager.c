#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct item {
    char   name[32];
    float  percentage;
    float  maxScore;
    int    numOfGrades;
    float  grades[32];
};

struct course {
    char   id[16];
    char   name[64];
    int item_num;
    struct item *items;
};

int course_num;
struct course *g_courses;

int line_len;
char line_buf[64];
int menu_getline(char *buf, int buf_len, int *line_len)
{
   	int c, i;

   	memset(buf, 0x00, buf_len);

   	for (i = 0; i < buf_len - 1; i++) {
        c = getchar();
        if (c == EOF) {
            break;
        }

        if (c == '\n') {
            break;
        }
        
		buf[i] = c;
   	}

   	if (c == EOF) {
		*line_len = i;
		return -1;
   	}

   	buf[i] = 0;
	*line_len = i;

   	return 0;
}

void coursePrint(void)
{
    int i;
    struct course * course;
    
    printf("\nThere're totally %d courses recorded:\n", course_num);

    for (i = 0; i < course_num; i++) {
        course = g_courses + i;
        printf("%d.      %s       %s\n", i + 1, course->id, course->name);
    }
}

void coursePrintDetail(struct course *course, int item_index)
{
    int j, k, g;
    struct item *item;
    char per_buf[64] = {0};
    char grade_buf[64] = {0};
    
    printf("\n        %s %s\n", course->id, course->name);
    printf("        -----------------------------------\n");
    printf("        | %-7s| %-21s| %-11s| %-11s| %s\n", "No.", "Item", "Percentage", "Full Mark", "Grades");
    
    for (j = 0; j < course->item_num; j++) {
        item = course->items + j;
        if (item_index != -1 && item_index == (j + 1)) {
            snprintf(per_buf, 64, "%.2f", item->percentage);
            snprintf(grade_buf, 64, "%.2f", item->maxScore);
            printf("        | %-7d| %-21s| %-11s| %-11s| ", j + 1, item->name, per_buf, grade_buf);
            g = 0;
            for (k = 0; k < item->numOfGrades; k++) {
                if (item->grades[k] == 0.0) {
                    continue;
                }
                
                printf("%d: %.2f  ", g, item->grades[k]);
                g++;
            }
            printf("\n");
            return;
        } else {
            snprintf(per_buf, 64, "%.2f", item->percentage);
            snprintf(grade_buf, 64, "%.2f", item->maxScore);
            printf("        | %-7d| %-21s| %-11s| %-11s| ", j + 1, item->name, per_buf, grade_buf);
            g = 0;
            for (k = 0; k < 32; k++) {
                if (item->grades[k] == 0.0) {
                    continue;
                }
                
                printf("%d: %.2f  ", g, item->grades[k]);
                g++;
            }
            printf("\n");
        }
    }
}


struct course *mallocCourse(char *id, char *name)
{
    struct course *course;

    if (g_courses == NULL) {
        g_courses = malloc(sizeof(struct course));
        course = g_courses;
    } else {
        g_courses = realloc(g_courses, (course_num + 1) * sizeof(struct course));
        course = g_courses + course_num;
    }

    course_num++;
    memset(course, 0x00, sizeof(struct course));
    strncpy(course->id, id, 15);
    strncpy(course->name, name, 63);

    printf("\nThe newly added course is %s, %s.\n", course->id, course->name);
    
    return course;
}

struct item *addCourseItem(struct course *course, struct item *pitem)
{
    struct item *item;

    if (course->items == NULL) {
        course->items = malloc(sizeof(struct item));
        item = course->items;
    } else {
        course->items = realloc(course->items, (course->item_num + 1) * sizeof(struct item));
        item = course->items + course->item_num;
    }

    course->item_num++;
    memset(item, 0x00, sizeof(struct item));
    memcpy(item, pitem, sizeof(struct item));

    return item;
}

struct course *findIndexCourse(int index)
{
    if (index < 0 || index > course_num) {
        return NULL;
    }
    
    return g_courses + (index - 1);
}

void delIndexCourse(int index)
{
    int i, j;
    int left_num;

    if (g_courses == NULL) {
        return;
    }
    
    left_num = course_num - 1;
    if (left_num <= 0) {
        free(g_courses);
        g_courses = NULL;
        course_num = 0;
        return;
    }

    j = 0;
    struct course *course = malloc(left_num * sizeof(struct course));
    for (i = 0; i < course_num; i++) {
        if (i + 1 == index) {
            continue;
        }

        memcpy(course + j, g_courses + i, sizeof(struct course));
        j++;
    }

    free(g_courses);
    g_courses = course;
    course_num = left_num;
    
    return;
}

void addCourse(void)
{
    char id[16];
    char name[64];
    struct course *course;
    int choice, i;
    struct item item;
    
    while (1) {
        coursePrint();
        
        printf("\nEnter NEW course ID (Character+Number, length < 10) or "
            "any single key back:");
        menu_getline(id, 16, &line_len);
        if (strlen(id) == 1) {
            return;
        }

        printf("Enter course name (less than 50 characters with spaces):");
        menu_getline(name, 64, &line_len);
        
        course = mallocCourse(id, name);
        printf("Continue to set the evaluation items for this course? (Y or y: yes; any other character: no): ");
        menu_getline(line_buf, 64, &line_len);
        if (line_buf[0] != 'Y' && line_buf[0] != 'y') {
            return;
        }
        printf("Enter number of evaluation items for the new course:");
        menu_getline(line_buf, 64, &line_len);
        choice = atoi(line_buf);
        if (choice == 0) {
            printf("\nitems is 0\n");
            continue;
        }
        printf("Input name, percentage, full marks for each item\n");
        printf("(attention: no space in names and the total percentage should be 100):");
        for (i = 0; i < choice; i++) {
            memset(&item, 0x00, sizeof(item));
            menu_getline(line_buf, 64, &line_len);
            sscanf(line_buf, "%s %f %f", item.name, &item.percentage, &item.maxScore);
            addCourseItem(course, &item);
        }
    }
    
}

void delCourse(void)
{
    int num;
    struct course *course;
    
    while (1) {
        coursePrint();
        
        printf("Enter a number to delete the related course or any other key back: ");
        menu_getline(line_buf, 64, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > course_num) {
            return;
        }

        delIndexCourse(num);
    }

    return;
}

void delAllCourse(void)
{
    free(g_courses);
    g_courses = NULL;
    course_num = 0;

    printf("All courses are deleted!\n");
    
    return;
}

void modifyCourseChoice(struct course *course)
{
    int choice, num, i;
    struct item *item;
    struct item in_item;

    while (1) {
        coursePrintDetail(course, -1);
        printf("============== Modify Course ==============\n");
        printf("1. Modify Course Name\n");
        printf("2. Modify Item Name\n");
        printf("3. Modify Item Percentage\n");
        printf("4. Modify Item Fullmark\n");
        printf("5. Add Items If No Item Yet\n");
        printf("============================================\n");
        printf("Enter a number to choose an operation or any other key back: ");
        menu_getline(line_buf, 64, &line_len);
        choice = atoi(line_buf);
        if (choice <= 0 || choice > 5) {
            return;
        }
        switch(choice){
			case 1:
                printf("You want to change name of course \"%s\" as :", course->name);
                memset(course->name, 0, 64);
                menu_getline(course->name, 63, &line_len);
		        break;
        
			case 2:
                printf("Enter the No. of an Item to be modified: ");
                menu_getline(line_buf, 64, &line_len);
                num = atoi(line_buf);
                if (num <= 0 || num > course->item_num) {
                    return;
                }
                printf("Enter a new name:");

                item = course->items + (num - 1);
                menu_getline(item->name, 32, &line_len);
		        break;
                
			case 3:
                printf("Enter the new percentages in order:");
                for (i = 0; i < course->item_num; i++) {
                    item = course->items + i;
                    menu_getline(line_buf, 64, &line_len);
                    sscanf(line_buf, "%f", &item->percentage);
                }
		        break;
                
			case 4:
                printf("Enter the No. of an Item to be modified: ");
                menu_getline(line_buf, 64, &line_len);
                num = atoi(line_buf);
                if (num <= 0 || num > course->item_num) {
                    return;
                }
                printf("Enter a new fullmark:");
                item = course->items + (num - 1);
                menu_getline(line_buf, 64, &line_len);
                sscanf(line_buf, "%f", &item->maxScore);
			    break;

			case 5:
                if (course->item_num > 0) {
                    printf("ERROR: Can't add items as they're exist already!\n");
                    return;
                }
                printf("Enter number of evaluation items for the new course:");
                menu_getline(line_buf, 64, &line_len);
                choice = atoi(line_buf);
                if (choice == 0) {
                    printf("\nitems is 0\n");
                    break;
                }
                printf("\nInput name, percentage, full marks for each item\n");
                printf("(attention: no space in names and the total percentage should be 100):");
                for (i = 0; i < choice; i++) {
                    memset(&in_item, 0x00, sizeof(in_item));
                    menu_getline(line_buf, 64, &line_len);
                    sscanf("%s %f %f", in_item.name, &in_item.percentage, &in_item.maxScore);
                    addCourseItem(course, &in_item);
                }
			    break;

			default: 
			    return;
		}
    }

    return;
}

void modifyCourse(void)
{
    int num, choice;
    struct course *course;
    
    while (1) {
        coursePrint();
        
        printf("Enter a number to modify the related course details or any other key back: ");
        menu_getline(line_buf, 64, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > course_num) {
            return;
        }

        course = findIndexCourse(num);
        if (course == NULL) {
            printf("number %d find cource error\n", num);
            return;
        }
        
        modifyCourseChoice(course);
    }

    return;
}

void EditCourse(void)
{
	while(1){
		printf("\n============== Edit Course ==============\n");
		printf("\t1. Add Course\n");
		printf("\t2. Delete Course\n");
		printf("\t3. Delete All Courses\n");
		printf("\t4. Modify Course\n");
		printf("\t5. Back\n");
		printf("=========================================\n\n");
		printf("Enter a number and choose an operation:");
		int choice;
        menu_getline(line_buf, 16, &line_len);
        choice  = atoi(line_buf);

		if(choice == 5) break;
		switch(choice){
			case 1: // Add Course
			    addCourse();
		        break;
        
			case 2: // Delete Course
			    delCourse();
		        break;
                
			case 3: // Delete All Courses
			    delAllCourse();
		        break;
                
			case 4: // Modify Course
			    modifyCourse();
			    break;
                
			default: 
			    return;
		}
	}
}

void delCourseGrade(struct item *item, int index)
{
    float tmp_grades[32];
    int i, j, tmp_num;

    tmp_num = item->numOfGrades - 1;
    if (tmp_num <= 0) {
        item->grades[0] = 0.0;
        item->numOfGrades = 0;
        return;
    }

    j = 0;
    memset(tmp_grades, 0, sizeof(tmp_grades));
    for (i = 0; i < item->numOfGrades; i++) {
        if (i == index) {
            continue;
        }
        
        tmp_grades[j] = item->grades[i];
        j++;
    }

    memcpy(item->grades, tmp_grades, sizeof(item->grades));
    item->numOfGrades--;
}

void delGradechoice2(struct course *course, struct item *item, int item_index)
{
    int num;
    
    while (1) {
        coursePrintDetail(course, item_index);
        printf("Enter the number of a grade to delete or any other key back: ");
        menu_getline(line_buf, 16, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > item->numOfGrades) {
            return;
        }
        delCourseGrade(item, num);
    }
}

void modifyGradechoice4(struct course *course, struct item *item, int item_index)
{
    int num;
    float grade;
    
    while (1) {
        coursePrintDetail(course, item_index);
        printf("Enter the number of a grade to modify or any other key back: ");
        menu_getline(line_buf, 16, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > item->numOfGrades) {
            return;
        }
        printf("You want to change Assignment Grade %d from %.2lf to: ", num, item->grades[num - 1]);
        menu_getline(line_buf, 16, &line_len);
        sscanf(line_buf, "%f", &grade);
        item->grades[num - 1] = grade;
    }
}


void modifyGradeChoice(struct course *course, struct item *item, int item_index)
{
    int choice, num;
    float grade;
    
    while (1) {
        coursePrintDetail(course, item_index);
        
        printf("============== Edit Grade ==============\n");
        printf("        1. Add Grade\n");
        printf("        2. Delete Grade\n");
        printf("        3. Delete All Grades\n");
        printf("        4. Modify Grade\n");
        printf("        5. Back\n");
        printf("==========================================\n");

        printf("Enter a number and choose an operation:");
        menu_getline(line_buf, 16, &line_len);
        choice = atoi(line_buf);
        if (choice <= 0 || choice > 5) {
            return;
        }
        switch (choice){
			case 1:
                while (1) {
                    printf("Enter a grade to added to the item or a negative number back: ");
                    menu_getline(line_buf, 16, &line_len);
                    sscanf(line_buf, "%f", &grade);
                    if (grade < 0) {
                        break;
                    }
                    item->grades[item->numOfGrades++] = grade;
                }
		        break;
        
			case 2:
                delGradechoice2(course, item, item_index);
                coursePrintDetail(course, item_index);
		        break;
                
			case 3:
                memset(item->grades, 0, sizeof(item->grades));
                item->numOfGrades = 0;
                coursePrintDetail(course, item_index);
		        break;
                
			case 4:
                modifyGradechoice4(course, item, item_index);
			    break;

			case 5:
			default: 
			    return;
		}
    }

    return;
}

void EditGrade(void)
{
    int num, choice;
    struct course *course;
    struct item *item;

	while(1) {
        coursePrint();
        printf("Enter a number to edit the related course grades or any other key back:\n  ");
        menu_getline(line_buf, 16, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > course_num) {
            return;
        }

        course = findIndexCourse(num);
        if (course == NULL) {
            printf("number %d find cource error\n", num);
            return;
        }
        coursePrintDetail(course, -1);
        printf("Enter the No. of an Item whose grades to be edited or any other key back\n: ");
        menu_getline(line_buf, 16, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > course->item_num) {
            return;
        }

        item = course->items + (num - 1);
        modifyGradeChoice(course, item, num);
	}
}

void viewCourse(void)
{
    int num;
    struct course *course;

	while(1) {
        coursePrint();
        printf("Enter a number to view the related course details or enter any other key back: ");
        menu_getline(line_buf, 16, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > course_num) {
            return;
        }

        course = findIndexCourse(num);
        if (course == NULL) {
            printf("number %d find cource error\n", num);
            return;
        }
        coursePrintDetail(course, -1);
	}
}

float getFinalData(struct course *course, float expected_grade)
{
    int i, j;
    float data, sum;
    struct item *item;
    struct item *final_item;

    data = 0;
    final_item = NULL;
    for (i = 0; i < course->item_num; i++) {
        item = course->items + i;
        if (strcmp("Final", item->name) == 0) {
            final_item = item;
            continue;
        }
                
        if (item->numOfGrades == 0) {
            continue;
        }

        sum = 0;
        for (j = 0; j < item->numOfGrades; j++) {
            sum += item->grades[j];
        }

        sum /= item->numOfGrades;
        sum = (sum / item->maxScore) * item->percentage;

        data += sum;
    }

    if (final_item == NULL) {
        printf("not find final\n");
        return 0.0;
    }
    
    data = ((expected_grade - data) / final_item->percentage) * final_item->maxScore;
    return data;
}

void Expected_Final_Grade_All(void)
{
    int i, Danger = 0;
    struct course *course;
    char final_buf[64] = {0};
    float data;
    int expected_grade;

    while (1) {
        printf("Enter an expected course total([0, 100]) or any negative back: ");
        menu_getline(line_buf, 16, &line_len);
        expected_grade = atoi(line_buf);
        if (expected_grade < 0 || expected_grade > 100) {
            return;
        }

        printf("  | %-16s| %-13s| %-51s| %-127s|\n", "(*)Danger", "Course Code", "Course Name", "Lowest Final for Expected Course Total");
        for (i = 0; i < course_num; i++) {
            course = g_courses + i;

            data = getFinalData(course, (float)expected_grade);
            if (data > 100.00) {
                Danger = 1;
            }
            
            snprintf(final_buf, 64, "%.2f", data);
            printf("  | %-16s| %-13s| %-51s| %-127s|\n", Danger ? "*" : " ", course->id, course->name,
                final_buf);
        }
    }


    return;
}

void Expected_Final_Grade_Single(void)
{
    int i, j, num;
    struct course *course;
    char percentage_buf[64] = {0};
    char avg_buf[64] = {0};
    char grade_buf[64] = {0};
    float data;
    int final_index, expected_grade;

    float Average, Grade, sum;
    struct item *item;
    struct item *final_item;

    while (1) {
        coursePrint();
        printf("Enter a number to select one course or any other key back: ");
        menu_getline(line_buf, 16, &line_len);
        num = atoi(line_buf);
        if (num <= 0 || num > course_num) {
            return;
        }

        printf("Enter an expected course total([0, 100]) or any negative back: ");
        menu_getline(line_buf, 16, &line_len);
        expected_grade = atoi(line_buf);
        if (expected_grade < 0 || expected_grade > 100) {
            return;
        }

        course = findIndexCourse(num);
        if (course == NULL) {
            printf("number %d find cource error\n", num);
            return;
        }

        sum = 0.0;
        printf("        -----------------------------------\n");
        printf("        | %-7s| %-21s| %-11s| %-21s| %-16s|\n", "No.", "Item", "Percentage", "Original Average", "Item Grade");
        for (i = 0; i < course->item_num; i++) {
            item = course->items + i;
            if (strcmp("Final", item->name) == 0) {
                final_item = item;
                final_index = i;
                continue;
            }
                    
            if (item->numOfGrades == 0) {
                continue;
            }

            Average = 0;
            for (j = 0; j < item->numOfGrades; j++) {
                Average += item->grades[j];
            }
            Average /= item->numOfGrades;
            Grade = (Average / item->maxScore) * item->percentage;
            sum += Grade;
            
            snprintf(percentage_buf, 64, "%.2f", item->percentage);
            snprintf(avg_buf, 64, "%.2f", Average);
            snprintf(grade_buf, 64, "%.2f", Grade);
            printf("        | %-7d| %-21s| %-11s| %-21s| %-16s|\n", i + 1, item->name, percentage_buf, avg_buf, grade_buf);
        }
        printf("        -----------------------------------\n");

        printf("        | %-7s| %-21s| %-11s| %-21s| %-16s|\n", "No.", "Item", "Percentage", "Fullmark", "Expected Grade");
        snprintf(percentage_buf, 64, "%.2f", final_item->percentage);
        snprintf(avg_buf, 64, "%.2f", final_item->maxScore);

        Grade = ((expected_grade - sum) / final_item->percentage) * final_item->maxScore;
        snprintf(grade_buf, 64, "%.2f", Grade);
        printf("        | %-7d| %-21s| %-11s| %-21s| %-16s|\n", final_index + 1, 
            final_item->name, percentage_buf, avg_buf, grade_buf);
        printf("ATTENTION: The final score should pass a certain threshold to pass the course!\n");
    }

    return;
}

void Expected_Final_Grade(void)
{
    int choice;

    printf("============== Calculate Expected Final ==============\n");
    printf("        1. View All Courses\n");
    printf("        2. View Single Course\n");
    printf("        3. Back\n");
    printf("=======================================================\n");
    printf("Enter a number to choose an operation: ");
    menu_getline(line_buf, 16, &line_len);
    choice = atoi(line_buf);
    if (choice <= 0 || choice > 3) {
        return;
    }

    switch(choice){
        case 1:
            Expected_Final_Grade_All();
            break;
        
        case 2:
            Expected_Final_Grade_Single();
            break;
                
        case 3:
        default: 
            return;
    }

    return;
}

int main(int argc, char *argv[])
{
	while(1){
		printf("\n============== Main Menu ==============\n");
    	printf("\t1. Edit Course\n");
    	printf("\t2. Edit Grade\n");
    	printf("\t3. Expected Final Grade\n");
    	printf("\t4. View Course\n");
    	printf("\t5. Exit\n");
    	printf("=======================================\n\n");
    	
		printf("Enter a number and choose an operation:");
		int choice;
        menu_getline(line_buf, 16, &line_len);
        choice  = atoi(line_buf);
		switch(choice){
			case 1: // Edit Course
		        EditCourse();
		        break;
            
			case 2: // Edit Grade
			    EditGrade();
	            break;
                    
			case 3: // Expected Final Grade
			    Expected_Final_Grade();
		        break;
                    
			case 4: // View Course
			    viewCourse();
		        break;
                    
			case 5: // Exit
		        return 1;
                
			default: 
			    break;
		}
	}
	
	return 0;
}

