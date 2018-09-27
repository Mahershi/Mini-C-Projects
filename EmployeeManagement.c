#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

/*----------------------------------------------------------------------------------------*/

COORD coord = {0,0};

void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*----------------------------------------------------------------------------------------*/

FILE *new_file, *read_file, *list_file, *write_password_file, *read_password_file, *write_log_file, *read_log_file;

char *user_password, *admin_password, *year, *buffer, *backup_date, *backup_time, last_action[30], action[30];

int startup_status = 0, concurrency_avoid = 0, admin_login_status = 0, user_login_status = 0;

struct employee
{
    char name[15];
    char lname[15];
    char *fname;
    char *date_of_join;
    long salary;
    int id;
    char contact[11];
    int position;
    char *buffer1;
    char status;
    char *buffer2;

    struct address
    {
        char line1[25];
        char line2[25];
        char line3[25];
        char line4[25];
    } addr;
};

struct dummy
{
    char name[15];
    char lname[15];
    char *date_of_join;
    long salary;
    int id;
    char contact[11];
    int position;
    char status;
    char operation[3];
    char *time;
} *d;

struct log
{
    char name[15];
    char lname[15];
    int id;
    int position;
    char *date;
    char *time;
    char operation[3];
} log;

/*----------------------------------------------------------------------------------------*/

int check_log_update(int );

int check_update_keyword(int, int, int, char[], char);
int check_update_doj_range(int, int, char*, char*, char);
int check_update_salary_range(int, int, long, long, char);
int check_update_status(int, int, char);
int check_update_position(int, int, int, char);
int data_format_procedure();
int format_data();
int set_admin_password(char []);
int set_user_password(char []);
int set_year(char []);
void first_startup();
void display_time();

char *password_input(char title[], char placeholder[]);
void record_operation(char []);
void from_password_file();
void to_password_file();
int total_entries();
int total_log_entries();

char *get_system_date();
void to_data_file(int, struct employee);
struct employee from_data_file(int );

void heading_particular_status();
void heading_all_status();
void log_heading();

void to_log_dummy(long , struct log);
int log_retrieving();
void log_list(long );

void list_particular_status1(int );
void list_all_status1(int );

void list_particular_status2(struct employee);
void list_all_status2(struct employee);

void restore_last_backup();
void perform_backup();
void check_data();
int admin_menu();


void entry_details(struct employee );

int entry_details_by_number(int , char);

int date_limit_check(char *, char *);

char *date_input(char []);

int get_order(char []);

void swapping_dummy(int ,int);
void copy_data_to_dummy(int, struct employee );

int search_by_number();
int search_by_name(char );
int get_position();

int restore_procedure(int, int);
int delete_procedure(int ,int );

int change_salary(struct employee );
int change_contact_number(struct employee );
int change_address(struct employee );

void alphabetic_order(char );
void employee_id_order(char );
int employee_position(char );
void salary_range(char );
void date_of_join_range(char );

int list_entry_category(char );

int new_entry();
int search_entry();
int edit_entry();
int list_entry_type();
int delete_entry();
int restore_entry();
void show_log();

int user_menu();

/*----------------------------------------------------------------------------------------*/

int main()
{
    char *password;
    int temp, count = 0;
    int return_check;
    int return_check2;

    return_check = mkdir("Data");
    return_check2 = mkdir("Backup");


    if(return_check==0 && return_check2==-1)
    {
        display_time();
        printf("\n\n No Data Found!!");
        printf("\n Restoring Data from Backup....");
        restore_last_backup();
        strcpy(last_action, "Backup Restored");
        to_password_file();
    }
    else if(return_check==0 && return_check2==0)
    {
        strcpy(last_action, "Data Scan");
        to_password_file();
        startup:

            display_time();
            printf("\n\n No Previous Data Found");
            printf("\n\n Executing Startup Procedure");
            printf("\n Press Enter to proceed!!");
            getch();
            mkdir("Data");
            mkdir("Backup");
            from_password_file();
            first_startup();
            printf("\n Startup Successfull");
            startup_status = 1;
            to_password_file();
            getch();
            perform_backup();
    }
    else if(return_check==-1 && return_check2==0)
    {
        from_password_file();
        if(startup_status==1)
        {
            strcpy(last_action, "Data Scan");
            to_password_file();
            display_time();

            printf("\n No Backup Found!! ");
            printf("\n Performing Backup...");
            perform_backup();

            strcpy(last_action, "Performed Backup");
            to_password_file();
        }
        else
        {
            strcpy(last_action, "Data Scan");
            to_password_file();
            display_time();
            printf("\n\n Startup was incomplete");
            printf("\n Erasing Incomplete Data or Directories Retrieved!!");
            data_format_procedure();
            strcpy(last_action, "Erased Incomplete Data");
            to_password_file();
            goto startup;
        }
    }

    from_password_file();

    if(startup_status==1)
    {
        strcpy(last_action, "Data Scan");
        to_password_file();

        display_time();

        printf("\n\n\t\t\t\tEMPLOYEE MANAGEMENT PROGRAM");
        printf("\n Coded by Mahershi Bhavsar (160160107005)");
        printf("\n\n Features:");
        printf("\n\n   USER MENU has following features:");
        printf("\n\n - Allows to add new entry of employee");
        printf("\n - Allows to Search among existing Entries");
        printf("\n - Allows to List out employees depending on different categories and requirements");
        printf("\n - Allows to Delete an Exiting entry");
        printf("\n - Allows to Restore an Deleted entry");
        printf("\n - Allows to Delete and Exiting entry");
        printf("\n - View LOGS. All the Action performed during runtime are recorded");
        printf("\n\n   ADMIN MENU has following features:");
        printf("\n\n - Change USER password");
        printf("\n - Change ADMIN password");
        printf("\n - Change Current YEAR (ONLY if System Date is INCORRECT)");
        printf("\n - Perform BackUP");
        printf("\n - Restore to Last BackUP Point");
        printf("\n - Format Data - Erase All Exisitng Backups and Data includind Password.");
        getch();

        login:

            password = password_input("\n\n\t\t\tLOGIN", "\n\n PASSWORD");

            if(strcmp(password, user_password)==0)
            {
                user_login_status = 1;
                to_password_file();
                temp = user_menu();
                user_login_status = 0;
                to_password_file();
                strcpy(last_action, "Logged OUT: USER");
                to_password_file();
                if(temp!=1)
                    display_time();
                log.position = 0;
                log.id = 0;
                strcpy(log.lname, "(null)");
                strcpy(log.name,"(null)");
                record_operation("uo");
            }
            else if(strcmp(password, admin_password)==0)
            {
                from_password_file();
                if(admin_login_status==0)
                {
                    admin_login_status = 1;
                    to_password_file();
                    temp = admin_menu();
                    admin_login_status = 0;
                    to_password_file();
                    if(temp==-1)
                        goto startup;

                    strcpy(last_action, "Logged OUT: ADMIN");
                    to_password_file();
                    if(temp!=1)
                        display_time();
                    log.position = 0;
                    log.id = 0;
                    strcpy(log.lname, "(null)");
                    strcpy(log.name,"(null)");
                    record_operation("ao");
                }
                else
                {
                    printf("\n Admin Already Logged IN");
                    getch();
                    goto login;
                }
            }
            else
            {
                log.position = 0;
                log.id = 0;
                strcpy(log.lname, "(null)");
                strcpy(log.name,"(null)");
                record_operation("fi");
                count++;
                printf("\n Incorrect Password");
                printf("\n %d tries left", (3-count));
                strcpy(last_action, "Failed LOGIN attempt");
                to_password_file();
                getch();
                if(count<3)
                    goto login;
            }
            if(temp==1)
                goto login;
            if(count<3)
                getch();
            else
                exit(0);
    }
    else
    {
        printf("\n Startup was incomplete");
        printf("\n Erasing Incomplete Data Retrieved!!");
        data_format_procedure();
        goto startup;
    }
}

/*----------------------------------------------------------------------------------------*/

int admin_menu()
{
    int temp;

    log.position = 0;
    log.id = 0;
    strcpy(log.lname, "(null)");
    strcpy(log.name,"(null)");
    record_operation("ai");

    menu:

        from_password_file();
        strcpy(last_action, "Logged IN: ADMIN");
        to_password_file();
        display_time();
        log.position = 0;
        log.id = 0;
        strcpy(log.lname, "(null)");
        strcpy(log.name,"(null)");
        printf("\n\t\tADMINISTRATIVE MENU");
        printf("\n\n 1. Change User Password");
        printf("\n 2. Change Admin Password");
        printf("\n 3. Change Current Year");
        printf("\n 4. Perform Data BackUp (Last Backed Up: %s, %c%c / %c%c / %c%c%c%c)", backup_time, backup_date[0], backup_date[1], backup_date[2], backup_date[3], backup_date[4], backup_date[5], backup_date[6], backup_date[7]);
        printf("\n 5. Restore to Last Backup Point (%s, %c%c / %c%c / %c%c%c%c)", backup_time, backup_date[0], backup_date[1], backup_date[2], backup_date[3], backup_date[4], backup_date[5], backup_date[6], backup_date[7]);
        printf("\n 6. Format All Data");
        printf("\n 7. Activity LOG");
        printf("\n 8. Switch LOG IN");
        printf("\n 9. Log Out and Exit");
        printf("\n\n Your Choice: ");

        switch(getch())
        {
            case '1':
                        if(user_login_status==1)
                        {
                            printf("\n User Logged IN");
                            getch();
                        }
                        else
                            temp = set_user_password("\n\t\tChange User Password");

                        break;
            case '2':
                        temp = set_admin_password("\n\t\tChange Admin Password");
                        break;
            case '3':
                        temp = set_year("\n\n\tNOTE: This feature should be used only if system date is INCORRECT!!\n\n\t\t\tChange Current Year");
                        break;
            case '4':
                        perform_backup();
                        to_password_file();
                        break;
            case '5':
                        restore_last_backup();
                        from_password_file();
                        break;
            case '6':
                        temp = format_data();
                        if(temp==1)
                            return -1;

                        break;
            case '7':
                        check_data();
                        show_log();
                        break;
            case '8':
                        return 1;
                        break;
            case '9':
                        return 0;
                        break;
            default:
                        printf("\n Invalid Choice!");
                        getch();
        }
    from_password_file();
    goto menu;
}

/*----------------------------------------------------------------------------------------*/

int user_menu()
{
    int temp;
    log.position = 0;
    log.id = 0;
    strcpy(log.lname, "(null)");
    strcpy(log.name,"(null)");
    record_operation("ui");
    strcpy(last_action, "Logged IN: USER");
    to_password_file();

    menu:

        check_data();

        display_time();

        printf("%100s", "Last Backed UP: ");
        printf("%s, %c%c / %c%c / %c%c%c%c", backup_time, backup_date[0], backup_date[1], backup_date[2], backup_date[3], backup_date[4], backup_date[5], backup_date[6], backup_date[7]);
        printf("\n\t\t\t\tEmployee Management Program");
        printf("\n\t\tUSER Main Menu");
        printf("\n\n 1. New Entry");
        printf("\n 2. Search Entry");
        printf("\n 3. Edit Entry");
        printf("\n 4. List Entries");
        printf("\n 5. Delete Entry");
        printf("\n 6. Restore Entry");
        printf("\n 7. Switch LOG IN");
        printf("\n 8. Log Out and Exit");
        printf("\n\n Your Choice: ");

        while(1)
        {
            if(kbhit())
                {
                    switch(getch())
                    {
                        case '1':
                                    check_data();
                                    from_password_file();
                                    if(concurrency_avoid==0)
                                    {
                                        concurrency_avoid = 1;
                                        to_password_file();
                                        temp = new_entry();
                                        concurrency_avoid = 0;
                                        to_password_file();
                                    }
                                    else
                                    {
                                        printf("\n\n New Entry In Progress. Try again later!");
                                        getch();
                                    }
                                    break;
                        case '2':
                                    check_data();
                                    temp = search_entry();
                                    break;
                        case '3':
                                    check_data();
                                    temp = edit_entry();
                                    break;
                        case '4':
                                    check_data();
                                    temp = list_entry_type();
                                    break;
                        case '5':
                                    check_data();
                                    temp = delete_entry();
                                    break;
                        case '6':
                                    check_data();
                                    temp = restore_entry();
                                    break;
                        case '7':
                                    return 1;
                                    break;
                        case '8':
                                    return 0;
                                    break;
                        default:
                                    printf("\n\n Invalid Choice!!");
                                    getch();
                    }
                    goto menu;
                }
                strcpy(action, last_action);
                from_password_file();
                if(strcmp(action, last_action)!=0)
                    goto menu;
        }
}

/*----------------------------------------------------------------------------------------*/

int new_entry()
{
    struct employee emp;
    int salary_check;
    char choice1, choice2;
    int index, space_count;
    int count_entries;

    entry:

        display_time();
        printf("\n\t\tNew Entry");
        emp.position = get_position();

        display_time();
        printf("\n\t\tNew Entry");
        switch(emp.position)
        {
            case -1:
                        return 0;
            case 0:
                        goto entry;
            case 1:
                        printf("\n\n Position\t : Manager");
                        break;
            case 2:
                        printf("\n\n Position\t : Senior Employee");
                        break;
            case 3:
                        printf("\n\n Position\t : Junior Employee");
                        break;
            default:
                        printf("\n\n Position\t :Error Receiving Position");
                        getch();
                        return 0;
        }

        printf("\n\n Employee Name");
        printf("\n\n First Name       : ");
        scanf("%s", emp.name);
        emp.name[0] = toupper(emp.name[0]);
        if(strlen(emp.name)>15)
        {
            printf("\n First Name Too Long!");
            getch();
            goto entry;
        }
        else if(strlen(emp.name)==0)
        {
            printf("\n Invalid First Name!");
            getch();
            goto entry;
        }
        else
        {
            printf(" Last Name        : ");
            scanf("%s", emp.lname);
            emp.lname[0] = toupper(emp.lname[0]);
            if(strlen(emp.lname)>15)
            {
                printf("\n Last Name Too Long!");
                getch();
                goto entry;
            }
            else if(strlen(emp.lname)==0)
            {
                printf("\n Invalid Last Name!");
                getch();
                goto entry;
            }
            else
            {
                printf(" Salary           : ");
                salary_check=scanf("%ld", &emp.salary);
                getchar();
                if(emp.salary>0 && salary_check==1)
                {
                    emp.id = (total_entries() + 1);
                    printf("\n Employee ID      : %d", emp.id);
                    printf("\n\n Date of Join\n 1.System Date\n 2.Manual Input\n\n Your Choice: ");
                    choice1 = getch();

                    for_date:

                        index = 0;
                        space_count = 0;

                        display_time();
                        printf("\n\t\tNew Entry");
                        if(emp.position==1)
                            printf("\n\n Position\t\t  : Manager");
                        else if(emp.position==2)
                            printf("\n\n Position\t\t  : Senior Employee");
                        else
                            printf("\n\n Position\t\t  : Junior Employee");

                        printf("\n\n Employee Name");
                        printf("\n\n First Name                : %s", emp.name);
                        printf("\n Last Name                 : %s", emp.lname);
                        printf("\n Salary                    : %ld", emp.salary);
                        printf("\n Employee ID               : %d", emp.id);

                        switch(choice1)
                        {
                            case '1':
                                        emp.date_of_join = get_system_date();
                                        printf("\n Date of Join (dd/mm/yyyy) : %c%c / %c%c / %c%c%c%c", emp.date_of_join[0], emp.date_of_join[1], emp.date_of_join[2], emp.date_of_join[3], emp.date_of_join[4], emp.date_of_join[5], emp.date_of_join[6], emp.date_of_join[7]);
                                        break;
                            case '2':
                                        emp.date_of_join = calloc(9, sizeof(char *));
                                        emp.date_of_join[4] = year[0];
                                        emp.date_of_join[5] = year[1];
                                        emp.date_of_join[6] = year[2];
                                        emp.date_of_join[7] = year[3];

                                        printf("\n Date of Join (dd/mm/yyyy) :    /    / %c%c%c%c", emp.date_of_join[4], emp.date_of_join[5], emp.date_of_join[6], emp.date_of_join[7]);

                                        while(index<4)
                                        {
                                            gotoxy(29+space_count, 12);

                                            emp.date_of_join[index] = getche();


                                            if(!(emp.date_of_join[index]>=47 && emp.date_of_join[index]<=58))
                                                goto for_date;

                                            if(index==0 || index==2 || index==4 || index==5 || index==6 || index==7)
                                                space_count++;
                                            else
                                                space_count+=4;

                                            index++;
                                        }
                                        emp.date_of_join[8]='\0';
                                        if(emp.date_of_join[0]>'3' || emp.date_of_join[2]>'1' || (emp.date_of_join[2]=='1' && emp.date_of_join[3]>'2') || (emp.date_of_join[0]=='0' && emp.date_of_join[1]=='0') || (emp.date_of_join[2]=='0' && emp.date_of_join[3]=='0') || (emp.date_of_join[0]=='3' && emp.date_of_join[1]>'1') )
                                        {
                                            printf("\n\n Invalid Date!!");
                                            getch();
                                            goto for_date;
                                        }
                                        break;
                            default:
                                        printf("\n\n Invalid Choice!!");
                                        getch();
                                        goto entry;
                        }

                    printf("\n\n\n Contact Information");
                    printf("\n\n Contact Number            : ");
                    scanf("%s", emp.contact);
                    getchar();
                    if(strlen(emp.contact)!=10)
                    {
                        printf("\n\n Invalid Contact Number!!");
                        getch();
                        goto for_date;
                    }
                    else
                    {
                        for(index=0;index<10;index++)
                        {
                            if(!(emp.contact[index]>=47 && emp.contact[index]<=58))
                            {
                                printf("\n Invalid Contact Number");
                                getch();
                                goto for_date;
                            }
                        }

                        printf("\n Address (4 lines)         : ");
                        gets(emp.addr.line1);
                        printf("                           : ");
                        gets(emp.addr.line2);
                        printf("                           : ");
                        gets(emp.addr.line3);
                        printf("                           : ");
                        gets(emp.addr.line4);

                        if(strlen(emp.addr.line1)>25 || strlen(emp.addr.line2)>25 || strlen(emp.addr.line3)>25 || strlen(emp.addr.line4)>25)
                        {
                            printf("\n Address Too Long");
                            getch();
                            goto for_date;
                        }
                        else if(strlen(emp.addr.line1)==0 || strlen(emp.addr.line2)==0 || strlen(emp.addr.line3)==0 || strlen(emp.addr.line4)==0)
                        {
                            printf("\n Invalid Address");
                            getch();
                            goto for_date;
                        }
                        else
                        {
                            printf("\n\n Confirm Entry (Y/N)       : ");
                            scanf("%c", &choice2);
                            if(choice2=='Y' || choice2=='y')
                            {
                                emp.buffer2 = calloc(35, sizeof(char *));

                                emp.status = 'A';
                                sprintf(emp.buffer2, " %d %s %s", emp.id, emp.name, emp.lname);
                                to_data_file(emp.id, emp);

                                list_file = fopen("Data/list.txt", "a");
                                fprintf(list_file, "%s\n", emp.buffer2);

                                fclose(list_file);
                                free(emp.buffer2);

                                strcpy(log.name, emp.name);
                                strcpy(log.lname, emp.lname);
                                log.id = emp.id;
                                log.position = emp.position;
                                record_operation("ne");
                                printf("\n Entry Successfull!!");
                                sprintf(last_action, "NEW ENTRY: %d", emp.id);
                                to_password_file();
                            }
                            else if(choice2=='N' || choice2=='n')
                            {
                                printf("\n Entry Discarded! Press Enter to Continue!");
                                strcpy(last_action, "NEW ENTRY Discarded");
                                to_password_file();
                            }
                            else
                            {
                                printf("\n Invalid Choice!!");
                            }
                        }
                    }
                }
                else
                {
                    printf("\n\n Invalid Salary");
                    getch();
                    goto entry;
                }
            }
        }
    getch();
    return 0;
}

/*----------------------------------------------------------------------------------------*/

int search_entry()
{
    int temp;

    search:

        display_time();
        printf("\n\t\tSearch Entry");

        list_file = fopen("Data/list.txt", "rb");
        if(list_file!=NULL)
        {
            fclose(list_file);
            printf("\n\n 1. Search By Employee ID");
            printf("\n 2. Search By Employee Name");
            printf("\n 3. Back To Main Menu");
            printf("\n\n Your Choice: ");

            while(1)
            {
                if(kbhit())
                {
                    switch(getch())
                    {
                        case '1':
                                    temp = search_by_number();
                                    if(temp==0)
                                        record_operation("ve");

                                    break;
                        case '2':
                                    temp = search_by_name('A');
                                    if(temp!=-1)
                                    {
                                        record_operation("ve");
                                        getch();
                                    }
                                    break;
                        case '3':
                                    return 0;
                        default:
                                    printf("\n\n Invalid Input!");
                                    getch();
                    }
                    goto search;
                }
                strcpy(action, last_action);
                from_password_file();
                if(strcmp(action, last_action)!=0)
                    goto search;
            }
        }
        else
        {
            printf("\n No Entries Yet");
        }
        getch();

        return 0;
}

/*----------------------------------------------------------------------------------------*/

int search_by_number()
{
    int employee_id;
    int temp;

    display_time();
    printf("\n\t\tSEARCH - BY EMPLOYEE ID");
    printf("\n\n Employee ID: ");
    scanf("%d", &employee_id);
    getchar();
    temp = entry_details_by_number(employee_id, 'A');

    if(temp!=-1)
        getch();
    else
        return -1;

    sprintf(last_action, "Entry Viewed: %d", temp);
    to_password_file();

    return 0;
}

/*----------------------------------------------------------------------------------------*/

int search_by_name(char employee_status)
{
    struct employee emp;
    int *flag;
    char chr;
    char name_lname[30], lname_name[30];
    char fullname[30];
    int count_entries = 0, new_count = 0;
    int v, z=0, i;
    int check, employee_id;
    int temp;
    int matches;

    int check_update;
    int active_shown;
    int deleted_shown;

    repeat:

        check_update = 0;
        active_shown = 0;
        deleted_shown = 0;

        matches = 0;

        new_count = total_entries();

        if(new_count!=count_entries)
        {
            flag = calloc(new_count+1, sizeof(int));
        }

        count_entries = new_count;

        do
        {
            backspace:

                display_time();

                if(employee_status=='A')
                    printf("\n\t\tSEARCH - BY EMPLOYEE NAME (Active Entries)");
                else
                    printf("\n\t\tSEARCH - BY EMPLOYEE NAME (Deleted Entries)");

                printf("\n Employee Name: ");

                for(v=0;v<z;v++)
                    printf("%c", fullname[v]);

                printf("\n\n\n\t\t\t\tSearch Result");
                heading_particular_status();

                if(z==0 && count_entries!=0)
                    matches = 1;

                if(z!=0)
                {
                    for(i=1;i<=count_entries;i++)
                    {
                        emp = from_data_file(i);
                        sprintf(name_lname, "%s %s", emp.name, emp.lname);
                        sprintf(lname_name, "%s %s", emp.lname, emp.name);

                        if((strncasecmp(fullname, name_lname, z)!=0 && strncasecmp(fullname, lname_name, z)!=0) || emp.status!=employee_status)
                        {
                            flag[i]=1;
                        }
                        else
                        {
                            matches = 1;
                            flag[i]=0;
                        }
                    }
                }

                if(matches!=0)
                {
                    for(i=1;i<=count_entries;i++)
                    {
                        emp = from_data_file(i);
                        if(flag[i]==0 && emp.status==employee_status)
                        {
                                if(emp.status=='A')
                                    active_shown++;
                                else
                                    deleted_shown++;

                                list_particular_status2(emp);
                        }
                    }
                }
                else
                {
                    printf("\n No Match Found!");
                }

                gotoxy(16+z, 3);

                while(1)
                {
                    if(kbhit())
                    {
                        chr = getche();
                        break;
                    }
                    check_update = check_update_keyword(active_shown, deleted_shown, z, fullname, employee_status);

                    strcpy(action, last_action);
                    from_password_file();
                    if(strcmp(action, last_action)!=0)
                        goto repeat;

                    if(check_update==1)
                        goto repeat;
                }

                if(chr==8)
                {
                    if(z==0)
                        continue;

                    z--;

                    for(i=1;i<=count_entries;i++)
                    {
                        emp = from_data_file(i);
                        sprintf(name_lname, "%s %s", emp.name, emp.lname);
                        sprintf(lname_name, "%s %s", emp.lname, emp.name);
                        if((strncasecmp(fullname, name_lname, z)==0 || strncasecmp(fullname, lname_name, z)==0) && emp.status==employee_status)
                            flag[i]=0;
                    }

                    goto backspace;
                }

                fullname[z]=chr;
                z++;
        }while(chr!='\r');

        printf("\n Enter Employee ID for more details and -1 to exit... ");
        check = scanf("%d", &employee_id);
        getchar();
        if(employee_id!=-1)
        {
            if(check==1)
            {
                temp = entry_details_by_number(employee_id, employee_status); //Temp is employee id, if it exits else 0

                if(temp!=0)
                {
                    sprintf(last_action, "Entry Viewed: %d", temp);
                    to_password_file();
                }
                return temp;
            }
            else
            {
                printf("\n Invalid Input!!");
                getch();
            }
        }

        return -1;
}

/*----------------------------------------------------------------------------------------*/

int edit_entry()
{
    int employee_id;
    int temp;
    int temp2;
    struct employee emp;

    edit_entry_label:

        display_time();
        printf("\n\t\tEDIT Entry");

        list_file = fopen("Data/list.txt", "rb");
        if(list_file!=NULL)
        {
            printf("\n 1. Enter Employee ID");
            printf("\n 2. Enter Employee Name");
            printf("\n 3. Back To Main Menu");
            printf("\n\n Your Choice: ");

            while(1)
            {
                if(kbhit())
                {
                    switch(getch())
                    {
                        case '1':
                                    printf("\n\n Enter Employee ID: ");
                                    scanf("%d", &employee_id);
                                    getchar();
                                    temp = entry_details_by_number(employee_id, 'A');
                                    break;
                        case '2':
                                    employee_id = search_by_name('A');
                                    if(employee_id!=-1)
                                        temp = 1;
                                    break;
                        case '3':
                                    return 0;
                        default:
                                    printf("\n\n Invalid Input!!");
                                    getch();
                                    goto edit_entry_label;
                                    break;
                    }
                    break;
                }
                strcpy(action, last_action);
                from_password_file();
                if(strcmp(action, last_action)!=0)
                    goto edit_entry_label;
            }


            if(temp==1)
            {
                emp = from_data_file(employee_id);
                printf("\n\n\n\tEDIT OPTIONS");
                printf("\n 1. Change Salary ");
                printf("\n 2. Change Contact Number");
                printf("\n 3. Change Address");
                printf("\n\n Your Choice: ");

                switch(getch())
                {
                    case '1':
                                temp2 = change_salary(emp);
                                break;
                    case '2':
                                temp2  = change_contact_number(emp);
                                break;
                    case '3':
                                temp2 = change_address(emp);
                                break;
                    default:
                                printf("\n\n Invalid Input!!");
                                getch();
                                goto edit_entry_label;
                }

                if(temp2==1)
                {
                    printf("\n\n Entry Editted");
                    strcpy(log.name, emp.name);
                    strcpy(log.lname, emp.lname);
                    log.id = emp.id;
                    log.position = emp.position;
                    record_operation("ee");
                    getch();
                    sprintf(last_action, "Entry Edited: %d", emp.id);
                    to_password_file();
                }
            }
        }
        else
        {
            printf("\n No Entries Yet!!");
            getch();
        }


}

/*----------------------------------------------------------------------------------------*/

int change_salary(struct employee emp)
{
    long new_salary;
    int check;

    display_time();
    printf("\n\t\t Change Salary");
    entry_details(emp);
    printf("\n\n Enter New Salary: ");
    check = scanf("%ld", &new_salary);
    if(check==1 && new_salary>0)
    {
        emp.salary = new_salary;
        to_data_file(emp.id, emp);
        entry_details(emp);
        return 1;
    }
    else if(new_salary==emp.salary)
    {
        printf("\n New Salary cannot be Same as Old");
        getch();
        return 0;
    }
    else
    {
        printf("\n Invalid Salary");
        getch();
        return 0;
    }
}
/*----------------------------------------------------------------------------------------*/

int change_contact_number(struct employee emp)
{
    char new_contact[11];
    int check;

    display_time();
    printf("\n\t\t Change Contact Number");
    entry_details(emp);
    printf("\n\n New Contact Number: ");
    scanf("%s", new_contact);
    if(strlen(new_contact)==10)
    {
        strcpy(emp.contact, new_contact);
        to_data_file(emp.id, emp);
        entry_details(emp);
        return 1;
    }
    else
    {
        printf("\n Invalid Contact Number!");
        getch();
        return 0;
    }
}

/*----------------------------------------------------------------------------------------*/

int change_address(struct employee emp)
{
    char new_l1[25];
    char new_l2[25];
    char new_l3[25];
    char new_l4[25];

    display_time();
    printf("\n\t\t Change Address");
    entry_details(emp);

    printf("\n New Address (4 lines)         : ");
    gets(new_l1);
    printf("                               : ");
    gets(new_l2);
    printf("                               : ");
    gets(new_l3);
    printf("                               : ");
    gets(new_l4);

    if(strlen(new_l1)>25 || strlen(new_l2)>25 || strlen(new_l3)>25 || strlen(new_l4)>25)
    {
        printf("\n Address Too Long");
        getch();
        return 0;
    }
    else if(strlen(new_l1)==0 || strlen(new_l2)==0 || strlen(new_l3)==0 || strlen(new_l4)==0)
    {
        printf("\n Invalid Address");
        getch();
        return 0;
    }
    else
    {
        strcpy(emp.addr.line1, new_l1);
        strcpy(emp.addr.line2, new_l2);
        strcpy(emp.addr.line3, new_l3);
        strcpy(emp.addr.line4, new_l4);
        to_data_file(emp.id, emp);
        entry_details(emp);
        return 1;
    }
}

/*----------------------------------------------------------------------------------------*/

int list_entry_type()
{
    int temp;

    list_entry_type_label:

        display_time();
        printf("\n\t\tList Entry");
        list_file = fopen("Data/list.txt", "rb");
        if(list_file!=NULL)
        {
            printf("\n\n 1. Active Entries");
            printf("\n 2. Deleted Entries");
            printf("\n 3. List All Entries");
            printf("\n 4. Back To Main Menu");
            printf("\n\n Your Choice: ");

            while(1)
            {
                if(kbhit())
                {
                    switch(getch())
                    {
                        case '1':
                                    temp = list_entry_category('A');
                                    break;
                        case '2':
                                    temp = list_entry_category('D');
                                    break;
                        case '3':
                                    temp = list_entry_category('C');
                                    break;
                        case '4':
                                    return 0;
                        default:
                                    printf("\n Invalid Choice!!");
                                    getch();
                    }
                    goto list_entry_type_label;
                }
                strcpy(action, last_action);
                from_password_file();
                if(strcmp(action, last_action)!=0)
                    goto list_entry_type_label;
            }
        }
        else
        {
            printf("\n No Entries Yet!");
        }
        getch();
}

/*----------------------------------------------------------------------------------------*/

int list_entry_category(char emp_status)
{
    int temp;

    list_entry_category_label:

        display_time();
        if(emp_status=='A')
            printf("\n\t\tLIST - Active Entries");
        else if(emp_status=='D')
            printf("\n\t\tLIST - Deleted Entries");
        else
            printf("\n\t\tLIST - All Entries");

        printf("\n\n 1. Employee ID Order");
        printf("\n 2. Alphabetic Order");
        printf("\n 3. Salary Range");
        printf("\n 4. Date of Join Range");
        printf("\n 5. Employee Postion");
        printf("\n 6. Back To LIST TYPE");
        printf("\n\n Your Choice: ");

        switch(getch())
        {
            case '1':
                        employee_id_order(emp_status);
                        break;
            case '2':
                        alphabetic_order(emp_status);
                        break;
            case '3':
                        salary_range(emp_status);
                        break;
            case '4':
                        date_of_join_range(emp_status);
                        break;
            case '5':
                        temp = employee_position(emp_status);
                        break;
            case '6':
                        return 0;
            default:
                        printf("\n Invalid Choice!!");
                        getch();
                        goto list_entry_category_label;
        }

        strcpy(last_action, "Entries Viewed");
        to_password_file();
}

/*----------------------------------------------------------------------------------------*/

void employee_id_order(char emp_status)
{
    struct employee emp;
    int flag;
    int index;
    int count_entries;
    int order = 0;

    int check;
    int active_shown;
    int deleted_shown;

    display_time();
    if(emp_status=='C')
        order = get_order("All Entries");
    else
        if(emp_status=='A')
            order = get_order("Active Entries");
        else
            order = get_order("Deleted Entries");

    repeat:

        display_time();
        flag = 0;
        deleted_shown = 0;
        active_shown = 0;
        count_entries = total_entries();

        if(emp_status=='C')
        {
            flag = 1;
            if(order==0)
            {
                printf("\n\t\tLIST - All Entries (Employee ID Order) - Ascending Order");
                heading_all_status();
                for(index=1; index<=count_entries; index++)
                {
                    if(emp.status=='A')
                        active_shown++;
                    else
                        deleted_shown++;
                    emp = from_data_file(index);
                    list_all_status2(emp);
                }
            }
            else
            {
                printf("\n\t\tLIST - All Entries (Employee ID Order) - Descending Order");
                heading_all_status();
                for(index=count_entries; index>0; index--)
                {
                    if(emp.status=='A')
                        active_shown++;
                    else
                        deleted_shown++;
                    emp = from_data_file(index);
                    list_all_status2(emp);
                }
            }
        }
        else
        {
            display_time();
            if(order==0)
            {
                if(emp_status=='A')
                    printf("\n\t\tLIST - Active Entries (Employee ID Order) - Ascending Order");
                else
                    printf("\n\t\tLIST - Deleted Entries (Employee ID Order) - Ascending Order");

                heading_particular_status();
                for(index=1; index<=count_entries; index++)
                {
                    emp = from_data_file(index);
                    if(emp.status==emp_status)
                    {
                        if(emp.status=='A')
                            active_shown++;
                        else
                            deleted_shown++;

                        flag = 1;
                        list_particular_status2(emp);
                    }
                }
            }
            else
            {
                if(emp.status=='A')
                    printf("\n\t\tLIST - Active Entries (Employee ID Order) - Descending Order");
                else
                    printf("\n\t\tLIST - Deleted Entries (Employee ID Order) - Descending Order");

                heading_particular_status();
                for(index=count_entries; index>0; index--)
                {
                    emp = from_data_file(index);
                    if(emp.status==emp_status)
                    {
                        if(emp.status=='A')
                            active_shown++;
                        else
                            deleted_shown++;

                        flag = 1;
                        list_particular_status2(emp);
                    }
                }
            }
        }

        if(flag==0)
            printf("\n No Entries Found!!");

        while(!kbhit())
        {
            check = check_update_status(active_shown, deleted_shown, emp_status);

            if(check==1)
                goto repeat;
        }
    getch();
}

/*----------------------------------------------------------------------------------------*/

void alphabetic_order(char emp_status)
{
    struct employee emp;
    int count_entries;
    int index, index_for_dummy, index2;
    int flag;

    int check;
    int active_shown;
    int deleted_shown;

    repeat:

        flag = 0;
        active_shown = 0;
        deleted_shown = 0;

        count_entries = total_entries();
        d = calloc(count_entries+1, sizeof(*d));

        if(emp_status=='C')
        {
            flag = 1;
            for(index=1, index_for_dummy=1; index<=count_entries; index++, index_for_dummy++)
            {
                if(emp.status=='A')
                    active_shown++;
                else
                    deleted_shown++;

                emp = from_data_file(index);
                copy_data_to_dummy(index_for_dummy, emp);
            }
        }
        else
        {
            for(index=1, index_for_dummy=1; index<=count_entries; index++)
            {
                emp = from_data_file(index);
                if(emp.status==emp_status)
                {
                    if(emp.status=='A')
                        active_shown++;
                    else
                        deleted_shown++;

                    flag = 1;
                    copy_data_to_dummy(index_for_dummy, emp);
                    index_for_dummy++;
                }
            }
        }

        for(index=1; index<index_for_dummy; index++)
        {
            for(index2=index+1; index2<index_for_dummy; index2++)
            {
                if(stricmp(d[index].lname, d[index2].lname)>0)
                    swapping_dummy(index, index2);
                else if(stricmp(d[index].lname, d[index2].lname)==0)
                {
                    if(stricmp(d[index].name, d[index2].name)>0)
                        swapping_dummy(index, index2);
                }
                else
                    continue;
            }
        }

        display_time();
        if(emp_status=='C')
        {
            printf("\n\t\tLIST - All Entries (Alphabetic Order)");
            heading_all_status();

            for(index=1; index<index_for_dummy;index++)
                list_all_status1(index);
        }
        else
        {
            if(emp_status=='A')
                printf("\n\t\tLIST - Active Enntries (Alphabetic Order)");
            else
                printf("\n\t\tLIST - Deleted Entries (Alphabetic Order)");

            heading_particular_status();
            for(index=1; index<index_for_dummy;index++)
                list_particular_status1(index);
        }
        if(flag==0)
            printf("\n\n No Entries Found!!");
        free(d);

        while(!kbhit())
        {
            check = check_update_status(active_shown, deleted_shown, emp_status);

            if(check==1)
                goto repeat;

        }
    getch();
}

/*----------------------------------------------------------------------------------------*/

int employee_position(char emp_status)
{
    struct employee emp;
    int position, count_entries;
    int flag;
    int index;

    int check;
    int active_shown;
    int deleted_shown;

    display_time();
    printf("\n\t\tLIST - By Employee Position");
    position = get_position();

    repeat:

        flag = 0;
        active_shown = 0;
        deleted_shown = 0;

        display_time();
        count_entries = total_entries();
        printf("\n\t\tLIST - By Employee Position");
        if(position==1)
            printf(" - Manager");
        else if(position==2)
            printf(" - Senior Employee");
        else if(position==3)
            printf(" - Junior Employee");
        else
        {
            printf("\n Error Receiving Position");
            getch();
            return 0;
        }

        if(emp_status=='C')
        {
            printf(" (All Entries)");
            heading_all_status();
            flag = 1;
            for(index=1; index<=count_entries; index++)
            {
                emp = from_data_file(index);
                if(emp.position==position)
                {
                    if(emp.status=='A')
                        active_shown++;
                    else
                        deleted_shown++;

                    list_all_status2(emp);
                }
            }
        }
        else
        {
            if(emp_status=='A')
                printf(" (Active Entries)");
            else
                printf(" Deleted Entries");

            heading_particular_status();
            for(index=1; index<=count_entries; index++)
            {
                emp = from_data_file(index);
                if(emp.status==emp_status && position==emp.position)
                {
                    if(emp.status=='A')
                        active_shown++;
                    else
                        deleted_shown++;

                    flag = 1;
                    list_particular_status2(emp);
                }
            }
        }
        if(flag==0)
            printf("\n No Entries Found!!");

        while(!kbhit())
        {
            check = check_update_position(active_shown, deleted_shown, position, emp_status);

            if(check==1)
                goto repeat;
        }
    getch();
}

/*----------------------------------------------------------------------------------------*/

void salary_range(char emp_status)
{
    struct employee emp;
    long upper_limit, lower_limit;
    int return_check1, return_check2;
    int flag;
    int index, index2, index_for_dummy;
    int count_entries;
    int check;
    int active_shown;
    int deleted_shown;

    display_time();
    printf("\n\t\tLIST - Salary Range");
    printf("\n\n Lower Limit: ");
    return_check1 = scanf("%ld", &lower_limit);
    getchar();
    printf("\n\n Upper Limit: ");
    return_check2 = scanf("%ld", &upper_limit);
    getchar();

    if(return_check1==1 && return_check1==1 && lower_limit<upper_limit)
    {
        repeat:

            active_shown = 0;
            deleted_shown = 0;
            check = 0;
            flag = 0;

            count_entries = total_entries();
            d = calloc(count_entries+1, sizeof(*d));

            if(emp_status=='C')
            {

                for(index=1, index_for_dummy=1; index<=count_entries; index++)
                {
                    emp  = from_data_file(index);
                    if(emp.salary>=lower_limit && emp.salary<=upper_limit)
                    {
                        if(emp.status=='A')
                            active_shown++;
                        else
                            deleted_shown++;
                        flag = 1;
                        copy_data_to_dummy(index_for_dummy, emp);
                        index_for_dummy++;
                    }
                }
            }
            else
            {

                for(index=1, index_for_dummy=1; index<=count_entries; index++)
                {
                    emp = from_data_file(index);
                    if(emp.salary>=lower_limit && emp.salary<=upper_limit && emp.status==emp_status)
                    {
                        if(emp.status=='A')
                            active_shown++;
                        else
                            deleted_shown++;
                        flag = 1;
                        copy_data_to_dummy(index_for_dummy, emp);
                        index_for_dummy++;
                    }
                }
            }

            for(index=1; index<index_for_dummy; index++)
            {
                for(index2=index+1; index2<index_for_dummy; index2++)
                {
                    if(d[index].salary < d[index2].salary)
                        swapping_dummy(index,index2);
                }
            }

            display_time();
            if(emp_status=='C')
            {
                printf("\n\t\tLIST - All Entries (Salary Range between %ld and %ld)", lower_limit, upper_limit);
                heading_all_status();
                for(index=1; index<index_for_dummy; index++)
                    list_all_status1(index);
            }
            else
            {
                if(emp_status=='A')
                    printf("\n\t\tLIST - Active Entries (Salary Range between %ld and %ld)", lower_limit, upper_limit);
                else
                    printf("\n\t\tLIST - Deleted Entries (Salary Range between %ld and %ld)", lower_limit, upper_limit);

                heading_particular_status();
                for(index=1; index<index_for_dummy; index++)
                    list_particular_status1(index);
            }

            if(flag==0)
                printf("\n No Entries Found!");
            free(d);

            while(!kbhit())
            {
                check = check_update_salary_range(active_shown, deleted_shown, lower_limit, upper_limit, emp_status);

                if(check==1)
                    goto repeat;
            }
    }
    else
        printf("\n Invalid Salary Input!");


    getch();
}

/*----------------------------------------------------------------------------------------*/

void date_of_join_range(char emp_status)
{
    struct employee emp;
    int upper_limit_check;
    int lower_limit_check;
    int index, index_for_dummy;
    char *lower_limit, *upper_limit;
    int order = 0, dummy = 0;
    int count_entries;
    int flag;

    int check;
    int active_shown;
    int deleted_shown;

    lower_limit = date_input("From Date");
    upper_limit = date_input("To Date");

    date:

        display_time();

        if(emp_status=='C')
            printf("\n\t\tLIST - All Entries");
        else
        {
            if(emp_status=='A')
                printf("\n\t\tLIST - Active Entries");
            else
                printf("\n\t\tLIST - Deleted Entries");
        }

        printf(" (FROM  %c%c / %c%c / %c%c%c%c  TO  %c%c / %c%c / %c%c%c%c)", lower_limit[0], lower_limit[1], lower_limit[2], lower_limit[3], lower_limit[4], lower_limit[5], lower_limit[6], lower_limit[7], upper_limit[0], upper_limit[1], upper_limit[2], upper_limit[3], upper_limit[4], upper_limit[5], upper_limit[6], upper_limit[7]);

        if(dummy==0)
        {
            printf("\n\n 1. Latest First");
            printf("\n 2. Oldest First");
            printf("\n\n Your Choice: ");

            switch(getch())
            {
                case '1':
                            dummy = 1;
                            order = 0;
                            break;
                case '2':
                            dummy = 1;
                            order = 1;
                            break;
                default:
                            printf("\n Invalid Choice");
                            getch();
            }
            if(dummy==0)
                goto date;

            repeat:

                display_time();

                if(emp_status=='C')
                    printf("\n\t\tLIST - All Entries");
                else
                {
                    if(emp_status=='A')
                        printf("\n\t\tLIST - Active Entries");
                    else
                        printf("\n\t\tLIST - Deleted Entries");
                }

                printf(" (FROM  %c%c / %c%c / %c%c%c%c  TO  %c%c / %c%c / %c%c%c%c)", lower_limit[0], lower_limit[1], lower_limit[2], lower_limit[3], lower_limit[4], lower_limit[5], lower_limit[6], lower_limit[7], upper_limit[0], upper_limit[1], upper_limit[2], upper_limit[3], upper_limit[4], upper_limit[5], upper_limit[6], upper_limit[7]);

                active_shown = 0;
                deleted_shown = 0;
                check = 0;
                flag = 0;

                count_entries = total_entries();


                d = calloc(count_entries+1, sizeof(*d));

                if(emp_status=='C')
                {
                    for(index=1, index_for_dummy=1; index<=count_entries; index++)
                    {
                        emp = from_data_file(index);
                        upper_limit_check = date_limit_check(emp.date_of_join, upper_limit);
                        lower_limit_check = date_limit_check(emp.date_of_join, lower_limit);

                        if((upper_limit_check==-1 && lower_limit_check==1) || (upper_limit_check==0 || lower_limit_check==0))
                        {
                            if(emp.status=='A')
                                active_shown++;
                            else
                                deleted_shown++;

                            flag = 1;
                            copy_data_to_dummy(index_for_dummy, emp);
                            index_for_dummy++;
                        }
                    }

                    heading_all_status();

                    if(order==0)
                    {
                        for(index = index_for_dummy-1; index>0; index--)
                            list_all_status1(index);
                    }
                    else
                    {
                        for(index = 1; index<index_for_dummy; index++)
                            list_all_status1(index);
                    }
                }
                else
                {
                    for(index=1, index_for_dummy=1; index<=count_entries; index++)
                    {
                        emp = from_data_file(index);
                        if(emp.status==emp_status)
                        {
                            upper_limit_check = date_limit_check(emp.date_of_join, upper_limit);
                            lower_limit_check = date_limit_check(emp.date_of_join, lower_limit);

                            if((upper_limit_check==-1 && lower_limit_check==1) || (upper_limit_check==0 || lower_limit_check==0))
                            {
                                if(emp.status=='A')
                                    active_shown++;
                                else
                                    deleted_shown++;

                                flag = 1;
                                copy_data_to_dummy(index_for_dummy, emp);
                                index_for_dummy++;
                            }
                        }
                    }

                    heading_particular_status();
                    if(order==0)
                    {
                        for(index = index_for_dummy-1; index>0; index--)
                            list_particular_status1(index);
                    }
                    else
                    {
                        for(index = 1; index<index_for_dummy; index++)
                            list_particular_status1(index);
                    }
                }
                if(flag==0)
                    printf("\n No Entries Found!");


                free(d);

                while(!kbhit())
                {
                    check = check_update_doj_range(active_shown, deleted_shown, lower_limit, upper_limit, emp_status);

                    if(check==1)
                        goto repeat;
                }
            getch();
        }
}

/*----------------------------------------------------------------------------------------*/

int date_limit_check(char *date, char *date_limit)
{
    if(date[4]>date_limit[4])
        return 1;
    else if(date[4]<date_limit[4])
        return -1;
    else
    {
        if(date[5]>date_limit[5])
            return 1;
        else if(date[5]<date_limit[5])
            return -1;
        else
        {
            if(date[6]>date_limit[6])
                return 1;
            else if(date[6]<date_limit[6])
                return -1;
            else
            {
                if(date[7]>date_limit[7])
                    return 1;
                else if(date[7]<date_limit[7])
                    return -1;
                else
                {
                    if(date[2]>date_limit[2])
                        return 1;
                    else if(date[2]<date_limit[2])
                        return -1;
                    else
                    {
                        if(date[3]>date_limit[3])
                            return 1;
                        else if(date[3]<date_limit[3])
                            return -1;
                        else
                        {
                            if(date[0]>date_limit[0])
                                return 1;
                            else if(date[0]<date_limit[0])
                                return -1;
                            else
                            {
                                if(date[1]>date_limit[1])
                                    return 1;
                                else if(date[1]<date_limit[1])
                                    return -1;
                                else
                                    return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------------------*/

char *date_input(char placeholder[])
{
    char *date;
    int index, space_count;

    date = calloc(8, sizeof(char *));

    date:

        index = 0;
        space_count = 0;
        display_time();
        printf("\n\t\tLIST - Date of Join Range");
        printf("\n\n %s (dd / mm / yyyy):    /    /", placeholder);

        while(index<8)
        {
            gotoxy(strlen(placeholder)+20+space_count, 5);
            date[index] = getche();
            if(date[index]==8)
                goto date;
            if(index==0 || index==2 || index==4 || index==5 || index==6 || index==7)
                space_count++;
            else
                space_count+=4;
            index++;
        }
        date[index]='\0';
        if(date[0]>'3' || date[0]<'0' || date[2]>'1' || (date[0]=='0' && date[1]=='0') || (date[2]=='0' && date[3]=='0'))
        {
            printf("\n Invalid Date!!");
            getch();
            goto date;
        }
        return (char *)date;
}

/*----------------------------------------------------------------------------------------*/

int delete_entry()
{
    int employee_id;
    int check_return;
    int temp;

    delete_entry_label:

        display_time();
        printf("\n\t\tDELETE ENTRY");
        list_file = fopen("Data/list.txt", "rb");
        if(list_file!=NULL)
        {
            fclose(list_file);
            printf("\n\n 1. Enter Employee ID");
            printf("\n 2. Search for ID");
            printf("\n 3. Back To Main Menu");
            printf("\n\n Your Choice: ");

            while(1)
            {
                if(kbhit())
                {
                    switch(getch())
                    {
                        case '1':
                                    enter_id:
                                    display_time();

                                    printf("\n\t\tDELETE ENTRY");
                                    printf("\n\n Employee ID: ");
                                    check_return = scanf("%d", &employee_id);
                                    getchar();
                                    if(check_return==1)
                                    {
                                        temp = delete_procedure(employee_id, 1);
                                    }
                                    else
                                    {
                                        printf("\n Invalid ID");
                                        getch();
                                        goto enter_id;
                                    }
                                    break;
                        case '2':
                                    employee_id = search_by_name('A');
                                    if(employee_id!=-1)
                                        temp = delete_procedure(employee_id, 0);
                                    break;
                        case '3':
                                    return 0;
                        default:
                                    printf("\n\n Invalid Input!!");
                                    getch();
                                    goto delete_entry_label;
                    }
                    break;
                }

                strcpy(action, last_action);
                from_password_file();
                if(strcmp(action, last_action)!=0)
                    goto delete_entry_label;
            }
        }
        else
        {
            printf("\n No Entries Yet!!");
            fclose(list_file);
            getch();
        }

return 0;
}

/*----------------------------------------------------------------------------------------*/

int delete_procedure(int employee_id, int show)
{
    struct employee emp;
    int flag = 0;
    char chr;
    char choice;
    char *password;

    delete_procedure:

        list_file = fopen("Data/list.txt", "rb");
        while((chr=fgetc(list_file))!=EOF)
        {
            emp.buffer2 = calloc(35, sizeof(char *));

            fscanf(list_file, "%[^\n]", emp.buffer2);
            sscanf(emp.buffer2, "%d %s %s", &emp.id, &emp.name, &emp.lname);
            if(emp.id==employee_id)
            {
                emp = from_data_file(employee_id);
                if(emp.status=='A')
                    flag = 1;
                break;
            }
        }
        fclose(list_file);
        free(emp.buffer2);
        if(flag==1)
        {
            if(show==1)
            {
                display_time();
                entry_details(emp);
            }
            show = 1;
            printf("\n\n Confirm Deletion of Entry (Y/N)?  ");
            choice = getche();
            if(choice=='Y' || choice=='y')
            {
                password = password_input("\n\t\tConfirm Deletion", "\n\n User Password");
                from_password_file();
                if(strcmp(password, user_password)==0)
                {
                    emp.status = 'D';
                    sprintf(last_action, "Entry Deleted: %d", emp.id);
                    to_data_file(employee_id, emp);

                    strcpy(log.name, emp.name);
                    strcpy(log.lname, emp.lname);
                    log.id = emp.id;
                    log.position = emp.position;
                    record_operation("de");
                    printf("\n Entry Deletion Successfull");

                    to_password_file();
                    getch();
                }
                else
                {
                    strcpy(log.name, emp.name);
                    strcpy(log.lname, emp.lname);
                    log.id = emp.id;
                    log.position = emp.position;
                    record_operation("da");
                    printf("\n Incorrect Password!!");
                    printf("\n Deletion Aborted!");
                    strcpy(last_action, "Deletion Aborted");
                    to_password_file();
                    getch();
                }
            }
            else if(choice=='N' || choice=='n')
            {
                strcpy(log.name, emp.name);
                strcpy(log.lname, emp.lname);
                log.id = emp.id;
                log.position = emp.position;
                record_operation("da");
                printf("\n Deletion Aborted!");
                strcpy(last_action, "Deletion Aborted");
                to_password_file();
                getch();
            }
            else
            {
                printf("\n\n Invalid Choice!!");
                getch();
                goto delete_procedure;
            }
    }
    else
    {
        if(show==1)
            printf("\n\n Entry Not Found");
        getch();
    }
    return 0;
}

/*----------------------------------------------------------------------------------------*/

int restore_entry()
{
    int check_return;
    int employee_id;
    int temp;

    restore_entry:

        display_time();
        printf("\n\t\tRESTORE ENTRY");
        list_file = fopen("Data/list.txt", "rb");
        if(list_file!=NULL)
        {
            fclose(list_file);
            printf("\n\n 1. Enter Employee ID");
            printf("\n 2. Search for ID");
            printf("\n 3. Back To Main Menu");
            printf("\n\n Your Choice: ");

            while(1)
            {
                if(kbhit())
                {
                    switch(getch())
                    {
                        case '1':
                                    enter_id:
                                    display_time();

                                    printf("\n\t\tRESTORE ENTRY");
                                    printf("\n\n Employee ID: ");
                                    check_return = scanf("%d", &employee_id);
                                    getchar();
                                    if(check_return==1)
                                    {
                                        temp = restore_procedure(employee_id, 1);
                                    }
                                    else
                                    {
                                        printf("\n Invalid ID");
                                        getch();
                                        goto enter_id;
                                    }
                                    break;
                        case '2':
                                    employee_id = search_by_name('D');
                                    if(employee_id!=-1)
                                        temp = restore_procedure(employee_id, 0);
                                    break;
                        case '3':
                                    return 0;
                        default:
                                    printf("\n\n Invalid Input!!");
                                    getch();
                                    goto restore_entry;
                    }

                    break;
                }

                strcpy(action, last_action);
                from_password_file();
                if(strcmp(action, last_action)!=0)
                    goto restore_entry;

            }
        }
        else
        {
            printf("\n No Entries Yet!!");
            fclose(list_file);
            getch();
        }

    return 0;
}

/*----------------------------------------------------------------------------------------*/

int restore_procedure(int employee_id, int show)
{
    struct employee emp;
    int flag = 0;
    char chr;
    char choice;
    char *password;

    restore_procedure:

        list_file = fopen("Data/list.txt", "rb");
        while((chr=fgetc(list_file))!=EOF)
        {
            emp.buffer2 = calloc(35, sizeof(char *));

            fscanf(list_file, "%[^\n]", emp.buffer2);
            sscanf(emp.buffer2, "%d %s %s", &emp.id, &emp.name, &emp.lname);
            if(emp.id==employee_id)
            {
                emp = from_data_file(employee_id);
                if(emp.status=='D')
                    flag = 1;
                break;
            }
        }
        fclose(list_file);
        free(emp.buffer2);
        if(flag==1)
        {
            if(show==1)
            {
                display_time();
                entry_details(emp);
            }
            show = 1;
            printf("\n\n Confirm Restoration of Entry (Y/N)?  ");
            choice = getche();
            if(choice=='Y' || choice=='y')
            {
                password = password_input("\n\t\tConfirm Restoration", "\n\n User Password");
                from_password_file();
                if(strcmp(password, user_password)==0)
                {
                    emp.status = 'A';
                    sprintf(last_action, "Entry Restored: %d", emp.id);
                    to_data_file(employee_id, emp);
                    strcpy(log.name, emp.name);
                    strcpy(log.lname, emp.lname);
                    log.id = emp.id;
                    log.position = emp.position;
                    record_operation("re");
                    printf("\n Entry Restoration Successfull");

                    to_password_file();
                    getch();
                }
                else
                {
                    strcpy(log.name, emp.name);
                    strcpy(log.lname, emp.lname);
                    log.id = emp.id;
                    log.position = emp.position;
                    record_operation("ra");
                    printf("\n Incorrect Password!!");
                    printf("\n Restoration Aborted!");
                    strcpy(last_action, "Restoration Aborted");
                    to_password_file();
                    getch();
                }
            }
        else if(choice=='N' || choice=='n')
        {
            strcpy(log.name, emp.name);
            strcpy(log.lname, emp.lname);
            log.id = emp.id;
            log.position = emp.position;
            record_operation("ra");
            printf("\n Restoration Aborted!");
            strcpy(last_action, "Restoration Aborted");
            to_password_file();
            getch();
        }
        else
        {
            printf("\n\n Invalid Choice!!");
            getch();
            goto restore_procedure;
        }
    }
    else
    {
        if(show==1)
            printf("\n\n Entry Not Found");
        getch();
    }
    return 0;
}

/*----------------------------------------------------------------------------------------*/

int entry_details_by_number(int employee_id, char employee_status)
{
    char chr;
    int flag=0;
    struct employee emp;

    list_file = fopen("Data/list.txt", "r");

    emp.buffer2 = calloc(35, sizeof(char *));

    while((chr=fgetc(list_file))!=EOF)
    {


        fscanf(list_file, "%[^\n]", emp.buffer2);
        sscanf(emp.buffer2, "%d %s %s", &emp.id, &emp.name, &emp.lname);

        if(emp.id==employee_id)
        {
            fclose(list_file);
            flag=1;
            emp = from_data_file(employee_id);
            break;
        }
    }

    free(emp.buffer2);

    if(flag==0 || (flag==1 && emp.status!=employee_status))
    {
        printf("\n Entry Not Found!!");
        getch();
        return -1;
    }
    else
    {
        entry_details(emp);
        return emp.id;
    }
}

/*----------------------------------------------------------------------------------------*/

void copy_data_to_dummy(int index, struct employee emp)
{
    d[index].date_of_join = malloc(sizeof(char *)*9);

    d[index].id = emp.id;
    d[index].position = emp.position;
    strncpy(d[index].name, emp.name, strlen(emp.name));

    strncpy(d[index].lname, emp.lname, strlen(emp.lname));
    strcpy(d[index].date_of_join, emp.date_of_join);
    strncpy(d[index].contact, emp.contact, strlen(emp.contact));

    d[index].salary = emp.salary;
    d[index].status = emp.status;
}

/*----------------------------------------------------------------------------------------*/

void swapping_dummy(int index, int index2)
{
    int temp;
    char name_temp[15];
    char contact_temp[11];
    char *date_of_join_temp;
    char status_temp;
    long salary_temp;

    date_of_join_temp = calloc(8, sizeof(char *));
    temp = d[index].id;
    d[index].id = d[index2].id;
    d[index2].id = temp;
    temp = d[index].position;
    d[index].position = d[index2].position;
    d[index2].position = temp;
    status_temp = d[index].status;
    d[index].status = d[index2].status;
    d[index2].status = d[index].status;
    strcpy(name_temp, d[index].name);
    strcpy(d[index].name, d[index2].name);
    strcpy(d[index2].name, name_temp);
    strcpy(name_temp, d[index].lname);
    strcpy(d[index].lname, d[index2].lname);
    strcpy(d[index2].lname, name_temp);
    strcpy(date_of_join_temp, d[index].date_of_join);
    strcpy(d[index].date_of_join, d[index2].date_of_join);
    strcpy(d[index2].date_of_join, date_of_join_temp);
    salary_temp = d[index].salary;
    d[index].salary = d[index2].salary;
    d[index2].salary = salary_temp;
    strcpy(contact_temp, d[index].contact);
    strcpy(d[index].contact, d[index2].contact);
    strcpy(d[index2].contact, contact_temp);
}

/*----------------------------------------------------------------------------------------*/

struct employee from_data_file(int employee_id)
{
    struct employee emp;
    int dummy_index;
    int number_of_digits=0;
    dummy_index = employee_id;

    while(dummy_index!=0)
    {
        dummy_index = dummy_index/10;
        number_of_digits++;
    }

    emp.date_of_join = malloc(sizeof(char *)*8);

    emp.buffer1 = calloc(70, sizeof(char *));

    emp.fname = malloc(sizeof(char *)*(number_of_digits+11));
    sprintf(emp.fname, "Data/en%d.txt", employee_id);
    read_file = fopen(emp.fname, "r");
    fscanf(read_file, "%c\n %[^\n] %[^\n] %[^\n] %[^\n] %[^\n]", &emp.status, emp.buffer1\
           , emp.addr.line1, emp.addr.line2, emp.addr.line3, emp.addr.line4);
    fclose(read_file);


    sscanf(emp.buffer1, "%d %s %s %ld %s %s %d", &emp.id, emp.name, emp.lname, &emp.salary\
           , emp.date_of_join, emp.contact, &emp.position);

    free(emp.buffer1);
    free(emp.fname);
    return emp;
}

/*----------------------------------------------------------------------------------------*/

int get_position()
{
    printf("\n\n Employee Position");
    printf("\n 1. Manager");
    printf("\n 2. Senior Employee");
    printf("\n 3. Junior Employee");
    printf("\n 4. Back to Main Menu");
    printf("\n\n Position: ");

    switch(getch())
    {
        case '1':
                    return 1;
        case '2':
                    return 2;
        case '3':
                    return 3;
        case '4':
                    return -1;
        default:
                    printf("\n\n Invalid Choice!!");
                    getch();
                    return 0;
    }
}

/*----------------------------------------------------------------------------------------*/

int total_entries()
{
    char chr;
    int i=1;

    list_file = fopen("Data/list.txt", "rb");

    if(list_file!=NULL)
    {
        while((chr=fgetc(list_file))!=EOF)
        {
            if(chr=='\n')
                i++;
        }
        fclose(list_file);
    }
    return i-1;
}

/*----------------------------------------------------------------------------------------*/

int total_log_entries()
{
    char chr;
    int i=1;

    read_log_file = fopen("Data/log.txt", "rb");

    if(read_log_file!=NULL)
    {
        while((chr=fgetc(read_log_file))!=EOF)
        {
            if(chr=='\n')
                i++;
        }
        fclose(read_log_file);
    }
    return i-1;
}

/*----------------------------------------------------------------------------------------*/

int total_entries_backedup()
{
    FILE *list_file_backup;

    char chr;
    int i=1;

    list_file_backup = fopen("Backup/listbk.txt", "rb");

    if(list_file_backup!=NULL)
    {
        while((chr=fgetc(list_file_backup))!=EOF)
        {
            if(chr=='\n')
                i++;
        }
        fclose(list_file_backup);
    }
    return i-1;
}

/*----------------------------------------------------------------------------------------*/

char *get_system_date()
{
    char date[10];
    char *return_date;
    _strdate(date);
    date[10]='\0';
    return_date = malloc(sizeof(char *)*9);
    return_date[0] = date[3];
    return_date[1] = date[4];
    return_date[2] = date[0];
    return_date[3] = date[1];
    return_date[4] = year[0];
    return_date[5] = year[1];
    return_date[6] = date[6];
    return_date[7] = date[7];
    return_date[8] = '\0';

    return (char *)return_date;
}

/*----------------------------------------------------------------------------------------*/

void entry_details(struct employee emp)
{
    repeat:

        display_time();
        printf("\n\t\tEntry Details");
        if(emp.position==1)
            printf("\n\n Position         : Manager");
        else if(emp.position==2)
            printf("\n\n Position         : Senior Employee");
        else
            printf("\n\n Position         : Junior Employee");
        printf("\n Employee ID      : %d", emp.id);
        printf("\n Employee Name    : %s %s", emp.name, emp.lname);
        printf("\n Salary           : %ld", emp.salary);
        printf("\n Date of Join     : %c%c / %c%c / %c%c%c%c", emp.date_of_join[0], emp.date_of_join[1], emp.date_of_join[2], emp.date_of_join[3], emp.date_of_join[4], emp.date_of_join[5], emp.date_of_join[6], emp.date_of_join[7]);
        printf("\n\n Contact Information");
        printf("\n\n Contact Number   : %s", emp.contact);
        printf("\n Address          : %s", emp.addr.line1);
        printf("\n                  : %s", emp.addr.line2);
        printf("\n                  : %s", emp.addr.line3);
        printf("\n                  : %s", emp.addr.line4);

        strcpy(log.name, emp.name);
        strcpy(log.lname, emp.lname);
        log.id = emp.id;
        log.position = emp.position;
}

/*----------------------------------------------------------------------------------------*/

void to_data_file(int index, struct employee emp)
{
    int number_of_digits=0;
    int dummy_index;

    dummy_index=index;

    while(dummy_index!=0)
    {
        dummy_index = dummy_index/10;
        number_of_digits++;
    }

    emp.fname = malloc(sizeof(char *)*(number_of_digits+11));
    emp.buffer1 = calloc(70, sizeof(char *));

    sprintf(emp.fname, "Data/en%d.txt", emp.id);
    sprintf(emp.buffer1, "%d %s %s %ld %s %s %d", emp.id, emp.name, emp.lname, emp.salary, emp.date_of_join, emp.contact, emp.position);
    new_file = fopen(emp.fname, "w");
    fprintf(new_file, "%c", emp.status);
    fprintf(new_file, "\n%s", emp.buffer1);
    fprintf(new_file, "\n%s", emp.addr.line1);
    fprintf(new_file, "\n%s", emp.addr.line2);
    fprintf(new_file, "\n%s", emp.addr.line3);
    fprintf(new_file, "\n%s", emp.addr.line4);
    fclose(new_file);
    free(emp.buffer1);
    free(emp.fname);
}

/*----------------------------------------------------------------------------------------*/

char *password_input(char title[], char placeholder[])
{
    int i, z=0;
    char *password;

    password = malloc(sizeof(char *)*21);

    backspace:

        system("cls");
        display_time();
        printf("%s", title);
        printf(" %s: ", placeholder);

        for(i=0;i<z;i++)
            printf("*");

        while(z<21)
        {
            password[z] = getch();
            if(password[z]==13)
            {
                if(z!=0)
                    break;
                else
                {
                    printf("\n Invalid Password!");
                    getch();
                    goto backspace;
                }
            }
            else if(password[z]==8)
            {
                if(z!=0)
                    z--;
                goto backspace;
            }
            else
                printf("*");
            z++;
        }
        password[z]='\0';
    return (char *)password;
}

/*----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------*/

void display_time()
{
    char time[5];

    system("cls");
    _strtime(time);
    time[5]='\0';
    from_password_file();
    printf(" %-87s Last Action   : %s\n %s", time, last_action, year);
}

/*----------------------------------------------------------------------------------------*/

void list_all_status2(struct employee emp)
{
    printf("\n%11d %15s %15s\t\t%10ld\t%14s\t\t  %c%c/%c%c/%c%c%c%c", emp.id, emp.lname, emp.name, emp.salary, emp.contact, emp.date_of_join[0], emp.date_of_join[1], emp.date_of_join[2], emp.date_of_join[3], emp.date_of_join[4], emp.date_of_join[5], emp.date_of_join[6], emp.date_of_join[7]);
    if(emp.position==1)
        printf("\t\t%15s", "Manager");
    else if(emp.position==2)
        printf("\t\t%15s", "Senior Employee");
    else if(emp.position==3)
        printf("\t\t%15s", "Junior Employee");
    else
        printf("\t\t%15s", "Error");
    if(emp.status=='A')
        printf("\t\t%7s", "Active");
    else if(emp.status=='D')
        printf("\t\t%7s", "Deleted");
    else
        printf("\t\t%7s", "Error");
}

/*----------------------------------------------------------------------------------------*/

void list_particular_status2(struct employee emp)
{
    printf("\n%11d %15s %15s\t\t%10ld\t%14s\t\t  %c%c/%c%c/%c%c%c%c", emp.id, emp.lname, emp.name, emp.salary, emp.contact, emp.date_of_join[0], emp.date_of_join[1], emp.date_of_join[2], emp.date_of_join[3], emp.date_of_join[4], emp.date_of_join[5], emp.date_of_join[6], emp.date_of_join[7]);
    if(emp.position==1)
        printf("\t\t%15s", "Manager");
    else if(emp.position==2)
        printf("\t\t%15s", "Senior Employee");
    else if(emp.position==3)
        printf("\t\t%15s", "Junior Employee");
    else
        printf("\t\t%15s", "Error");
}

/*----------------------------------------------------------------------------------------*/

int get_order(char category[])
{
    printf("\n\t\tLIST - %s (Employee ID Order)", category);
    printf("\n\n 1. Ascending Order");
    printf("\n 2. Descending Order");
    printf("\n\n Your Choice: ");

    switch(getch())
    {
        case '1':
                    return 0;
        case '2':
                    return 1;
        default:
                    printf("\n\n Invalid Choice! Showing in Default Ascending Order");
                    getch();
                    return 0;
    }
}

/*----------------------------------------------------------------------------------------*/

void heading_all_status()
{
    printf("\n\n\n%11s %15s %15s\t\t%10s\t%13s\t\t%s\t\t%15s\t\t%7s\n%11s", "Employee", "Last Name", "First Name", "Salary", "Contact Number", "Date of Join", "Position", "Status", "ID");
    printf("\n   ----------------------------------------------------------------------------------------------------------------------------------------------------------------");
}

/*----------------------------------------------------------------------------------------*/

void heading_particular_status()
{
    printf("\n\n\n%11s %15s %15s\t\t%10s\t%13s\t\t%s\t\t%15s\n%11s", "Employee", "Last Name", "First Name", "Salary", "Contact Number", "Date of Join", "Position", "ID");
    printf("\n   ----------------------------------------------------------------------------------------------------------------------------------------------------------------");
}

/*----------------------------------------------------------------------------------------*/

void log_heading()
{
    printf("\n\n\n %11s   %-22s %-20s %-20s %-20s %-15s  %15s %10s", "SR NO", "Operation", "Employee ID", "Last Name", "First Name", "Position", " Date", "Time");
    printf("\n   -----------------------------------------------------------------------------------------------------------------------------------------------------------------");
}

/*----------------------------------------------------------------------------------------*/

void log_list(long index)
{
    printf("\n %11ld   ", index);
    if(strcmp(d[index].operation, "ui")==0)
        printf("%-22s ", "User Login");
    else if(strcmp(d[index].operation, "ai")==0)
        printf("%-22s ", "Admin Login");
    else if(strcmp(d[index].operation, "uo")==0)
        printf("%-22s ", "User Logout");
    else if(strcmp(d[index].operation, "ao")==0)
        printf("%-22s ", "Admin Logout");
    else if(strcmp(d[index].operation, "ne")==0)
        printf("%-22s ", "New Entry");
    else if(strcmp(d[index].operation, "ve")==0)
        printf("%-22s ", "Entry Visited");
    else if(strcmp(d[index].operation, "ee")==0)
        printf("%-22s ", "Entry Edited");
    else if(strcmp(d[index].operation, "de")==0)
        printf("%-22s ", "Entry Deleted");
    else if(strcmp(d[index].operation, "re")==0)
        printf("%-22s ", "Entry Restored");
    else if(strcmp(d[index].operation, "ap")==0)
        printf("%-22s ", "Admin Password Changed");
    else if(strcmp(d[index].operation, "up")==0)
        printf("%-22s ", "User Password Changed");
    else if(strcmp(d[index].operation, "yr")==0)
        printf("%-22s ", "Year Changed");
    else if(strcmp(d[index].operation, "bk")==0)
        printf("%-22s ", "Data Backup");
    else if(strcmp(d[index].operation, "rb")==0)
        printf("%-22s ", "Data Restored");
    else if(strcmp(d[index].operation, "fi")==0)
        printf("%-22s ", "Failed Login Attempt");
    else if(strcmp(d[index].operation, "da")==0)
        printf("%-22s ", "Deletion Aborted");
    else if(strcmp(d[index].operation, "ra")==0)
        printf("%-22s ", "Restoration Aborted");
    else
        printf("%-22s ", "Error Retrieving");

    if(d[index].id!=0)
        printf("%-20d %-20s %-20s %-15d\t   %c%c/%c%c/%c%c%c%c %10s", d[index].id, d[index].lname, d[index].name\
               , d[index].position, d[index].date_of_join[0], d[index].date_of_join[1], d[index].date_of_join[2]\
               , d[index].date_of_join[3], d[index].date_of_join[4], d[index].date_of_join[5]\
               , d[index].date_of_join[6], d[index].date_of_join[7], d[index].time);
    else
        printf("%-20s %-20s %-20s %-15s\t   %c%c/%c%c/%c%c%c%c %10s", d[index].id, d[index].lname, d[index].name\
               , d[index].position, d[index].date_of_join[0], d[index].date_of_join[1], d[index].date_of_join[2]\
               , d[index].date_of_join[3], d[index].date_of_join[4], d[index].date_of_join[5]\
               , d[index].date_of_join[6], d[index].date_of_join[7], d[index].time);
}

/*----------------------------------------------------------------------------------------*/

void list_particular_status1(int index)
{
    printf("\n%11d %15s %15s\t\t%10ld\t%14s\t\t  %c%c/%c%c/%c%c%c%c", d[index].id, d[index].lname, d[index].name, d[index].salary, d[index].contact\
           , d[index].date_of_join[0], d[index].date_of_join[1], d[index].date_of_join[2], d[index].date_of_join[3], d[index].date_of_join[4]\
           , d[index].date_of_join[5], d[index].date_of_join[6], d[index].date_of_join[7]);
    if(d[index].position==1)
        printf("\t\t%15s", "Manager");
    else if(d[index].position==2)
        printf("\t\t%15s", "Senior Employee");
    else if(d[index].position==3)
        printf("\t\t%15s", "Junior Employee");
    else
        printf("\t\t%15s", "Error");
}

/*----------------------------------------------------------------------------------------*/

void list_all_status1(int index)
{
    printf("\n%11d %15s %15s\t\t%10ld\t%14s\t\t  %c%c/%c%c/%c%c%c%c", d[index].id, d[index].lname, d[index].name, d[index].salary, d[index].contact, d[index].date_of_join[0], d[index].date_of_join[1], d[index].date_of_join[2], d[index].date_of_join[3], d[index].date_of_join[4], d[index].date_of_join[5], d[index].date_of_join[6], d[index].date_of_join[7]);
    if(d[index].position==1)
        printf("\t\t%15s", "Manager");
    else if(d[index].position==2)
        printf("\t\t%15s", "Senior Employee");
    else if(d[index].position==3)
        printf("\t\t%15s", "Junior Employee");
    else
        printf("\t\t%15s", "Error");
    if(d[index].status=='A')
        printf("\t\t%7s", "Active");
    else if(d[index].status=='D')
        printf("\t\t%7s", "Deleted");
    else
        printf("\t\t%7s", "Error");
}

/*----------------------------------------------------------------------------------------*/

void record_operation(char operation[])
{
    char *buffer[60];

    log.time = calloc(5, sizeof(char *));
    log.date = calloc(8, sizeof(char *));
    _strtime(log.time);
    log.time[5]='\0';
    log.date = get_system_date();
    sprintf(buffer, " %d %s %s %s %d %s %s", log.id, log.name, log.lname, log.time, log.position, log.date, operation);
    write_log_file = fopen("Data/log.txt", "a");
    fprintf(write_log_file, "%s\n", buffer);
    fclose(write_log_file);
    free(log.time);
    free(log.date);
}

/*----------------------------------------------------------------------------------------*/

void perform_backup()
{
    FILE *data_file_backup, *list_file_backup, *password_file_backup, *data_file, *password_file, *list_file;

    int count_entries;
    int index, dummy_index;
    int number_of_digits;
    char *filename, *filename_OG;
    char chr;
    int check;
    int check2;

    check = mkdir("Data");

    if(check==0)
    {
        rmdir("Data");
        printf("\n No Data Found to Backup!");
    }

    else
    {
        check2 = mkdir("Backup");

        count_entries = total_entries();

        for(index=1; index<=count_entries; index++)
        {
            dummy_index = index;
            number_of_digits = 0;
            while(dummy_index!=0)
            {
                dummy_index /=10;
                number_of_digits++;
            }

            filename_OG = malloc(sizeof(char *)*(number_of_digits+11));
            sprintf(filename_OG, "Data/en%d.txt", index);

            filename = malloc(sizeof(char *)*(number_of_digits+13));
            sprintf(filename, "Backup/enbk%d.txt", index);

            data_file = fopen(filename_OG, "rb");
            data_file_backup = fopen(filename, "w");
            while((chr=fgetc(data_file))!=EOF)
            {
                fprintf(data_file_backup, "%c", chr);
            }
            fclose(data_file_backup);
            fclose(data_file);
        }

        if(list_file = fopen("Data/list.txt", "rb"))
        {
            list_file_backup = fopen("Backup/listbk.txt", "w");
            while((chr=fgetc(list_file))!=EOF)
            {
                fprintf(list_file_backup, "%c", chr);
            }
            fclose(list_file_backup);
            fclose(list_file);
        }
        password_file = fopen("Data/password.txt", "rb");
        password_file_backup = fopen("Backup/passwordbk.txt", "w");
        while((chr=fgetc(password_file))!=EOF)
        {
            fprintf(password_file_backup, "%c", chr);
        }
        fclose(password_file_backup);
        fclose(password_file);

        backup_time = calloc(5, sizeof(char *));

        _strtime(backup_time);
        backup_time[5]='\0';
        backup_date = get_system_date();
        to_password_file();
        log.position = 0;
        log.id = 0;
        strcpy(log.lname, "(null)");
        strcpy(log.name,"(null)");
        record_operation("bk");
        printf("\n\n Data Backup Successfull!!");
        free(data_file_backup);
    }
    getch();
}

/*----------------------------------------------------------------------------------------*/

void restore_last_backup()
{
    FILE *data_file_backup, *list_file_backup, *password_file_backup, *data_file, *password_file, *list_file;

    int count_backup_entries;
    int count_entries;
    int index, dummy_index;
    int number_of_digits;
    char *filename, *filename_OG;
    char chr;
    int check;
    int check2;

    check = mkdir("Backup");

    if(check==0)
    {
        rmdir("Backup");
        printf("\n No Backup Found");
    }

    else
    {
        check2 = mkdir("Data");

        count_backup_entries = total_entries_backedup();
        count_entries = total_entries();

        for(index=1; index<=count_entries; index++)
        {
            dummy_index = index;
            number_of_digits = 0;
            while(dummy_index!=0)
            {
                dummy_index /=10;
                number_of_digits++;
            }

            filename_OG = malloc(sizeof(char *)*(number_of_digits+11));
            sprintf(filename_OG, "Data/en%d.txt", index);

            remove(filename_OG);
         }

        remove("Data/list.txt");
        remove("Data/password.txt");
        rmdir("Data");
        mkdir("Data");

        for(index=1; index<=count_backup_entries; index++)
        {
            dummy_index = index;
            number_of_digits = 0;
            while(dummy_index!=0)
            {
                dummy_index /=10;
                number_of_digits++;
            }

            filename_OG = malloc(sizeof(char *)*(number_of_digits+11));
            sprintf(filename_OG, "Data/en%d.txt", index);

            filename = malloc(sizeof(char *)*(number_of_digits+13));
            sprintf(filename, "Backup/enbk%d.txt", index);


            data_file_backup = fopen(filename, "rb");
            data_file = fopen(filename_OG, "w");
            while((chr=fgetc(data_file_backup))!=EOF)
            {
                fprintf(data_file, "%c", chr);
            }
            fclose(data_file);
            fclose(data_file_backup);
        }


        if((list_file_backup = fopen("Backup/listbk.txt", "rb")))
        {
            list_file = fopen("Data/list.txt", "w");
            while((chr=fgetc(list_file_backup))!=EOF)
            {
                fprintf(list_file, "%c", chr);
            }
            fclose(list_file);
            fclose(list_file_backup);
        }

        password_file_backup = fopen("Backup/passwordbk.txt", "rb");
        password_file = fopen("Data/password.txt", "w");

        while((chr=fgetc(password_file_backup))!=EOF)
        {

            fprintf(password_file, "%c", chr);

        }
        fclose(password_file);
        fclose(password_file_backup);

        from_password_file();

        log.position = 0;
        log.id = 0;
        strcpy(log.lname, "(null)");
        strcpy(log.name,"(null)");

        record_operation("rb");

        printf("\n\n Data Restore Successfull!!");
        free(data_file_backup);
    }
    getch();
}

/*----------------------------------------------------------------------------------------*/

int log_retrieving()
{
    long count_log_entries;
    char chr;
    long index, index_for_dummy = 0;
    char *buffer;

    count_log_entries = total_log_entries();

    log.date = calloc(8, sizeof(char *));
    log.time = calloc(5, sizeof(char *));
    buffer = calloc(60, sizeof(char *));
    d = calloc(count_log_entries+1, "sizeof(*d)");

    read_log_file = fopen("Data/log.txt", "rb");

    while((chr=fgetc(read_log_file))!=EOF)
    {
        fscanf(read_log_file, "%[^\n]", buffer);
        sscanf(buffer, "%d %s %s %s %d %s %s", &log.id, &log.name, &log.lname, log.time, &log.position, log.date, &log.operation);
        index_for_dummy++;
        to_log_dummy(index_for_dummy, log);
    }

    for(index=1; index<index_for_dummy; index++)
        log_list(index);

    free(log.date);
    free(log.time);
    free(buffer);
    free(d);

    return count_log_entries;
}

/*----------------------------------------------------------------------------------------*/

void to_log_dummy(long index, struct log log)
{
    d[index].date_of_join = calloc(8, sizeof(char *));
    d[index].time = calloc(5, sizeof(char *));

    strcpy(d[index].name, log.name);
    strcpy(d[index].lname, log.lname);
    strcpy(d[index].date_of_join, log.date);
    strcpy(d[index].time, log.time);
    d[index].position = log.position;
    d[index].id = log.id;
    strcpy(d[index].operation, log.operation);
}

/*----------------------------------------------------------------------------------------*/

void show_log()
{
    int check;
    int count;

    log:

        check = 0;
        display_time();
        printf("\n\t\tLOGS");
        log_heading();
        count = log_retrieving();

        while(!kbhit())
        {
            check = check_log_update(count);
            strcpy(action, last_action);
            from_password_file();
            if(check==1)
                goto log;
        }
    getch();
}

/*----------------------------------------------------------------------------------------*/

void first_startup()
{
    int temp, temp2, temp3;

    log.position = 0;
    log.id = 0;
    strcpy(log.lname, "Startup");
    strcpy(log.name,"Procedure");

    year:
        temp3 = set_year("\n\t\tSet Current Year");
        if(temp3!=1)
            goto year;
    admin:
        temp = set_admin_password("\n\t\tSet ADMIN Password");
        if(temp!=1)
            goto admin;

    user:
        temp2 = set_user_password("\n\t\tSet USER Password");
        if(temp2!=1)
            goto user;

    if(temp==1 && temp2==1 && temp3==1)
    {
        to_password_file();
    }
}

/*----------------------------------------------------------------------------------------*/

int set_user_password(char title[])
{
    char *new_user_password, *confirm_user_password;

    new_user_password = password_input(title, "\n\n New User Password (MIN 8 MAX 20)");
    if(strcmp(user_password, new_user_password)==0)
    {
        printf("\n New Password Cannot Be Same as Old!");
        getch();
        return 0;
    }
    else if(strcmp(new_user_password, admin_password)==0)
    {
        printf("\n User and Admin Password cannot be same!");
        getch();
        return 0;
    }
    else if(strlen(new_user_password)<8)
    {
        printf("\n Password Too Short");
        getch();
        return 0;
    }
    else if(strlen(new_user_password)>20)
    {
        printf("\n Password Too Long");
        getch();
        return 0;
    }
    else
    {
        confirm_user_password = password_input(title, "\n\n Confirm User Password");
        if(strcmp(new_user_password, confirm_user_password)==0)
        {
            strcpy(user_password, new_user_password);
            to_password_file();
            record_operation("up");
            printf("\n User Password Changed!");
            getch();
            return 1;
        }
        else
        {
            printf("\n Password Don't Match");
            getch();
            return 0;
        }
    }
}

/*----------------------------------------------------------------------------------------*/

int set_admin_password(char title[])
{
    char *new_admin_password, *confirm_admin_password;

    new_admin_password = password_input(title, "\n\n New Admin Password (MIN 8 MAX 20)");
    if(strcmp(admin_password, new_admin_password)==0)
    {
        printf("\n New Password Cannot Be Same as Old!");
        getch();
        return 0;
    }
    else if(strcmp(user_password, new_admin_password)==0)
    {
        printf("\n User and Admin Password cannot be same!");
        getch();
        return 0;
    }
    else if(strlen(new_admin_password)<8)
    {
        printf("\n Password Too Short");
        getch();
        return 0;
    }
    else if(strlen(new_admin_password)>20)
    {
        printf("\n Password Too Long");
        getch();
        return 0;
    }
    else
    {
        confirm_admin_password = password_input(title, "\n\n Confirm Admin Password");
        if(strcmp(new_admin_password, confirm_admin_password)==0)
        {
            strcpy(admin_password, new_admin_password);
            to_password_file();
            record_operation("ap");
            printf("\n Admin Password Changed!");
            getch();
            return 1;
        }
        else
        {
            printf("\n Password Don't Match");
            getch();
            return 0;
        }
    }
}

/*----------------------------------------------------------------------------------------*/

int set_year(char title[])
{
    char *year_dummy;

    display_time();
    printf("%s", title);
    printf("\n\n Current Year: ");
    scanf("%s", year_dummy);

    if(strlen(year_dummy)!=4 || strcmp(year_dummy, "0000")==0)
    {
        printf("\n Invalid Year");
        getch();
        return 0;
    }
    else
    {
        strcpy(year, year_dummy);
        to_password_file();
        record_operation("yr");
        printf("\n Year Changed!");
        getch();
        return 1;
    }
}

/*----------------------------------------------------------------------------------------*/

void to_password_file()
{
    buffer = calloc(65, sizeof(char *));
    sprintf(buffer, "%s %s %s %s %s %d %d %d %d", admin_password, user_password, backup_date, backup_time, year, startup_status, concurrency_avoid, admin_login_status, user_login_status);
    write_password_file = fopen("Data/password.txt", "w");
    fprintf(write_password_file, "%s\n", buffer);
    fprintf(write_password_file, "%s", last_action);
    fclose(write_password_file);
    free(buffer);
}

/*----------------------------------------------------------------------------------------*/

void from_password_file()
{
    int i = 0;
    char chr;

    user_password = malloc(sizeof(char *)*20);
    admin_password = malloc(sizeof(char *)*20);
    year = malloc(sizeof(char *)*4);
    backup_date = malloc(sizeof(char *)*8);
    backup_time = calloc(5, sizeof(char *));
    buffer = malloc(sizeof(char *)*65);

    //To Avoid Garbage Value on First Execution
    strcpy(backup_date, "xxxxxxxx");
    strcpy(backup_time, "xx:xx");
    strcpy(year, "xxxx");
    read_password_file = fopen("Data/password.txt", "r");
    fscanf(read_password_file, "%[^\n] %[^\n]", buffer, last_action);

    fclose(read_password_file);
    //printf("\nLast: %s", last_action);
    sscanf(buffer, "%s %s %s %s %s %d %d %d %d", admin_password, user_password, backup_date, backup_time, year, &startup_status, &concurrency_avoid, &admin_login_status, &user_login_status);
    free(buffer);
}

/*----------------------------------------------------------------------------------------*/

int format_data()
{
    char choice;
    int temp;
    char *password;

    display_time();
    printf("\n\t\tFormat Data");
    printf("\n This Feature will erase all existing Data, including passwords and backup. Startup procedure will be executed");
    printf("\n Confirm Data Format (Y/N)? ");
    choice = getche();

    if(choice=='y' || choice=='Y')
    {
        password = password_input("\n\t\tAuthenticate Data Format", "\n\n Admin Password");
        from_password_file();
        if(strcmp(password, admin_password)==0)
        {
            temp = data_format_procedure();
            return 1;
        }
        else
        {
            printf("\n Incorrect Password");
            printf("\n Data Format Aborted!");
        }
    }
    else if(choice=='N' || choice=='n')
    {
        printf("\n Data Format Aborted!");
    }
    else
    {
        printf("\n Invalid Choice!!");
        printf("\n Data Format Aborted!");
    }
    getch();
    return 0;
}

/*----------------------------------------------------------------------------------------*/

int data_format_procedure()
{
    FILE *data_file_backup;
    char *filename;
    int count_entries, count_backedup_entries;
    int index, dummy_index, number_of_digits = 0;
    int check_erase, check_erase2;

    count_entries = total_entries();
    count_backedup_entries = total_entries_backedup();

    system("rmdir /s /q Data");
    system("rmdir /s /q Backup");

    //Try Deleting folders
    check_erase = rmdir("Data");
    check_erase2 = rmdir("Backup");

    //Verify folder delete (Failure is Success)
    if(check_erase!=0 && check_erase2!=0)
        printf("\n Data Format Successful!!");
    else
    {
        printf("\n Data Format Successful!!");
        printf("\n Directory Delete Failed!! (Possible Issue: Presence of Unexpected Files and Sub-Directories)");
    }

    getch();
    return 0;
}

/*----------------------------------------------------------------------------------------*/

void check_data()
{
    int return_check;
    int return_check2;

    return_check = mkdir("Data");
    return_check2 = mkdir("Backup");
    if(return_check==0 && return_check2==-1)
    {
        display_time();
        printf("\n\n Data Missing!!");
        printf("\n Restoring Data from Backup....");
        restore_last_backup();
    }
}

/*----------------------------------------------------------------------------------------*/

int check_update_status(int active_shown, int deleted_shown, char status)
{
    struct employee emp;
    int index;
    int count_entries;
    int active_entries = 0;
    int deleted_entries = 0;

    count_entries = total_entries();
    for(index = 1; index<=count_entries; index++)
    {
        emp = from_data_file(index);
        if(status=='A')
        {
            if(emp.status==status)
                active_entries++;
        }
        else if(status=='D')
        {
            if(emp.status=='D')
                deleted_entries++;
        }
        else
        {
            if(emp.status=='A')
                active_entries++;
            else
                deleted_entries++;
        }

    }

    if(status=='A')
    {
        if(active_entries!=active_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='D')
    {
        if(deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='C')
    {
        if(active_entries!=active_shown || deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }

}

/*----------------------------------------------------------------------------------------*/

int check_update_position(int active_shown, int deleted_shown, int position, char status)
{
    struct employee emp;
    int index;
    int count_entries;
    int active_entries = 0;
    int deleted_entries = 0;


    count_entries = total_entries();
    for(index = 1; index<=count_entries; index++)
    {
        emp = from_data_file(index);
        if(emp.position==position)
        {
            if(status=='A')
            {
                if(emp.status==status)
                    active_entries++;
            }
            else if(status=='D')
            {
                if(emp.status=='D')
                    deleted_entries++;
            }
            else
            {
                if(emp.status=='A')
                    active_entries++;
                else
                    deleted_entries++;
            }
        }

    }

    if(status=='A')
    {
        if(active_entries!=active_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='D')
    {
        if(deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='C')
    {
        if(active_entries!=active_shown || deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
}

/*----------------------------------------------------------------------------------------*/

int check_update_salary_range(int active_shown, int deleted_shown, long lower, long upper, char status)
{
    struct employee emp;
    int index;
    int count_entries;
    int active_entries = 0;
    int deleted_entries = 0;

    count_entries = total_entries();

    for(index=1; index<=count_entries; index++)
    {
        emp = from_data_file(index);

        if(emp.salary>=lower && emp.salary<=upper)
        {
            if(status=='A')
            {
                if(emp.status==status)
                    active_entries++;
            }
            else if(status=='D')
            {
                if(emp.status==status)
                    deleted_entries++;
            }
            else
            {
                if(emp.status=='A')
                    active_entries++;
                else
                    deleted_entries++;
            }
        }
    }

    if(status=='A')
    {
        if(active_entries!=active_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='D')
    {
        if(deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='C')
    {
        if(active_entries!=active_shown || deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
}

/*----------------------------------------------------------------------------------------*/

int check_update_doj_range(int active_shown, int deleted_shown, char *lower, char *upper, char status)
{
    struct employee emp;
    int index;
    int count_entries;
    int active_entries = 0;
    int deleted_entries = 0;
    int lower_limit_check = 0;
    int upper_limit_check = 0;

    count_entries = total_entries();

    for(index=1; index<=count_entries; index++)
    {
        emp = from_data_file(index);
        upper_limit_check = date_limit_check(emp.date_of_join, upper);
        lower_limit_check = date_limit_check(emp.date_of_join, lower);

        if((upper_limit_check==-1 && lower_limit_check==1) || (upper_limit_check==0 || lower_limit_check==0))
        {

            if(status=='A')
            {
                if(emp.status==status)
                    active_entries++;
            }
            else if(status=='D')
            {
                if(emp.status==status)
                    deleted_entries++;
            }
            else
            {
                if(emp.status=='A')
                    active_entries++;
                else
                    deleted_entries++;
            }
        }
    }

    if(status=='A')
    {
        if(active_entries!=active_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='D')
    {
        if(deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='C')
    {
        if(active_entries!=active_shown || deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
}

/*----------------------------------------------------------------------------------------*/

int check_update_keyword(int active_shown, int deleted_shown, int z, char keyword[], char status)
{
    struct employee emp;
    int index;
    int count_entries;
    int active_entries = 0;
    int deleted_entries = 0;
    char name_lname[30];
    char lname_name[30];

    count_entries = total_entries();

    for(index=1; index<=count_entries; index++)
    {
        emp = from_data_file(index);
        if(emp.status==status)
        {
            sprintf(name_lname, "%s %s", emp.name, emp.lname);
            sprintf(lname_name, "%s %s", emp.lname, emp.name);

            if(strncasecmp(name_lname, keyword, z)==0 || strncasecmp(lname_name, keyword, z)==0)
            {
                if(emp.status=='A')
                    active_entries++;
                else
                    deleted_entries++;
            }
        }
    }

    if(status=='A')
    {
        if(active_entries!=active_shown)
            return 1;
        else
            return 0;
    }
    else if(status=='D')
    {
        if(deleted_entries!=deleted_shown)
            return 1;
        else
            return 0;
    }
}

/*----------------------------------------------------------------------------------------*/

int check_log_update(int shown)
{
    int new_count;

    new_count = total_log_entries();

    if(new_count!=shown)
        return 1;
    else
        return 0;
}
/*----------------------------------------------------------------------------------------*/
// END


