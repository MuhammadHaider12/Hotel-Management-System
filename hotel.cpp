#include<iostream>
#include<fstream>
using namespace std;
void initializeHotel(int **rooms,int *roomPerFloor,int &floor ){
    cout<<"Enter total number of Floor: ";
    cin>>floor;
    int i,j;
    for(i=0;i<floor;i++){
        cout<<"Enter room for floor "<< i << ": ";
        cin>>roomPerFloor[i];
        for(j=0;j<roomPerFloor[i];j++)
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
void saveHotelData(int** rooms, int floors, int *roomsPerFloor, const char* filename){
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


void displayHotelStatus(int** rooms, int floors, int roomsPerFloor);
// → Display all floors and rooms with their occupancy status.

void resetHotel(int** rooms, int floors, int roomsPerFloor);
// → Mark all rooms as free.
int main(){
      int **rooms;
      int *roomsPerFloor;
      int floors;
      



    return 0;
}