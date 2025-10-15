# Advanced CLI Billing Software in C

This is a comprehensive command-line billing and invoice management system written in C. It allows users to generate detailed invoices, search for past invoices, and manage a persistent product database.

---

## Features ✨

-   **Persistent Product Database**: The product catalog is saved to `products.csv`, so your data is never lost.
-   **Full Product Management**: A dedicated menu to **add**, **delete**, and **view** all products in the catalog.
-   **Dynamic Memory**: Handles any number of products without a fixed limit, thanks to dynamic memory allocation.
-   **Invoice Generation**: Create detailed invoices for customers by selecting items from the product catalog.
-   **Search Functionality**: Easily find and display previously generated invoices by searching for the customer's name.
-   **Menu-Driven Interface**: A clean and user-friendly command-line interface for easy navigation.

---

## How It Works

The application's core is its product management system, which uses a `products.csv` file as its database.

1.  **On Startup**: The program reads `products.csv` and loads the product catalog into memory. If the file doesn't exist, it starts fresh.
2.  **Product Management**: Any changes made via the "Manage Products" menu (adding or deleting) are instantly saved back to `products.csv`.
3.  **Invoice Creation**: When generating an invoice, the system displays the current list of products for you to select from. Each invoice is then saved as a separate text file (e.g., `Invoice_Hasan_Raza.txt`).

---

## Prerequisites

To compile and run this project, you will need:
-   A C compiler, such as **GCC** (GNU Compiler Collection).

---

## How to Compile and Run

1.  **Save the Code**: Save the source code into a file named `billing.c`.

2.  **Open a Terminal**: Navigate to the directory where you saved the file.

3.  **Compile the Program**: Run the following command to compile the code. This will create an executable file.
    ```bash
    gcc billing.c -o billing_app
    ```

4.  **Run the Application**: Execute the compiled program from your terminal:
    ```bash
    ./billing_app
    ```
    On Windows, you might just run:
    ```bash
    billing_app.exe
    ```

---

## Important First Steps ⚠️

When you run the program for the first time, the product catalog will be empty. **You must add products before you can generate an invoice.**

1.  Start the program.
2.  Choose option `3. Manage Products` from the main menu.
3.  Choose option `1. Add New Product` to add items to your catalog.
4.  Return to the main menu. You can now successfully generate invoices!