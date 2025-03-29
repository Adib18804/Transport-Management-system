//Transport Management System
#include <bits/stdc++.h>

using namespace std;
enum Role
{
    ADMIN,
    DRIVER,
    PASSENGER
};

enum BusStatus
{
    ACTIVE,
    MAINTENANCE,
    OUT_OF_SERVICE
};

struct User
{
    string username;
    string password;
    Role role;
    string adminRole;
    string nidNumber;
    string licenseNumber;
    string address;
    string phoneNumber;
    string assignedBus;
};

struct Bus
{
    string busNumber;
    string model;
    int capacity;
    string type;
    string company;
    BusStatus status;
    time_t lastMaintenance;
    int maintenanceInterval;
};

struct DriverAttendance
{
    string username;
    string busNumber;
    time_t startTime;
    time_t endTime;
    bool isCompleted;
};

struct LostItem
{
    string description;
    string location;
    time_t date;
    string reportedBy;
};

struct Complaint
{
    string content;
    string status;
    string reference;
    string submittedBy;
    time_t date;
};

// Databases
vector<User> users;
vector<Bus> buses;
vector<DriverAttendance> driverAttendances;
vector<LostItem> lostItems;
vector<Complaint> complaints;

// File names
const string USERS_FILE = "users.txt";
const string BUSES_FILE = "buses.txt";
const string ATTENDANCE_FILE = "attendance.txt";
const string LOSTITEMS_FILE = "lostitems.txt";
const string COMPLAINTS_FILE = "complaints.txt";

// Admin UIDs
const map<string, string> ADMIN_UIDS =
{
    {"CHAIRMAN", "CHAIRMAN123"},
    {"MANAGER", "MANAGER123"},
    {"ACCOUNT_OFFICER", "ACCOUNT123"},
    {"ADMIN_EMP1", "ADMIN1"},
    {"ADMIN_EMP2", "ADMIN2"},
    {"ADMIN_EMP3", "ADMIN3"},
    {"ADMIN_EMP4", "ADMIN4"},
    {"ADMIN_EMP5", "ADMIN5"}
};

// Utility functions
vector<string> splitString(const string &s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

string getCurrentDateTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buf);
}

bool file_exists(const string &filename)
{
    ifstream f(filename);
    return f.good();
}

void createDefaultAdmin()
{
    if (any_of(users.begin(), users.end(),
               [](const User &u)
{
    return u.role == ADMIN;
}
{
    return;
}

User admin;
admin.username = "admin";
                 admin.password = "admin123";
                 admin.role = ADMIN;
                 admin.adminRole = "CHAIRMAN";

                 users.push_back(admin);
                 cout << "SECURITY NOTICE: Default admin created (Change password immediately!)\n";
                 cout << "Username: admin\nPassword: admin123\n";
}

// File operations
void saveUsers()
{
    ofstream file(USERS_FILE);
    if (!file.is_open())
    {
        cerr << "Error saving users data!\n";
        return;
    }

    for (const auto &user : users)
    {
        file << user.username << "|" << user.password << "|" << user.role;
        if (user.role == ADMIN)
        {
            file << "|" << user.adminRole;
        }
        file << "\n";
    }
}

void loadUsers()
{
    ifstream file(USERS_FILE);
    if (!file.is_open())
    {
        cerr << "Creating new users database...\n";
        createDefaultAdmin();
        return;
    }

    users.clear();
    string line;
    while (getline(file, line))
    {
        vector<string> parts = splitString(line, '|');
        if (parts.size() >= 3)
        {
            try
            {
                User user;
                user.username = parts[0];
                user.password = parts[1];
                user.role = static_cast<Role>(stoi(parts[2]));

                if (user.role == ADMIN && parts.size() > 3)
                {
                    user.adminRole = parts[3];
                }
                users.push_back(user);
            }
            catch (...)
            {
                cerr << "Warning: Skipped invalid user record\n";
            }
        }
    }

    if (users.empty())
    {
        createDefaultAdmin();
    }
}

void saveBuses()
{
    ofstream file(BUSES_FILE);
    if (!file.is_open())
    {
        cerr << "Error saving buses data!\n";
        return;
    }

    for (const auto &bus : buses)
    {
        file << bus.busNumber << "|" << bus.model << "|" << bus.capacity << "|"
             << bus.type << "|" << bus.company << "|" << bus.status << "|"
             << bus.lastMaintenance << "|" << bus.maintenanceInterval << "\n";
    }
}

void loadBuses()
{
    ifstream file(BUSES_FILE);
    buses.clear();
    string line;
    while (getline(file, line))
    {
        vector<string> parts = splitString(line, '|');
        if (parts.size() == 8)
        {
            try
            {
                Bus bus;
                bus.busNumber = parts[0];
                bus.model = parts[1];
                bus.capacity = stoi(parts[2]);
                bus.type = parts[3];
                bus.company = parts[4];
                bus.status = static_cast<BusStatus>(stoi(parts[5]));
                bus.lastMaintenance = stol(parts[6]);
                bus.maintenanceInterval = stoi(parts[7]);
                buses.push_back(bus);
            }
            catch (...)
            {
                cerr << "Warning: Skipped invalid bus record\n";
            }
        }
    }
}

void saveAttendances()
{
    ofstream file(ATTENDANCE_FILE);
    if (!file.is_open())
    {
        cerr << "Error saving attendance data!\n";
        return;
    }

    for (const auto &att : driverAttendances)
    {
        file << att.username << "|" << att.busNumber << "|"
             << att.startTime << "|" << att.endTime << "|"
             << att.isCompleted << "\n";
    }
}

void deleteAllUsers(User *admin)
{
    if (!admin || admin->role != ADMIN)
    {
        cout << "Access denied! Only admins can delete users.\n";
        return;
    }

    cout << "\n===== DELETE ALL USERS =====\n";
    cout << "WARNING: This will permanently delete ALL user accounts!\n";
    cout << "This action cannot be undone!\n";
    cout << "Type 'CONFIRM DELETE ALL' to proceed: ";

    string confirmation;
    cin.ignore();
    getline(cin, confirmation);

    if (confirmation == "CONFIRM DELETE ALL")
    {
        // Keep only the current admin account
        users.erase(remove_if(users.begin(), users.end(),
                              [admin](const User &u)
        {
            return u.username != admin->username;
        }),
        users.end());

        saveUsers();
        cout << "All users deleted except your admin account.\n";
    }
    else
    {
        cout << "Deletion cancelled.\n";
    }
}

void loadAttendances()
{
    ifstream file(ATTENDANCE_FILE);
    driverAttendances.clear();
    string line;
    while (getline(file, line))
    {
        vector<string> parts = splitString(line, '|');
        if (parts.size() == 5)
        {
            try
            {
                DriverAttendance att;
                att.username = parts[0];
                att.busNumber = parts[1];
                att.startTime = stol(parts[2]);
                att.endTime = stol(parts[3]);
                att.isCompleted = (parts[4] == "1");
                driverAttendances.push_back(att);
            }
            catch (...)
            {
                cerr << "Warning: Skipped invalid attendance record\n";
            }
        }
    }
}

void saveLostItems()
{
    ofstream file(LOSTITEMS_FILE);
    if (!file.is_open())
    {
        cerr << "Error saving lost items data!\n";
        return;
    }

    for (const auto &item : lostItems)
    {
        file << item.description << "|"
             << item.location << "|"
             << item.date << "|"
             << item.reportedBy << "\n";
    }
}

void loadLostItems()
{
    ifstream file(LOSTITEMS_FILE);
    lostItems.clear();
    string line;
    while (getline(file, line))
    {
        vector<string> parts = splitString(line, '|');
        if (parts.size() >= 4)
        {
            try
            {
                LostItem item;
                item.description = parts[0];
                item.location = parts[1];
                item.date = stol(parts[2]);
                item.reportedBy = parts[3];
                lostItems.push_back(item);
            }
            catch (...)
            {
                cerr << "Warning: Skipped invalid lost item record\n";
            }
        }
    }
}

void saveComplaints()
{
    ofstream file(COMPLAINTS_FILE);
    if (!file.is_open())
    {
        cerr << "Error saving complaints data!\n";
        return;
    }

    for (const auto &complaint : complaints)
    {
        file << complaint.content << "|"
             << complaint.status << "|"
             << complaint.reference << "|"
             << complaint.submittedBy << "|"
             << complaint.date << "\n";
    }
}

void loadComplaints()
{
    ifstream file(COMPLAINTS_FILE);
    complaints.clear();
    string line;
    while (getline(file, line))
    {
        vector<string> parts = splitString(line, '|');
        if (parts.size() >= 5)
        {
            try
            {
                Complaint complaint;
                complaint.content = parts[0];
                complaint.status = parts[1];
                complaint.reference = parts[2];
                complaint.submittedBy = parts[3];
                complaint.date = stol(parts[4]);
                complaints.push_back(complaint);
            }
            catch (...)
            {
                cerr << "Warning: Skipped invalid complaint record\n";
            }
        }
    }
}

// Authentication functions
User *loginUser(Role requiredRole)
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (auto &user : users)
    {
        if (user.username == username && user.password == password)
        {
            if (user.role == requiredRole)
            {
                cout << "Login successful!\n";
                return &user;
            }
            else
            {
                cout << "This account doesn't have required privileges!\n";
                return nullptr;
            }
        }
    }
    cout << "Invalid username or password!\n";
    return nullptr;
}

void registerUser()
{
    User newUser;
    int roleChoice;
    string adminUid;
    string adminRole;

    // Username input with validation
    while (true)
    {
        cout << "Enter username (no spaces): ";
        cin >> newUser.username;

        bool usernameExists = false;
        for (const auto &user : users)
        {
            if (user.username == newUser.username)
            {
                usernameExists = true;
                break;
            }
        }

        if (!usernameExists)
            break;
        cout << "Username already exists! Please choose a different username.\n";
    }

    // Password input
    cout << "Enter password: ";
    cin >> newUser.password;

    // Role selection with validation
    while (true)
    {
        cout << "Select role:\n";
        cout << "1. Driver\n";
        cout << "2. Passenger\n";
        cout << "3. Admin (requires UID)\n";
        cout << "Enter choice (1-3): ";

        if (!(cin >> roleChoice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (roleChoice >= 1 && roleChoice <= 3)
            break;
        cout << "Invalid choice. Please enter 1, 2, or 3.\n";
    }

    // Handle driver registration
    if (roleChoice == 1)
    {
        newUser.role = DRIVER;

        cout << "Enter NID Number: ";
        cin >> newUser.nidNumber;

        cout << "Enter License Number: ";
        cin >> newUser.licenseNumber;

        cin.ignore(); // Clear buffer
        cout << "Enter Address: ";
        getline(cin, newUser.address);

        // Phone number validation
        while (true)
        {
            cout << "Enter Phone Number (digits only): ";
            getline(cin, newUser.phoneNumber);
            if (all_of(newUser.phoneNumber.begin(), newUser.phoneNumber.end(), ::isdigit))
            {
                break;
            }
            cout << "Invalid phone number. Only digits are allowed.\n";
        }

        cout << "Assign to Bus (leave empty if none): ";
        getline(cin, newUser.assignedBus);
    }
    // Handle passenger registration
    else if (roleChoice == 2)
    {
        newUser.role = PASSENGER;
        newUser.nidNumber = "";
        newUser.licenseNumber = "";
        newUser.address = "";
        newUser.phoneNumber = "";
        newUser.assignedBus = "";
    }
    // Handle admin registration
    else if (roleChoice == 3)
    {
        // Admin role selection with validation
        while (true)
        {
            cout << "Available admin roles:\n";
            cout << "1. Chairman\n2. Manager\n3. Account Officer\n";
            cout << "4. Admin Employee 1\n5. Admin Employee 2\n6. Admin Employee 3\n";
            cout << "7. Admin Employee 4\n8. Admin Employee 5\n";
            cout << "Select admin role (1-8): ";

            int adminRoleChoice;
            if (!(cin >> adminRoleChoice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (adminRoleChoice >= 1 && adminRoleChoice <= 8)
            {
                const string roles[] =
                {
                    "CHAIRMAN", "MANAGER", "ACCOUNT_OFFICER",
                    "ADMIN_EMP1", "ADMIN_EMP2", "ADMIN_EMP3",
                    "ADMIN_EMP4", "ADMIN_EMP5"
                };
                adminRole = roles[adminRoleChoice - 1];
                break;
            }
            cout << "Invalid selection. Please try again.\n";
        }

        // UID verification
        cout << "Enter UID for " << adminRole << ": ";
        cin >> adminUid;

        if (ADMIN_UIDS.find(adminRole) == ADMIN_UIDS.end() ||
                ADMIN_UIDS.at(adminRole) != adminUid)
        {
            cout << "Invalid UID for " << adminRole << "! Admin registration failed.\n";
            return;
        }

        newUser.role = ADMIN;
        newUser.adminRole = adminRole;
        newUser.nidNumber = "";
        newUser.licenseNumber = "";
        newUser.address = "";
        newUser.phoneNumber = "";
        newUser.assignedBus = "";
    }

    users.push_back(newUser);
    saveUsers();
    cout << "\nUser registered successfully!\n";
}

void logoutUser(User *&loggedInUser)
{
    if (loggedInUser)
    {
        cout << "Successfully logged out user: " << loggedInUser->username << endl;
        loggedInUser = nullptr;
    }
}

// Driver functions
void recordDriverAttendance(User *driver)
{
    if (!driver || driver->role != DRIVER)
    {
        cout << "Only drivers can record attendance!\n";
        return;
    }

    DriverAttendance att;
    att.username = driver->username;

    cout << "Enter bus number: ";
    cin >> att.busNumber;

    cout << "1. Start Shift\n2. End Shift\nEnter choice: ";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        // Check if driver already has active shift
        auto activeShift = find_if(driverAttendances.begin(), driverAttendances.end(),
                                   [&](const DriverAttendance &a)
        {
            return a.username == driver->username && !a.isCompleted;
        });

        if (activeShift != driverAttendances.end())
        {
            cout << "You already have an active shift!\n";
            return;
        }

        att.startTime = time(0);
        att.isCompleted = false;
        cout << "Shift started at " << ctime(&att.startTime);
        driverAttendances.push_back(att);
    }
    else if (choice == 2)
    {
        auto activeShift = find_if(driverAttendances.begin(), driverAttendances.end(),
                                   [&](const DriverAttendance &a)
        {
            return a.username == driver->username && !a.isCompleted;
        });

        if (activeShift != driverAttendances.end())
        {
            activeShift->endTime = time(0);
            activeShift->isCompleted = true;
            cout << "Shift ended at " << ctime(&activeShift->endTime);
            cout << "Duration: " << (activeShift->endTime - activeShift->startTime) / 3600.0 << " hours\n";
        }
        else
        {
            cout << "No active shift found!\n";
            return;
        }
    }
    else
    {
        cout << "Invalid choice!\n";
        return;
    }

    saveAttendances();
}

void viewDriverAttendance(User *driver)
{
    if (!driver || driver->role != DRIVER)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== Your Attendance Records =====\n";
    bool found = false;

    for (const auto &att : driverAttendances)
    {
        if (att.username == driver->username)
        {
            found = true;
            tm *start_tm = localtime(&att.startTime);
            tm *end_tm = att.isCompleted ? localtime(&att.endTime) : nullptr;

            cout << "Bus: " << att.busNumber << "\n";
            cout << "Start: " << put_time(start_tm, "%Y-%m-%d %H:%M:%S");

            if (att.isCompleted)
            {
                cout << "\nEnd: " << put_time(end_tm, "%Y-%m-%d %H:%M:%S");
                cout << "\nDuration: " << (att.endTime - att.startTime) / 60 << " minutes\n";
            }
            else
            {
                cout << "\nStatus: Shift in progress\n";
            }
            cout << "------------------------\n";
        }
    }

    if (!found)
    {
        cout << "No records found!\n";
    }
}

// Bus management functions
void addBus(User *user)
{
    if (!user || user->role != ADMIN)
    {
        cout << "Only admins can add buses!\n";
        return;
    }

    Bus newBus;
    cout << "Enter bus number: ";
    cin >> newBus.busNumber;

    // Check if bus already exists
    if (find_if(buses.begin(), buses.end(),
                [&](const Bus &b)
{
    return b.busNumber == newBus.busNumber;
}) != buses.end())
    {
        cout << "Bus with this number already exists!\n";
        return;
    }

    cout << "Enter bus model: ";
    cin >> newBus.model;

    cout << "Enter capacity: ";
    while (!(cin >> newBus.capacity) || newBus.capacity <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid capacity. Please enter a positive number: ";
    }

    cout << "Enter type: ";
    cin >> newBus.type;
    cout << "Enter company name: ";
    cin >> newBus.company;

    newBus.status = ACTIVE;
    newBus.lastMaintenance = time(0);

    cout << "Enter maintenance interval (days): ";
    while (!(cin >> newBus.maintenanceInterval) || newBus.maintenanceInterval <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid interval. Please enter a positive number: ";
    }

    buses.push_back(newBus);
    saveBuses();
    cout << "Bus added successfully!\n";
}

void viewAllBuses()
{
    if (buses.empty())
    {
        cout << "No buses found!\n";
        return;
    }

    cout << "\n===== All Buses =====\n";
    for (const auto &bus : buses)
    {
        cout << "Bus Number: " << bus.busNumber << "\n";
        cout << "Model: " << bus.model << "\n";
        cout << "Capacity: " << bus.capacity << "\n";
        cout << "Type: " << bus.type << "\n";
        cout << "Company: " << bus.company << "\n";
        cout << "Status: ";
        switch (bus.status)
        {
        case ACTIVE:
            cout << "Active";
            break;
        case MAINTENANCE:
            cout << "In Maintenance";
            break;
        case OUT_OF_SERVICE:
            cout << "Out of Service";
            break;
        }
        cout << "\nLast Maintenance: " << ctime(&bus.lastMaintenance);
        cout << "Next Maintenance Due: " << bus.maintenanceInterval << " days\n";
        cout << "------------------------\n";
    }
}

void markBusMaintenance(User *user)
{
    if (!user || user->role != ADMIN)
    {
        cout << "Only admins can mark buses for maintenance!\n";
        return;
    }

    string busNumber;
    cout << "Enter bus number to mark for maintenance: ";
    cin >> busNumber;

    auto bus = find_if(buses.begin(), buses.end(),
                       [&](const Bus &b)
    {
        return b.busNumber == busNumber;
    });

    if (bus != buses.end())
    {
        bus->status = MAINTENANCE;
        bus->lastMaintenance = time(0);
        saveBuses();
        cout << "Bus marked for maintenance successfully!\n";
    }
    else
    {
        cout << "Bus not found!\n";
    }
}

void viewMaintenanceSchedule()
{
    time_t now = time(0);
    cout << "\n===== Maintenance Schedule =====\n";
    for (const auto &bus : buses)
    {
        time_t nextMaintenance = bus.lastMaintenance + bus.maintenanceInterval * 24 * 60 * 60;
        cout << "Bus: " << bus.busNumber << " - ";
        cout << "Last Maintenance: " << ctime(&bus.lastMaintenance);
        cout << "Next Maintenance Due: " << ctime(&nextMaintenance);
        if (nextMaintenance < now)
        {
            cout << "!!! OVERDUE !!!\n";
        }
        cout << "------------------------\n";
    }
}

// User management functions
void changePassword(User *user)
{
    if (!user)
    {
        cout << "You must be logged in to change your password!\n";
        return;
    }

    string currentPassword, newPassword;
    cout << "Enter current password: ";
    cin >> currentPassword;

    if (user->password != currentPassword)
    {
        cout << "Incorrect current password!\n";
        return;
    }

    cout << "Enter new password: ";
    cin >> newPassword;

    user->password = newPassword;
    saveUsers();
    cout << "Password changed successfully!\n";
}

void deleteUser(User *user)
{
    if (!user || user->role != ADMIN)
    {
        cout << "Only admins can delete users!\n";
        return;
    }

    string username;
    cout << "Enter username to delete: ";
    cin >> username;

    // Prevent self-deletion
    if (user->username == username)
    {
        cout << "You cannot delete your own account!\n";
        return;
    }

    auto it = remove_if(users.begin(), users.end(),
                        [&](const User &u)
    {
        return u.username == username;
    });

    if (it != users.end())
    {
        users.erase(it, users.end());
        saveUsers();
        cout << "User deleted successfully!\n";
    }
    else
    {
        cout << "User not found!\n";
    }
}

// Lost and Found functions
void lostAndFound(User *passenger)
{
    if (!passenger || passenger->role != PASSENGER)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== Lost and Found =====\n";
    cout << "1. Report Lost Item\n";
    cout << "2. View Found Items\n";
    cout << "3. Back\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        LostItem item;
        cout << "Describe the lost item: ";
        cin.ignore();
        getline(cin, item.description);
        cout << "Where was it lost? ";
        getline(cin, item.location);
        item.date = time(0);
        item.reportedBy = passenger->username;

        lostItems.push_back(item);
        saveLostItems();
        cout << "Thank you. Your lost item has been reported.\n";
        break;
    }
    case 2:
    {
        cout << "\n=== Found Items ===\n";
        if (lostItems.empty())
        {
            cout << "No items found yet.\n";
        }
        else
        {
            for (size_t i = 0; i < lostItems.size(); i++)
            {
                cout << i + 1 << ". " << lostItems[i].description
                     << " (Found at: " << lostItems[i].location << " on "
                     << ctime(&lostItems[i].date) << ")\n";
            }
        }
        break;
    }
    case 3:
        return;
    default:
        cout << "Invalid choice!\n";
    }
}

// Complaint functions
void complainBox(User *passenger)
{
    if (!passenger || passenger->role != PASSENGER)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== Complain Box =====\n";
    cout << "1. Submit a Complaint\n";
    cout << "2. View Your Complaints\n";
    cout << "3. Back\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        Complaint complaint;
        cout << "Enter your complaint: ";
        cin.ignore();
        getline(cin, complaint.content);

        complaint.status = "Pending";
        complaint.reference = "REF" + to_string(rand() % 9000 + 1000);
        complaint.submittedBy = passenger->username;
        complaint.date = time(0);

        complaints.push_back(complaint);
        saveComplaints();
        cout << "Complaint submitted. Reference #: " << complaint.reference << endl;
        break;
    }
    case 2:
    {
        cout << "\n=== Your Complaints ===\n";
        bool found = false;
        for (const auto &complaint : complaints)
        {
            if (complaint.submittedBy == passenger->username)
            {
                found = true;
                cout << "Reference: " << complaint.reference << "\n";
                cout << "Status: " << complaint.status << "\n";
                cout << "Date: " << ctime(&complaint.date);
                cout << "Content: " << complaint.content << "\n";
                cout << "------------------------\n";
            }
        }
        if (!found)
        {
            cout << "No complaints found!\n";
        }
        break;
    }
    case 3:
        return;
    default:
        cout << "Invalid choice!\n";
    }
}

// Admin report functions
void viewAllDriverAttendance(User *admin)
{
    if (!admin || admin->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== All Driver Attendance Records =====\n";
    for (const auto &att : driverAttendances)
    {
        tm *start_tm = localtime(&att.startTime);
        tm *end_tm = att.isCompleted ? localtime(&att.endTime) : nullptr;

        cout << "Driver: " << att.username << " | Bus: " << att.busNumber << "\n";
        cout << "Start: " << put_time(start_tm, "%Y-%m-%d %H:%M:%S");
        if (att.isCompleted)
        {
            cout << "\nEnd: " << put_time(end_tm, "%Y-%m-%d %H:%M:%S");
            cout << "\nDuration: " << (att.endTime - att.startTime) / 60 << " minutes\n";
        }
        else
        {
            cout << "\nStatus: Shift in progress\n";
        }
        cout << "------------------------\n";
    }
}

void viewDriverDetailsByBus(User *admin)
{
    if (!admin || admin->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    string busNumber;
    cout << "Enter bus number: ";
    cin >> busNumber;

    bool found = false;
    for (const auto &user : users)
    {
        if (user.role == DRIVER && user.assignedBus == busNumber)
        {
            found = true;
            cout << "\n===== Driver Details =====\n";
            cout << "Username: " << user.username << "\n";
            cout << "NID Number: " << user.nidNumber << "\n";
            cout << "License Number: " << user.licenseNumber << "\n";
            cout << "Address: " << user.address << "\n";
            cout << "Phone: " << user.phoneNumber << "\n";
            cout << "Assigned Bus: " << user.assignedBus << "\n";

            // Show attendance summary
            int shifts = 0;
            double totalHours = 0;
            for (const auto &att : driverAttendances)
            {
                if (att.username == user.username && att.isCompleted)
                {
                    shifts++;
                    totalHours += (att.endTime - att.startTime) / 3600.0;
                }
            }
            cout << "Completed Shifts: " << shifts << "\n";
            cout << "Total Hours Worked: " << totalHours << "\n";
            cout << "------------------------\n";
        }
    }

    if (!found)
    {
        cout << "No driver assigned to this bus or bus doesn't exist.\n";
    }
}

void assignDriverToBus(User *admin)
{
    if (!admin || admin->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    string username, busNumber;
    cout << "Enter driver username: ";
    cin >> username;
    cout << "Enter bus number: ";
    cin >> busNumber;

    // Check if bus exists
    if (find_if(buses.begin(), buses.end(),
                [&](const Bus &b)
{
    return b.busNumber == busNumber;
}) == buses.end())
    {
        cout << "Bus not found!\n";
        return;
    }

    for (auto &user : users)
    {
        if (user.username == username && user.role == DRIVER)
        {
            user.assignedBus = busNumber;
            saveUsers();
            cout << "Driver assigned successfully!\n";
            return;
        }
    }
    cout << "Driver not found!\n";
}

void busUtilizationReport(User *admin)
{
    if (!admin || admin->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== Bus Utilization Report =====\n";
    for (const auto &bus : buses)
    {
        cout << "Bus " << bus.busNumber << ":\n";
        cout << "Capacity: " << bus.capacity << "\n";
        cout << "Status: ";
        switch (bus.status)
        {
        case ACTIVE:
            cout << "Active";
            break;
        case MAINTENANCE:
            cout << "In Maintenance";
            break;
        case OUT_OF_SERVICE:
            cout << "Out of Service";
            break;
        }
        cout << "\n------------------------\n";
    }
}

void driverPerformanceReport(User *admin)
{
    if (!admin || admin->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== Driver Performance Report =====\n";
    for (const auto &user : users)
    {
        if (user.role == DRIVER)
        {
            int shifts = 0;
            double totalHours = 0;
            int lateShifts = 0;

            for (const auto &att : driverAttendances)
            {
                if (att.username == user.username && att.isCompleted)
                {
                    shifts++;
                    double duration = (att.endTime - att.startTime) / 3600.0;
                    totalHours += duration;
                    if (duration < 8)
                        lateShifts++;
                }
            }

            cout << "Driver: " << user.username << "\n";
            cout << "Assigned Bus: " << user.assignedBus << "\n";
            cout << "Total Shifts: " << shifts << "\n";
            cout << "Average Shift Length: "
                 << (shifts ? totalHours / shifts : 0) << " hours\n";
            cout << "On-Time Completion: "
                 << (shifts ? 100 - (lateShifts * 100 / shifts) : 100) << "%\n";
            cout << "------------------------\n";
        }
    }
}

void generateRevenueReport(User *user)
{
    if (!user || user->role != ADMIN)
    {
        cout << "Only admins can generate revenue reports!\n";
        return;
    }

    cout << "\n===== Revenue Report =====\n";
    cout << "No ticket system available in this version.\n";
}

// Menu functions
void showLoginMenu()
{
    cout << "\n===== Public Transport Management System =====\n";
    cout << "1. Login as Admin\n";
    cout << "2. Login as Driver\n";
    cout << "3. Login as Passenger\n";
    cout << "4. Register New User\n";
    cout << "5. Exit\n";
    cout << "Enter choice: ";
}

void showAdminMenu()
{
    cout << "\n===== Admin Menu =====\n";
    cout << "1. Bus Management\n";
    cout << "2. User Management\n";
    cout << "3. Reports\n";
    cout << "4. Maintenance Alerts\n";
    cout << "5. Financial Management\n";
    cout << "6. System Settings\n";
    cout << "7. View All Driver Attendance\n";
    cout << "8. View Driver Details by Bus\n";
    cout << "9. Assign Driver to Bus\n";
    cout << "10. Bus Utilization Report\n";
    cout << "11. Driver Performance Report\n";
    cout << "12. Logout\n";
    cout << "Enter choice: ";
}

void showDriverMenu()
{
    cout << "\n===== Driver Menu =====\n";
    cout << "1. Record Attendance\n";
    cout << "2. View My Attendance\n";
    cout << "3. Logout\n";
    cout << "Enter choice: ";
}

void showPassengerMenu()
{
    cout << "\n===== Passenger Menu =====\n";
    cout << "1. Lost and Found\n";
    cout << "2. Complain Box\n";
    cout << "3. Logout\n";
    cout << "Enter choice: ";
}

void showBusManagementMenu()
{
    cout << "\n===== Bus Management =====\n";
    cout << "1. Add New Bus\n";
    cout << "2. View All Buses\n";
    cout << "3. Mark Bus for Maintenance\n";
    cout << "4. View Maintenance Schedule\n";
    cout << "5. Back to Main Menu\n";
    cout << "Enter choice: ";
}

void showUserManagementMenu()
{
    cout << "\n===== User Management =====\n";
    cout << "1. Register New User\n";
    cout << "2. Change Password\n";
    cout << "3. Delete User\n";
    cout << "4. Delete All Users\n";
    cout << "5. Back to Main Menu\n";
    cout << "Enter choice: ";
}

void showReportsMenu()
{
    cout << "\n===== Reports =====\n";
    cout << "1. Generate Revenue Report\n";
    cout << "2. Back to Main Menu\n";
    cout << "Enter choice: ";
}

void showMaintenanceAlerts(User *user)
{
    if (!user || user->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    time_t now = time(0);
    cout << "\n===== Maintenance Alerts =====\n";
    bool overdueFound = false;

    for (const auto &bus : buses)
    {
        time_t nextMaintenance = bus.lastMaintenance + bus.maintenanceInterval * 24 * 60 * 60;
        if (nextMaintenance < now)
        {
            cout << "🚨 Bus " << bus.busNumber << " is OVERDUE for maintenance!\n";
            overdueFound = true;
        }
        else if ((nextMaintenance - now) < (3 * 24 * 60 * 60))
        {
            cout << "⚠️ Bus " << bus.busNumber << " needs maintenance soon ("
                 << (nextMaintenance - now) / 86400 << " days left)\n";
            overdueFound = true;
        }
    }

    if (!overdueFound)
    {
        cout << "No maintenance alerts at this time.\n";
    }
}

void financialManagement(User *user)
{
    if (!user || user->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== Financial Management =====\n";
    cout << "1. View Revenue Reports\n";
    cout << "2. View Expenses\n";
    cout << "3. Profit/Loss Statement\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        generateRevenueReport(user);
        break;
    case 2:
        cout << "Expense tracking coming soon!\n";
        break;
    case 3:
        cout << "Profit/loss reports coming soon!\n";
        break;
    default:
        cout << "Invalid choice!\n";
    }
}

void systemSettings(User *user)
{
    if (!user || user->role != ADMIN)
    {
        cout << "Access denied!\n";
        return;
    }

    cout << "\n===== System Settings =====\n";
    cout << "1. Set Maintenance Interval\n";
    cout << "2. Backup Database\n";
    cout << "3. Restore Database\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        cout << "Enter new maintenance interval (days): ";
        int days;
        cin >> days;
        if (days > 0)
        {
            for (auto &bus : buses)
            {
                bus.maintenanceInterval = days;
            }
            saveBuses();
            cout << "Maintenance interval updated!\n";
        }
        else
        {
            cout << "Invalid interval. Must be positive.\n";
        }
        break;
    }
    case 2:
        cout << "Database backup feature coming soon!\n";
        break;
    case 3:
        cout << "Database restore feature coming soon!\n";
        break;
    default:
        cout << "Invalid choice!\n";
    }
}

// Main function
int main()
{
    // Load all data
    loadUsers();
    loadBuses();
    loadAttendances();
    loadLostItems();
    loadComplaints();

    User *loggedInUser = nullptr;
    int mainChoice, subChoice;

    while (true)
    {
        if (!loggedInUser)
        {
            showLoginMenu();
            cin >> mainChoice;

            switch (mainChoice)
            {
            case 1:
                loggedInUser = loginUser(ADMIN);
                break;
            case 2:
                loggedInUser = loginUser(DRIVER);
                break;
            case 3:
                loggedInUser = loginUser(PASSENGER);
                break;
            case 4:
                registerUser();
                break;
            case 5:
                // Save all data before exiting
                saveUsers();
                saveBuses();
                saveAttendances();
                saveLostItems();
                saveComplaints();
                return 0;
            default:
                cout << "Invalid choice!\n";
            }
        }
        else
        {
            switch (loggedInUser->role)
            {
            case ADMIN:
            {
                showAdminMenu();
                cin >> mainChoice;

                switch (mainChoice)
                {
                case 1:
                {
                    do
                    {
                        showBusManagementMenu();
                        cin >> subChoice;
                        switch (subChoice)
                        {
                        case 1:
                            addBus(loggedInUser);
                            break;
                        case 2:
                            viewAllBuses();
                            break;
                        case 3:
                            markBusMaintenance(loggedInUser);
                            break;
                        case 4:
                            viewMaintenanceSchedule();
                            break;
                        case 5:
                            break;
                        default:
                            cout << "Invalid choice!\n";
                        }
                    }
                    while (subChoice != 5);
                    break;
                }
                case 2:
                {
                    do
                    {
                        showUserManagementMenu();
                        cin >> subChoice;
                        switch (subChoice)
                        {
                        case 1:
                            registerUser();
                            break;
                        case 2:
                            changePassword(loggedInUser);
                            break;
                        case 3:
                            deleteUser(loggedInUser);
                            break;
                        case 4:
                            deleteAllUsers(loggedInUser);
                            break;
                        case 5:
                            break;
                        default:
                            cout << "Invalid choice!\n";
                        }
                    }
                    while (subChoice != 5);
                    break;
                }
                case 3:
                {
                    do
                    {
                        showReportsMenu();
                        cin >> subChoice;
                        switch (subChoice)
                        {
                        case 1:
                            generateRevenueReport(loggedInUser);
                            break;
                        case 2:
                            break;
                        default:
                            cout << "Invalid choice!\n";
                        }
                    }
                    while (subChoice != 2);
                    break;
                }
                case 4:
                    showMaintenanceAlerts(loggedInUser);
                    break;
                case 5:
                    financialManagement(loggedInUser);
                    break;
                case 6:
                    systemSettings(loggedInUser);
                    break;
                case 7:
                    viewAllDriverAttendance(loggedInUser);
                    break;
                case 8:
                    viewDriverDetailsByBus(loggedInUser);
                    break;
                case 9:
                    assignDriverToBus(loggedInUser);
                    break;
                case 10:
                    busUtilizationReport(loggedInUser);
                    break;
                case 11:
                    driverPerformanceReport(loggedInUser);
                    break;
                case 12:
                    logoutUser(loggedInUser);
                    break;
                default:
                    cout << "Invalid choice!\n";
                }
                break;
            }
            case DRIVER:
            {
                showDriverMenu();
                cin >> mainChoice;

                switch (mainChoice)
                {
                case 1:
                    recordDriverAttendance(loggedInUser);
                    break;
                case 2:
                    viewDriverAttendance(loggedInUser);
                    break;
                case 3:
                    logoutUser(loggedInUser);
                    break;
                default:
                    cout << "Invalid choice!\n";
                }
                break;
            }
            case PASSENGER:
            {
                showPassengerMenu();
                cin >> mainChoice;

                switch (mainChoice)
                {
                case 1:
                    lostAndFound(loggedInUser);
                    break;
                case 2:
                    complainBox(loggedInUser);
                    break;
                case 3:
                    logoutUser(loggedInUser);
                    break;
                default:
                    cout << "Invalid choice!\n";
                }
                break;
            }
            }
        }
    }
    return 0;
}
