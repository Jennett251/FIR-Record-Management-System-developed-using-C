
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "fir.dat"
#define MAX 100

// Structure for date
struct Date
{
    int day;
    int month;
    int year;
};

// Structure for FIR
struct FIR
{
    int caseNo;
    char complainant[50];
    struct Date date;
    char location[50];
    char description[200];
    char status[20];
};

// Case-insensitive string comparison
int compareIgnoreCase(char a[], char b[])
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (tolower((unsigned char)a[i]) !=
            tolower((unsigned char)b[i]))
            return 0;

        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

// Register a new FIR
void registerFIR()
{
    struct FIR f;
    FILE *fp = fopen(FILE_NAME, "ab");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    printf("\nEnter Case Number: ");
    scanf("%d", &f.caseNo);

    // Check for duplicate case number
    struct FIR temp;
    FILE *check = fopen(FILE_NAME, "rb");

    if (check != NULL)
    {
        while (fread(&temp, sizeof(temp), 1, check) == 1)
        {
            if (temp.caseNo == f.caseNo)
            {
                printf("Case number already exists!\n");
                fclose(check);
                fclose(fp);
                return;
            }
        }
        fclose(check);
    }

    printf("Enter Complainant Name: ");
    scanf(" %49[^\n]", f.complainant);

    printf("Enter Date (DD MM YYYY): ");
    scanf("%d %d %d", &f.date.day,
          &f.date.month, &f.date.year);

    printf("Enter Location: ");
    scanf(" %49[^\n]", f.location);

    printf("Enter Description: ");
    scanf(" %199[^\n]", f.description);

    printf("Enter Status (Pending/Closed/Investigating): ");
    scanf(" %19s", f.status);

    fwrite(&f, sizeof(f), 1, fp);
    fclose(fp);

    printf("\nFIR Registered Successfully!\n");
}

// Display all FIR records
void displayFIR()
{
    struct FIR f;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("\nNo FIR records found!\n");
        return;
    }

    printf("\n========== FIR RECORDS ==========\n");

    while (fread(&f, sizeof(f), 1, fp) == 1)
    {
        printf("\nCase Number: %d", f.caseNo);
        printf("\nComplainant: %s", f.complainant);
        printf("\nDate: %02d-%02d-%04d",
               f.date.day, f.date.month, f.date.year);
        printf("\nLocation: %s", f.location);
        printf("\nDescription: %s", f.description);
        printf("\nStatus: %s\n", f.status);
        printf("--------------------------------\n");
    }

    fclose(fp);
}

// Search FIR by case number
void searchByCaseNo()
{
    struct FIR f;
    int number, found = 0;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Case Number to Search: ");
    scanf("%d", &number);

    while (fread(&f, sizeof(f), 1, fp) == 1)
    {
        if (f.caseNo == number)
        {
            printf("\nCase Found!\n");
            printf("Complainant: %s\n", f.complainant);
            printf("Status: %s\n", f.status);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Case Not Found!\n");

    fclose(fp);
}

// Search FIR by status (case-insensitive)
void searchByStatus()
{
    struct FIR f;
    char search[20];
    int found = 0;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Status to Search: ");
    scanf(" %19s", search);

    while (fread(&f, sizeof(f), 1, fp) == 1)
    {
        if (compareIgnoreCase(search, f.status))
        {
            printf("\nCase Number: %d", f.caseNo);
            printf("\nComplainant: %s", f.complainant);
            printf("\nStatus: %s\n", f.status);
            found = 1;
        }
    }

    if (!found)
        printf("No matching cases found!\n");

    fclose(fp);
}

// Update FIR status
void updateStatus()
{
    struct FIR f;
    int number, found = 0;

    FILE *fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Case Number: ");
    scanf("%d", &number);

    while (fread(&f, sizeof(f), 1, fp) == 1)
    {
        if (f.caseNo == number)
        {
            printf("Current Status: %s\n", f.status);

            printf("Enter New Status: ");
            scanf(" %19s", f.status);

            fseek(fp, -(long)sizeof(f), SEEK_CUR);
            fwrite(&f, sizeof(f), 1, fp);

            printf("Status Updated Successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Case Not Found!\n");

    fclose(fp);
}

// Display FIRs registered on a particular date
void searchByDate()
{
    struct FIR f;
    int d, m, y, found = 0;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Date (DD MM YYYY): ");
    scanf("%d %d %d", &d, &m, &y);

    while (fread(&f, sizeof(f), 1, fp) == 1)
    {
        if (f.date.day == d &&
            f.date.month == m &&
            f.date.year == y)
        {
            printf("\nCase Number: %d", f.caseNo);
            printf("\nComplainant: %s", f.complainant);
            printf("\nStatus: %s\n", f.status);
            found = 1;
        }
    }

    if (!found)
        printf("No FIR found on this date!\n");

    fclose(fp);
}

// Sort FIR records by case number
void sortByCaseNo()
{
    struct FIR f[MAX], temp;
    int count = 0;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    while (count < MAX &&
           fread(&f[count], sizeof(struct FIR), 1, fp) == 1)
        count++;

    fclose(fp);

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (f[j].caseNo > f[j + 1].caseNo)
            {
                temp = f[j];
                f[j] = f[j + 1];
                f[j + 1] = temp;
            }
        }
    }

    printf("\nFIR Records Sorted by Case Number:\n");

    for (int i = 0; i < count; i++)
    {
        printf("Case No: %d | Name: %s | Status: %s\n",
               f[i].caseNo, f[i].complainant, f[i].status);
    }
}

// Main menu
int main()
{
    int choice;

    do
    {
        printf("\n\n====== FIR RECORD MANAGEMENT SYSTEM ======\n");
        printf("1. Register FIR\n");
        printf("2. Display All FIRs\n");
        printf("3. Search by Case Number\n");
        printf("4. Search by Status\n");
        printf("5. Update FIR Status\n");
        printf("6. Search FIR by Date\n");
        printf("7. Sort by Case Number\n");
        printf("8. Exit\n");

        printf("\nEnter your Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                registerFIR();
                break;

            case 2:
                displayFIR();
                break;

            case 3:
                searchByCaseNo();
                break;

            case 4:
                searchByStatus();
                break;

            case 5:
                updateStatus();
                break;

            case 6:
                searchByDate();
                break;

            case 7:
                sortByCaseNo();
                break;

            case 8:
                printf("Thank you for using FIR Management System!\n");
                break;

            default:
                printf("Invalid Choice! Try Again.\n");
        }

    } while (choice != 8);

    return 0;
}
