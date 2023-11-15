#include <utility> 
#include <iostream>
#include <set>
#include <float.h>

using namespace std;

typedef pair<int, int> Pair;

struct color {
    int red;
    int green;
    int blue;
};

class Cell {
    private:
        color clr;
        bool wall;
        Pair coord;
        bool node_type = false;
    public:
        bool isWall() {
            return wall;
        }

        color getColor() {
            return clr;
        }

        Pair getCoordinates() {
            return coord;
        }

        bool isNode() {
            return node_type;
        }

        void setColor(int r, int g, int b) {
            clr = color{r, g, b};
            wall = (r==0 && g==0 && b==0);
        }

        void setCoord(int x, int y) {
            coord = Pair(x, y);
        }

        void node() {
            node_type = true;
        }

        
};

class Node{
    private:
        Node* parent;
        Pair coordinates;
        Node** neighbours;
        int neighbour_count = 0;
        double f, g, h;
    public:
        Node() {
            f = DBL_MAX;
            g = DBL_MAX;
            neighbours = (Node**)malloc(sizeof(Node*)*4);
            if(neighbours == NULL) {
                printf("malloc error");
                exit(1);
            }
        }


        Node** getNeighbours() {
            return neighbours;
        }

        Pair getCoord() {
            return coordinates;
        }

        void setCoord(Pair c) {
            coordinates = c;
        }

        void setParent(Node* c) {
            parent = c;
        }
 
        double getF() {
            return f;
        }

        double getG() {
            return g;
        }

        Node* getParent() {
            return parent;
        }

        void setF(double n) {
            f = n;
        }

        void setG(double n) {
            g = n;
        }

        void addAdj (Node* n) {
            neighbours[neighbour_count] = n;
            neighbour_count++;
        }

        bool operator<(const Node& b) const {
            return f < b.f;
        }

        int neighbour_cnt() {
            return neighbour_count;
        }

};

class Graph{
    private:
        Node start, end;
        int height, width;
        int node_n = 0;
        Cell* maze;
        Node* graph;
    public:
        Graph(Cell* cells, int h, int w) {
            //  Set maze variables
            maze = cells;
            height = h;
            width = w;

            //  Find start and end
            for(int i=0; i<width; i++) {
                if(!maze[i].isWall()) {
                    start.setCoord(maze[i].getCoordinates());
                    maze[i].node();
                }
                if(!maze[width*(height-1)+i].isWall()) {
                    end.setCoord(maze[width*(height-1) + i].getCoordinates());
                    maze[width*(height-1) + i].node();
                }
            }

            //  Scan the maze for nodes
            for(int i=1; i<height-1; i++) {
                for(int j=1; j<width-1; j++) {
                    //  If the cell is in intersection add it as a node
                    if(!onPath(maze[i*width + j]) && !maze[i*height+j].isWall()) {
                        node_n++;
                        maze[i*height + j].node();
                    }
                }
            }
            //  Allocate space for nodes (+2 is for the starting and ending nodes)
            graph = (Node*) malloc(sizeof(Node)*(node_n+2));
            if(graph == NULL) {
                printf("error");
            }

            graph[0] = start;

            int idunnowhatimdoing;

            node_n = 1;
            for(int i=1; i<height-1; i++) {
                for(int j=1; j<width-1; j++) {
                    if(maze[i*width+j].isNode()) {
                        graph[node_n] = Node();
                        graph[node_n].setCoord(maze[i*width+j].getCoordinates());
                        node_n++;

                        //  Add left and up neighbour if exists
                        for(int k=j-1; k>0; k--) {
                            if(maze[i*height + k].isWall()){
                                break;
                            }
                            if(maze[i*height + k].isNode()) {
                                graph[node_n-1].addAdj(&graph[node_n-2]);
                                graph[node_n-2].addAdj(&graph[node_n-1]);
                                break;
                            }
                        }
                        for(int k=i-1; k>=0; k--) {
                            if(maze[k*height + j].isWall()) {
                                break;
                            }
                            if(maze[k*height + j].isNode()) {
                                for(int l=node_n-1; l>=0; l--) {
                                    if(graph[l].getCoord().first == k && graph[l].getCoord().second == j){
                                        graph[node_n-1].addAdj(&graph[l]);
                                        graph[l].addAdj(&graph[node_n-1]);
                                        break;
                                    } 
                                }
                                break;
                            }
                        }
                        
                    }
                }
            }
            for(int i=end.getCoord().first-1; i>=0; i--) {
                if(maze[i*height+end.getCoord().second].isNode()) {
                    for(int j=0; j<node_n; j++) {
                        if(graph[j].getCoord().first == i && graph[j].getCoord().second == end.getCoord().second) {
                            end.addAdj(&graph[j]);
                            end.setParent(&graph[j]);
                            idunnowhatimdoing = j;
                            break;
                        }
                    }
                    break;
                }
            }
            graph[node_n] = end;
            graph[idunnowhatimdoing].addAdj(&graph[node_n]);
            node_n++;
        }

        bool onPath(Cell c) {
            int x=c.getCoordinates().first, y=c.getCoordinates().second;
            bool up_wall = maze[(x+1)*height + y].isWall();
            bool down_wall = maze[(x-1)*height + y].isWall();
            bool left_wall = maze[x*height + y - 1].isWall();
            bool right_wall = maze[x*height + y + 1].isWall();
            bool center_wall = maze[x*height+y].isWall();
            
            bool check_horizontal = up_wall && down_wall && !left_wall && !right_wall;
            bool check_vetical = !up_wall && !down_wall && left_wall && right_wall;
            
            return (check_horizontal != check_vetical) && !center_wall;
        }

        void printNodes() {
            cout<<node_n<<endl;
            for(int i=0; i<node_n; i++) {
                printf("Node: %d: x: %d, y: %d\nNeighbours:\t", i+1, graph[i].getCoord().first, graph[i].getCoord().second);
                for(int j=0;j<graph[i].neighbour_cnt();j++) {
                    Node* neighbour = graph[i].getNeighbours()[j];
                    printf("%d|%d, ", neighbour->getCoord().first, neighbour->getCoord().second);
                }
                cout<<endl;

            }
        }

        void solve() {
            bool closedSet[height][width];
            memset(closedSet, false, sizeof(closedSet));

            set<Node*> openSet;
            

            graph[0].setG(0);
            graph[0].setF(0);

            openSet.emplace(&graph[0]);
            
            while(!openSet.empty()) {
                Node* n = *openSet.begin();

                if(n -> getCoord() == end.getCoord()) {
                    printPath(n);
                }

                openSet.erase(openSet.begin());

                // printf("Expanding %d|%d\n", n->getCoord().first, n->getCoord().second);
                

                for(int i=0; i<n -> neighbour_cnt(); i++) {
                    double tentative_g;
                    Node* neighbour = n -> getNeighbours()[i];

                    tentative_g = n -> getG() + distance(*n, *neighbour);
                    // if(n->getCoord().first == 9 && n->getCoord().second == 35){
                    //     printf("tentative: %f, neighbour g: %f\t %d|%d\n", tentative_g, neighbour->getG(), neighbour->getCoord().first, 
                    //     neighbour->getCoord().second);
                    // }
                    if(tentative_g < neighbour -> getG()) {
                        neighbour -> setParent(n);
                        neighbour -> setG(tentative_g);
                        neighbour -> setF(tentative_g + distance(*neighbour, end));
                        if(!closedSet[neighbour -> getCoord().first][neighbour -> getCoord().second]) {
                            openSet.emplace(neighbour);
                            closedSet[neighbour -> getCoord().first][neighbour -> getCoord().second] = true;
                        }
                    }
                }

            }
        }

        int distance(Node a, Node b) {
            return abs(a.getCoord().first - b.getCoord().first) + abs(a.getCoord().second - b.getCoord().second);
        }

        void printPath(Node* n) {

            while(n != &graph[0]) {
                trace_path(n, n->getParent());
                
                n = n->getParent();
                }
        }

        void trace_path(Node* n, Node* parent) {
            int i, j, x, y;
            i = n->getCoord().first;
            j = n->getCoord().second;
            x = parent->getCoord().first;
            y = parent->getCoord().second;

            if(i<x) {
                for(int n=i; n<=x; n++){
                    maze[n*height+j].setColor(255, 0, 0);
                }
            }else {
                for(int n=x; n<=i; n++){
                    maze[n*height+j].setColor(255, 0, 0);
                }
            }
            if(j<y) {
                for(int n=j; n<=y; n++){
                    maze[i*height+n].setColor(255, 0, 0);
                }
            }else {
                for(int n=y; n<=j; n++){
                    maze[i*height+n].setColor(255, 0, 0);
                }
            }
        }

};


