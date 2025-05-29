#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EVENTS 100

typedef struct {
    int day, month, year;
    int hour, minute;
} DateTime;

typedef struct {
    char title[51];
    DateTime start;
    DateTime end;
} Event;

Event *events[MAX_EVENTS];
int event_count = 0;

int compareDateTime(DateTime a, DateTime b);
int isConflict(Event *newEvent, int skipIndex);
void printEvent(const Event *e, int index);
void addEvent();
void viewEvents();
void deleteEvent();
void freeEvents();
int validateDate(int day, int month, int year);
int validateTime(int hour, int minute);
void flushInput();

int main() {
    int choice;

    printf("******* SOPHIA'S Event Scheduler *******\n");
    do {
        printf("\nMenu:\n");
        printf("1. Add Event\n");
        printf("2. View Events\n");
        printf("3. Delete Event\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            flushInput();
            continue;
        }
        flushInput();

        switch (choice) {
            case 1:
                addEvent();
                break;
            case 2:
                viewEvents();
                break;
            case 3:
                deleteEvent();
                break;
            case 4:
                printf("Exiting. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please select between 1 and 4.\n");
                break;
        }
    } while (choice != 4);

    freeEvents();
    return 0;
}

int compareDateTime(DateTime a, DateTime b) {
    if (a.year != b.year)
        return a.year - b.year;
    if (a.month != b.month)
        return a.month - b.month;
    if (a.day != b.day)
        return a.day - b.day;
    if (a.hour != b.hour)
        return a.hour - b.hour;
    return a.minute - b.minute;
}

int isConflict(Event *newEvent, int skipIndex) {
    for (int i = 0; i < event_count; i++) {
        if (i == skipIndex)
            continue;
        Event *e = events[i];
        if (compareDateTime(newEvent->start, e->end) < 0 &&
            compareDateTime(newEvent->end, e->start) > 0) {
            return 1;
        }
    }
    return 0;
}

void printEvent(const Event *e, int index) {
    if (index >= 0) {
        printf("%3d | ", index + 1);
    }
    printf("%-50s | %02d/%02d/%4d %02d:%02d | %02d/%02d/%4d %02d:%02d\n",
           e->title,
           e->start.day, e->start.month, e->start.year, e->start.hour, e->start.minute,
           e->end.day, e->end.month, e->end.year, e->end.hour, e->end.minute);
}

void insertEventSorted(Event *newEvent) {
    if (event_count == 0) {
        events[0] = newEvent;
        event_count++;
        return;
    }
    int i = event_count - 1;
    while (i >= 0 && compareDateTime(events[i]->start, newEvent->start) > 0) {
        events[i + 1] = events[i];
        i--;
    }
    events[i + 1] = newEvent;
    event_count++;
}

void addEvent() {
    if (event_count >= MAX_EVENTS) {
        printf("Event list is full. Cannot add more.\n");
        return;
    }

    Event *newEvent = malloc(sizeof(Event));
    if (!newEvent) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter event title (max 50 characters): ");
    fgets(newEvent->title, sizeof(newEvent->title), stdin);
    newEvent->title[strcspn(newEvent->title, "\n")] = 0;

    if (strlen(newEvent->title) == 0) {
        printf("Title cannot be empty.\n");
        free(newEvent);
        return;
    }

    printf("Enter start date:\n");
    printf(" Day (1-31): ");
    if (scanf("%d", &newEvent->start.day) != 1 || !validateDate(newEvent->start.day, 1, 2000)) {
        printf("Invalid day. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }
    printf(" Month (1-12): ");
    if (scanf("%d", &newEvent->start.month) != 1 || !validateDate(1, newEvent->start.month, 2000)) {
        printf("Invalid month. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }
    printf(" Year (> 1900): ");
    if (scanf("%d", &newEvent->start.year) != 1 || newEvent->start.year < 1900) {
        printf("Invalid year. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }

    if (!validateDate(newEvent->start.day, newEvent->start.month, newEvent->start.year)) {
        printf("Invalid start date.\n");
        free(newEvent);
        flushInput();
        return;
    }

    printf("Enter start time:\n");
    printf(" Hour (0-23): ");
    if (scanf("%d", &newEvent->start.hour) != 1 || !validateTime(newEvent->start.hour, 0)) {
        printf("Invalid hour. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }
    printf(" Minute (0-59): ");
    if (scanf("%d", &newEvent->start.minute) != 1 || !validateTime(newEvent->start.minute, 1)) {
        printf("Invalid minute. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }

    printf("Enter end date:\n");
    printf(" Day (1-31): ");
    if (scanf("%d", &newEvent->end.day) != 1 || !validateDate(newEvent->end.day, 1, 2000)) {
        printf("Invalid day. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }
    printf(" Month (1-12): ");
    if (scanf("%d", &newEvent->end.month) != 1 || !validateDate(1, newEvent->end.month, 2000)) {
        printf("Invalid month. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }
    printf(" Year (> 1900): ");
    if (scanf("%d", &newEvent->end.year) != 1 || newEvent->end.year < 1900) {
        printf("Invalid year. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }

    if (!validateDate(newEvent->end.day, newEvent->end.month, newEvent->end.year)) {
        printf("Invalid end date.\n");
        free(newEvent);
        flushInput();
        return;
    }

    printf("Enter end time:\n");
    printf(" Hour (0-23): ");
    if (scanf("%d", &newEvent->end.hour) != 1 || !validateTime(newEvent->end.hour, 0)) {
        printf("Invalid hour. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }
    printf(" Minute (0-59): ");
    if (scanf("%d", &newEvent->end.minute) != 1 || !validateTime(newEvent->end.minute, 1)) {
        printf("Invalid minute. Aborting add.\n");
        free(newEvent);
        flushInput();
        return;
    }
    flushInput();

    if (compareDateTime(newEvent->end, newEvent->start) <= 0) {
        printf("Error: End date/time must be after start date/time.\n");
        free(newEvent);
        return;
    }

    if (isConflict(newEvent, -1)) {
        printf("Error: This event conflicts with an existing event.\n");
        free(newEvent);
        return;
    }

    insertEventSorted(newEvent);

    printf("Event added successfully!\n");
}

void viewEvents() {
    if (event_count == 0) {
        printf("\nNo events to show.\n");
        return;
    }
    printf("\n%-3s | %-50s | %-16s | %-16s\n", "No.", "Title", "Start (DD/MM/YYYY HH:MM)", "End (DD/MM/YYYY HH:MM)");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < event_count; i++) {
        printEvent(events[i], i);
    }
}

void deleteEvent() {
    if (event_count == 0) {
        printf("No events to delete.\n");
        return;
    }

    viewEvents();
    printf("Enter the number of the event to delete (0 to cancel): ");
    int delIndex;
    if (scanf("%d", &delIndex) != 1) {
        printf("Invalid input.\n");
        flushInput();
        return;
    }
    flushInput();
    if (delIndex == 0) {
        printf("Delete cancelled.\n");
        return;
    }
    if (delIndex < 1 || delIndex > event_count) {
        printf("Invalid event number.\n");
        return;
    }
    delIndex--;

    free(events[delIndex]);
    for (int i = delIndex; i < event_count - 1; i++) {
        events[i] = events[i + 1];
    }
    event_count--;

    printf("Event deleted successfully.\n");
}

void freeEvents() {
    for (int i = 0; i < event_count; i++) {
        free(events[i]);
    }
    event_count = 0;
}

int validateDate(int day, int month, int year) {
    if (year < 1900 || month < 1 || month > 12 || day < 1) {
        return 0;
    }

    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    int leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    if (leap && month == 2) {
        if (day > 29) return 0;
        else return 1;
    }

    if (day > daysInMonth[month - 1]) return 0;

    return 1;
}

int validateTime(int val, int isMinute) {
    if (isMinute) {
        return val >= 0 && val <= 59;
    } else {
        return val >= 0 && val <= 23;
    }
}

void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
