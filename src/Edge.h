#ifndef EDGE_H
#define EDGE_H
#include <iostream>


#include <iostream>
#include <string>

class Edge {
private:


public:

    int vertex_A;
    int vertex_B;
    int weight;
    int row;
    int column;
    std::string direction;
    int id;

 Edge() = default; // Default constructor
    Edge(int vertex_A, int vertex_B, int weight, int id, int row, int column, std::string direction)
        : vertex_A(vertex_A), vertex_B(vertex_B), weight(weight), id(id), row(row), column(column), direction(direction) {}

    bool operator==(const Edge& other) const {
        return weight == other.weight;
    }

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }

    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }

    bool operator<=(const Edge& other) const {
        return weight <= other.weight;
    }

    bool operator>=(const Edge& other) const {
        return weight >= other.weight;
    }

    bool operator!=(const Edge& other) const {
        return weight != other.weight;
    }

    int myWeight() const {
        return weight;
    }

    int myVertex_A() const {
        return vertex_A;
    }

    int myID() {
        return id;
    }

    int myVertex_B() const {
        return vertex_B;
    }

    int myRow() const {
        return row;
    }

    int myColumn() const {
        return column;
    }

    std::string myDirection() const {
        return direction;
    }

    void present() const {
        std::cout << "Hello, this is my vertex_a: " << vertex_A << " this is my vertex_B: " << vertex_B << " this is my direction: " << direction << std::endl;
    }
};


#endif // EDGE_H