#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <limits>
using namespace std;

// Abstract base class
class Equipment {
protected:
    int id;
    string name;
    string condition;

public:
    Equipment(int i, string n, string c)
        : id(i), name(n), condition(c) {}

    virtual void display() const = 0;

    virtual string getType() const = 0;

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getCondition() const {
        return condition;
    }

    virtual ~Equipment() = default;
};

// Derived class: Component
class Component : public Equipment {
private:
    int quantity;
    int minimumStock;

public:
    Component(int i, string n, string c,
              int q, int m)
        : Equipment(i, n, c),
          quantity(q), minimumStock(m) {}

    void display() const override {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nType: Component"
             << "\nCondition: " << condition
             << "\nQuantity: " << quantity
             << "\nMinimum Stock: " << minimumStock
             << "\nStock Status: "
             << (quantity <= minimumStock
                 ? "LOW STOCK" : "Sufficient")
             << "\n";
    }

    string getType() const override {
        return "Component";
    }

    int getQuantity() const {
        return quantity;
    }

    void updateStock(int q) {
        if (q >= 0)
            quantity = q;
        else
            cout << "Invalid quantity!\n";
    }

    int getMinimumStock() const {
        return minimumStock;
    }
};

// Derived class: Instrument
class Instrument : public Equipment {
private:
    string faultStatus;

public:
    Instrument(int i, string n, string c,
               string f)
        : Equipment(i, n, c),
          faultStatus(f) {}

    void display() const override {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nType: Instrument"
             << "\nCondition: " << condition
             << "\nFault Status: " << faultStatus
             << "\n";
    }

    string getType() const override {
        return "Instrument";
    }

    string getFaultStatus() const {
        return faultStatus;
    }

    void updateFaultStatus(string status) {
        faultStatus = status;
    }
};

// Fault reporting class
class FaultReport {
private:
    int equipmentId;
    string description;
    string status;

public:
    FaultReport(int id, string d)
        : equipmentId(id),
          description(d),
          status("Pending") {}

    int getEquipmentId() const {
        return equipmentId;
    }

    string getStatus() const {
        return status;
    }

    void updateStatus(string s) {
        status = s;
    }

    void display() const {
        cout << "\nEquipment ID: " << equipmentId
             << "\nFault: " << description
             << "\nStatus: " << status << "\n";
    }
};

// Manager class
class LabManager {
private:
    vector<shared_ptr<Equipment>> equipment;
    vector<FaultReport> faults;

public:
    void addComponent() {
        int id, quantity, minimum;
        string name, condition;

        cout << "Enter ID: ";
        cin >> id;

        if (findEquipment(id) != nullptr) {
            cout << "ID already exists!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter component name: ";
        getline(cin, name);

        cout << "Enter condition: ";
        getline(cin, condition);

        cout << "Enter quantity: ";
        cin >> quantity;

        cout << "Enter minimum stock: ";
        cin >> minimum;

        if (quantity < 0 || minimum < 0) {
            cout << "Invalid stock values!\n";
            return;
        }

        equipment.push_back(make_shared<Component>(
            id, name, condition, quantity, minimum));

        cout << "Component added successfully!\n";
    }

    void addInstrument() {
        int id;
        string name, condition, status;

        cout << "Enter ID: ";
        cin >> id;

        if (findEquipment(id) != nullptr) {
            cout << "ID already exists!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter instrument name: ";
        getline(cin, name);

        cout << "Enter condition: ";
        getline(cin, condition);

        cout << "Enter fault status: ";
        getline(cin, status);

        equipment.push_back(make_shared<Instrument>(
            id, name, condition, status));

        cout << "Instrument added successfully!\n";
    }

    shared_ptr<Equipment> findEquipment(int id) {
        for (auto &e : equipment) {
            if (e->getId() == id)
                return e;
        }
        return nullptr;
    }

    void displayAll() const {
        if (equipment.empty()) {
            cout << "No equipment records found.\n";
            return;
        }

        for (const auto &e : equipment)
            e->display();
    }

    void searchEquipment() {
        int id;
        cout << "Enter equipment ID: ";
        cin >> id;

        auto e = findEquipment(id);

        if (e != nullptr)
            e->display();
        else
            cout << "Equipment not found!\n";
    }

    void updateStock() {
        int id, quantity;

        cout << "Enter component ID: ";
        cin >> id;

        auto e = findEquipment(id);

        if (e == nullptr || e->getType() != "Component") {
            cout << "Component not found!\n";
            return;
        }

        auto c = dynamic_pointer_cast<Component>(e);

        cout << "Enter new quantity: ";
        cin >> quantity;

        if (quantity < 0) {
            cout << "Quantity cannot be negative!\n";
            return;
        }

        c->updateStock(quantity);
        cout << "Stock updated successfully!\n";
    }

    void reportFault() {
        int id;
        string description;

        cout << "Enter instrument ID: ";
        cin >> id;

        auto e = findEquipment(id);

        if (e == nullptr || e->getType() != "Instrument") {
            cout << "Instrument not found!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Describe the fault: ";
        getline(cin, description);

        faults.emplace_back(id, description);

        auto inst = dynamic_pointer_cast<Instrument>(e);
        inst->updateFaultStatus("Fault Reported");

        cout << "Fault report created successfully!\n";
    }

    void viewFaults() const {
        if (faults.empty()) {
            cout << "No fault reports found.\n";
            return;
        }

        for (const auto &f : faults)
            f.display();
    }

    void updateFault() {
        int id;
        cout << "Enter equipment ID for repair update: ";
        cin >> id;

        bool found = false;

        for (auto &f : faults) {
            if (f.getEquipmentId() == id) {
                string status;
                cout << "Enter status (Pending/In Progress/Resolved): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, status);

                f.updateStatus(status);
                found = true;
            }
        }

        if (!found) {
            cout << "No fault report found for this ID.\n";
            return;
        }

        auto e = findEquipment(id);

        if (e != nullptr &&
            e->getType() == "Instrument") {
            auto inst = dynamic_pointer_cast<Instrument>(e);
            inst->updateFaultStatus("Repair status updated");
        }

        cout << "Repair status updated successfully!\n";
    }

    void saveData() const {
        ofstream out("equipment.txt");

        if (!out) {
            cout << "Could not save equipment data!\n";
            return;
        }

        for (const auto &e : equipment) {
            if (auto c = dynamic_pointer_cast<Component>(e)) {
                out << "C|" << c->getId() << "|"
                    << c->getName() << "|"
                    << c->getCondition() << "|"
                    << c->getQuantity() << "|"
                    << c->getMinimumStock() << "\n";
            }
            else if (auto i = dynamic_pointer_cast<Instrument>(e)) {
                out << "I|" << i->getId() << "|"
                    << i->getName() << "|"
                    << i->getCondition() << "|"
                    << i->getFaultStatus() << "\n";
            }
        }

        out.close();

        ofstream fout("faults.txt");

        if (!fout) {
            cout << "Equipment saved, but fault file could not be opened!\n";
            return;
        }

        for (const auto &f : faults) {
            fout << f.getEquipmentId() << "|"
                 << f.getStatus() << "\n";
        }

        cout << "Data saved successfully!\n";
    }
};

int main() {
    LabManager lab;
    int choice;

    do {
        cout << "\n===== CIRCUITCARE SYSTEM =====\n";
        cout << "1. Add Component\n";
        cout << "2. Add Instrument\n";
        cout << "3. Display All Equipment\n";
        cout << "4. Search Equipment\n";
        cout << "5. Update Component Stock\n";
        cout << "6. Report Instrument Fault\n";
        cout << "7. View Fault Reports\n";
        cout << "8. Update Repair Status\n";
        cout << "9. Save Data\n";
        cout << "0. Save and Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1: lab.addComponent(); break;
            case 2: lab.addInstrument(); break;
            case 3: lab.displayAll(); break;
            case 4: lab.searchEquipment(); break;
            case 5: lab.updateStock(); break;
            case 6: lab.reportFault(); break;
            case 7: lab.viewFaults(); break;
            case 8: lab.updateFault(); break;
            case 9: lab.saveData(); break;
            case 0:
                lab.saveData();
                cout << "Thank you for using CircuitCare!\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}