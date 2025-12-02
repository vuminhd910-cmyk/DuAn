#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define MAX_TRIPS 100
#define MAX_TICKETS 500
#define TICKET_PRICE 150.0

struct Location {
    char name[50];
    char address[100];
};

struct Trip {
    char tripId[20];
    struct Location departure;
    struct Location destination;
    char date[20];
    int totalSeats;
    int bookedSeats;
};

struct Passenger {
    char name[50];
    char phone[15];
};

struct Ticket {
    char ticketId[20];
    char tripId[20];
    struct Passenger passenger;
    int seatNumber;
    double price;
    int paymentStatus;
    char date[20];
};

struct Trip trips[MAX_TRIPS];
struct Ticket tickets[MAX_TICKETS];

int tripCount = 0;
int ticketCount = 0;

void menu();
void F01_addTrip();
void F02_updateTrip();
void F03_bookTicket();
void F04_checkTicketStatus();
void F05_listTrips(int pageNumber, int pageSize);
void F06_payTicket();
void F07_manageTicketStatus();
void F08_generateReport();

int findTripIndexById(const char *tripId) {
    for (int i = 0; i < tripCount; i++) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            return i;
        }
    }
    return -1;
}

int findTicketIndexById(const char *ticketId) {
    for (int i = 0; i < ticketCount; i++) {
        if (strcmp(tickets[i].ticketId, ticketId) == 0) {
            return i;
        }
    }
    return -1;
}

void removeNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void trim_spaces(char *str)
{
    int len = strlen(str);

    while (len > 0 && str[len - 1] == ' ')
    {
        str[len - 1] = '\0';
        len--;
    }

    int start = 0;

    while (str[start] == ' ')
    {
        start++;
    }

    if (start > 0)
    {
        memmove(str, str + start, strlen(str + start) + 1);
    }
}

 
int readLine(char *buf, size_t size) {
    if (fgets(buf, (int)size, stdin) == NULL) return 0;
    removeNewline(buf);
    trim_spaces(buf);
    return 1;
}

 
void pressEnterToContinue() {
    char tmp[4];
    printf("\n>> Bam ENTER de tiep tuc...");
     
    if (fgets(tmp, sizeof(tmp), stdin) == NULL) {
         
    }
}

 
bool isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int daysInMonth(int m, int y) {
    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) return 31;
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    if (m == 2) return isLeapYear(y) ? 29 : 28;
    return 0;
}

 
int parseDate(const char *s, int *d, int *m, int *y) {
    if (!s || strlen(s) < 8) return 0;
    int dd=0, mm=0, yy=0;
    if (sscanf(s, "%d/%d/%d", &dd, &mm, &yy) != 3) return 0;
    *d = dd; *m = mm; *y = yy;
    return 1;
}

int isValidDateStr(const char *s) {
    int d,m,y;
    if (!parseDate(s, &d, &m, &y)) return 0;
    if (y < 1900 || y > 9999) return 0;  
    if (m < 1 || m > 12) return 0;
    int dim = daysInMonth(m,y);
    if (d < 1 || d > dim) return 0;
    return 1;
}

 
int compareDates(const char *a, const char *b) {
    int da,ma,ya;
    int db,mb,yb;
    if (!parseDate(a,&da,&ma,&ya) || !parseDate(b,&db,&mb,&yb)) {
         
        int cmp = strcmp(a,b);
        if (cmp < 0) return -1;
        if (cmp > 0) return 1;
        return 0;
    }
    if (ya < yb) return -1;
    if (ya > yb) return 1;
    if (ma < mb) return -1;
    if (ma > mb) return 1;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

 
bool dateInRange(const char *d, const char *from, const char *to) {
    if (from && strlen(from) > 0) {
        if (!isValidDateStr(from) || !isValidDateStr(d)) return false;
        if (compareDates(d, from) < 0) return false;
    }
    if (to && strlen(to) > 0) {
        if (!isValidDateStr(to) || !isValidDateStr(d)) return false;
        if (compareDates(d, to) > 0) return false;
    }
    return true;
}

 
bool isNumericString(const char *s) {
    if (!s) return false;
    int len = strlen(s);
    
    if (len != 10) return false;  
    for (int i = 0; i < len; ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

void F01_addTrip() {
    printf("\nThem Chuyen Xe Moi\n");
    if (tripCount >= MAX_TRIPS) {
        printf("? Loi: He thong da day, khong the them chuyen xe moi.\n");
        return;
    }

    struct Trip newTrip;
    char buffer[256];
    int valid = 0;

    do {
        printf("Nhap ID Chuyen xe (TripId, duy nhat): ");
        if (!readLine(buffer, sizeof(buffer))) continue;
        if (strlen(buffer) == 0 || strlen(buffer) >= 20) {
            printf("?? Loi: TripId khong duoc de trong va phai nho hon 20 ky tu.\n");
            continue;
        }

        if (findTripIndexById(buffer) != -1) {
            printf("?? Loi: TripId nay da ton tai. Vui long nhap ID khac.\n");
        } else {
            strcpy(newTrip.tripId, buffer);
            valid = 1;
        }
    } while (valid == 0);
    valid = 0;

    do {
        printf("Nhap Ten Dia diem Khoi hanh: ");
        if (!readLine(newTrip.departure.name, sizeof(newTrip.departure.name))) continue;
        if (strlen(newTrip.departure.name) == 0) {
            printf("?? Loi: Ten dia diem khoi hanh khong duoc de trong.\n");
        } else {
            valid = 1;
        }
    } while (valid == 0);
    valid = 0;

    do {
        printf("Nhap Dia chi Khoi hanh: ");
        if (!readLine(newTrip.departure.address, sizeof(newTrip.departure.address))) continue;
        if (strlen(newTrip.departure.address) == 0) {
            printf("?? Loi: Dia chi khoi hanh khong duoc de trong.\n");
        } else {
            valid = 1;
        }
    } while (valid == 0);
    valid = 0;

    do {
        printf("Nhap Ten Dia diem Den: ");
        if (!readLine(newTrip.destination.name, sizeof(newTrip.destination.name))) continue;
        if (strlen(newTrip.destination.name) == 0) {
            printf("?? Loi: Ten dia diem den khong duoc de trong.\n");
        } else {
            valid = 1;
        }
    } while (valid == 0);
    valid = 0;

    do {
        printf("Nhap Dia chi Den: ");
        if (!readLine(newTrip.destination.address, sizeof(newTrip.destination.address))) continue;
        if (strlen(newTrip.destination.address) == 0) {
            printf("?? Loi: Dia chi den khong duoc de trong.\n");
        } else {
            valid = 1;
        }
    } while (valid == 0);
    valid = 0;

     
    do {
        printf("Nhap Ngay Khoi hanh (DD/MM/YYYY): ");
        if (!readLine(newTrip.date, sizeof(newTrip.date))) continue;
        if (strlen(newTrip.date) == 0) {
            printf("?? Loi: Ngay khoi hanh khong duoc de trong.\n");
            continue;
        }
        if (!isValidDateStr(newTrip.date)) {
            printf("?? Loi: Ngay khong hop le. Hay nhap theo dinh dang DD/MM/YYYY va la ngay hop le.\n");
            continue;
        }
        valid = 1;
    } while (valid == 0);
    valid = 0;

    do {
        printf("Nhap Tong so Ghe (TotalSeats > 0): ");
        if (!readLine(buffer, sizeof(buffer))) continue;
        if (sscanf(buffer, "%d", &newTrip.totalSeats) == 1) {
            if (newTrip.totalSeats <= 0) {
                printf("?? Loi: Tong so ghe phai lon hon 0.\n");
            } else {
                valid = 1;
            }
        } else {
            printf("?? Loi: Gia tri nhap khong hop le. Vui long nhap so nguyen.\n");
        }
    } while (valid == 0);

    newTrip.bookedSeats = 0;
    trips[tripCount++] = newTrip;
    printf("? Them chuyen xe %s thanh cong. Tong ghe: %d\n", newTrip.tripId, newTrip.totalSeats);
}

void F02_updateTrip() {
    printf("\n## ?? Cap Nhat Thong Tin Chuyen Xe ##\n");
    char tripId[20];
    char buffer[100];

    printf("Nhap ID Chuyen xe can cap nhat: ");
    if (!readLine(tripId, sizeof(tripId))) {
        printf("? Loi doc ID.\n");
        return;
    }

    int index = findTripIndexById(tripId);

    if (index == -1) {
        printf("? Loi: TripId **%s** khong ton tai trong he thong.\n", tripId);
        return;
    }

    struct Trip *t = &trips[index];

    printf("\n--- Thong tin hien tai cua chuyen xe %s ---\n", t->tripId);
    printf("Ngay: %s\n", t->date);
    printf("Khoi hanh: %s\n", t->departure.name);
    printf("Den: %s\n", t->destination.name);
    printf("Tong ghe: %d | Ghe da dat: %d\n", t->totalSeats, t->bookedSeats);
    printf("------------------------------------------\n");

    do {
        printf("Nhap Ngay Khoi hanh MOI (DD/MM/YYYY, Enter de giu nguyen: %s): ", t->date);
        if (!readLine(buffer, sizeof(buffer))) {
            printf("?? Giu nguyen Ngay Khoi hanh cu.\n");
            break;
        }
        if (strlen(buffer) > 0) {
            if (!isValidDateStr(buffer)) {
                printf("?? Loi: Ngay khong hop le. Giu nguyen Ngay cu.\n");
            } else {
                strcpy(t->date, buffer);
            }
        } else {
            
        }
        break;
    } while (1);

    do {
        printf("Nhap Ten Dia diem Khoi hanh MOI (Enter de giu nguyen: %s): ", t->departure.name);
        if (!readLine(buffer, sizeof(buffer))) {
            printf("?? Giu nguyen Ten Dia diem Khoi hanh cu.\n");
            break;
        }
        if (strlen(buffer) > 0) strcpy(t->departure.name, buffer);
        break;
    } while (1);

    do {
        printf("Nhap Dia chi Khoi hanh MOI (Enter de giu nguyen: %s): ", t->departure.address);
        if (!readLine(buffer, sizeof(buffer))) {
            printf("?? Giu nguyen Dia chi Khoi hanh cu.\n");
            break;
        }
        if (strlen(buffer) > 0) strcpy(t->departure.address, buffer);
        break;
    } while (1);

    do{
        printf("Nhap Ten Dia diem Den MOI (Enter de giu nguyen: %s): ", t->destination.name);
        if (!readLine(buffer, sizeof(buffer))) {
            printf("?? Giu nguyen Ten Dia diem Den cu.\n");
            break;
        }
        if (strlen(buffer) > 0) strcpy(t->destination.name, buffer);
        break;
    } while (1);

    do {
        printf("Nhap Dia chi Den MOI (Enter de giu nguyen: %s): ", t->destination.address);
        if (!readLine(buffer, sizeof(buffer))) {
            printf("?? Giu nguyen Dia chi Den cu.\n");
            break;
        }
        if (strlen(buffer) > 0) strcpy(t->destination.address, buffer);
        break;
    } while (1);

    printf("Nhap Tong so Ghe MOI (TotalSeats, nhap 0 hoac ky tu de giu nguyen: %d): ", t->totalSeats);
    int newTotalSeats = 0;
    if (readLine(buffer, sizeof(buffer))) {
        if (sscanf(buffer, "%d", &newTotalSeats) == 1 && newTotalSeats > 0) {
            if (newTotalSeats >= t->bookedSeats) {
                t->totalSeats = newTotalSeats;
            } else {
                printf("?? Loi: Tong so ghe moi (%d) khong the nho hon so ghe da dat (%d). Giu nguyen gia tri cu.\n", newTotalSeats, t->bookedSeats);
            }
        } else {
             printf("?? Giu nguyen Tong so ghe.\n");
        }
    } else {
         printf("?? Giu nguyen Tong so ghe.\n");
    }

    printf("? Cap nhat chuyen xe **%s** thanh cong.\n", t->tripId);
}

void F03_bookTicket() {
    printf("\n## ?? Dat Ve Xe Khach ##\n");

    if (ticketCount >= MAX_TICKETS) {
        printf("? Loi: He thong ve da day, khong the dat them ve.\n");
        return;
    }

    char tripId[20];
    printf("Nhap ID Chuyen xe muon dat: ");
    if (!readLine(tripId, sizeof(tripId))) {
        printf("? Loi doc ID.\n");
        return;
    }

    int tripIndex = findTripIndexById(tripId);

    if (tripIndex == -1) {
        printf("? Loi: TripId **%s** khong ton tai.\n", tripId);
        return;
    }

    struct Trip *currentTrip = &trips[tripIndex];

    if (currentTrip->bookedSeats >= currentTrip->totalSeats) {
        printf("? Loi: Chuyen xe **%s** da het ve.\n", tripId);
        return;
    }

    struct Ticket newTicket;
    char buffer[64];
    int valid = 0;

    sprintf(newTicket.ticketId, "T%03d", ticketCount + 1);
    strcpy(newTicket.tripId, tripId);

    do {
        printf("Nhap Ten Hanh khach: ");
        if (!readLine(newTicket.passenger.name, sizeof(newTicket.passenger.name))) continue;
        if (strlen(newTicket.passenger.name) == 0) {
            printf("?? Loi: Ten hanh khach khong duoc de trong.\n");
        } else valid = 1;
    } while (valid == 0);
    valid = 0;

     
    do {
        printf("Nhap So dien thoai (chi nhap chu so, phai 10 chu so): ");
        if (!readLine(newTicket.passenger.phone, sizeof(newTicket.passenger.phone))) continue;
        if (!isNumericString(newTicket.passenger.phone)) {
            printf("?? Loi: So dien thoai khong hop le. Phai chua CHINH XAC 10 chu so. Vui long nhap lai.\n");
        } else {
            valid = 1;
        }
    } while (valid == 0);
    valid = 0;

    int availableSeats = currentTrip->totalSeats - currentTrip->bookedSeats;
    printf("?? Con **%d** ghe trong (Tong: %d). \n", availableSeats, currentTrip->totalSeats);

    int validSeat = 0;

    do {
        printf("Nhap So ghe muon dat (1 - %d): ", currentTrip->totalSeats);
        if (!readLine(buffer, sizeof(buffer))) continue;
        if (sscanf(buffer, "%d", &newTicket.seatNumber) == 1) {
            if (newTicket.seatNumber >= 1 && newTicket.seatNumber <= currentTrip->totalSeats) {
                bool isBooked = false;
                for(int i = 0; i < ticketCount; i++) {
                    if (strcmp(tickets[i].tripId, tripId) == 0 &&
                        tickets[i].seatNumber == newTicket.seatNumber &&
                        tickets[i].paymentStatus != -1)
                    {
                        isBooked = true;
                        break;
                    }
                }
                if (isBooked) {
                    printf("?? Loi: Ghe so %d da co nguoi dat/thanh toan/bi khoa. Vui long chon ghe khac.\n", newTicket.seatNumber);
                } else {
                    validSeat = 1;
                }
            } else {
                printf("?? Loi: So ghe khong hop le. Vui long nhap lai.\n");
            }
        } else {
            printf("?? Loi: Gia tri nhap khong hop le. Vui long nhap so nguyen.\n");
        }
    } while (validSeat == 0);

    newTicket.price = TICKET_PRICE;
    newTicket.paymentStatus = 0;

     
    do {
        printf("Nhap Ngay dat ve (DD/MM/YYYY): ");
        if (!readLine(newTicket.date, sizeof(newTicket.date))) continue;
        if (strlen(newTicket.date) == 0) {
             printf("?? Loi: Ngay dat ve khong duoc de trong.\n");
             continue;
        }
        if (!isValidDateStr(newTicket.date)) {
            printf("?? Loi: Ngay dat ve khong hop le. Hay nhap DD/MM/YYYY.\n");
            continue;
        }
        valid = 1;
    } while (valid == 0);

    tickets[ticketCount++] = newTicket;
    currentTrip->bookedSeats++;

    printf("? Dat ve **%s** (Ghe: %d) cho chuyen xe **%s** thanh cong!\n",
           newTicket.ticketId, newTicket.seatNumber, newTicket.tripId);
    printf("Gia ve: %.2lf VND. Trang thai: **CHUA THANH TOAN**. Vui long thanh toan (F06) de xac nhan.\n", newTicket.price);
}

 
void F04_checkTicketStatus() {
    printf("\n## ?? Kiem Tra Tinh Trang Ve(enter de tiep tuc) ##\n");
    char ticketId[20];
    char buffer[100];
    int valid = 0;

    do {
        printf("Nhap ID Ve can kiem tra: ");
        if (!readLine(buffer, sizeof(buffer))) continue;
        if (strlen(buffer) == 0) {
            printf("?? Loi: ID Ve khong duoc de trong. Vui long nhap lai.\n");
        } else if (strlen(buffer) >= 20) {
            printf("?? Loi: ID Ve qua dai.\n");
        } else {
            strcpy(ticketId, buffer);
            valid = 1;
        }
    } while (valid == 0);

    int index = findTicketIndexById(ticketId);

    if (index == -1) {
        printf("? Loi: TicketId **%s** khong ton tai trong he thong.\n", ticketId);
        return;
    }

    struct Ticket *t = &tickets[index];

    int tripIndex = findTripIndexById(t->tripId);
    struct Trip *trip = (tripIndex != -1) ? &trips[tripIndex] : NULL;

    printf("\n--- Thong tin Ve **%s** ---\n", t->ticketId);
    printf("Hanh khach: %s (SDT: %s)\n", t->passenger.name, t->passenger.phone);
    printf("So ghe: **%d**\n", t->seatNumber);
    printf("Gia ve: %.2lf VND\n", t->price);
    printf("Ngay dat: %s\n", t->date);

    printf("Chuyen xe: %s", t->tripId);
    if (trip) {
        printf(" (Tu %s den %s, Ngay: %s)", trip->departure.name, trip->destination.name, trip->date);
    }
    printf("\n");

    printf("Trang thai Thanh toan: ");
    switch (t->paymentStatus) {
        case 0:
            printf("?? **CHUA THANH TOAN**\n");
            break;
        case 1:
            printf("?? **DA THANH TOAN** (Khong the huy)\n");
            break;
        case -1:
            printf("? **DA HUY**\n");
            break;
        case -2:
            printf("?? **DA KHOA** (Khong the thanh toan)\n");
            break;
        default:
            printf("? Khong xac dinh\n");
            break;
    }
    printf("-----------------------------------\n");
}

 
void F05_listTrips(int pageNumber, int pageSize) {
     
    if (pageSize <= 0) pageSize = 5;
    if (pageNumber <= 0) pageNumber = 1;

    printf("\n## ?? Danh Sach Chuyen Xe (List Trips) ##\n");

    if (tripCount == 0) {
        printf("? He thong chua co chuyen xe nao de hien thi.\n");
        return;
    }

    int totalPages = (tripCount + pageSize - 1) / pageSize;
    if (pageNumber > totalPages) {
        printf("? Loi: So trang khong hop le. Tong so trang = %d\n", totalPages);
        return;
    }

    int currentPage = pageNumber;
    char input[32];

    while (1) {
        int start = (currentPage - 1) * pageSize;
        int end = start + pageSize;
        if (end > tripCount) end = tripCount;

        printf("\n--- Trang %d/%d ---\n", currentPage, totalPages);
        printf("+----------------------+------------------------+------------------------+------------+------------+-----------+\n");
        printf("| %-20s | %-22s | %-22s | %-10s | %-10s | %-9s |\n",
               "TripId", "Khoi Hanh", "Diem Den", "Ngay", "Tong Ghe", "Da Dat");
        printf("+----------------------+------------------------+------------------------+------------+------------+-----------+\n");

        for (int i = start; i < end; i++) {
            struct Trip t = trips[i];
            printf("| %-20s | %-22s | %-22s | %-10s | %-10d | %-9d |\n",
                   t.tripId, t.departure.name, t.destination.name, t.date, t.totalSeats, t.bookedSeats);
        }
        printf("+----------------------+------------------------+------------------------+------------+------------+-----------+\n");

        printf("\n[HDSD] Nhap '-1' de ve trang truoc, '+1' de den trang sau, so trang (1-%d) de nhay den trang, '0' de thoat.\n", totalPages);
        printf("Nhap lua chon: ");
        if (!readLine(input, sizeof(input))) {
            printf("\n? Loi doc du lieu. Thoat.\n");
            return;
        }

        if (strlen(input) == 0) {
            printf("?? Lua chon khong hop le (khong duoc de trong). Vui long nhap lai.\n");
            continue;
        }

        if (strcmp(input, "+1") == 0) {
            if (currentPage < totalPages) currentPage++;
            else printf("?? Dang o trang cuoi.\n");
            continue;
        } else if (strcmp(input, "-1") == 0) {
            if (currentPage > 1) currentPage--;
            else printf("?? Dang o trang dau.\n");
            continue;
        } else if (strcmp(input, "0") == 0) {
            return;
        } else {
            bool isNumber = true;
            for (size_t i = 0; i < strlen(input); ++i) {
                if (!(input[i] >= '0' && input[i] <= '9')) {
                    isNumber = false;
                    break;
                }
            }
            if (!isNumber) {
                printf("?? Lua chon khong hop le. Vui long nhap -1, +1, so trang hoac 0.\n");
                continue;
            }
            int requested = atoi(input);
            if (requested >= 1 && requested <= totalPages) {
                currentPage = requested;
                continue;
            } else {
                printf("?? Loi: So trang %d khong ton tai. Vui long chon lai.\n", requested);
                continue;
            }
        }
    }
}

void F06_payTicket() {
    printf("\n## ?? Thanh Toan Ve(enter de tiep tuc) ##\n");
    char ticketId[20];
    char buffer[100];
    int valid = 0;
    do {
        printf("Nhap ID Ve can thanh toan: ");
        if (!readLine(buffer, sizeof(buffer))) continue;
        if (strlen(buffer) == 0) {
            printf("?? Loi: ID Ve khong duoc de trong. Vui long nhap lai.\n");
        } else if (strlen(buffer) >= 20) {
             printf("?? Loi: ID Ve qua dai.\n");
        } else {
            strcpy(ticketId, buffer);
            valid = 1;
        }
    } while (valid == 0);

    int index = findTicketIndexById(ticketId);
    if (index == -1) {
        printf("? Loi: TicketId **%s** khong ton tai.\n", ticketId);
        return;
    }

    struct Ticket *t = &tickets[index];

    if (t->paymentStatus == -1) {
        printf("? Loi: Ve **%s** da **HUY**. Khong the thanh toan.\n", t->ticketId);
        return;
    }
    if (t->paymentStatus == -2) {
        printf("? Loi: Ve **%s** da **KHOA**. Khong the thanh toan.\n", t->ticketId);
        return;
    }

    if (t->paymentStatus == 1) {
        printf("?? Ve **%s** da **DA THANH TOAN** truoc do.\n", t->ticketId);
        return;
    }

    printf("?? Xac nhan thanh toan cho ve **%s** (Gia: %.2lf VND)? (y/n): ", t->ticketId, t->price);
    if (!readLine(buffer, sizeof(buffer))) {
        printf("?? Huy bo thanh toan do loi doc du lieu.\n");
        return;
    }
    if (buffer[0] == 'y' || buffer[0] == 'Y') {
        t->paymentStatus = 1;
        printf("? Thanh toan ve **%s** thanh cong! Trang thai: **DA THANH TOAN**.\n", t->ticketId);
    } else {
        printf("?? Huy bo thanh toan cho ve **%s**.\n", t->ticketId);
    }
}

void F07_manageTicketStatus() {
    printf("\n## ?? Quan Ly Trang Thai Ve (Khoa/Huy)(enter de tiep tuc) ##\n");
    char ticketId[20];
    char buffer[100];
    int valid = 0;
    do {
        printf("Nhap ID Ve can quan ly trang thai: ");
        if (!readLine(buffer, sizeof(buffer))) continue;
        if (strlen(buffer) == 0) {
            printf("?? Loi: ID Ve khong duoc de trong. Vui long nhap lai.\n");
        } else if (strlen(buffer) >= 20) {
            printf("?? Loi: ID Ve qua dai.\n");
        } else {
            strcpy(ticketId, buffer);
            valid = 1;
        }
    } while (valid == 0);

    int index = findTicketIndexById(ticketId);
    if (index == -1) {
        printf("? Loi: TicketId **%s** khong ton tai.\n", ticketId);
        return;
    }

    struct Ticket *t = &tickets[index];

    if (t->paymentStatus == -1) {
        printf("? Loi: Ve **%s** da **HUY**. Khong the thao tac them.\n", t->ticketId);
        return;
    }
    if (t->paymentStatus == -2) {
        printf("? Loi: Ve **%s** da **KHOA**. Khong the thao tac them.\n", t->ticketId);
        return;
    }
    if (t->paymentStatus == 1) {
        printf("?? Ve **%s** da **THANH TOAN**. Chi co the **KHOA**, khong the **HUY**.\n", t->ticketId);
    }

    printf("\nChon thao tac cho ve **%s** (Trang thai hien tai: %s):\n", t->ticketId, (t->paymentStatus == 0) ? "CHUA THANH TOAN" : "DA THANH TOAN");
    printf("1. Khoa Ve (Vo hieu hoa, khong cho thanh toan)\n");
    printf("2. Huy Ve (Chi ap dung voi ve chua thanh toan, giai phong ghe)\n");
    printf("0. Quay lai\n");
    printf("Lua chon: ");

    if (!readLine(buffer, sizeof(buffer))) return;
    int choice = -1;
    if (sscanf(buffer, "%d", &choice) != 1) return;

    switch (choice) {
        case 1:
            t->paymentStatus = -2;
            printf("?? Khoa ve **%s** thanh cong. Ve nay khong the thanh toan.\n", t->ticketId);
            break;
        case 2:
            if (t->paymentStatus == 1) {
                printf("? Loi: Ve **%s** da **THANH TOAN**. Khong the HUY.\n", t->ticketId);
                break;
            }
            if (t->paymentStatus == 0) {
                t->paymentStatus = -1;
                int tripIndex = findTripIndexById(t->tripId);
                if (tripIndex != -1) {
                    trips[tripIndex].bookedSeats--;
                }
                printf("? Huy ve **%s** thanh cong. Ghe da duoc giai phong.\n", t->ticketId);
            }
            break;
        case 0:
            printf("Quay lai menu chinh.\n");
            break;
        default:
            printf("Lua chon khong hop le.\n");
            break;
    }
}

 
void F08_generateReport() {
    printf("\n## ?? Bao Cao Thong Ke & Doanh Thu (Revenue & Ticket Statistics Report) ##\n");

    char fromDate[20] = "";
    char toDate[20] = "";
    char buffer[64];
    int type = 0;

     
    printf("Nhap fromDate (DD/MM/YYYY) (Enter de bo qua): ");
    readLine(fromDate, sizeof(fromDate));
    if (strlen(fromDate) > 0) {
        if (!isValidDateStr(fromDate)) {
            printf("?? Loi: fromDate khong hop le. Hay dung DD/MM/YYYY.\n");
            return;
        }
    }

    
    printf("Nhap toDate (DD/MM/YYYY) (Enter de bo qua): ");
    readLine(toDate, sizeof(toDate));
    if (strlen(toDate) > 0) {
        if (!isValidDateStr(toDate)) {
            printf("?? Loi: toDate khong hop le. Hay dung DD/MM/YYYY.\n");
            return;
        }
    }

     
    if (strlen(fromDate) > 0 && strlen(toDate) > 0) {
        if (compareDates(fromDate, toDate) > 0) {
            printf("?? Loi: fromDate phai nho hon hoac bang toDate.\n");
            return;
        }
    }

     
    printf("Chon kieu bao cao (1=Tong doanh thu, 2=Thong ke theo chuyen xe, 3=Thong ke tong hop trong khoang thoi gian): ");
    if (!readLine(buffer, sizeof(buffer))) {
        printf("?? Loi doc type.\n");
        return;
    }
    if (sscanf(buffer, "%d", &type) != 1 || !(type >=1 && type <=3)) {
        printf("?? Loi: Type khong hop le. Phai la 1, 2 hoac 3.\n");
        return;
    }

     
    if (ticketCount == 0 && type != 1) {
        printf("? He thong khong co ve nao de thong ke.\n");
        return;
    }
    if (tripCount == 0 && type == 2) {
        printf("? He thong khong co chuyen xe nao de thong ke.\n");
        return;
    }

     
    if (type == 1) {
        double totalRevenue = 0.0;
        int paidCount = 0;
        for (int i = 0; i < ticketCount; ++i) {
            struct Ticket *t = &tickets[i];
            if (t->paymentStatus == 1) {
                if ( (strlen(fromDate) == 0 && strlen(toDate) == 0) || dateInRange(t->date, fromDate, toDate) ) {
                    totalRevenue += t->price;
                    paidCount++;
                }
            }
        }
        printf("\n--- Tong doanh thu ---\n");
        printf("Tong so ve da thanh toan (trong khoang loc): %d\n", paidCount);
        printf("Tong doanh thu (VND): %.2lf\n", totalRevenue);
        return;
    }

     
    if (type == 2) {
        printf("\n--- Thong ke theo chuyen xe ---\n");
        printf("+----------------------+------------+------------+------------+--------------------+\n");
        printf("| %-20s | %-10s | %-10s | %-10s | %-18s |\n", "TripId", "Da Dat", "Da TT", "Da Huy", "Doanh Thu (VND)");
        printf("+----------------------+------------+------------+------------+--------------------+\n");
        double grandTotalRevenue = 0.0;
        int anyShown = 0;
        for (int i = 0; i < tripCount; ++i) {
            struct Trip *tr = &trips[i];
            int countBooked = 0;
            int countPaid = 0;
            int countCanceled = 0;
            double tripRevenue = 0.0;
             
            for (int j = 0; j < ticketCount; ++j) {
                struct Ticket *tk = &tickets[j];
                if (strcmp(tk->tripId, tr->tripId) != 0) continue;
                
                if (!(strlen(fromDate)==0 && strlen(toDate)==0)) {
                    if (!dateInRange(tk->date, fromDate, toDate)) continue;
                }
                 
                if (tk->paymentStatus >= 0 || tk->paymentStatus == -2) countBooked++;
                if (tk->paymentStatus == 1) {
                    countPaid++;
                    tripRevenue += tk->price;
                } else if (tk->paymentStatus == -1) countCanceled++;
            }
             
            if (countBooked > 0 || countPaid > 0 || countCanceled > 0) {
                anyShown = 1;
                grandTotalRevenue += tripRevenue;
                printf("| %-20s | %-10d | %-10d | %-10d | %-18.2lf |\n",
                       tr->tripId, countBooked, countPaid, countCanceled, tripRevenue);
            }
        }
        if (!anyShown) {
            printf("| %-20s | %-10s | %-10s | %-10s | %-18s |\n", "Khong co chuyen xe", "-", "-", "-", "-");
        }
        printf("+----------------------+------------+------------+------------+--------------------+\n");
        printf("| %-20s | %-10s | %-10s | %-10s | %-18.2lf |\n", "TONG KET", "", "", "", grandTotalRevenue);
        printf("+----------------------+------------+------------+------------+--------------------+\n");
        return;
    }

     
    if (type == 3) {
        int totalBooked = 0;
        int totalPaid = 0;
        int totalCanceled = 0;
        double totalRevenue = 0.0;
        for (int i = 0; i < ticketCount; ++i) {
            struct Ticket *t = &tickets[i];
            if (!(strlen(fromDate)==0 && strlen(toDate)==0)) {
                if (!dateInRange(t->date, fromDate, toDate)) continue;
            }
            if (t->paymentStatus >= 0 || t->paymentStatus == -2) totalBooked++;
            if (t->paymentStatus == 1) {
                totalPaid++;
                totalRevenue += t->price;
            } else if (t->paymentStatus == -1) totalCanceled++;
        }
        printf("\n--- Thong ke tong hop trong khoang thoi gian ---\n");
        if (strlen(fromDate) > 0 || strlen(toDate) > 0) {
            printf("Loc theo: fromDate='%s' toDate='%s'\n", fromDate, toDate);
        } else {
            printf("Khong loc theo ngay (tat ca du lieu)\n");
        }
        printf("So ve da dat: %d\n", totalBooked);
        printf("So ve da thanh toan: %d\n", totalPaid);
        printf("So ve da huy: %d\n", totalCanceled);
        printf("Tong doanh thu (VND): %.2lf\n", totalRevenue);
        return;
    }
}

void menu() {
	system("cls");  
    printf("================================================\n");
    printf("|       HE THONG QUAN LY VE XE KHACH           |\n");
    printf("================================================\n");
    printf("|1 : Them chuyen xe moi                        |\n");
    printf("|2 : Cap nhat thong tin chuyen xe              |\n");
    printf("|3 : Dat ve                                    |\n");
    printf("|4 : Kiem tra tinh trang ve                    |\n");
    printf("|5 : Liet ke chuyen xe                         |\n");
    printf("|6 : Thanh toan ve                             |\n");
    printf("|7 : Quan ly trang thai ve (Khoa, Huy)         |\n");
    printf("|8 : Bao cao thong ke & doanh thu              |\n");
    printf("|0 : Thoat                                     |\n");
    printf("------------------------------------------------\n");
    printf("Nhap lua chon cua ban: ");
}

int main() {
    int choice;
    char input[16];

    strcpy(trips[0].tripId, "HN-SG-001");
    strcpy(trips[0].departure.name, "Ha Noi");
    strcpy(trips[0].destination.name, "Sai Gon");
    strcpy(trips[0].date, "20/12/2025");
    trips[0].totalSeats = 40;
    trips[0].bookedSeats = 2;

    strcpy(trips[1].tripId, "HCM-DN-002");
    strcpy(trips[1].departure.name, "HCM");
    strcpy(trips[1].destination.name, "Da Nang");
    strcpy(trips[1].date, "21/12/2025");
    strcpy(trips[1].departure.address, "50 C.Dien Bien Phu");
    strcpy(trips[1].destination.address, "100 P.Nguyen Van Linh");
    trips[1].totalSeats = 30;
    trips[1].bookedSeats = 0;

    strcpy(trips[2].tripId, "DN-HN-003");
    strcpy(trips[2].departure.name, "Da Nang");
    strcpy(trips[2].destination.name, "Ha Noi");
    strcpy(trips[2].date, "22/12/2025");
    strcpy(trips[2].departure.address, "200 QL1A");
    strcpy(trips[2].destination.address, "300 P.Giai Phong");
    trips[2].totalSeats = 35;
    trips[2].bookedSeats = 1;

    strcpy(trips[3].tripId, "SG-VT-004");
    strcpy(trips[3].departure.name, "Sai Gon");
    strcpy(trips[3].destination.name, "Vung Tau");
    strcpy(trips[3].date, "23/12/2025");
    strcpy(trips[3].departure.address, "789 P.Nguyen Hue");
    strcpy(trips[3].destination.address, "1 P.Thuy Van");
    trips[3].totalSeats = 25;
    trips[3].bookedSeats = 0;

    strcpy(trips[4].tripId, "CT-KG-005");
    strcpy(trips[4].departure.name, "Can Tho");
    strcpy(trips[4].destination.name, "Kien Giang");
    strcpy(trips[4].date, "24/12/2025");
    strcpy(trips[4].departure.address, "90 P.Mau Than");
    strcpy(trips[4].destination.address, "12 C.30 Thang 4");
    trips[4].totalSeats = 28;
    trips[4].bookedSeats = 0;

    strcpy(trips[5].tripId, "NA-QB-006");
    strcpy(trips[5].departure.name, "Nghe An");
    strcpy(trips[5].destination.name, "Quang Binh");
    strcpy(trips[5].date, "25/12/2025");
    strcpy(trips[5].departure.address, "A QL1A");
    strcpy(trips[5].destination.address, "B QL1A");
    trips[5].totalSeats = 20;
    trips[5].bookedSeats = 0;

    tripCount = 6;

    strcpy(tickets[0].ticketId, "T001");
    strcpy(tickets[0].tripId, "HN-SG-001");
    strcpy(tickets[0].passenger.name, "NGUYEN LE LINH CHI");
    strcpy(tickets[0].passenger.phone, "0918865983");
    tickets[0].seatNumber = 10;
    tickets[0].price = TICKET_PRICE;
    tickets[0].paymentStatus = 1;
    strcpy(tickets[0].date, "10/12/2025");

    strcpy(tickets[1].ticketId, "T002");
    strcpy(tickets[1].tripId, "HN-SG-001");
    strcpy(tickets[1].passenger.name, "VU MINH DUC");
    strcpy(tickets[1].passenger.phone, "0979284767");
    tickets[1].seatNumber = 12;
    tickets[1].price = TICKET_PRICE;
    tickets[1].paymentStatus = 0;
    strcpy(tickets[1].date, "11/12/2025");
    ticketCount = 2;

    do {
        menu();
        if (!readLine(input, sizeof(input))) {
            printf("? Lua chon khong hop le. Vui long nhap lai.\n");
            choice = -1;
            continue;
        }
        if (sscanf(input, "%d", &choice) != 1) {
            printf("? Lua chon khong hop le. Vui long nhap so.\n");
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: F01_addTrip(); break;
            case 2: F02_updateTrip(); break;
            case 3: F03_bookTicket(); break;
            case 4: F04_checkTicketStatus(); break;
            case 5: F05_listTrips(1, 5); break;  
            case 6: F06_payTicket(); break;
            case 7: F07_manageTicketStatus(); break;
            case 8: F08_generateReport(); break;
            case 0: printf("\n?? Cam on ban da su dung he thong. Hen gap lai!\n"); break;
            default: printf("? Chuc nang khong ton tai. Vui long chon lai.\n"); break;
        }

        if (choice != 0) {
            pressEnterToContinue();
        }
    } while (choice != 0);

    return 0;
}


