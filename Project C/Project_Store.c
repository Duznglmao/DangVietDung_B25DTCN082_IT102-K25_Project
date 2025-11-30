#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_PRODUCTS 100
#define MAX_TRANSACTIONS 1000
#define ITEMS_PER_PAGE 10
#define MAX_INT_VALUE 1000000000
#define MAX_DIGITS 10

typedef struct {
    char id[10];
    char name[50];
    char unit[10];
    int qty;
    int price;
    int status; 
} Product;

typedef struct {
    char productId[10];
    int qty;
    char type; 
    time_t time;
} Transaction;

Product products[MAX_PRODUCTS];
int productCount = 0;

Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

const char *UNITS[] = {"Cai", "Thanh", "Soi", "Hop", "Thung", "Kg", "Lit"};
const int UNITS_COUNT = 7;

void toLower(const char *src, char *dest) {
    for (int i = 0; src[i]; i++) {
        dest[i] = tolower(src[i]);
    }
    dest[strlen(src)] = '\0';
}

void trim(char *str) {
    char *start = str;
    while (*start && isspace(*start)) start++;
    
    if (*start == '\0') {
        *str = '\0';
        return;
    }
    
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace(*end)) end--;
    *(end + 1) = '\0';
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int findProduct(const char *id) {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

int isValidInteger(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i = 1;
    }
    
    if (str[i] == '\0') return 0;
    
    while (str[i] != '\0') {
        if (!isdigit(str[i])) return 0;
        i++;
    }
    return 1;
}

int inputInt(const char *prompt, int allowEmpty, int allowZero) {
    char line[100];
    
    while (1) {
        printf("%s", prompt);
        
        if (!fgets(line, sizeof(line), stdin)) continue;
        trim(line);
        
        if (strlen(line) == 0) {
            if (allowEmpty) return -1;
            printf("Vui long nhap gia tri!\n");
            continue;
        }
        
        if (line[0] == '-') {
            printf("Gia tri khong duoc am!\n");
            continue;
        }
        
        if (!isValidInteger(line) || strlen(line) > MAX_DIGITS) {
            printf("Gia tri khong hop le! Chi duoc nhap so nguyen duong.\n");
            continue;
        }
        
        int value;
        if (sscanf(line, "%d", &value) != 1 || value > MAX_INT_VALUE) {
            printf("Gia tri qua lon! Vui long nhap toi da %d.\n", MAX_INT_VALUE);
            continue;
        }
        
        if (!allowZero && value == 0) {
            printf("Gia tri phai lon hon 0!\n");
            continue;
        }
        
        return value;
    }
}

void inputStr(const char *prompt, char *dest, int maxLen, int allowEmpty) {
    char line[256];
    
    while (1) {
        printf("%s", prompt);
        
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        
        line[strcspn(line, "\n")] = 0;
        trim(line);
        
        if (!allowEmpty && strlen(line) == 0) {
            printf("Khong duoc de trong hoac chi co khoang trang!\n");
            continue;
        }
        
        if (strlen(line) >= maxLen) {
            printf("Canh bao: Du lieu qua dai, se bi cat bot!\n");
        }
        
        strncpy(dest, line, maxLen - 1);
        dest[maxLen - 1] = '\0';
        return;
    }
}

const char* getStatusText(int status) {
    if (status) {
        return "Con su dung";
    } else {
        return "Da khoa";
    }
}

const char* getStatusShortText(int status) {
    if (status) {
        return "SD";
    } else {
        return "Khoa";
    }
}

void selectUnit(char *dest) {
    printf("\n╔════════════════════════════╗\n");
    printf("║    CHON DON VI TINH        ║\n");
    printf("╠════════════════════════════╣\n");
    for (int i = 0; i < UNITS_COUNT; i++) {
        printf("║ %d. %-23s ║\n", i + 1, UNITS[i]);
    }
    printf("╚════════════════════════════╝\n");
    
    int choice = inputInt("Lua chon don vi: ", 0, 1);
    
    if (choice >= 1 && choice <= UNITS_COUNT) {
        strcpy(dest, UNITS[choice - 1]);
    } else {
        printf("Lua chon khong hop le! Mac dinh chon 'Cai'\n");
        strcpy(dest, "Cai");
    }
}

void initDefaultProducts() {
    const char *names[] = {
        "Laptop Dell Inspiron", "Chuot khong day Logitech", "Ban phim co RGB",
        "Man hinh LG 24 inch", "Tai nghe Sony WH-1000", "USB Kingston 32GB",
        "O cung SSD Samsung 500GB", "RAM DDR4 8GB", "Webcam HD 1080p",
        "Loa Bluetooth JBL", "Hub USB 3.0 4 cong", "Cap HDMI 2m",
        "Sac du phong 20000mAh", "But stylus Apple Pencil", "Gia do laptop",
        "Tui chong soc laptop", "Den ban LED", "Quat tan nhiet USB",
        "Pad chuot gaming", "Ke mon hinh go"
    };
    
    const char *units[] = {
        "Cai", "Cai", "Cai", "Cai", "Cai", "Cai", "Cai", "Thanh", "Cai", "Cai",
        "Cai", "Soi", "Cai", "Cai", "Cai", "Cai", "Cai", "Cai", "Cai", "Cai"
    };
    
    int prices[] = {
        15000000, 350000, 850000, 3500000, 6500000, 150000, 2200000, 800000,
        550000, 1200000, 250000, 80000, 450000, 2800000, 350000, 280000,
        320000, 120000, 180000, 420000
    };
    
    int quantities[] = {
        15, 45, 30, 20, 12, 80, 25, 40, 35, 28, 50, 60, 38, 18, 42, 55, 33, 48, 65, 37
    };
    
    for (int i = 0; i < 20; i++) {
        sprintf(products[i].id, "SP%03d", i + 1);
        strcpy(products[i].name, names[i]);
        strcpy(products[i].unit, units[i]);
        products[i].qty = quantities[i];
        products[i].price = prices[i];
        products[i].status = 1;
    }
    
    productCount = 20;
}

void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        printf("\n>>> Danh sach day! Khong the them.\n\n");
        return;
    }

    Product p;
    memset(&p, 0, sizeof(Product));
    p.status = 1;
    char temp_id[10];

    while (1) {
        inputStr("Ma hang: ", temp_id, sizeof(temp_id), 0);
        
        if (findProduct(temp_id) != -1) {
            printf("\n=== LOI KHI THEM MOI ===\n");
            printf("- Ma hang [%s] da ton tai! Vui long nhap lai.\n", temp_id);
        } else {
            strcpy(p.id, temp_id);
            break;
        }
    }

    inputStr("Ten hang: ", p.name, sizeof(p.name), 0);
    selectUnit(p.unit);
    p.qty = inputInt("So luong: ", 0, 0); 	
    p.price = inputInt("Gia: ", 0, 1); 	 

    products[productCount] = p;
    productCount++;
    printf("\nThem moi hang hoa thanh cong\n\n");
}

void updateProduct() {
    char id[10];
    inputStr("\nMa hang can cap nhat: ", id, sizeof(id), 0);

    int idx = findProduct(id);
    if (idx == -1) {
        printf("Vat tu <%s> khong ton tai trong danh sach.\n\n", id);
        return;
    }

    printf("\nCap nhat thong tin cho san pham: %s\n", products[idx].name);
    printf("De trong (Enter) de giu nguyen\n\n");

    char temp[50];
    printf("Ten moi (Hien tai: %s): ", products[idx].name);
    inputStr("", temp, sizeof(temp), 1);
    if (strlen(temp) > 0) {
        strcpy(products[idx].name, temp);
    }

    printf("Don vi hien tai: %s\n", products[idx].unit);
    printf("Ban co muon doi don vi? (Nhap 1 de doi, Enter de giu nguyen): ");
    int changeUnit = inputInt("", 1, 1);
    if (changeUnit == 1) {
        selectUnit(products[idx].unit);
    }

    printf("So luong moi (Hien tai: %d): ", products[idx].qty);
    int val = inputInt("", 1, 1); 
    if (val >= 0) {
        products[idx].qty = val;
    }

    printf("Gia moi (Hien tai: %d): ", products[idx].price);
    val = inputInt("", 1, 1); 	
    if (val >= 0) {
        products[idx].price = val;
    }

    printf("\nCap nhat hang hoa <%s> thanh cong\n\n", products[idx].id);
}

void toggleStatus() {
    char id[10];
    inputStr("\nMa hang can khoa/mo: ", id, sizeof(id), 0);

    int idx = findProduct(id);
    if (idx == -1) {
        printf("Vat tu <%s> khong ton tai trong danh sach.\n\n", id);
        return;
    }

    products[idx].status = 1 - products[idx].status;
    
    printf("\nDoi trang thai hang hoa <%s> thanh cong, trang thai moi: %s\n\n", 
           products[idx].id, getStatusText(products[idx].status));
}

void searchProduct() {
    char keyword[50];
    inputStr("\nTu khoa (ma/ten): ", keyword, sizeof(keyword), 0);

    char keyLower[50];
    toLower(keyword, keyLower);

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                KET QUA TIM KIEM                                ║\n");
    printf("╠═════╤══════════╤══════════════════════╤═════════╤══════════╤═══════════════════╣\n");
    printf("║ STT │ Ma hang  │ Ten hang             │ So luong│   Gia    │  Trang thai       ║\n");
    printf("╠═════╪══════════╪══════════════════════╪═════════╪══════════╪═══════════════════╣\n");

    int found = 0;
    for (int i = 0; i < productCount; i++) {
        int match = 0;
        
        if (strcmp(products[i].id, keyword) == 0) {
            match = 1;
        } else {
            char nameLower[50];
            toLower(products[i].name, nameLower);
            if (strstr(nameLower, keyLower)) {
                match = 1;
            }
        }

        if (match) {
            found++;
            printf("║ %3d │ %-8s │ %-19s │ %7d │ %8d │ %-17s ║\n",
                   found, products[i].id, products[i].name, 
                   products[i].qty, products[i].price, getStatusText(products[i].status));
        }
    }

    if (found == 0) {
        printf("║                  Khong tim thay thong tin nao phu hop!                         ║\n");
    } else {
        printf("╠═════╧══════════╧══════════════════════╧═════════╧══════════╧═══════════════════╣\n");
        printf("║ %30s Tim thay %2d san pham %26s ║\n", "", found, ""); 
    }
    
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n\n");
}

void displayPage(int page) {
    int start = (page - 1) * ITEMS_PER_PAGE;
    int end = start + ITEMS_PER_PAGE;
    if (end > productCount) end = productCount;

    int totalPages = (productCount + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                   DANH SACH HANG HOA                                   ║\n");
    printf("╠═════╤══════════╤═══════════════════════════╤════════╤═════════╤═══════════╤════════════╣\n");
    printf("║ STT │ Ma hang  │ Ten hang                  │ Don vi │ So luong│    Gia    │ Trang thai ║\n");
    printf("╠═════╪══════════╪═══════════════════════════╪════════╪═════════╪═══════════╪════════════╣\n");

    for (int i = start; i < end; i++) {
        printf("║ %3d │ %-8s │ %-25s │ %-6s │ %7d │ %9d │ %-9s  ║\n",
               i + 1, products[i].id, products[i].name, 
               products[i].unit, products[i].qty, products[i].price, getStatusShortText(products[i].status));
    }

    printf("╠═════╧══════════╧═══════════════════════════╧════════╧═════════╧═══════════╧════════════╣\n");
    printf("║ Trang %2d/%-2d %38s Tong: %2d san pham %8s          ║\n", 
           page, totalPages, "", productCount, "");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════╝\n");
}

void viewProducts() {
    if (productCount == 0) {
        printf("\n>>> Danh sach hang hoa rong\n\n");
        return;
    }

    int page = 1;
    int totalPages = (productCount + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

    while (1) {
        displayPage(page);
        
        int newPage = inputInt("\nNhap trang (0 de thoat): ", 0, 1); 	
        
        if (newPage == 0) {
            printf("\nDa thoat che do xem trang.\n\n");
            break;
        }
        
        if (newPage > 0 && newPage <= totalPages) {
            page = newPage;
        } else {
            printf("So trang phai tu 1 den %d!\n", totalPages);
        }
    }
}

void sortProducts() {
    if (productCount == 0) {
        printf("\n>>> Danh sach hang hoa rong\n\n");
        return;
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║             SAP XEP DANH SACH          ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║ 1. Sap xep theo ten (A -> Z)           ║\n");
    printf("║ 2. Sap xep theo so luong (tang)        ║\n");
    printf("╚════════════════════════════════════════╝\n");

    int choice = inputInt("Lua chon cua ban: ", 0, 1); 	

    for (int i = 0; i < productCount - 1; i++) {
        for (int j = 0; j < productCount - i - 1; j++) {
            int swap = 0;

            if (choice == 1) {
                char n1[50], n2[50];
                toLower(products[j].name, n1);
                toLower(products[j + 1].name, n2);
                if (strcmp(n1, n2) > 0) swap = 1;
            } else if (choice == 2) {
                if (products[j].qty > products[j + 1].qty) swap = 1;
            } 

            if (swap) {
                Product temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }

    if (choice == 1 || choice == 2) {
        printf("\nDa sap xep danh sach thanh cong\n");
        viewProducts();
    } else {
        printf("\n>>> Lua chon khong hop le!\n\n");
    }
}

void transaction() {
    char id[10];
    inputStr("\nMa hang: ", id, sizeof(id), 0);

    int idx = findProduct(id);
    if (idx == -1) {
        printf("Vat tu <%s> khong ton tai trong danh sach\n\n", id);
        return;
    }

    if (products[idx].status == 0) {
        printf("Vat tu <%s> dang bi khoa, khong the thuc hien giao dich\n\n", id);
        return;
    }

    printf("╔═══════════════════════════════════╗\n");
    printf("║         CHON LOAI GIAO DICH       ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║ 1. Nhap kho                       ║\n");
    printf("║ 2. Xuat kho                       ║\n");
    printf("╚═══════════════════════════════════╝\n");
    
    int type = inputInt("Lua chon: ", 0, 1); 	
    
    if (type != 1 && type != 2) {
        printf("Lua chon khong hop le!\n\n");
        return;
    }

    int qty = inputInt("So luong: ", 0, 0); 	
    
    if (type == 2 && qty > products[idx].qty) {
        printf("So luong xuat hang hoa <%s> vuot qua so luong hien co!\n", id);
        printf("Giao dich that bai, thong tin hang hoa khong thay doi\n\n");
        return;
    }

    if (transactionCount < MAX_TRANSACTIONS) {
        strcpy(transactions[transactionCount].productId, id);
        transactions[transactionCount].qty = qty;
        
        if (type == 1) {
            transactions[transactionCount].type = 'I';
        } else {
            transactions[transactionCount].type = 'O';
        }
        
        transactions[transactionCount].time = time(NULL);
        transactionCount++;
    }

    if (type == 1) {
        products[idx].qty += qty;
        printf("\nNhap hang hoa <%s> thanh cong (Ton: %d)\n\n", id, products[idx].qty);
    } else {
        products[idx].qty -= qty;
        printf("\nXuat hang hoa <%s> thanh cong (Ton: %d)\n\n", id, products[idx].qty);
    }
}

void displayHistoryPage(const char *id, int page, int totalTransactions, int *transIndices) {
    int start = (page - 1) * ITEMS_PER_PAGE;
    int end = start + ITEMS_PER_PAGE;
    if (end > totalTransactions) end = totalTransactions;

    int totalPages = (totalTransactions + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                             LICH SU GIAO DICH - %-8s                       ║\n", id);
    printf("╠═════╤═══════════════════════╤══════════════╤═══════════════════════════════════╣\n");
    printf("║ STT │    Thoi gian          │    Loai      │             So luong              ║\n");
    printf("╠═════╪═══════════════════════╪══════════════╪═══════════════════════════════════╣\n");

    for (int i = start; i < end; i++) {
        int idx = transIndices[i];
        char timeStr[20];
        const char *type_text;
        
        if (transactions[idx].type == 'I') {
            type_text = "Nhap kho";
        } else {
            type_text = "Xuat kho";
        }
        
        strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M", localtime(&transactions[idx].time));
        
        printf("║ %3d │ %-21s │ %-12s │ %33d ║\n",
               i + 1, timeStr, type_text, transactions[idx].qty);
    }

    printf("╠═════╧═══════════════════════╧══════════════╧═══════════════════════════════════╣\n");
    printf("║ Trang %2d/%-2d %32s Tong: %2d giao dich %8s       ║\n", 
           page, totalPages, "", totalTransactions, "");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
}

void viewHistory() {
    char id[10];
    inputStr("\nMa hang: ", id, sizeof(id), 0);

    int transIndices[MAX_TRANSACTIONS];
    int totalTransactions = 0;
    
    for (int i = 0; i < transactionCount; i++) {
        if (strcmp(transactions[i].productId, id) == 0) {
            transIndices[totalTransactions++] = i;
        }
    }

    if (totalTransactions == 0) {
        printf("\n");
        printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                               LICH SU GIAO DICH - %-8s                     ║\n", id);
        printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
        printf("║                  Vat tu <%s> chua co giao dich nhap/xuat                    ║\n", id);
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n\n");
        return;
    }

    int page = 1;
    int totalPages = (totalTransactions + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

    while (1) {
        displayHistoryPage(id, page, totalTransactions, transIndices);
        
        int newPage = inputInt("\nNhap trang (0 de thoat): ", 0, 1); 	
        
        if (newPage == 0) {
            printf("\nDa thoat che do xem lich su.\n\n");
            break;
        }
        
        if (newPage > 0 && newPage <= totalPages) {
            page = newPage;
        } else {
            printf("So trang phai tu 1 den %d!\n", totalPages);
        }
    }
}

void mainMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║        QUAN LY CUA HANG VAT TU         ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║  1. Quan ly san pham                   ║\n");
    printf("║  2. Quan ly giao dich                  ║\n");
    printf("║  0. Thoat chuong trinh                 ║\n");
    printf("╚════════════════════════════════════════╝\n");
}

void firstMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║            QUAN LY SAN PHAM            ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║  1. Them san pham                      ║\n");
    printf("║  2. Cap nhat thong tin                 ║\n");
    printf("║  3. Khoa / Mo khoa san pham            ║\n");
    printf("║  4. Tim kiem                           ║\n");
    printf("║  5. Xem danh sach (phan trang)         ║\n");
    printf("║  6. Sap xep                            ║\n");
    printf("║  0. Quay lai                           ║\n");
    printf("╚════════════════════════════════════════╝\n");
}

void secondMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║           QUAN LY GIAO DICH            ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║  1. Nhap kho / Xuat kho                ║\n");
    printf("║  2. Xem lich su giao dich              ║\n");
    printf("║  0. Quay lai                           ║\n");
    printf("╚════════════════════════════════════════╝\n");
}

int main() {
    initDefaultProducts();
    
    int choice;

    do {
        mainMenu();
        choice = inputInt("Chon chuc nang: ", 0, 1); 	

        if (choice == 0) {
            printf("\nCam on ban da su dung chuong trinh!\n");
            printf("Chuc mot ngay tot lanh!\n\n");
        } 
        else if (choice == 1) {
            int sub;
            do {
                firstMenu();
                sub = inputInt("Chon chuc nang: ", 0, 1); 

                if (sub == 1) {
                    addProduct();
                } else if (sub == 2) {
                    updateProduct();
                } else if (sub == 3) {
                    toggleStatus();
                } else if (sub == 4) {
                    searchProduct();
                } else if (sub == 5) {
                    viewProducts();
                } else if (sub == 6) {
                    sortProducts();
                } else if (sub == 0) {
                    printf("Da quay lai menu chinh.\n");
                } else {
                    printf("\n>>> Lua chon khong hop le!\n\n");
                }

            } while (sub != 0);
        } 
        else if (choice == 2) {
            int sub;
            do {
                secondMenu();
                sub = inputInt("Chon chuc nang: ", 0, 1); 	

                if (sub == 1) {
                    transaction();
                } else if (sub == 2) {
                    viewHistory();
                } else if (sub == 0) {
                    printf("Da quay lai menu chinh.\n");
                } else {
                    printf("\n>>> Lua chon khong hop le!\n\n");
                }

            } while (sub != 0);
        } 
        else {
            printf("\n>>> Lua chon khong hop le!\n\n");
        }

    } while (choice != 0);

    return 0;
}