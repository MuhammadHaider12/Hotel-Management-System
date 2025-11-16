#include<iostream>
#include<fstream>
using namespace std;
void initializeHotel(int **rooms,int *roomsPerFloor,int &floors ){
    cout<<"Enter total number of Floor: ";
    cin>>floors;
    roomsPerFloor = new int [floors];
    rooms = new int*[floors];
    int i,j;
    for(i=0;i<floors;i++){
        cout<<"Enter room for floor "<< i << ": ";
        cin>>roomsPerFloor[i];
        rooms[i] = new int[roomsPerFloor[i]];
        for(j=0;j<roomsPerFloor[i];j++)
            rooms[i][j] = 0;
    }
        
}
void loadHotelData(int** rooms,  int *roomsPerFloor,int floors, const char* filename){
    ifstream fil(filename);
    int i,j;
    fil>>floors;
    for(i=0;i<floors;i++){
        fil>>roomsPerFloor[i];
        for(j=0;j<roomsPerFloor[i];j++)
            fil>>rooms[i][j];
    }
}
//  Save room occupancy back to a file before program exit.
void saveHotelData(int** rooms, int *roomsPerFloor, int floors, const char* filename){
      ofstream out(filename);
      out<<floors;
      out<<"\n";
      int i,j;
      for(i=0;i<floors;i++){
        out<<roomsPerFloor[i]<<endl;
        for(j=0;j<roomsPerFloor[i];j++)
            out<<rooms[i][j]<<' ';
      }
}

// → Display all floors and rooms with their occupancy status.

void displayHotelStatus(int** rooms, int *roomsPerFloor, int floors){
    int i,j,tfree=0,toccup=0,room=0;
    cout<<"Our Hotel has total "<<floors<<" floors"<<endl;
    for(i=0;i<floors;i++){
        int free=0,occup=0;
        cout<<"Floor "<<i+1<<": "<<endl;
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
        cout<<"Total Free Rooms on this floor: "<<free<<endl;
        cout<<"Total Occupied Rooms on this floor: "<<occup<<endl;
        cout<<endl;
    }
    cout<<endl;
    cout<<"Total rooms in hotel: "<<room<<endl;
    cout<<"Total free Rooms in hotel: "<<tfree<<endl;
    cout<<"Total occupied Rooms in hotel: "<<toccup<<endl;
}
// → Mark all rooms as free.
void resetHotel(int** rooms, int *roomsPerFloor, int floors){
    int i,j;
    for(i=0;i<floors;i++){
        for(j=0;j<roomsPerFloor[i];i++)
            rooms[i][j] = 0;
    }

}

int main(){
      int **rooms;
      int *roomsPerFloor;
      int floors;
      



    return 0;
}