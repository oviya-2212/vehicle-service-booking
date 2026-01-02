#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ServiceBooking {
    int bookingId;
    char customerName[50];
    char vehicleNumber[20];
    char serviceType[50];
    char date[15];
    char time[10];
    int status; // 0 = active, 1 = cancelled
};

/* Function Declarations */
void bookService();
void displayBookings();
void searchBooking();
void updateBooking();
void cancelBooking();

/* Main Function */
int main() {
    int choice;

    do {
        printf("\n-----------------------------------------\n");
        printf("VEHICLE SERVICE BOOKING SYSTEM\n");
        printf("-----------------------------------------\n");
        printf("1. Book Service\n");
        printf("2. Display All Bookings\n");
        printf("3. Search Booking\n");
        printf("4. Update Booking\n");
        printf("5. Cancel Booking\n");
        printf("0. Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: bookService(); break;
            case 2: displayBookings(); break;
            case 3: searchBooking(); break;
            case 4: updateBooking(); break;
            case 5: cancelBooking(); break;
            case 0: printf("Exiting program...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}

/* Book Service */
void bookService() {
    FILE *fp = fopen("bookings.txt", "a");
    struct ServiceBooking b;

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Booking ID: ");
    scanf("%d", &b.bookingId);
    printf("Enter Customer Name: ");
    scanf(" %[^\n]", b.customerName);
    printf("Enter Vehicle Number: ");
    scanf(" %[^\n]", b.vehicleNumber);
    printf("Enter Service Type: ");
    scanf(" %[^\n]", b.serviceType);
    printf("Enter Date (DD/MM/YYYY): ");
    scanf(" %[^\n]", b.date);
    printf("Enter Time (HH:MM): ");
    scanf(" %[^\n]", b.time);

    b.status = 0;

    fprintf(fp, "%d|%s|%s|%s|%s|%s|%d\n",
            b.bookingId, b.customerName, b.vehicleNumber,
            b.serviceType, b.date, b.time, b.status);

    fclose(fp);
    printf("Booking created successfully!\n");
}

/* Display Bookings */
void displayBookings() {
    FILE *fp = fopen("bookings.txt", "r");
    struct ServiceBooking b;
    int found = 0;

    if (fp == NULL) {
        printf("No bookings found.\n");
        return;
    }

    printf("\nBookingID  Customer   Vehicle     Service        Date        Time   Status\n");

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d\n",
                  &b.bookingId, b.customerName, b.vehicleNumber,
                  b.serviceType, b.date, b.time, &b.status) != EOF) {
        printf("%-10d %-10s %-10s %-13s %-10s %-6s %s\n",
               b.bookingId, b.customerName, b.vehicleNumber,
               b.serviceType, b.date, b.time,
               b.status == 0 ? "Active" : "Cancelled");
        found = 1;
    }

    if (!found)
        printf("No bookings available.\n");

    fclose(fp);
}

/* Search Booking */
void searchBooking() {
    FILE *fp = fopen("bookings.txt", "r");
    struct ServiceBooking b;
    int id, found = 0;
    char name[50];

    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    printf("Enter Booking ID or Customer Name: ");
    scanf("%s", name);

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d\n",
                  &b.bookingId, b.customerName, b.vehicleNumber,
                  b.serviceType, b.date, b.time, &b.status) != EOF) {

        id = atoi(name);
        if (b.bookingId == id || strcmp(b.customerName, name) == 0) {
            printf("\nBooking Found:\n");
            printf("ID: %d\nCustomer: %s\nVehicle: %s\nService: %s\nDate: %s\nTime: %s\nStatus: %s\n",
                   b.bookingId, b.customerName, b.vehicleNumber,
                   b.serviceType, b.date, b.time,
                   b.status == 0 ? "Active" : "Cancelled");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Booking not found.\n");

    fclose(fp);
}

/* Update Booking */
void updateBooking() {
    FILE *fp = fopen("bookings.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct ServiceBooking b;
    int id, found = 0;

    printf("Enter Booking ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d\n",
                  &b.bookingId, b.customerName, b.vehicleNumber,
                  b.serviceType, b.date, b.time, &b.status) != EOF) {

        if (b.bookingId == id && b.status == 0) {
            printf("Enter New Service Type: ");
            scanf(" %[^\n]", b.serviceType);
            printf("Enter New Date: ");
            scanf(" %[^\n]", b.date);
            printf("Enter New Time: ");
            scanf(" %[^\n]", b.time);
            found = 1;
        }

        fprintf(temp, "%d|%s|%s|%s|%s|%s|%d\n",
                b.bookingId, b.customerName, b.vehicleNumber,
                b.serviceType, b.date, b.time, b.status);
    }

    fclose(fp);
    fclose(temp);

    remove("bookings.txt");
    rename("temp.txt", "bookings.txt");

    if (found)
        printf("Booking updated successfully!\n");
    else
        printf("Booking not found or already cancelled.\n");
}

/* Cancel Booking */
void cancelBooking() {
    FILE *fp = fopen("bookings.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct ServiceBooking b;
    int id, found = 0;

    printf("Enter Booking ID to cancel: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d\n",
                  &b.bookingId, b.customerName, b.vehicleNumber,
                  b.serviceType, b.date, b.time, &b.status) != EOF) {

        if (b.bookingId == id && b.status == 0) {
            b.status = 1;
            found = 1;
        }

        fprintf(temp, "%d|%s|%s|%s|%s|%s|%d\n",
                b.bookingId, b.customerName, b.vehicleNumber,
                b.serviceType, b.date, b.time, b.status);
    }

    fclose(fp);
    fclose(temp);

    remove("bookings.txt");
    rename("temp.txt", "bookings.txt");

    if (found)
        printf("Booking cancelled successfully!\n");
    else
        printf("Booking not found or already cancelled.\n");
}
