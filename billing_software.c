#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define PRODUCT_FILE "products.csv" // File to store the product catalog

// --- Structures ---
typedef struct {
    int id;
    char name[50];
    float price;
} Product;

typedef struct {
    char name[50];
    float price;
    int qty;
} Item;

typedef struct {
    char customer[100];
    char date[50];
    int numOfItems;
    Item items[50];
} Invoice;

// --- Global Variables for Dynamic Product Catalog ---
Product *productCatalog = NULL;
int numProducts = 0;
int catalogCapacity = 0;

// --- Function Prototypes ---
void loadProductsFromFile();
void saveProductsToFile();
void manageProducts();
void addProduct();
void deleteProduct();
void displayProductCatalog();
void generateNewInvoice();
void searchForInvoice();
void clearInputBuffer();
void generateFilename(const char* input, char* output);

// --- Main Function ---
int main() {
    loadProductsFromFile(); // Load the catalog from the file at the start
    int choice;
    while (1) {
        system("clear || cls");
        printf("\n\t============ ABC SUPERMARKET BILLING ============\n");
        printf("\n\t\t\t1. Generate New Invoice");
        printf("\n\t\t\t2. Search for Invoice");
        printf("\n\t\t\t3. Manage Products");
        printf("\n\t\t\t4. Exit");
        printf("\n\n\t\t\t-----------------------\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            choice = 0; // Invalid input
        }
        clearInputBuffer(); 

        switch (choice) {
            case 1: generateNewInvoice(); break;
            case 2: searchForInvoice(); break;
            case 3: manageProducts(); break;
            case 4:
                printf("Exiting the program. Goodbye!\n");
                free(productCatalog); // Free allocated memory before exit
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
    return 0;
}

// --- Product Management Functions ---

void manageProducts() {
    int choice;
    while(1) {
        system("clear || cls");
        printf("\n\t--- Product Management ---\n");
        printf("\t1. Add New Product\n");
        printf("\t2. Delete Product\n");
        printf("\t3. View All Products\n");
        printf("\t4. Return to Main Menu\n");
        printf("\t--------------------------\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            choice = 0;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addProduct(); break;
            case 2: deleteProduct(); break;
            case 3: displayProductCatalog(); break;
            case 4: return; // Go back to the main menu
            default: printf("Invalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

void addProduct() {
    // Check if we need to resize the dynamic array
    if (numProducts >= catalogCapacity) {
        catalogCapacity = (catalogCapacity == 0) ? 10 : catalogCapacity * 2; // Start with 10, then double
        Product *temp = realloc(productCatalog, catalogCapacity * sizeof(Product));
        if (temp == NULL) {
            printf("Error: Memory allocation failed!\n");
            return;
        }
        productCatalog = temp;
    }

    Product newProduct;
    // Generate a new unique ID
    int maxId = 0;
    for (int i = 0; i < numProducts; i++) {
        if (productCatalog[i].id > maxId) {
            maxId = productCatalog[i].id;
        }
    }
    newProduct.id = maxId + 1;

    printf("Enter new product name: ");
    fgets(newProduct.name, 50, stdin);
    newProduct.name[strcspn(newProduct.name, "\n")] = 0;

    printf("Enter product price: ");
    scanf("%f", &newProduct.price);
    clearInputBuffer();

    productCatalog[numProducts] = newProduct;
    numProducts++;
    saveProductsToFile(); // Immediately save the change
    printf("Product '%s' added successfully with ID %d!\n", newProduct.name, newProduct.id);
}

void deleteProduct() {
    displayProductCatalog();
    if (numProducts == 0) return;

    int idToDelete;
    printf("\nEnter the ID of the product to delete: ");
    scanf("%d", &idToDelete);
    clearInputBuffer();

    int foundIndex = -1;
    for (int i = 0; i < numProducts; i++) {
        if (productCatalog[i].id == idToDelete) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Error: Product with ID %d not found.\n", idToDelete);
    } else {
        printf("Product '%s' has been deleted.\n", productCatalog[foundIndex].name);
        // Shift elements to the left to fill the gap
        for (int i = foundIndex; i < numProducts - 1; i++) {
            productCatalog[i] = productCatalog[i + 1];
        }
        numProducts--;
        saveProductsToFile(); // Save changes
    }
}

// --- File I/O for Products ---

void loadProductsFromFile() {
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (fp == NULL) {
        // File doesn't exist, which is fine. Start with an empty catalog.
        return;
    }
    
    // Read products from CSV format: id,name,price
    int id;
    char name[50];
    float price;
    while (fscanf(fp, "%d,%49[^,],%f\n", &id, name, &price) == 3) {
        if (numProducts >= catalogCapacity) {
            catalogCapacity = (catalogCapacity == 0) ? 10 : catalogCapacity * 2;
            productCatalog = realloc(productCatalog, catalogCapacity * sizeof(Product));
        }
        productCatalog[numProducts].id = id;
        strcpy(productCatalog[numProducts].name, name);
        productCatalog[numProducts].price = price;
        numProducts++;
    }
    fclose(fp);
}

void saveProductsToFile() {
    FILE *fp = fopen(PRODUCT_FILE, "w");
    if (fp == NULL) {
        printf("CRITICAL ERROR: Could not save product data!\n");
        return;
    }
    for (int i = 0; i < numProducts; i++) {
        fprintf(fp, "%d,%s,%.2f\n", productCatalog[i].id, productCatalog[i].name, productCatalog[i].price);
    }
    fclose(fp);
}

// --- Invoice and Utility Functions (largely unchanged, but updated) ---

void displayProductCatalog() {
    printf("\n--- Available Products ---\n");
    if (numProducts == 0) {
        printf("No products in the catalog. Please add some first.\n");
        return;
    }
    printf("ID\tName\t\tPrice\n");
    printf("----------------------------------\n");
    for (int i = 0; i < numProducts; i++) {
        printf("%d\t%-15s\t$%.2f\n", productCatalog[i].id, productCatalog[i].name, productCatalog[i].price);
    }
    printf("----------------------------------\n");
}

void generateNewInvoice() {
    // (This function remains mostly the same as the previous version)
    Invoice inv;
    float total = 0;

    printf("\n--- Generate New Invoice ---\n");
    printf("Enter customer name: ");
    fgets(inv.customer, 100, stdin);
    inv.customer[strcspn(inv.customer, "\n")] = 0;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(inv.date, sizeof(inv.date), "%Y-%m-%d", tm);
    
    if (numProducts == 0) {
        printf("\nCannot generate invoice. No products available in the catalog.\n");
        return;
    }
    displayProductCatalog();

    printf("\nHow many unique items to bill? ");
    scanf("%d", &inv.numOfItems);
    clearInputBuffer();

    for (int i = 0; i < inv.numOfItems; i++) {
        int productId;
        printf("\n--- Item %d ---\n", i + 1);
        printf("Enter product ID: ");
        scanf("%d", &productId);
        clearInputBuffer();

        int found = 0;
        for (int j = 0; j < numProducts; j++) {
            if (productCatalog[j].id == productId) {
                strcpy(inv.items[i].name, productCatalog[j].name);
                inv.items[i].price = productCatalog[j].price;
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Invalid Product ID. Please try again.\n");
            i--; continue;
        }

        printf("Enter quantity for %s: ", inv.items[i].name);
        scanf("%d", &inv.items[i].qty);
        clearInputBuffer();
        total += inv.items[i].qty * inv.items[i].price;
    }

    char filename[120];
    generateFilename(inv.customer, filename);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) { printf("Error: Could not save invoice.\n"); return; }
    
    fprintf(fp, "\t\t\tABC SUPERMARKET\n");
    fprintf(fp, "Date: %s\nInvoice To: %s\n", inv.date, inv.customer);
    fprintf(fp, "--------------------------------------------------\n");
    fprintf(fp, "%-20s\tQty\t\tTotal\n", "Items");
    fprintf(fp, "--------------------------------------------------\n");
    for (int i = 0; i < inv.numOfItems; i++) {
        fprintf(fp, "%-20s\t%d\t\t$%.2f\n", inv.items[i].name, inv.items[i].qty, inv.items[i].qty * inv.items[i].price);
    }
    fprintf(fp, "--------------------------------------------------\n");
    fprintf(fp, "Sub Total\t\t\t\t$%.2f\n", total);
    float discount = 0.10 * total;
    fprintf(fp, "Discount @10%%\t\t\t\t$%.2f\n", discount);
    fprintf(fp, "\t\t\t\t\t----------\n");
    float grandTotal = total - discount;
    fprintf(fp, "Grand Total\t\t\t\t$%.2f\n", grandTotal);
    fprintf(fp, "\t\t\t\t\t----------\n");
    fclose(fp);
    
    printf("\nInvoice generated and saved as %s\n", filename);
}

void searchForInvoice() {
    // (This function is unchanged)
    char customerName[100];
    char filename[120];
    
    printf("\n--- Search for Invoice ---\n");
    printf("Enter customer name to search: ");
    fgets(customerName, 100, stdin);
    customerName[strcspn(customerName, "\n")] = 0;

    generateFilename(customerName, filename);
    
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Invoice for '%s' not found.\n", customerName);
        return;
    }

    printf("\n--- Invoice Found ---\n\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }
    fclose(fp);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void generateFilename(const char* input, char* output) {
    sprintf(output, "Invoice_%s.txt", input);
    for (int i = 0; output[i] != '\0'; i++) {
        if (isspace(output[i])) {
            output[i] = '_';
        }
    }
}