
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

const int PRODUCT_COUNT = 8;

class Product {
public:
    int id;
    string name;
    double price;
    int stock;
};

class CartItem {
public:
    int productId;
    string name;
    double price;
    int quantity;
    CartItem* next;

    CartItem(int id, string n, double p, int q) {
        productId = id;
        name = n;
        price = p;
        quantity = q;
        next = nullptr;
    }
};

class ShoppingCart {
private:
    Product products[PRODUCT_COUNT];
    CartItem* cartHead;

    void initializeProducts() {
        products[0] = {101, "Laptop", 45000, 5};
        products[1] = {102, "Smartphone", 18000, 10};
        products[2] = {103, "Headphones", 1500, 20};
        products[3] = {104, "Keyboard", 800, 15};
        products[4] = {105, "Mouse", 500, 25};
        products[5] = {106, "Monitor", 9000, 8};
        products[6] = {107, "Backpack", 1200, 12};
        products[7] = {108, "USB Cable", 250, 30};
    }

    Product* findProduct(int id) {
        for (int i = 0; i < PRODUCT_COUNT; i++) {
            if (products[i].id == id)
                return &products[i];
        }
        return nullptr;
    }

    CartItem* findCartItem(int id) {
        CartItem* temp = cartHead;

        while (temp != nullptr) {
            if (temp->productId == id)
                return temp;

            temp = temp->next;
        }

        return nullptr;
    }

    double calculateTotal() {
        double total = 0;
        CartItem* temp = cartHead;

        while (temp != nullptr) {
            total += temp->price * temp->quantity;
            temp = temp->next;
        }

        return total;
    }

    void clearCart() {
        while (cartHead != nullptr) {
            CartItem* temp = cartHead;
            cartHead = cartHead->next;
            delete temp;
        }
    }

public:
    ShoppingCart() {
        cartHead = nullptr;
        initializeProducts();
    }

    // 1. Display products
    void displayProducts() {
        cout << "\n================ PRODUCT CATALOG ================\n";

        cout << left
             << setw(10) << "ID"
             << setw(20) << "Product"
             << setw(15) << "Price (Rs.)"
             << "Stock\n";

        for (int i = 0; i < PRODUCT_COUNT; i++) {
            cout << left
                 << setw(10) << products[i].id
                 << setw(20) << products[i].name
                 << setw(15) << fixed << setprecision(2)
                 << products[i].price
                 << products[i].stock << '\n';
        }
    }

    // 2. Search product
    void searchProduct() {
        int id;

        cout << "\nEnter Product ID: ";
        cin >> id;

        Product* p = findProduct(id);

        if (p == nullptr) {
            cout << "Product not found!\n";
            return;
        }

        cout << "\nProduct Found!\n";
        cout << "ID    : " << p->id << '\n';
        cout << "Name  : " << p->name << '\n';
        cout << "Price : Rs. " << p->price << '\n';
        cout << "Stock : " << p->stock << '\n';
    }

    // 3. Add product to cart
    void addToCart() {
        int id, quantity;

        displayProducts();

        cout << "\nEnter Product ID: ";
        cin >> id;

        Product* p = findProduct(id);

        if (p == nullptr) {
            cout << "Invalid Product ID!\n";
            return;
        }

        cout << "Enter Quantity: ";

        if (!(cin >> quantity) || quantity <= 0) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            cout << "Quantity must be positive!\n";
            return;
        }

        CartItem* existing = findCartItem(id);

        int alreadyInCart = existing ? existing->quantity : 0;

        if (quantity + alreadyInCart > p->stock) {
            cout << "Insufficient stock!\n";
            cout << "Available stock: " << p->stock << '\n';
            return;
        }

        if (existing != nullptr) {
            existing->quantity += quantity;
        } else {
            CartItem* item = new CartItem(
                p->id, p->name, p->price, quantity
            );

            item->next = cartHead;
            cartHead = item;
        }

        cout << p->name << " added to cart successfully!\n";
    }

    // 4. Display cart
    void displayCart() {
        if (cartHead == nullptr) {
            cout << "\nYour shopping cart is empty!\n";
            return;
        }

        CartItem* temp = cartHead;

        cout << "\n================ YOUR CART ================\n";
        cout << left
             << setw(10) << "ID"
             << setw(20) << "Product"
             << setw(12) << "Price"
             << setw(10) << "Quantity"
             << "Amount\n";

        while (temp != nullptr) {
            cout << left
                 << setw(10) << temp->productId
                 << setw(20) << temp->name
                 << setw(12) << temp->price
                 << setw(10) << temp->quantity
                 << temp->price * temp->quantity << '\n';

            temp = temp->next;
        }

        cout << "--------------------------------------------\n";
        cout << "Total: Rs. " << calculateTotal() << '\n';
    }

    // 5. Remove product from cart
    void removeFromCart() {
        int id;

        cout << "\nEnter Product ID to remove: ";
        cin >> id;

        CartItem* temp = cartHead;
        CartItem* prev = nullptr;

        while (temp != nullptr && temp->productId != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Product not found in cart!\n";
            return;
        }

        if (prev == nullptr)
            cartHead = temp->next;
        else
            prev->next = temp->next;

        delete temp;

        cout << "Product removed from cart!\n";
    }

    // 6. Update quantity
    void updateQuantity() {
        int id, quantity;

        cout << "\nEnter Product ID: ";
        cin >> id;

        CartItem* item = findCartItem(id);

        if (item == nullptr) {
            cout << "Product not found in cart!\n";
            return;
        }

        cout << "Enter new quantity (0 to remove): ";

        if (!(cin >> quantity) || quantity < 0) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            cout << "Invalid quantity!\n";
            return;
        }

        if (quantity == 0) {
            removeCartItemById(id);
            cout << "Product removed from cart!\n";
            return;
        }

        Product* p = findProduct(id);

        if (p == nullptr || quantity > p->stock) {
            cout << "Insufficient stock!\n";
            return;
        }

        item->quantity = quantity;
        cout << "Quantity updated successfully!\n";
    }

    void removeCartItemById(int id) {
        CartItem* temp = cartHead;
        CartItem* prev = nullptr;

        while (temp != nullptr && temp->productId != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr)
            return;

        if (prev == nullptr)
            cartHead = temp->next;
        else
            prev->next = temp->next;

        delete temp;
    }

    // 7. Sort products by price
    void sortProductsByPrice() {
        for (int i = 0; i < PRODUCT_COUNT - 1; i++) {
            for (int j = 0; j < PRODUCT_COUNT - i - 1; j++) {
                if (products[j].price > products[j + 1].price) {
                    swap(products[j], products[j + 1]);
                }
            }
        }

        cout << "Products sorted by price successfully!\n";
        displayProducts();
    }

    // 8. Checkout
    void checkout() {
        if (cartHead == nullptr) {
            cout << "\nCart is empty. Add products first!\n";
            return;
        }

        // Verify stock before checkout
        CartItem* temp = cartHead;

        while (temp != nullptr) {
            Product* p = findProduct(temp->productId);

            if (p == nullptr || temp->quantity > p->stock) {
                cout << "Stock unavailable for "
                     << temp->name << ". Checkout cancelled.\n";
                return;
            }

            temp = temp->next;
        }

        cout << "\n=============== ORDER RECEIPT ===============\n";
        displayCart();

        // Reduce stock after successful checkout
        temp = cartHead;

        while (temp != nullptr) {
            Product* p = findProduct(temp->productId);
            p->stock -= temp->quantity;
            temp = temp->next;
        }

        cout << "\nOrder placed successfully!\n";
        cout << "Thank you for shopping with us!\n";

        clearCart();
    }

    ~ShoppingCart() {
        clearCart();
    }
};

int main() {
    ShoppingCart shop;
    int choice;

    do {
        cout << "\n\n======================================";
        cout << "\n     ONLINE SHOPPING CART SYSTEM";
        cout << "\n======================================";
        cout << "\n1. Display Products";
        cout << "\n2. Search Product";
        cout << "\n3. Add to Cart";
        cout << "\n4. View Cart";
        cout << "\n5. Remove Product from Cart";
        cout << "\n6. Update Quantity";
        cout << "\n7. Sort Products by Price";
        cout << "\n8. Checkout";
        cout << "\n0. Exit";
        cout << "\nEnter Your Choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            continue;
        }

        switch (choice) {
            case 1:
                shop.displayProducts();
                break;
            case 2:
                shop.searchProduct();
                break;
            case 3:
                shop.addToCart();
                break;
            case 4:
                shop.displayCart();
                break;
            case 5:
                shop.removeFromCart();
                break;
            case 6:
                shop.updateQuantity();
                break;
            case 7:
                shop.sortProductsByPrice();
                break;
            case 8:
                shop.checkout();
                break;
            case 0:
                cout << "Thank you for using Shopping Cart System!\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
