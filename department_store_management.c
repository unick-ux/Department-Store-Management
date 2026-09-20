#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_PRODUCTS 100
#define MAX_CART_ITEMS 100
#define MAX_NAME_LENGTH 50
#define MAX_CUSTOMER_NAME 100
#define LOW_STOCK_LIMIT 5

#define PRODUCT_FILE "products.txt"
#define SALES_FILE "sales.txt"

struct Product
{
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
};

struct CartItem
{
    int productId;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
    float total;
};

struct Product products[MAX_PRODUCTS];
struct CartItem cart[MAX_CART_ITEMS];

int productCount = 0;
int cartCount = 0;

float totalSales = 0.0;
int billNumber = 1001;


// Utility functions //

void clearInputBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void pauseScreen()
{
    printf("\nPress Enter to continue...");
    getchar();
}

void printLine(int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("-");
    }

    printf("\n");
}

void getString(char *str, int size)
{
    if (fgets(str, size, stdin) != NULL)
    {
        str[strcspn(str, "\n")] = '\0';
    }
}

int getInteger(const char *message)
{
    int value;

    while (1)
    {
        printf("%s", message);

        if (scanf("%d", &value) == 1)
        {
            clearInputBuffer();
            return value;
        }

        printf("Invalid input! Please enter a number.\n");
        clearInputBuffer();
    }
}

float getFloat(const char *message)
{
    float value;

    while (1)
    {
        printf("%s", message);

        if (scanf("%f", &value) == 1)
        {
            clearInputBuffer();
            return value;
        }

        printf("Invalid input! Please enter a valid number.\n");
        clearInputBuffer();
    }
}


// File handling //

void loadProducts()
{
    FILE *fp = fopen(PRODUCT_FILE, "r");

    if (fp == NULL)
    {
        printf("No existing product file found.\n");
        printf("Starting with an empty inventory.\n");
        return;
    }

    productCount = 0;

    while (productCount < MAX_PRODUCTS &&
           fscanf(fp, "%d|%49[^|]|%f|%d\n",
                  &products[productCount].id,
                  products[productCount].name,
                  &products[productCount].price,
                  &products[productCount].quantity) == 4)
    {
        productCount++;
    }

    fclose(fp);

    printf("%d product(s) loaded successfully.\n", productCount);
}

void saveProducts()
{
    FILE *fp = fopen(PRODUCT_FILE, "w");

    if (fp == NULL)
    {
        printf("Error: Unable to save product data!\n");
        return;
    }

    for (int i = 0; i < productCount; i++)
    {
        fprintf(fp, "%d|%s|%.2f|%d\n",
                products[i].id,
                products[i].name,
                products[i].price,
                products[i].quantity);
    }

    fclose(fp);
}

void loadSalesData()
{
    FILE *fp = fopen(SALES_FILE, "r");

    if (fp == NULL)
    {
        totalSales = 0.0;
        billNumber = 1001;
        return;
    }

    fscanf(fp, "%f %d", &totalSales, &billNumber);

    fclose(fp);
}

void saveSalesData()
{
    FILE *fp = fopen(SALES_FILE, "w");

    if (fp == NULL)
    {
        printf("Warning: Could not save sales data.\n");
        return;
    }

    fprintf(fp, "%.2f %d\n", totalSales, billNumber);

    fclose(fp);
}


// Product management //

void addProduct()
{
    if (productCount >= MAX_PRODUCTS)
    {
        printf("\nProduct storage is full!\n");
        return;
    }

    int id = getInteger("\nEnter Product ID: ");

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            printf("Product ID already exists!\n");
            return;
        }
    }

    products[productCount].id = id;

    printf("Enter Product Name: ");
    getString(products[productCount].name, MAX_NAME_LENGTH);

    if (strlen(products[productCount].name) == 0)
    {
        printf("Product name cannot be empty!\n");
        return;
    }

    float price = getFloat("Enter Product Price: ");

    if (price < 0)
    {
        printf("Price cannot be negative!\n");
        return;
    }

    int quantity = getInteger("Enter Product Quantity: ");

    if (quantity < 0)
    {
        printf("Quantity cannot be negative!\n");
        return;
    }

    products[productCount].price = price;
    products[productCount].quantity = quantity;

    productCount++;

    saveProducts();

    printf("\nProduct added successfully!\n");
}

void displayProducts()
{
    if (productCount == 0)
    {
        printf("\nNo products available in inventory.\n");
        return;
    }

    printf("\n");
    printLine(80);

    printf("%-8s %-30s %-15s %-10s\n",
           "ID", "PRODUCT NAME", "PRICE", "QUANTITY");

    printLine(80);

    for (int i = 0; i < productCount; i++)
    {
        printf("%-8d %-30s %-15.2f %-10d",
               products[i].id,
               products[i].name,
               products[i].price,
               products[i].quantity);

        if (products[i].quantity <= LOW_STOCK_LIMIT)
        {
            printf("  LOW STOCK");
        }

        printf("\n");
    }

    printLine(80);
}

void searchProduct()
{
    int id = getInteger("\nEnter Product ID to search: ");

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            printf("\n========== PRODUCT DETAILS ==========\n");
            printf("Product ID : %d\n", products[i].id);
            printf("Name       : %s\n", products[i].name);
            printf("Price      : %.2f\n", products[i].price);
            printf("Quantity   : %d\n", products[i].quantity);

            if (products[i].quantity <= LOW_STOCK_LIMIT)
            {
                printf("Status     : LOW STOCK\n");
            }
            else
            {
                printf("Status     : IN STOCK\n");
            }

            printf("=====================================\n");

            return;
        }
    }

    printf("Product not found!\n");
}

void updateProduct()
{
    int id = getInteger("\nEnter Product ID to update: ");

    int index = -1;

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Product not found!\n");
        return;
    }

    printf("\nProduct Found: %s\n", products[index].name);

    printf("\n1. Update Product Name\n");
    printf("2. Update Price\n");
    printf("3. Update Quantity\n");
    printf("4. Update Everything\n");

    int choice = getInteger("Enter your choice: ");

    switch (choice)
    {
        case 1:
            printf("Enter new product name: ");
            getString(products[index].name, MAX_NAME_LENGTH);

            printf("Product name updated successfully!\n");
            break;

        case 2:
        {
            float price = getFloat("Enter new price: ");

            if (price < 0)
            {
                printf("Price cannot be negative!\n");
                return;
            }

            products[index].price = price;

            printf("Price updated successfully!\n");
            break;
        }

        case 3:
        {
            int quantity = getInteger("Enter new quantity: ");

            if (quantity < 0)
            {
                printf("Quantity cannot be negative!\n");
                return;
            }

            products[index].quantity = quantity;

            printf("Quantity updated successfully!\n");
            break;
        }

        case 4:
        {
            printf("Enter new product name: ");
            getString(products[index].name, MAX_NAME_LENGTH);

            float price = getFloat("Enter new price: ");

            if (price < 0)
            {
                printf("Price cannot be negative!\n");
                return;
            }

            int quantity = getInteger("Enter new quantity: ");

            if (quantity < 0)
            {
                printf("Quantity cannot be negative!\n");
                return;
            }

            products[index].price = price;
            products[index].quantity = quantity;

            printf("Product updated successfully!\n");
            break;
        }

        default:
            printf("Invalid choice!\n");
            return;
    }

    saveProducts();
}

void deleteProduct()
{
    int id = getInteger("\nEnter Product ID to delete: ");

    int index = -1;

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Product not found!\n");
        return;
    }

    printf("\nProduct: %s\n", products[index].name);

    printf("Are you sure you want to delete this product?\n");
    printf("1. Yes\n");
    printf("2. No\n");

    int choice = getInteger("Enter choice: ");

    if (choice != 1)
    {
        printf("Deletion cancelled.\n");
        return;
    }

    for (int i = index; i < productCount - 1; i++)
    {
        products[i] = products[i + 1];
    }

    productCount--;

    saveProducts();

    printf("Product deleted successfully!\n");
}


// Inventory //

void checkLowStock()
{
    int found = 0;

    printf("\n========== LOW STOCK PRODUCTS ==========\n");

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].quantity <= LOW_STOCK_LIMIT)
        {
            printf("ID: %d | %-25s | Stock: %d\n",
                   products[i].id,
                   products[i].name,
                   products[i].quantity);

            found = 1;
        }
    }

    if (!found)
    {
        printf("No low-stock products.\n");
    }

    printf("========================================\n");
}


// Cart functions //

void addToCart()
{
    if (cartCount >= MAX_CART_ITEMS)
    {
        printf("Cart is full!\n");
        return;
    }

    int id = getInteger("\nEnter Product ID: ");

    int productIndex = -1;

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            productIndex = i;
            break;
        }
    }

    if (productIndex == -1)
    {
        printf("Product not found!\n");
        return;
    }

    int quantity = getInteger("Enter Quantity: ");

    if (quantity <= 0)
    {
        printf("Quantity must be greater than zero.\n");
        return;
    }

    for (int i = 0; i < cartCount; i++)
    {
        if (cart[i].productId == id)
        {
            if (cart[i].quantity + quantity >
                products[productIndex].quantity)
            {
                printf("Not enough stock!\n");
                printf("Available stock: %d\n",
                       products[productIndex].quantity);
                return;
            }

            cart[i].quantity += quantity;

            cart[i].total =
                cart[i].quantity * cart[i].price;

            printf("Cart updated successfully!\n");
            return;
        }
    }

    if (quantity > products[productIndex].quantity)
    {
        printf("Not enough stock!\n");
        printf("Available stock: %d\n",
               products[productIndex].quantity);
        return;
    }

    cart[cartCount].productId =
        products[productIndex].id;

    strcpy(cart[cartCount].name,
           products[productIndex].name);

    cart[cartCount].price =
        products[productIndex].price;

    cart[cartCount].quantity = quantity;

    cart[cartCount].total =
        cart[cartCount].price *
        cart[cartCount].quantity;

    cartCount++;

    printf("Product added to cart successfully!\n");
}

void viewCart()
{
    if (cartCount == 0)
    {
        printf("\nCart is empty.\n");
        return;
    }

    float grandTotal = 0.0;

    printf("\n");
    printLine(90);

    printf("%-8s %-25s %-12s %-10s %-12s\n",
           "ID",
           "PRODUCT",
           "PRICE",
           "QTY",
           "TOTAL");

    printLine(90);

    for (int i = 0; i < cartCount; i++)
    {
        printf("%-8d %-25s %-12.2f %-10d %-12.2f\n",
               cart[i].productId,
               cart[i].name,
               cart[i].price,
               cart[i].quantity,
               cart[i].total);

        grandTotal += cart[i].total;
    }

    printLine(90);

    printf("%-57s %.2f\n",
           "GRAND TOTAL:", grandTotal);
}

void removeFromCart()
{
    if (cartCount == 0)
    {
        printf("\nCart is empty.\n");
        return;
    }

    int id = getInteger("\nEnter Product ID to remove from cart: ");

    int index = -1;

    for (int i = 0; i < cartCount; i++)
    {
        if (cart[i].productId == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Product is not in the cart.\n");
        return;
    }

    for (int i = index; i < cartCount - 1; i++)
    {
        cart[i] = cart[i + 1];
    }

    cartCount--;

    printf("Product removed from cart.\n");
}

void clearCart()
{
    cartCount = 0;

    printf("Cart cleared successfully!\n");
}


// Billing //

void generateBill()
{
    if (cartCount == 0)
    {
        printf("\nCart is empty. Add products first.\n");
        return;
    }

    char customerName[MAX_CUSTOMER_NAME];

    printf("\nEnter Customer Name: ");
    getString(customerName, MAX_CUSTOMER_NAME);

    if (strlen(customerName) == 0)
    {
        strcpy(customerName, "Walk-in Customer");
    }

    float subtotal = 0.0;

    for (int i = 0; i < cartCount; i++)
    {
        subtotal += cart[i].total;
    }

    float discount = 0.0;

    if (subtotal >= 5000.0)
    {
        discount = subtotal * 0.05;
    }

    float afterDiscount = subtotal - discount;

    float tax = afterDiscount * 0.05;

    float grandTotal = afterDiscount + tax;

    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);

    printf("\n\n");

    printLine(70);

    printf("                 DEPARTMENT STORE\n");
    printf("                       BILL\n");

    printLine(70);

    printf("Bill Number : %d\n", billNumber);
    printf("Customer    : %s\n", customerName);

    if (localTime != NULL)
    {
        printf("Date        : %02d-%02d-%04d\n",
               localTime->tm_mday,
               localTime->tm_mon + 1,
               localTime->tm_year + 1900);

        printf("Time        : %02d:%02d:%02d\n",
               localTime->tm_hour,
               localTime->tm_min,
               localTime->tm_sec);
    }

    printLine(70);

    printf("%-5s %-25s %-10s %-8s %-12s\n",
           "No.",
           "Product",
           "Price",
           "Qty",
           "Total");

    printLine(70);

    for (int i = 0; i < cartCount; i++)
    {
        printf("%-5d %-25s %-10.2f %-8d %-12.2f\n",
               i + 1,
               cart[i].name,
               cart[i].price,
               cart[i].quantity,
               cart[i].total);
    }

    printLine(70);

    printf("%-50s %.2f\n",
           "Subtotal:", subtotal);

    printf("%-50s %.2f\n",
           "Discount:", discount);

    printf("%-50s %.2f\n",
           "Tax (5%):", tax);

    printLine(70);

    printf("%-50s %.2f\n",
           "GRAND TOTAL:", grandTotal);

    printLine(70);

    printf("             Thank you for shopping!\n");

    printLine(70);

    for (int i = 0; i < cartCount; i++)
    {
        for (int j = 0; j < productCount; j++)
        {
            if (products[j].id == cart[i].productId)
            {
                products[j].quantity -= cart[i].quantity;
                break;
            }
        }
    }

    totalSales += grandTotal;

    billNumber++;

    saveProducts();
    saveSalesData();

    cartCount = 0;

    printf("\nSale completed successfully!\n");
}


// Sales report //

void salesReport()
{
    printf("\n");

    printLine(50);

    printf("              SALES REPORT\n");

    printLine(50);

    printf("Total Sales       : %.2f\n", totalSales);
    printf("Next Bill Number  : %d\n", billNumber);
    printf("Total Products    : %d\n", productCount);

    printLine(50);

    checkLowStock();
}


// Main menu //

void mainMenu()
{
    while (1)
    {
        printf("\n\n");

        printLine(55);

        printf("        DEPARTMENT STORE MANAGEMENT SYSTEM\n");

        printLine(55);

        printf("1.  Add Product\n");
        printf("2.  Display Products\n");
        printf("3.  Search Product\n");
        printf("4.  Update Product\n");
        printf("5.  Delete Product\n");
        printf("6.  Low Stock Report\n");
        printf("7.  Add Product to Cart\n");
        printf("8.  View Cart\n");
        printf("9.  Remove Product from Cart\n");
        printf("10. Clear Cart\n");
        printf("11. Generate Bill\n");
        printf("12. Sales Report\n");
        printf("13. Save Data\n");
        printf("14. Exit\n");

        printLine(55);

        int choice = getInteger("Enter your choice: ");

        switch (choice)
        {
            case 1:
                addProduct();
                break;

            case 2:
                displayProducts();
                break;

            case 3:
                searchProduct();
                break;

            case 4:
                updateProduct();
                break;

            case 5:
                deleteProduct();
                break;

            case 6:
                checkLowStock();
                break;

            case 7:
                addToCart();
                break;

            case 8:
                viewCart();
                break;

            case 9:
                removeFromCart();
                break;

            case 10:
                clearCart();
                break;

            case 11:
                generateBill();
                break;

            case 12:
                salesReport();
                break;

            case 13:
                saveProducts();
                saveSalesData();

                printf("All data saved successfully!\n");
                break;

            case 14:
                saveProducts();
                saveSalesData();

                printf("\nData saved successfully.\n");
                printf("Thank you for using the Department Store Management System!\n");

                return;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}


// Main function //

int main()
{
    printf("\n");

    printLine(55);

    printf("     WELCOME TO DEPARTMENT STORE SYSTEM\n");

    printLine(55);

    loadProducts();
    loadSalesData();

    mainMenu();

    return 0;
}
