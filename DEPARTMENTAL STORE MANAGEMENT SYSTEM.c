#include <stdio.h>
#include <string.h>

#define MAX 100

struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
};

struct Product p[MAX];
int count = 0;

// Load from File//



//Untill you delete the product using code the product will be save in the device//

void loadFromFile()
{
    FILE *fp = fopen("products.txt", "r");

    if (fp == NULL)
    {
        printf("No existing data file found. Starting fresh!\n");
        return;
    }

    count = 0;
    while (fscanf(fp, "%d %s %f %d",
                  &p[count].id,
                  p[count].name,
                  &p[count].price,
                  &p[count].quantity)==4)
    {
        count++;
        if (count >= MAX)  // Important:  Prevent buffer overflow//
        {
            printf("Maximum product limit reached while loading!\n");
            break;
        }
    }

    fclose(fp);
    printf("Loaded %d products from file!\n", count);
}

// Add Product//


void addProduct()
{
    if (count >= MAX)
    {
        printf("Product storage is full!\n");
        return;
    }

    printf("\nEnter Product ID: ");
    scanf("%d", &p[count].id);


    for (int i = 0; i < count; i++)
    {
        if (p[i].id == p[count].id)
        {
            printf("Product ID already exists!\n");
            return;
        }
    }

    printf("Enter Product Name: ");
    scanf("%s", p[count].name);

    printf("Enter Price: ");
    scanf("%f", &p[count].price);

    printf("Enter Quantity: ");
    scanf("%d", &p[count].quantity);

    count++;
    printf("Product Added Successfully!\n");
}


    //Search product//


void searchProduct()
{
    int id, found = 0;

    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (p[i].id == id)
        {
            printf("\n--- Product Found ---\n");
            printf("ID: %d\n", p[i].id);
            printf("Name: %s\n", p[i].name);
            printf("Price: %.2f\n", p[i].price);
            printf("Quantity: %d\n", p[i].quantity);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Product not found!\n");
    }
}

// Display Products//

void displayProducts()
{
    if (count == 0)
    {
        printf("\nNo products in inventory!\n");
        return;
    }

    printf("\n--- Product List ---\n");
    for (int i = 0; i < count; i++)
    {
        printf("ID: %d | Name: %s | Price: %.2f | Qty: %d\n",
               p[i].id, p[i].name, p[i].price, p[i].quantity);
    }
}

// Update Stock//

void updateStock()
{
    int id, choice, found = 0;

    printf("Enter Product ID to update: ");
    scanf("%d", &id);

    printf("\n");

    for (int i = 0; i < count; i++)
    {
        if (p[i].id == id)
        {
            printf("1. Update Quantity\n");
            printf("2. Update Price\n");
            printf("Enter choice: ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                printf("Enter new quantity: ");
                scanf("%d", &p[i].quantity);
                printf("Quantity Updated!\n");
            }
            else if (choice == 2)
            {
                printf("Enter new price: ");
                scanf("%f", &p[i].price);
                printf("Price Updated!\n");
            }
            else
            {
                printf("Invalid choice!\n");
            }

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Product not found!\n");
}

        //Delete product//

void deleteProduct()
{
    int id, found = 0;

    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (p[i].id == id)
        {
            for (int j = i; j < count - 1; j++)
            {
                p[j] = p[j + 1];
            }

            count--;
            found = 1;

            printf("Product Deleted Successfully!\n");
            break;
        }
    }

    if (!found)
    {
        printf("Product not found!\n");
    }
}

// Generate Bill//

void generateBill()
{
    int id, qty;
    float total = 0.0;

    printf("\nEnter Product ID: ");
    scanf("%d", &id);

    printf("Enter Quantity: ");
    scanf("%d", &qty);

    for (int i = 0; i < count; i++)
    {
        if (p[i].id == id)
        {
            if (p[i].quantity >= qty)
            {
                float cost = qty * p[i].price;
                total += cost;
                p[i].quantity -= qty;

                printf("\n--- BILL ---\n");
                printf("Product: %s\n", p[i].name);
                printf("Quantity: %d\n", qty);
                printf("Total Price: %.2f\n", cost);
                printf("Remaining Stock: %d\n", p[i].quantity);
            }
            else
            {
                printf("Not enough stock! Available: %d\n", p[i].quantity);
            }
            return;
        }
    }
    printf("Product not found!\n");
}

// Save to File//


void saveToFile()
{
    FILE *fp = fopen("products.txt", "w");

    if (fp == NULL)
    {
        printf("Error saving to file!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d %s %.2f %d\n",
                p[i].id, p[i].name, p[i].price, p[i].quantity);
    }

    fclose(fp);
    printf("Data Saved to File!\n");
}

// Menu//

int main()
{
    int choice;

    loadFromFile();

    while (1)
    {
        printf("\n===== Department Store Menu =====\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Update Stock\n");
        printf("4. Generate Bill\n");
        printf("5. Save to File\n");
        printf("6. Search Product\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            displayProducts();
            break;
        case 3:
            updateStock();
            break;
        case 4:
            generateBill();
            break;
        case 5:
            saveToFile();
            break;
        case 6:
            searchProduct();
            break;
        case 7:
            deleteProduct();
            break;
        case 8:

            saveToFile();
            printf("Thank you for using the system!\n");
            return 0;
        default:
            printf("Invalid choice!\n");
        }
    }
}



