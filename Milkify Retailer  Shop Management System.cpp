#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// ------------------------------ Structs ----------------------------------
struct User {
    string username;
    string password;
    string email;
    string role;
};

struct Product {
    int id;
    string name;
    float price;
    int quantity;
};

struct Order {
    string orderID;
    string customerName;
    string phone;
    string status;
};

struct OrderItem {
    int productId;
    int quantity;
};

// ------------------------- Global Variables -------------------------------
string currentUser = "";
string currentRole = "";

// ------------------------- Helper Functions -------------------------------
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void showHeader() {
    cout << "===========================================\n";
    cout << "         WELCOME TO MILKIFY SHOP           \n";
    cout << "===========================================\n";
}

// ------------------------- User Management -------------------------------
void registerUser(string role) {
    ofstream file("users.txt", ios::app);
    if (!file) {
        cout << "Error opening user file!\n";
        return;
    }

    User user;
    cout << "Enter new username: ";
    cin >> user.username;
    cout << "Enter new password: ";
    cin >> user.password;
    cout << "Enter email: ";
    cin >> user.email;

    user.role = toLowerCase(role);
    file << user.username << " " << user.password << " " << user.email << " " << user.role << endl;
    file.close();
    cout << "Registration successful as " << user.role << "!\n";
}

bool loginUser(string role) {
    ifstream file("users.txt");
    if (!file) {
        cout << "No users registered yet.\n";
        return false;
    }

    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    User user;
    string fileUsername, filePassword, fileEmail, fileRole;
    role = toLowerCase(role);

    while (file >> fileUsername >> filePassword >> fileEmail >> fileRole) {
        if (fileUsername == username && filePassword == password && fileRole == role) {
            currentUser = username;
            currentRole = role;
            cout << "Login successful as " << role << "! Welcome, " << currentUser << "!\n";
            return true;
        }
    }

    file.close();
    cout << "Invalid login credentials or wrong role selected.\n";
    return false;
}

// ------------------------- Product Management -------------------------------
void addProduct() {
    Product p;

    cout << "Enter Product ID: ";
    while (!(cin >> p.id)) {
        cin.clear(); // Clear error flag
        cin.ignore(1000, '\n'); // Discard invalid input
        cout << "Invalid input! Enter a valid Product ID: ";
    }

    cout << "Enter Product Name: ";
    cin >> p.name;

    cout << "Enter Product Price: ";
    while (!(cin >> p.price)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Enter a valid Product Price: ";
    }

    cout << "Enter Product Quantity: ";
    while (!(cin >> p.quantity)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Enter a valid Product Quantity: ";
    }

    ofstream file("products.txt", ios::app);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    file << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    file.close();
    cout << "? Product added successfully!\n";
}


void displayProducts() {
    ifstream file("products.txt");
    Product p;
    cout << "\nID\tName\tPrice\tQuantity\n";
    while (file >> p.id >> p.name >> p.price >> p.quantity) {
        cout << p.id << "\t" << p.name << "\t" << p.price << "\t" << p.quantity << endl;
    }
    file.close();
}

void updateProduct() {
    int id;
    cout << "Enter Product ID to update: ";
    cin >> id;

    ifstream in("products.txt");
    ofstream out("temp.txt");
    Product p;
    bool found = false;

    while (in >> p.id >> p.name >> p.price >> p.quantity) {
        if (p.id == id) {
            found = true;
            cout << "Enter new name: ";
            cin >> p.name;
            cout << "Enter new price: ";
            cin >> p.price;
            cout << "Enter new quantity: ";
            cin >> p.quantity;
        }
        out << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
    }

    in.close(); out.close();
    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (found) cout << "Product updated successfully.\n";
    else cout << "Product ID not found.\n";
}

void deleteProduct() {
    int id;
    cout << "Enter Product ID to delete: ";
    cin >> id;

    ifstream in("products.txt");
    ofstream out("temp.txt");
    Product p;
    bool found = false;

    while (in >> p.id >> p.name >> p.price >> p.quantity) {
        if (p.id != id) {
            out << p.id << " " << p.name << " " << p.price << " " << p.quantity << endl;
        } else {
            found = true;
        }
    }

    in.close(); out.close();
    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (found) cout << "Product deleted successfully.\n";
    else cout << "Product not found.\n";
}

// ------------------------- Order Management -------------------------------
void takeOrder();
void confirmOrder();
void cancelOrder();
void viewOrders();

// ------------------------- Role Menus -------------------------------
void adminMenu() {
    int c;
    do {
        cout << "\n--- Admin Panel ---\n";
        cout << "1. Add Product\n2. View Products\n3. Update Product\n4. Delete Product\n5. Logout\n";
        cout << "Enter choice: ";
        cin >> c;
        switch (c) {
            case 1: addProduct(); break;
            case 2: displayProducts(); break;
            case 3: updateProduct(); break;
            case 4: deleteProduct(); break;
            case 5: return;
            default: cout << "Invalid option.\n";
        }
    } while (true);
}

void agentMenu() {
    int c;
    do {
        cout << "\n--- Agent Panel ---\n";
        cout << "1. Take Order\n2. Confirm Order\n3. Cancel Order\n4. View Orders\n5. Logout\n";
        cout << "Enter choice: ";
        cin >> c;
        switch (c) {
            case 1: takeOrder(); break;
            case 2: confirmOrder(); break;
            case 3: cancelOrder(); break;
            case 4: viewOrders(); break;
            case 5: return;
            default: cout << "Invalid option.\n";
        }
    } while (true);
}
 void printBill(map<int, int>& orderMap, float& totalBill) {
    ifstream in("products.txt");
    Product p;

    cout << "\n------- BILLING DETAILS -------\n";
    cout << "Product\tQty\tPrice\tTotal\tStock Left\n";

    while (in >> p.id >> p.name >> p.price >> p.quantity) {
        if (orderMap.count(p.id)) {
            int orderedQty = orderMap[p.id];
            if (p.quantity >= orderedQty) {
                float itemTotal = orderedQty * p.price;
                totalBill += itemTotal;
                cout << p.name << "\t" << orderedQty << "\t" << p.price
                     << "\t" << itemTotal << "\t" << (p.quantity - orderedQty) << endl;
            } else {
                cout << "? Not enough stock for: " << p.name << endl;
            }
        }
    }

    in.close();
}

void takeOrder() {
    vector<OrderItem> orderItems;
    char choice;
    float totalBill = 0;

    do {
        OrderItem item;
        cout << "Enter Product ID: ";
        cin >> item.productId;
        cout << "Enter Quantity: ";
        cin >> item.quantity;
        orderItems.push_back(item);
        cout << "Add another item? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    // Map to store product ID to total quantity
    map<int, int> orderMap;
    for (int i = 0; i < orderItems.size(); i++) {
        orderMap[orderItems[i].productId] += orderItems[i].quantity;
    }

    // Print bill separately
    printBill(orderMap, totalBill);

    // Read and update product file
    ifstream in("products.txt");
    ofstream temp("temp.txt");
    ofstream orderFile("orders.txt", ios::app);

    Product p;
    bool success = true;
    vector<Product> updatedList;

    while (in >> p.id >> p.name >> p.price >> p.quantity) {
        if (orderMap.count(p.id)) {
            int q = orderMap[p.id];
            if (p.quantity >= q) {
                p.quantity -= q;
            } else {
                success = false;
            }
        }
        updatedList.push_back(p);
    }

    if (success) {
        for (int i = 0; i < updatedList.size(); i++) {
            temp << updatedList[i].id << " " << updatedList[i].name << " "
                 << updatedList[i].price << " " << updatedList[i].quantity << endl;
        }

        string orderID, name, phone;
        cout << "\nEnter Order ID: ";
        cin >> orderID;
        cout << "Enter Customer Name: ";
        cin >> name;
        cout << "Enter Phone Number: ";
        cin >> phone;

        orderFile << orderID << " " << name << " " << phone << " Pending" << endl;
        cout << "? Order placed successfully!\n";
        cout << "?? Total Bill: " << totalBill << " PKR\n";
    } else {
        cout << "? Order failed due to stock issue.\n";
    }

    in.close(); temp.close(); orderFile.close();
    remove("products.txt");
    rename("temp.txt", "products.txt");
}

void confirmOrder() {
    string id;
    cout << "Enter Order ID to confirm: ";
    cin >> id;

    ifstream in("orders.txt");
    ofstream out("temp.txt");
    Order o;
    bool found = false;

    while (in >> o.orderID >> o.customerName >> o.phone >> o.status) {
        if (o.orderID == id) {
            o.status = "Confirmed";
            found = true;
        }
        cout << o.orderID << " " << o.customerName << " " << o.phone << " " << o.status << endl;
    }

    in.close(); out.close();
    remove("orders.txt");
    rename("temp.txt", "orders.txt");

    if (found) cout << "Order confirmed.\n";
    else cout << "Order not found.\n";
}

 
void cancelOrder() {
    string id;
    cout << "Enter Order ID to cancel: ";
    cin >> id;

    ifstream in("orders.txt");
    ofstream out("temp.txt");
    Order o;
    bool found = false;

    while (in >> o.orderID >> o.customerName >> o.phone >> o.status) {
        if (o.orderID == id) {
            o.status = "Cancelled";
            found = true;
        }
        cout << o.orderID << " " << o.customerName << " " << o.phone << " " << o.status << endl;
    }

    in.close(); out.close();
    remove("orders.txt");
    rename("temp.txt", "orders.txt");

    if (found) cout << "Order cancelled.\n";
    else cout << "Order not found.\n";
}

void viewOrders() {
    ifstream in("orders.txt");
    Order o;
    cout << "\nID\tName\tPhone\tStatus\n";
    while (in >> o.orderID >> o.customerName >> o.phone >> o.status) {
        cout << o.orderID << "\t" << o.customerName << "\t" << o.phone << "\t" << o.status << endl;
    }
    in.close();
}


// ------------------------- Main Function -------------------------------
int main() {
    showHeader();
    int c;
    do {
        cout << "\n1. Register\n2. Login\n3. Exit\n";
        cout << "Enter choice: ";
        cin >> c;

        if (c == 1) {
            int role;
            cout << "Register as:\n1. Admin\n2. Agent\nChoose: ";
            cin >> role;
            registerUser(role == 1 ? "admin" : "agent");
        }
        else if (c == 2) {
            int role;
            cout << "Login as:\n1. Admin\n2. Agent\nChoose: ";
            cin >> role;
            if (loginUser(role == 1 ? "admin" : "agent")) {
                if (role == 1) adminMenu();
                else agentMenu();
            }
        }
        else if (c == 3) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    } while (true);

    return 0;
}

