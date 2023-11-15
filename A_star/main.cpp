#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include "./graph.cpp"
using namespace std;

int main(int argc, const char * argv[]) {
    ifstream image;
    ofstream solvedImage;
    Cell* maze;
    chrono::milliseconds start, finish;
    image.open("braid200.ppm");
    solvedImage.open("solved.ppm");
    string sometin;
    
    int height, width, number;
    int r, g, b;   
    image>>sometin>>height>>width>>number;
    cout<<sometin<<" "<<height<<" "<<width<<" "<<number<<endl;
    
    maze = (Cell*) malloc(sizeof(Cell)*height*width);
    if(maze == NULL){
        cout<<"error 404"<<endl;
        exit(1);
    }
    for(int i=0; i<height; i++){
        for (int j = 0; j < width; j++)
        {
            image>>r>>g>>b;
            maze[i*height + j].setColor(r, g, b);
            maze[i*height + j].setCoord(i, j);
        }
    }
    image.close();

    start = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
    );
    
    Graph test = Graph(maze, height, width);
    //test.printNodes();
    
    test.solve();
    finish = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
    );

    printf("Execution time: %ld\n", (long)finish.count()-start.count());


    solvedImage<<sometin<<" "<<height<<" "<<width<<" "<<number<<endl;
    for(int i=0; i<height; i++){
        for (int j = 0; j < width; j++)
        {
            color clr = maze[i*height+j].getColor();

            solvedImage<<clr.red<<endl;
            solvedImage<<clr.green<<endl;
            solvedImage<<clr.blue<<endl;
        }
    }

    solvedImage.close();
    return 0;
}