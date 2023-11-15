//
//  main.cpp
//  maze
//
//  Created by Liutauras Kuolis on 14/07/2022.
//
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

int endX, endY;
int height, width;

enum Status {undiscovered, discovered, explored};

class Cell
{
private:
    bool black;
    int x, y;
    int red, green, blue;
    Status status;
    int predX=0, predY=0;
    
public:
    void setCell(int a, int b, int c, int i, int j){
        black = a==0 && b==0 && c==0;
        red = a;
        green = b;
        blue = c;
        x = j;
        y = i;
        status = undiscovered;
    }

    int getPredX(){
        return predX;
    }

    int getPredY(){
        return predY;
    }

    Status getStatus(){return status;}

    void setPred(int pX, int pY){
        predX = pX;
        predY = pY;
    }

    void setRed(){
        red = 255;
        blue = 0;
        green = 0;
    }

    int getRed(){
        return red;
    }

    int getGreen(){
        return green;
    }

    int getBlue(){
        return blue;
    }


    void addNeighbors(queue<Cell>& Q, Cell* maze){
        if(y==0){
            
            maze[(y+1)*height + x].setPred(this -> getX(), this -> getY());
            maze[(y+1)*height + x].setRed();
            maze[(y+1)*height + x].setStatus(discovered);
            Q.push(maze[(y+1)*height + x]);
        }else{
            if(!maze[(y-1)*height+x].isWall()){
                if (maze[(y-1)*height + x].getStatus() == undiscovered)
                {
                    
                    maze[(y-1)*height + x].setPred(this -> getX(), this -> getY());
                    maze[(y-1)*height + x].setStatus(discovered);
                    Q.push(maze[(y-1)*height + x]);
                }
            }
            if(!maze[(y+1)*height+x].isWall()){
                if (maze[(y+1)*height + x].getStatus() == undiscovered)
                {
                    
                    maze[(y+1)*height + x].setPred(this -> getX(), this -> getY());
                    maze[(y+1)*height + x].setStatus(discovered);
                    Q.push(maze[(y+1)*height + x]);
                }
            }
            if(!maze[y*height + x +1].isWall()){
                if (maze[y*height + x+1].getStatus() == undiscovered)
                {
                    
                    maze[y*height + x +1].setPred(this -> getX(), this -> getY());
                    maze[y*height + x +1].setStatus(discovered);
                    Q.push(maze[y*height + x +1]);
                }
            }
            if(!maze[y*height + x - 1].isWall()){
                if (maze[y*height + x - 1].getStatus() == undiscovered)
                {
                    
                    maze[y*height + x - 1].setPred(this -> getX(), this -> getY());
                    maze[y*height + x - 1].setStatus(discovered);
                    Q.push(maze[y*height + x -1]);
                }
            }
        }
        
        
    }

    void setStatus(Status s) {status = s;}

    bool isWall() {return black;}

    int getX() {return x;}

    int getY() {return y;}
};

int main(int argc, const char * argv[]) {
    ifstream image;
    ofstream solvedImage;
    Cell* maze;
    Cell start, end;
    chrono::milliseconds tStart, tFinish;
    queue<Cell> Q;
    
    image.open("braid200.ppm");
    solvedImage.open("tiny_solved.ppm");
    string sometin;
    int number;
    int red, green, blue;
    Cell test;
    test.setCell(0, 0, 0, 0, 3);
    cout<<test.getGreen()<<endl;
    image>>sometin>>height>>width>>number;
    cout<<sometin<<height<<width<<number<<endl;
    maze = (Cell*) malloc(sizeof(Cell)*height*width);
    if(maze == NULL){
        cout<<"error 404"<<endl;
        exit(1);
    }
    for(int i=0; i<height; i++){
        for (int j = 0; j < width; j++)
        {
            image>>red>>green>>blue;
            maze[i*height+j].setCell(red, green, blue, i, j);
            if(i==0 && !maze[i*height+j].isWall()){
                start=maze[i*height+j];
                maze[i*height+j].setRed();
            }else if(i == height-1 && !maze[i*height+j].isWall()){
                end=maze[i*height+j];
                endX = j;
                endY = i;
            }
        }
    }
    tStart = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
    );

    start.setPred(0, 0);
    Q.push(start);

    while(!Q.empty()){
        Cell current = Q.front();
        if (current.getX() == endX && current.getY() == endY){
            
            while(current.getPredX() != 0 && current.getPredY() != 0){
                maze[current.getY()*height + current.getX()].setRed();
                current = maze[current.getPredY()*height + current.getPredX()];
            }
            while(!Q.empty()){
                Q.pop();
            }
        }else{
            current.addNeighbors(Q, maze);
            Q.pop();
            current.setStatus(explored);
        }
        
        
        
        
    }
    tFinish = chrono::duration_cast< chrono::milliseconds >(
        chrono::system_clock::now().time_since_epoch()
    );

    solvedImage<<sometin<<"\n";
    solvedImage<<height<<" "<<width<<"\n";
    solvedImage<<number<<"\n";
    for(int i=0; i<height; i++){
        for (int j = 0; j < width; j++)
        {
            solvedImage<<maze[i*height+j].getRed()<<" "<<maze[i*height+j].getGreen()<<" "<<maze[i*height+j].getBlue()<<"\n";
            
        }
    }
    

    printf("Execution time: %ld\n", (long)tFinish.count()-tStart.count());

    

    return 0;
    
}
