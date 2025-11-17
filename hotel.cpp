#include<iostream>
#include<fstream>
using namespace std;
void initializeHotel(int **&rooms,int *&roomsPerFloor,int &floors ){
    cout<<"Enter total number of Floor: ";
    cin>>floors;
    roomsPerFloor = new int [floors];
    rooms = new int*[floors];
    int i,j;
    for(i=0;i<floors;i++){
        cout<<"Enter room for floor "<< i+1 << ": ";
        cin>>roomsPerFloor[i];
        rooms[i] = new int[roomsPerFloor[i]];
        for(j=0;j<roomsPerFloor[i];j++)
            rooms[i][j] = 0;
    }
        
}
void loadHotelData(int** &rooms,  int* &roomsPerFloor,int &floors, const char* filename){
    ifstream fil(filename);
    int i,j;
    fil>>floors;
    roomsPerFloor = new int[floors];
    rooms = new int*[floors];
    for(i=0;i<floors;i++){
        fil>>roomsPerFloor[i];
        rooms[i] = new int[roomsPerFloor[i]];
        for(j=0;j<roomsPerFloor[i];j++)
            fil>>rooms[i][j];
    }
}
//  Save room occupancy back to a file before program exit.
void saveHotelData(int** rooms, int *roomsPerFloor, int floors, const char* filename){
      ofstream out(filename);
      if(!out) {
        cout << "Error: Cannot open file for writing!\n";
        return;
    }
      out<<floors<<"\n";
           int i,j;
      for(i=0;i<floors;i++){
        out<<roomsPerFloor[i]<<"\n";
        for(j=0;j<roomsPerFloor[i];j++)
            out<<rooms[i][j]<<' ';
         out<<"\n";
    }
}
     

// → Display all floors and rooms with their occupancy status.

void displayHotelStatus(int** rooms, int *roomsPerFloor, int floors){
    int i,j,tfree=0,toccup=0,room=0;
    cout<<"Our Hotel has total "<<floors<<" floors"<<endl;
    for(i=0;i<floors;i++){
        int free=0,occup=0;
        cout<<"Floor "<<(i+1)<<": "<<endl;
        cout<<"Total Rooms on this Floor: "<<roomsPerFloor[i]<<endl;
        for(j=0;j<roomsPerFloor[i];j++){
            cout<<"Room "<<j+1<<": ";
            if(rooms[i][j]){
                cout<<"Occupied\t";
                occup++;
            }
            else{
                cout<<"Free\t";
                free++;
            }
            room++;
        }
        tfree+=free;
        toccup+=occup;
        cout<<"\nTotal Free Rooms on this floor: "<<free<<endl;
        cout<<"Total Occupied Rooms on this floor: "<<occup<<endl;
        cout<<endl;
    }
    cout<<endl;
    cout<<"Total rooms in hotel: "<<room<<endl;
    cout<<"Total free Rooms in hotel: "<<tfree<<endl;
    cout<<"Total occupied Rooms in hotel: "<<toccup<<endl<<endl;
}
// → Mark all rooms as free.
void resetHotel(int** rooms, int *roomsPerFloor, int floors){
    int i,j;
    for(i=0;i<floors;i++){
        for(j=0;j<roomsPerFloor[i];j++)
            rooms[i][j] = 0;
    }
}

// → Append a client record to a text file.
void addClientRecord(const char* filename, const char* name, int age, int floor, int roomNo){
        ofstream out(filename,ios::app);
        if (!out) {
            cout << "Error: Cannot open file " << filename << " for writing.\n";
            return;
        }
        out<<name<<' '<<age<<' '<<floor<<' '<<roomNo<<"\n";
       out.close();
}

// → Remove a client from the record file when they check out.
void removeClientRecord(const char* filename,int floorNo,int roomNo){
    string name;
    int age,floor,roomN;
    ifstream fil(filename);
    ofstream out("temp.txt");
    while(fil>>name>>age>>floor>>roomN){
        if(roomNo!=roomN || floorNo!=floor){
            out<<name<<' '<<age<<' '<<floor<<' '<<roomN<<"\n";
        }
    }
    fil.close();
    out.close();
    remove(filename);
    rename("temp.txt",filename);
}

// → Show list of all clients with room numbers.
void displayAllClients(const char* filename){
    string name;
    int age,floor,roomN;
    ifstream file(filename);
    if(!file){
        cout<<"Error reading file\n";
        return;
    }
    while(file>>name>>age>>floor>>roomN){
            cout<<"Name: "<<name<<"\tAge: "<<age<<"\tFloor: "<<floor<<"\tRoom:"<<roomN<<"\n";   
    }

}

// → Check if a client record already exists for a given room.
bool isClientExist(const char* filename, int roomNo){
    string name;
    int age,floor,roomN;
    ifstream fil(filename);
    while(fil>>name>>age>>floor>>roomN){
        if(roomNo==roomN){
            return true;
        }
    }
    fil.close();
    return false;
}

// → Edit client details in file.
void updateClientInfo(const char* filename,int floorNo, int roomNo,const char* newName, int newAge){
    string name;
    int age,floor,roomN;
    ifstream fil(filename);
    ofstream out("temp.txt");
    while(fil>>name>>age>>floor>>roomN){
        if(roomNo == roomN && floorNo == floor){
            out<<newName<<' '<<newAge<<' '<<floor<<' '<<roomN<<"\n";
        }
        else out<<name<<' '<<age<<' '<<floor<<' '<<roomN<<"\n";
    }
    fil.close();
    out.close();
    remove(filename);
    rename("temp.txt",filename);

}
// → Check if a specific room is free.
bool isRoomFree(int** rooms, int floor, int roomNo){
    if(rooms[floor][roomNo-1]) return false;
    return true;
}

// → Mark room as occupied.
void occupyRoom(int** &rooms, int floor, int roomNo){
    rooms[floor][roomNo-1] = 1;
    cout<<"Room "<<roomNo<<" on Floor "<<floor <<" is occupied "<<endl; 
}

// → Mark room as free.
void freeRoom(int** &rooms, int floor, int roomNo){
     rooms[floor][roomNo-1] = 0;
    cout<<"Room "<<roomNo<<" on Floor "<<floor <<" is free now "<<endl; 
}

// → Count available rooms on one floor.
int countFreeRoomsOnFloor(int** rooms, int floor, int *roomsPerFloor){
    int i,freeRooms = roomsPerFloor[floor];
    for(i=0;i<roomsPerFloor[floor];i++){
        if(rooms[floor][i]) freeRooms--;
    }
    return freeRooms;
}

// → Count occupied rooms on one floor.
int countOccupiedRoomsOnFloor(int** rooms, int floor, int *roomsPerFloor){
     int i,OccupiedRooms = 0;
    for(i=0;i<roomsPerFloor[floor];i++){
        if(rooms[floor][i]) OccupiedRooms++;
    }
    return OccupiedRooms;
}

int main(){
      int **rooms;
      int *roomsPerFloor;
      int floors;
    //   initializeHotel(rooms,roomsPerFloor,floors);
    //   displayHotelStatus(rooms,roomsPerFloor,floors);
    //   saveHotelData(rooms,roomsPerFloor,floors,"hotel.txt");
    //   displayHotelStatus(rooms,roomsPerFloor,floors);
    //   loadHotelData(rooms,roomsPerFloor,floors,"hotel.txt");
    //   displayHotelStatus( rooms,roomsPerFloor,floors);
    //   addClientRecord("client.txt","Haider",19,2,4);
    //   addClientRecord("client.txt","Shaigan",14,1,4);
      
    //   if(isClientExist("client.txt",4)) cout<< "Room not free";
    //  updateClientInfo("client.txt",4,"ali rehman",90);
     // removeClientRecord("client.txt",4,1);
    // displayAllClients("client.txt");
    //  addClientRecord("client.txt","Haider",19,2,4);
    //  addClientRecord("client.txt","Huda",19,1,4);
  
    
    // updateClientInfo("client.txt",2,4,"Ha",12);
    //    displayAllClients("client.txt");

    return 0;
}