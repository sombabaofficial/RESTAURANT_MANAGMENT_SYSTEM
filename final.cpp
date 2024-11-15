#include <bits/stdc++.h>

using namespace std;

int flag = 0;

// Base class for Login with pure virtual function
class Login {
protected:
    string username;
    string password;

public:
    Login(const string &uname, const string &pass) : username(uname), password(pass) {}

    // Pure virtual function for login
    virtual bool loginUser(const string &uname, const string &pass) = 0;
};

// Customer class inheriting from Login
class Customer : public Login {
public:
    Customer(const string &uname, const string &pass) : Login(uname, pass) {}

    bool loginUser(const string &uname, const string &pass) override {
        return (uname == username && pass == password);
    }
};



// Menu Item
class MenuItem {
public:
    int id;
    string name;
    double price;

    MenuItem(int id, const string &name, double price) : id(id), name(name), price(price) {}

    void display() const {
        cout << setw(5) << id << setw(25) << name << setw(10) << price << "\n";
    }
};



// Menu class
class Menu {
private:
    vector<MenuItem> items;

public:
    Menu(const string &filename) {
        loadMenu(filename);
    }

    void loadMenu(const string &filename) {
        items.clear();
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            if (line.empty()) continue; // Skip empty lines

            stringstream ss(line);
            string idStr, name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, priceStr, ',');

            try {
                int id = stoi(idStr);
                double price = stod(priceStr);
                items.emplace_back(id, name, price);
            } catch (...) {
                cerr << "Error: Invalid data in menu file: " << line << "\n";
            }
        }
    }

    void displayMenu() const {
        cout << "Menu:\n";
        cout << setw(5) << "ID" << setw(25) << "Name" << setw(10) << "Price\n";
        cout << "-----------------------------------------------\n";
        for (const auto &item : items) {
            item.display();
        }
    }

    vector<MenuItem> &getItems() {
        return items;
    }

    void saveMenu(const string &filename) const {
        ofstream file(filename);
        for (const auto &item : items) {
            file << item.id << "," << item.name << "," << item.price << "\n";
        }
    }
};




// Staff class inheriting from Login
class Staff : public Login {
public:
    Staff(const string &uname, const string &pass) : Login(uname, pass) {}

    bool loginUser(const string &uname, const string &pass) override {
        return (uname == username && pass == password);
    }

    void manageMenu(Menu &menu, const string &filename) {
        int choice;
        while (true) {
            cout << "\nStaff Menu Management:\n";
            cout << "1. Add Item\n2. Edit Item\n3. Delete Item\n4. Display Menu\n0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 0) break;

            switch (choice) {
                case 1: {
                    int id;
                    string name;
                    double price;
                    cout << "Enter ID: ";
                    cin >> id;
                    cout << "Enter Name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter Price: ";
                    cin >> price;
                    menu.getItems().emplace_back(id, name, price);
                    break;
                }
                case 2: {
                    int id;
                    cout << "Enter ID of item to edit: ";
                    cin >> id;
                    bool found = false;
                    for (auto &item : menu.getItems()) {
                        if (item.id == id) {
                            found = true;
                            cout << "Enter new Name: ";
                            cin.ignore();
                            getline(cin, item.name);
                            cout << "Enter new Price: ";
                            cin >> item.price;
                            break;
                        }
                    }
                    if (!found) cout << "Item not found.\n";
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter ID of item to delete: ";
                    cin >> id;
                    auto &items = menu.getItems();
                    auto it = remove_if(items.begin(), items.end(), [&](const MenuItem &item) { return item.id == id; });
                    if (it != items.end()) {
                        items.erase(it, items.end());
                        cout << "Item deleted.\n";
                    } else {
                        cout << "Item not found.\n";
                    }
                    break;
                }
                case 4:
                    menu.displayMenu();
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
            menu.saveMenu(filename);
        }
    }
};


// UserManager class to handle file I/O for Customer and Staff users
class UserManager {
private:
    vector<Customer> customers;
    vector<Staff> staff;

public:
    UserManager() {
        loadUsers();
    }

    void registerUser(const string &role, const string &uname, const string &pass) {
        if (role == "Customer") {
            customers.push_back(Customer(uname, pass));
            saveToFile("customers.csv", uname, pass);
            cout << "Customer registered successfully.\n";
        }  else {
            cout << "Invalid role. Registration failed.\n";
        }
    }

    void loginUser(const string &role, const string &uname, const string &pass, Menu &menu, const string &filename) {
        if (role == "Customer") {
            for (Customer &customer : customers) {
                if (customer.loginUser(uname, pass)) {
                    cout << "Login successful for Customer.\n";
                    flag = 1;
                    return;
                }
            }
        } else if (role == "Staff") {
            for (Staff &s : staff) {
                if (s.loginUser(uname, pass)) {
                    cout << "Login successful for Staff.\n";
                    s.manageMenu(menu, filename);
                    return;
                }
            }
        }
        cout << "Login failed. Invalid credentials.\n";
    }

private:
    void saveToFile(const string &filename, const string &uname, const string &pass) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << uname << "," << pass << "\n";
            file.close();
        } else {
            cout << "Unable to open file for saving.\n";
        }
    }

    void loadUsers() {
        loadFromFile("customers.csv", "Customer");
        loadFromFile("staff.csv", "Staff");
    }

    void loadFromFile(const string &filename, const string &role) {
        ifstream file(filename);
        string line, uname, pass;
        if (file.is_open()) {
            while (getline(file, line)) {
                stringstream ss(line);
                getline(ss, uname, ',');
                getline(ss, pass, ',');

                if (role == "Customer") {
                    customers.push_back(Customer(uname, pass));
                } else if (role == "Staff") {
                    staff.push_back(Staff(uname, pass));
                }
            }
            file.close();
        } else {
            cout << "Unable to open file for loading.\n";
        }
    }
};



class CustomerItem {
public:
    int id;
    std::string name;
    double price;

    CustomerItem(int id, const std::string& name, double price)
        : id(id), name(name), price(price) {}

    void displayItem() const {
        std::cout << std::setw(5) << id << std::setw(25) << name << std::setw(10) << price << "\n";
    }
};

class CustomerMenu {
private:
    std::vector<CustomerItem> items;

public:
    CustomerMenu(const std::string& filename) {
        loadCustomerMenu(filename);
    }

    void loadCustomerMenu(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue;  // Skip empty lines

            std::stringstream ss(line);
            std::string idStr, name, priceStr;

            std::getline(ss, idStr, ',');
            std::getline(ss, name, ',');
            std::getline(ss, priceStr, ',');

            try {
                int id = std::stoi(idStr);
                double price = std::stod(priceStr);
                items.emplace_back(id, name, price);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid data in menu file: " << line << "\n";
                continue;  // Skip the current line and move to the next
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: Number out of range in menu file: " << line << "\n";
                continue;  // Skip the current line and move to the next
            }
        }
    }

    void displayCustomerMenu() const {
        std::cout << "Customer Menu:\n";
        std::cout << std::setw(5) << "ID" << std::setw(25) << "Name" << std::setw(10) << "Price\n";
        std::cout << "-----------------------------------------------\n";
        for (const auto& item : items) {
            item.displayItem();
        }
    }

    const CustomerItem* getCustomerItemById(int id) const {
        for (const auto& item : items) {
            if (item.id == id) {
                return &item;
            }
        }
        return nullptr;
    }
};


class Order {
private:
    std::vector<CustomerItem> orderItems;

public:
    void addOrderItem(const CustomerItem& item) {
        orderItems.push_back(item);
    }

    void displayOrder() const {
        std::cout << "\nOrder Summary:\n";
        std::cout << std::setw(25) << "Item" << std::setw(10) << "Price\n";
        std::cout << "------------------------------------\n";
        for (const auto& item : orderItems) {
            std::cout << std::setw(25) << item.name << std::setw(10) << item.price << "\n";
        }
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : orderItems) {
            total += item.price;
        }
        return total;
    }
};



int main() {
    UserManager userManager;
    Menu menu("menu.csv");

    string role, selectrole;
    loginpage:

    cout << "Choose login as (a) Customer or (b) Staff: ";
    cin >> selectrole;

    if (selectrole == "a") {
        role = "Customer";

        customeruser:           //goto statement

        cout<<"Are you a new Customer ? (y/n)"<<endl;
        string ans;
        cin>>ans;
           string username, password;
        if (ans == "y") {
            // Register new user
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            userManager.registerUser(role, username, password);
            goto loginpage;
        } 

        else if(ans=="n"){
             cout << "Enter username: ";
        string username;
        cin >> username;
        cout << "Enter password: ";
        string password;
        cin >> password;
        userManager.loginUser(role, username, password, menu, "menu.csv");
        }
        else {
            cout<<"Invalid input"<<endl;
            goto customeruser;
        }
       
    } 
    else if (selectrole == "b") {
        role = "Staff";
        cout << "Enter username: ";
        string username;
        cin >> username;
        cout << "Enter password: ";
        string password;
        cin >> password;
        userManager.loginUser(role, username, password, menu, "menu.csv");
    } 
    else {
        cout << "Invalid role.\n";
        goto loginpage;
    }

    if(flag==1){
         CustomerMenu customerMenu("menu.csv");
         Order order;
    int choice;

    while (true) {
        customerMenu.displayCustomerMenu();
        std::cout << "\nEnter the item ID to add to your order (0 to finish): ";
        std::cin >> choice;

        if (choice == 0) break;

        const CustomerItem* item = customerMenu.getCustomerItemById(choice);
        if (item) {
            order.addOrderItem(*item);
            std::cout << item->name << " added to order.\n";
        } else {
            std::cout << "Invalid item ID. Please try again.\n";
        }
    }

    order.displayOrder();
    std::cout << "\nTotal Bill: " << std::fixed << std::setprecision(2) << order.calculateTotal() << "\n";
    cout<<"------------------------------------"<<endl;
    goto loginpage;

    }

    return 0;
}
