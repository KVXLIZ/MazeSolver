#include <iostream>
#include <tuple>
#include <list>
#include "./cell.cpp"

using namespace std;

int height, width;

class Node{
    private:
        bool null = true;
        tuple<int, int> coordinates;
        int distance;
        list<Node*> adj;
    public:
        void addAdj(Node* n){
            adj.emplace_back(n);
        } 

        Node() {}
        Node(int x, int y){
            coordinates = make_tuple(x, y);
            null = false;
        }
        
        tuple<int, int> getCoordinates() {return coordinates;}
        int getD() {return distance;}

        


};

class Edge{
    private:
        tuple<Node*, Node*> nodes;
        int dist;
    public:
        Edge(Node* u, Node* v){
            nodes = make_tuple(u, v);
            dist = get<0>(v -> getCoordinates()) - get<0>(u -> getCoordinates()) + get<1>(v -> getCoordinates()) - get<1>(u -> getCoordinates());
        }
        tuple <Node*, Node*> getNodes(){
            return nodes;
        }
        int getDist(){
            return dist;
        }
};

class Graph{
    private:
        Node start;
        Node end;
        list<Node> nodes;
        list<Edge> edges;
        Cell* maze;
    public:
        Graph(Cell* m, int h, int w){
            height = h;
            width = w;
            maze = m;
            for(int i = 0; i < width; i++){
                
                if(!m[i].isWall()){
                    start = Node(0, i);
                }
                if(!m[height * (height - 1) + i].isWall()){
                    end = Node(height - 1, i);
                }
            }
            for (int i = 1; i < height-1; i++)
            {
                for (int j = 1; j < width-1; j++)
                {
                
                    
                    if (!onPath(m, i, j))
                    {
                        Node n = Node(i, j);
                        Node* up_node = upNode(i, j);
                        Node* left_node = leftNode(i, j);
                        if (up_node == NULL)
                        {
                            
                        }else{
                            n.addAdj(up_node);
                            edges.emplace_back(up_node, &n);
                        }
                        if (left_node == NULL)
                        {
                            
                        }else{
                            n.addAdj(up_node);
                            edges.emplace_back(up_node, &n);
                        }
                        nodes.emplace_back(n);
                        cout<<get<0>(n.getCoordinates())<<" "<<get<1>(n.getCoordinates())<<endl;
                        
                    
                    }
                }
                
            }
        }

        Node* upNode(int x, int y){
            int max = -1;
            Node* upNode = NULL;
            for (size_t i = 0; i < nodes.size(); i++)
            {
                if (get<1>(nodes.front().getCoordinates()) == y && 
                get<0>(nodes.front().getCoordinates()) < x && 
                get<0>(nodes.front().getCoordinates()) > max)
                {
                    upNode = &nodes.front();
                    max = get<0>(nodes.front().getCoordinates());
                }
                nodes.emplace_back(nodes.front());
                nodes.pop_front();
                
            }
            return upNode;
            
        }

        Node* leftNode(int x, int y){
            int max = -1;
            Node* left_node = NULL;
            for (size_t i = 0; i < nodes.size(); i++)
            {
                if (get<1>(nodes.front().getCoordinates()) < y && 
                get<0>(nodes.front().getCoordinates()) == x && 
                get<1>(nodes.front().getCoordinates()) > max)
                {
                    left_node= &nodes.front();
                    max = get<1>(nodes.front().getCoordinates());
                }
                nodes.emplace_back(nodes.front());
                nodes.pop_front();
                
            }
            return left_node;
            
        }

        void printNodes(){
            for (size_t i = 0; i < nodes.size(); i++)
            {
                cout<<get<0>(nodes.front().getCoordinates())<<" "<<get<1>(nodes.front().getCoordinates())<<endl;
                nodes.emplace_back(nodes.front());
                nodes.pop_front();
            }
            
        }

        bool onPath(Cell* maze, int i, int j){
            int x = j;
            int y = i;
            return (maze[(y+1)*height + x].isWall() && maze[(y-1)*height + x].isWall() ||
                maze[y*height + x + 1].isWall() && maze[y*height + x - 1].isWall()) && 
                !maze[y*height + x].isWall();
        }
        
            
};

        




