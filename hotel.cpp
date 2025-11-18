

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <cstdio>   
#include <cstdlib>  
using namespace std;

struct Client {
    string name;
    int age;
    int floor;
    int roomNo;
};


bool validHotelState(int **rooms, int *roomsPerFloor, int floors) {
    return rooms != nullptr && roomsPerFloor != nullptr && floors > 0;
}

void initializeHotel(int **&rooms, int *&roomsPerFloor, int &floors) {
    cout << "Enter total number of floors (>=1). Floors are 0-based (enter 0 for first): ";
    cin >> floors;
    if (floors <= 0) {
        cout << "Invalid number of floors.\n";
        floors = 0;
        return;
    }
    roomsPerFloor = new int[floors];
    rooms = new int*[floors];
    for (int i = 0; i < floors; ++i) {
        cout << "Enter room count for floor " << i << " : ";
        cin >> roomsPerFloor[i];
        if (roomsPerFloor[i] <= 0) roomsPerFloor[i] = 0;
        rooms[i] = new int[roomsPerFloor[i]];
        for (int j = 0; j < roomsPerFloor[i]; ++j)
            rooms[i][j] = 0;
    }
}

void loadHotelData(const char* filename, int** &rooms, int* &roomsPerFloor, int &floors) {
    ifstream fil(filename);
    if (!fil) {
        cout << "Hotel data file not found: " << filename << endl;
        return;
    }
    // free previous if any
    if (rooms) {
        for (int i = 0; i < floors; ++i) delete[] rooms[i];
        delete[] rooms;
        rooms = nullptr;
    }
    if (roomsPerFloor) {
        delete[] roomsPerFloor;
        roomsPerFloor = nullptr;
    }

    fil >> floors;
    if (!fil || floors <= 0) {
        cout << "Invalid file format or empty file.\n";
        floors = 0;
        return;
    }
    roomsPerFloor = new int[floors];
    rooms = new int*[floors];
    for (int i = 0; i < floors; ++i) {
        fil >> roomsPerFloor[i];
        if (roomsPerFloor[i] < 0) roomsPerFloor[i] = 0;
        rooms[i] = new int[roomsPerFloor[i]];
        for (int j = 0; j < roomsPerFloor[i]; ++j) {
            fil >> rooms[i][j];
            if (!fil) rooms[i][j] = 0;
        }
    }
    fil.close();
}

// Save room occupancy back to a file before program exit.
void saveHotelData(const char* filename, int** rooms, int *roomsPerFloor, int floors) {
    ofstream out(filename);
    if(!out) {
        cout << "Error: Cannot open file for writing!\n";
        return;
    }
    out << floors << "\n";
    for (int i = 0; i < floors; ++i) {
        out << roomsPerFloor[i] << "\n";
        for (int j = 0; j < roomsPerFloor[i]; ++j)
            out << rooms[i][j] << ' ';
        out << "\n";
    }
    out.close();
}

// To display hotel status
void displayHotelStatus(int** rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    int tfree = 0, toccup = 0, totalRooms = 0;
    cout << "Our Hotel has total " << floors << " floors\n";
    for (int i = 0; i < floors; ++i) {
        int freeR = 0, occup = 0;
        cout << "Floor " << i << ":\n";
        cout << "Total Rooms on this Floor: " << roomsPerFloor[i] << '\n';
        for (int j = 0; j < roomsPerFloor[i]; ++j) {
            cout << "Room " << (j + 1) << ": ";
            if (rooms[i][j]) {
                cout << "Occupied\t";
                occup++;
            } else {
                cout << "Free\t";
                freeR++;
            }
            totalRooms++;
        }
        tfree += freeR;
        toccup += occup;
        cout << "\nTotal Free Rooms on this floor: " << freeR << '\n';
        cout << "Total Occupied Rooms on this floor: " << occup << '\n' << endl;
    }
    cout << "Total rooms in hotel: " << totalRooms << '\n';
    cout << "Total free Rooms in hotel: " << tfree << '\n';
    cout << "Total occupied Rooms in hotel: " << toccup << "\n\n";
}

// Mark all rooms as free.
void resetHotel(int** rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    for (int i = 0; i < floors; ++i)
        for (int j = 0; j < roomsPerFloor[i]; ++j)
            rooms[i][j] = 0;
}

// Append a client record to a text file.
void addClientRecord(const char* filename, const char* name, int age, int floor, int roomNo) {
    ofstream out(filename, ios::app);
    if (!out) {
        cout << "Error: Cannot open file " << filename << " for writing.\n";
        return;
    }
    out << name << ' ' << age << ' ' << floor << ' ' << roomNo << "\n";
    out.close();
}

// Remove a client from the record file when they check out.
void removeClientRecord(const char* filename, int floorNo, int roomNo) {
    ifstream fil(filename);
    if(!fil) {
        cout << "Client file not found.\n";
        return;
    }
    ofstream out("temp.txt");
    string name;
    int age, floor, roomN;
    while (fil >> name >> age >> floor >> roomN) {
        if (roomNo != roomN || floorNo != floor) {
            out << name << ' ' << age << ' ' << floor << ' ' << roomN << "\n";
        }
    }
    fil.close();
    out.close();
    remove(filename);
    rename("temp.txt", filename);
}

// Show list of all clients with room numbers.
void displayAllClients(const char* filename, int* roomsPerFloor, int floors) {
    ifstream file(filename);
    if(!file) {
        cout << "Error reading file\n";
        return;
    }
    string name;
    int age, floor, roomN;
    while (file >> name >> age >> floor >> roomN) {
        if (floor >= 0 && floor < floors && roomN >= 1 && roomN <= roomsPerFloor[floor])
            cout << "Name: " << name << "\tAge: " << age << "\tFloor: " << floor << "\tRoom:" << roomN << "\n";
        else
            cout << "Name: " << name << "\tAge: " << age << "\tFloor: " << floor << "\tRoom:" << roomN << " (invalid/unknown)\n";
    }
    file.close();
}

// Check if a client record already exists for a given room.
bool isClientExist(const char* filename, int *roomsPerFloor, int floors, int Floor, int roomNo) {
    if (Floor < 0 || Floor >= floors || roomNo <= 0 || roomNo > roomsPerFloor[Floor]) {
        cout << "Invalid floor or room number\n";
        return false;
    }
    string name;
    int age, floor, roomN;
    ifstream fil(filename);
    if(!fil) return false;
    while (fil >> name >> age >> floor >> roomN) {
        if (roomNo == roomN && floor == Floor) {
            fil.close();
            return true;
        }
    }
    fil.close();
    return false;
}

// Edit client details in file.
void updateClientInfo(const char* filename, const char* newName, int newAge, int floorNo, int roomNo) {
    ifstream fil(filename);
    if(!fil) {
        cout << "Client file not found.\n";
        return;
    }
    ofstream out("temp.txt");
    string name;
    int age, floor, roomN;
    bool updated = false;
    while (fil >> name >> age >> floor >> roomN) {
        if (roomNo == roomN && floorNo == floor) {
            out << newName << ' ' << newAge << ' ' << floor << ' ' << roomN << "\n";
            updated = true;
        } else {
            out << name << ' ' << age << ' ' << floor << ' ' << roomN << "\n";
        }
    }
    fil.close();
    out.close();
    remove(filename);
    rename("temp.txt", filename);
    if (!updated) cout << "Warning: client record not found to update.\n";
}

// Check if a specific room is free. roomNo is 1-based.
bool isRoomFree(int** rooms, int floor, int roomNo, int *roomsPerFloor = nullptr, int floors = 0) {
    if (!rooms) {
        cout << "Hotel not initialized.\n";
        return false;
    }
    if (floor < 0 || floor >= floors) {
        cout << "Invalid floor.\n";
        return false;
    }
    if (roomNo <= 0 || roomNo > roomsPerFloor[floor]) {
        cout << "Invalid room number.\n";
        return false;
    }
    return (rooms[floor][roomNo - 1] == 0);
}

// Mark room as occupied.
void occupyRoom(int** &rooms, int* roomsPerFloor, int floors, int floor, int roomNo) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    if (floor < 0 || floor >= floors || roomNo <= 0 || roomNo > roomsPerFloor[floor]) {
        cout << "Invalid floor or room number\n";
        return;
    }
    rooms[floor][roomNo - 1] = 1;
    cout << "Room " << roomNo << " on Floor " << floor << " is occupied.\n";
}

// Mark room as free.
void freeRoom(int** &rooms, int* roomsPerFloor, int floors, int floor, int roomNo) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    if (floor < 0 || floor >= floors || roomNo <= 0 || roomNo > roomsPerFloor[floor]) {
        cout << "Invalid floor or room number\n";
        return;
    }
    rooms[floor][roomNo - 1] = 0;
    cout << "Room " << roomNo << " on Floor " << floor << " is free now.\n";
}

// Count available rooms on one floor.
int countFreeRoomsOnFloor(int** rooms, int *roomsPerFloor, int floor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors) || floor < 0 || floor >= floors) return 0;
    int freeRooms = roomsPerFloor[floor];
    for (int i = 0; i < roomsPerFloor[floor]; ++i)
        if (rooms[floor][i]) freeRooms--;
    return freeRooms;
}

// Count occupied rooms on one floor.
int countOccupiedRoomsOnFloor(int** rooms, int *roomsPerFloor, int floor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors) || floor < 0 || floor >= floors) return 0;
    int occupied = 0;
    for (int i = 0; i < roomsPerFloor[floor]; ++i)
        if (rooms[floor][i]) occupied++;
    return occupied;
}

// All free rooms in hotel
int totalFreeRooms(int **rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) return 0;
    int count = 0;
    for (int i = 0; i < floors; ++i)
        for (int j = 0; j < roomsPerFloor[i]; ++j)
            if (rooms[i][j] == 0)
                count++;
    return count;
}

// Total Occupied Rooms
int totalOccupiedRooms(int **rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) return 0;
    int count = 0;
    for (int i = 0; i < floors; ++i)
        for (int j = 0; j < roomsPerFloor[i]; ++j)
            if (rooms[i][j] == 1)
                count++;
    return count;
}

// To see free Rooms on a particular floor (asks user)
void listFreeRooms(int **rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    int floor;
    bool found = false;
    cout << "Free rooms on which floor? (0-based): ";
    cin >> floor;
    if (floor < 0 || floor >= floors) {
        cout << "Invalid floor.\n";
        return;
    }
    for (int i = 0; i < roomsPerFloor[floor]; ++i)
        if (rooms[floor][i] == 0) {
            cout << (i + 1) << endl;
            found = true;
        }
    if (!found)
        cout << "None\n";
}

// First free room in our hotel
void findFirstFreeRoom(int **rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    for (int i = 0; i < floors; ++i)
        for (int j = 0; j < roomsPerFloor[i]; ++j)
            if (rooms[i][j] == 0) {
                cout << "First Free Room:  Floor: " << i << " Room No: " << (j + 1) << endl;
                return;
            }
    cout << "No Free Room Available\n";
}

// Show the details of a particular room. roomNo is 1-based.
void showRoomDetails(const char* clientFile, int **rooms, int *roomsPerFloor, int floors, int floor, int roomNo) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    if (floor < 0 || floor >= floors) {
        cout << "Invalid floor number.\n";
        return;
    }
    if (roomNo <= 0 || roomNo > roomsPerFloor[floor]) {
        cout << "Invalid room number.\n";
        return;
    }
    cout << "Room " << roomNo << " on floor " << floor << endl;
    cout << "Status: " << (rooms[floor][roomNo - 1] == 0 ? "Free" : "Occupied") << endl;
    ifstream fin(clientFile);
    if (!fin) {
        cout << "Client file not found." << endl;
        return;
    }
    if (rooms[floor][roomNo - 1] == 0) {
        cout << "No client assigned." << endl;
        fin.close();
        return;
    }
    cout << "Client Info: " << endl;
    string name;
    int age, f, r;
    while (fin >> name >> age >> f >> r) {
        if (f == floor && r == roomNo) {
            cout << "Name: " << name << endl << "Age: " << age << endl;
            fin.close();
            return;
        }
    }
    fin.close();
    cout << "Client record not found in file" << endl;
}

// Saving into binary file
void saveClientDataBinary(const char* filename, const char* name, int age, int floor, int roomNo) {
    ofstream out(filename, ios::binary | ios::app);
    if (!out) {
        cout << "Error opening binary file." << endl;
        return;
    }
    int nameLen = (int)strlen(name);
    if (nameLen <= 0) {
        cout << "Invalid name.\n";
        out.close();
        return;
    }
    out.write((char*)&nameLen, sizeof(nameLen));
    out.write((char*)name, nameLen);
    out.write((char*)&age, sizeof(age));
    out.write((char*)&floor, sizeof(floor));
    out.write((char*)&roomNo, sizeof(roomNo));
    out.close();
}

// Loading from a binary file
void loadClientDataBinary(const char* filename) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Binary file not found." << endl;
        return;
    }
    while (true) {
        int nameLen = 0;
        if (!fin.read((char*)&nameLen, sizeof(int))) break;
        if (nameLen <= 0 || nameLen > 500) {
            cout << "Corrupted record: invalid name length (" << nameLen << ")\n";
            break;
        }
        char* name = new char[nameLen + 1];
        if (!fin.read(name, nameLen)) {
            cout << "Corrupted file: name read failed\n";
            delete[] name;
            break;
        }
        name[nameLen] = '\0';
        int age = 0, f = 0, r = 0;
        if (!fin.read((char*)&age, sizeof(age)) ||
            !fin.read((char*)&f, sizeof(f)) ||
            !fin.read((char*)&r, sizeof(r))) {
            cout << "Corrupted file: partial record\n";
            delete[] name;
            break;
        }
        cout << "Name: " << name
             << ", Age: " << age
             << ", Floor: " << f
             << ", Room: " << r
             << endl;
        delete[] name;
    }
    fin.close();
}

// Counting clients in a text file
int countClientsinFile(const char* filename) {
    ifstream fin(filename);
    if (!fin) return 0;
    int count = 0;
    string name;
    int age, floor, room;
    while (fin >> name >> age >> floor >> room) {
        count++;
    }
    fin.close();
    return count;
}

// Backing up a text file
void backupClientFile(const char* srcFile, const char* backupFile) {
    ifstream fin(srcFile);
    ofstream fout(backupFile);
    if (!fin || !fout) {
        cout << "File error." << endl;
        return;
    }
    string line;
    while (getline(fin, line)) {
        fout << line << endl;
    }
    fin.close();
    fout.close();
}

// Binary backup (generic)
void backup(const char* srcFile, const char* backupFile) {
    ifstream fin(srcFile, ios::binary);
    ofstream fout(backupFile, ios::binary);
    if (!fin) {
        cout << "Source file not found.\n";
        return;
    }
    if (!fout) {
        cout << "Backup file cannot be created.\n";
        return;
    }
    fout << fin.rdbuf();
    cout << "Backup created successfully: " << backupFile << "\n";
}

// Sorting the list
void sortClientsByName(const char* filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Client file not found." << endl;
        return;
    }
    Client arr[200];
    int count = 0;
    while (count < 200 && fin >> arr[count].name >> arr[count].age >> arr[count].floor >> arr[count].roomNo) {
        count++;
    }
    fin.close();
    for (int i = 0; i < count - 1; ++i)
        for (int j = 0; j < count - i - 1; ++j)
            if (arr[j].name > arr[j + 1].name)
                swap(arr[j], arr[j + 1]);
    ofstream fout(filename);
    for (int i = 0; i < count; ++i) {
        fout << arr[i].name << " " << arr[i].age << " " << arr[i].floor << " " << arr[i].roomNo << endl;
    }
    fout.close();
}

// Searching for a client by name
void searchClientByName(const char* filename, const char* target) {
    ifstream fin(filename);
    if (!fin) {
        cout << "File not found." << endl;
        return;
    }
    string name;
    int age, floor, room;
    bool found = false;
    while (fin >> name >> age >> floor >> room) {
        if (name == target) {
            cout << "Found: " << name << ", Age: " << age << ", Floor: " << floor << ", Room: " << room << endl;
            found = true;
        }
    }
    if (!found)
        cout << "Client not found." << endl;
    fin.close();
}

// Occupancy Rate
void calculateOccupancyRate(int **rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    int freeR = totalFreeRooms(rooms, roomsPerFloor, floors);
    int occR = totalOccupiedRooms(rooms, roomsPerFloor, floors);
    int total = freeR + occR;
    if (total == 0) {
        cout << "Hotel has no rooms." << endl;
        return;
    }
    double rate = (occR * 100.0) / total;
    cout << "Occupancy Rate: " << fixed << setprecision(2) << rate << "%\n";
}

// Floor Summary
void displayFloorSummary(int **rooms, int *roomsPerFloor, int floors) {
    if (!validHotelState(rooms, roomsPerFloor, floors)) {
        cout << "Hotel not initialized.\n";
        return;
    }
    cout << "FLOOR SUMMARY" << endl;
    for (int i = 0; i < floors; ++i) {
        int freeR = 0, occR = 0;
        for (int j = 0; j < roomsPerFloor[i]; ++j) {
            if (rooms[i][j] == 0) freeR++;
            else occR++;
        }
        cout << "Floor " << i << " => Free: " << freeR << ", Occupied: " << occR << endl;
    }
}

void clearBinaryFile(const char* filename) {
    ofstream out(filename, ios::binary);
    out.close();
}
void clear(const char* filename) {
    ofstream out(filename);
    out.close();
}

// ---------- Main ----------
int main() {
    int **rooms = nullptr;
    int *roomsPerFloor = nullptr;
    int floors = 0;

    const char clientFile[] = "clients.txt";
    const char hotelFile[]  = "hotel.txt";
    const char binClientFile[] = "clients.bin";
    int choice;

    while (true) {
        cout << "\n========== HOTEL MANAGEMENT SYSTEM ==========\n";
        cout << "1. Initialize Hotel\n";
        cout << "2. Load Hotel Data\n";
        cout << "3. Save Hotel Data\n";
        cout << "4. Display Hotel Status\n";
        cout << "5. Reset Hotel (free all rooms)\n";
        cout << "6. Add Client Record\n";
        cout << "7. Remove Client\n";
        cout << "8. Display All Clients\n";
        cout << "9. Update Client Info\n";
        cout << "10. Check Room Free?\n";
        cout << "11. Occupy Room\n";
        cout << "12. Free Room\n";
        cout << "13. List Free Rooms On Floor\n";
        cout << "14. First Free Room\n";
        cout << "15. Show Room Details\n";
        cout << "16. Sort Clients by Name\n";
        cout << "17. Search Client by Name\n";
        cout << "18. Occupancy Rate\n";
        cout << "19. Floor Summary\n";
        cout << "20. Clear Files Data\n";
        cout << "21. BackUp Files Data\n";
        cout << "22. Save Client Data to Binary File\n";
        cout << "23. Load Client Data from Binary File\n";
        cout << "24. Client Count in File\n";
        cout << "25. Do Client exist?\n";
        cout << "0. Exit\n";
        cout << "=============================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting... Goodbye!\n";
            // cleanup
            if (rooms) {
                for (int i = 0; i < floors; ++i) delete[] rooms[i];
                delete[] rooms;
                rooms = nullptr;
            }
            if (roomsPerFloor) {
                delete[] roomsPerFloor;
                roomsPerFloor = nullptr;
            }
            break;
        }

        switch (choice) {
            case 1:
                initializeHotel(rooms, roomsPerFloor, floors);
                break;

            case 2:
                loadHotelData(hotelFile, rooms, roomsPerFloor, floors);
                break;

            case 3:
                saveHotelData(hotelFile, rooms, roomsPerFloor, floors);
                break;

            case 4:
                displayHotelStatus(rooms, roomsPerFloor, floors);
                break;

            case 5:
                resetHotel(rooms, roomsPerFloor, floors);
                break;

            case 6: {
                string name;
                int age, f, r;
                cout << "Enter name age floor(0-based) room(1-based): ";
                cin >> name >> age >> f >> r;
                addClientRecord(clientFile, name.c_str(), age, f, r);
                occupyRoom(rooms, roomsPerFloor, floors, f, r);
                break;
            }

            case 7: {
                int f, r;
                cout << "Enter floor(0-based) room(1-based) to remove: ";
                cin >> f >> r;
                removeClientRecord(clientFile, f, r);
                freeRoom(rooms, roomsPerFloor, floors, f, r);
                break;
            }

            case 8:
                displayAllClients(clientFile, roomsPerFloor, floors);
                break;

            case 9: {
                string name;
                int age, f, r;
                cout << "Enter newName newAge floor(0-based) room(1-based): ";
                cin >> name >> age >> f >> r;
                updateClientInfo(clientFile, name.c_str(), age, f, r);
                break;
            }

            case 10: {
                int f, r;
                cout << "Enter floor(0-based) room(1-based): ";
                cin >> f >> r;
                cout << (isRoomFree(rooms, f, r, roomsPerFloor, floors) ? "Free\n" : "Occupied\n");
                break;
            }

            case 11: {
                int f, r;
                cout << "Enter floor(0-based) room(1-based): ";
                cin >> f >> r;
                occupyRoom(rooms, roomsPerFloor, floors, f, r);
                break;
            }

            case 12: {
                int f, r;
                cout << "Enter floor(0-based) room(1-based): ";
                cin >> f >> r;
                freeRoom(rooms, roomsPerFloor, floors, f, r);
                break;
            }

            case 13:
                listFreeRooms(rooms, roomsPerFloor, floors);
                break;

            case 14:
                findFirstFreeRoom(rooms, roomsPerFloor, floors);
                break;

            case 15: {
                int f, r;
                cout << "Enter floor(0-based) room(1-based): ";
                cin >> f >> r;
                showRoomDetails(clientFile, rooms, roomsPerFloor, floors, f, r);
                break;
            }

            case 16:
                sortClientsByName(clientFile);
                break;

            case 17: {
                string target;
                cout << "Enter name: ";
                cin >> target;
                searchClientByName(clientFile, target.c_str());
                break;
            }

            case 18:
                calculateOccupancyRate(rooms, roomsPerFloor, floors);
                break;

            case 19:
                displayFloorSummary(rooms, roomsPerFloor, floors);
                break;

            case 20:
                clear(hotelFile);
                clear(clientFile);
                clearBinaryFile(binClientFile);
                cout << "Files cleared.\n";
                break;

            case 21:
                backup(hotelFile, "hotel_backup.txt");
                backup(clientFile, "clients_backup.txt");
                break;

            case 22: {
                string name;
                int age, floor, roomNo;
                cout << "Enter name age floor(0-based) room(1-based): ";
                cin >> name >> age >> floor >> roomNo;
                saveClientDataBinary(binClientFile, name.c_str(), age, floor, roomNo);
                break;
            }
            case 23:
                loadClientDataBinary(binClientFile);
                break;
            case 24: {
                int count = countClientsinFile(clientFile);
                cout << "Total clients in file: " << count << endl;
                break;
            }
            case 25: {
                int floor, roomNo;
                cout << "Enter floor(0-based) and room number(1-based) to check: ";
                cin >> floor >> roomNo;
                if (isClientExist(clientFile, roomsPerFloor, floors, floor, roomNo))
                    cout << "Client exists for room " << roomNo << " on floor " << floor << endl;
                else
                    cout << "No client for room " << roomNo << " on floor " << floor << endl;
                break;
            }

            default:
                cout << "Invalid choice!\n";
        }
    }

    return 0;
}
