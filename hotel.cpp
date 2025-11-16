#include<iostream>
#include<fstream>
using namespace std;
void initializeHotel(int **room,int *roomPerFloor,int &floor ){
    cout<<"Enter total number of Floor: ";
    cin>>floor;
    int i,j;
    for(i=0;i<floor;i++){
        cout<<"Enter room for floor "<< i << ": ";
        cin>>roomPerFloor[i];
        for(j=0;j<roomPerFloor[i];j++)
            room[i][j] = 0;
    }
        
}
void loadHotelData(int** rooms,  int *roomsPerFloor,int floors, const char* filename){
    ifstream fil(filename);
    int i,j;
    fil>>floors;
    for(i=0;i<floors;i++){
        fil>>roomsPerFloor[i];
        for(j=0;j<)
    }
        
    



}
int main(){
      int **rooms;
      int *roomsPerFloor;
      int floors;
      



    return 0;
}