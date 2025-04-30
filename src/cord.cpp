class Cord {
private:
    int row;
    int column;

public:
    Cord(int row, int column) : row(row), column(column) {}

    int getRow() const {
        return row;
    }

    int getColumn() const {
        return column;
    }
};
