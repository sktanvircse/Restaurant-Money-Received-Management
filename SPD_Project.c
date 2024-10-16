#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items
{
    char item[20];
    float price;
    int qty;
};

struct orders
{
    char customer[50];
    char date[50];
    int numOfItems;
    struct items itm[50];
};

void saveFoodItemsToFile(char foodItems[5][20], float foodPrices[5])
{
    FILE *fp = fopen("foodItems.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        fprintf(fp, "%s %f\n", foodItems[i], foodPrices[i]);
    }

    fclose(fp);
}

void readFoodItemsFromFile(char foodItems[5][20], float foodPrices[5])
{
    FILE *fp = fopen("foodItems.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file for reading!\n");
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%s %f", foodItems[i], &foodPrices[i]);
    }

    fclose(fp);
}

// functions to generate biils
void generateBillHeader(char name[50], char date[30])
{
    printf("\n\n");
    printf("\tFifth Friend's Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate:%s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------");
    printf("\n\n");
}
void generateBillBody(char item[30], int qty, float price)
{
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void generateBillFooter(float total)
{
    printf("\n");
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float VAT = netTotal * 0.15;
    float grandTotal = netTotal + VAT;
    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", dis);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nVAT @15%s\t\t\t%.2f", "%", VAT);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}
int main()
{

    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill = 'y', contFlag = 'y';
    char name[50];
    float discountPercentage;

    FILE *fp;

    // Food items with prices
    char foodItems[5][20] = {"Burger", "Pizza", "Pasta", "Samosa", "Water"};
    float foodPrices[5] = {120, 480, 350, 120, 25};

    // Save food items and prices to file
    saveFoodItemsToFile(foodItems, foodPrices);

    // Read food items and prices from file
    readFoodItemsFromFile(foodItems, foodPrices);

    while (contFlag == 'y')
    {
        system("cls");
        float total = 0;
        int invoiceFound = 0;
        printf("\t============Fifth Friend's RESTAURANT============");
        printf("\n\nPlease select your prefered operation");
        printf("\n\n1.Generate Invoice  (Custom Items) ");
        printf("\n2.Show all Invoices");
        printf("\n3.Search Invoice");
        printf("\n4.Generate Invoice (Predefined Food Items)");
        printf("\n5.Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &opt);
        fgetc(stdin);
        switch (opt)
        {
        case 1:
            system("cls");
            printf("\nPlease enter the name of the customer:\t");
            fgets(ord.customer, 50, stdin);
            ord.customer[strlen(ord.customer) - 1] = 0;
            strcpy(ord.date, __DATE__);
            printf("\nPlease enter the number of items:\t");
            scanf("%d", &n);
            ord.numOfItems = n;
            for (int i = 0; i < n; i++)
            {
                fgetc(stdin);
                printf("\n\n");
                printf("Please enter the item %d:\t", i + 1);
                fgets(ord.itm[i].item, 20, stdin);
                ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0;
                printf("Please enter the quantity:\t");
                scanf("%d", &ord.itm[i].qty);
                printf("Please enter the unit price:\t");
                scanf("%f", &ord.itm[i].price);
                total += ord.itm[i].qty * ord.itm[i].price;
            }

            generateBillHeader(ord.customer, ord.date);
            for (int i = 0; i < ord.numOfItems; i++)
            {
                generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
            }
            generateBillFooter(total);

            printf("\nDo you want to save the invoice [y/n]:\t");
            scanf("%s", &saveBill);

            if (saveBill == 'y')
            {
                fp = fopen("RestaurantBill.dat", "a+");
                fwrite(&ord, sizeof(struct orders), 1, fp);
                if (fwrite != 0)
                    printf("\nSuccessfully saved");
                else
                    printf("\nError saving");
                fclose(fp);
            }
            break;

        case 2:
            system("cls");
            fp = fopen("RestaurantBill.dat", "r");
            printf("\n  *****Your Previous Invoices*****\n");
            while (fread(&order, sizeof(struct orders), 1, fp))
            {
                float tot = 0;
                generateBillHeader(order.customer, order.date);
                for (int i = 0; i < order.numOfItems; i++)
                {
                    generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                    tot += order.itm[i].qty * order.itm[i].price;
                }
                generateBillFooter(tot);
            }
            fclose(fp);
            break;

        case 3:
            printf("Enter the name of the customer:\t");
            // fgetc(stdin);
            fgets(name, 50, stdin);
            name[strlen(name) - 1] = 0;
            system("cls");
            fp = fopen("RestaurantBill.dat", "r");
            printf("\t*****Invoice of %s*****", name);
            while (fread(&order, sizeof(struct orders), 1, fp))
            {
                float tot = 0;
                if (!strcmp(order.customer, name))
                {
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++)
                    {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                    invoiceFound = 1;
                }
            }
            if (!invoiceFound)
            {
                printf("Sorry the invoice for %s doesnot exists", name);
            }
            fclose(fp);
            break;

        case 5:
            printf("\n\t\t Bye Bye :)\n\n");
            exit(0);
            break;

        case 4:
            system("cls");
            // Read food items and prices from file
            readFoodItemsFromFile(foodItems, foodPrices);

            printf("\nPlease enter the name of the customer:\t");
            fgets(ord.customer, 50, stdin);
            ord.customer[strlen(ord.customer) - 1] = 0;
            strcpy(ord.date, __DATE__);
            printf("\nPlease select the food items and enter their quantities:\n");

            printf("\nNo\tItem\t\tPrice\n");
            for (int i = 0; i < 5; i++)
            {
                printf("%d\t%s\t\t%.2f\n", i + 1, foodItems[i], foodPrices[i]);
            }

            printf("\nEnter the number of items you want to order: ");
            scanf("%d", &n);
            ord.numOfItems = n;
            for (int i = 0; i < n; i++)
            {
                printf("Enter the item number: ");
                int itemNumber;
                scanf("%d", &itemNumber);
                strcpy(ord.itm[i].item, foodItems[itemNumber - 1]);
                ord.itm[i].price = foodPrices[itemNumber - 1];
                printf("Enter the quantity: ");
                scanf("%d", &ord.itm[i].qty);
                total += ord.itm[i].qty * ord.itm[i].price;
            }

            generateBillHeader(ord.customer, ord.date);
            for (int i = 0; i < ord.numOfItems; i++)
            {
                generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
            }
            generateBillFooter(total);

            printf("\nDo you want to save the invoice [y/n]:\t");
            scanf("%s", &saveBill);

            if (saveBill == 'y')
            {
                fp = fopen("RestaurantBill.dat", "a+");
                fwrite(&ord, sizeof(struct orders), 1, fp);
                if (fwrite != 0)
                    printf("\nSuccessfully saved");
                else
                    printf("\nError saving");
                fclose(fp);
            }
            break;

        default:
            printf("Sorry invalid option");
            break;
        }
        printf("\nDo you want to perform another operation?[y/n]:\t");
        scanf("%s", &contFlag);
    }
    printf("\n\t\t Bye Bye :)\n\n");
    printf("\n\n");

    return 0;
}
