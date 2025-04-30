#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <array>

char first_bit = 'E';
char second_bit ='W';
char third_bit = 'S';
char fourth_bit ='N';
class Player {
private:
    int height;
    int anchor;
    char direction;
    int **diagram;

public:
    Player(int h, int a, int **diag) : height(h), anchor(a), direction('S') {
        this-> diagram = diag;
    }
    void turnRight() {
        if (direction == 'N') direction = 'E';
        else if (direction == 'E') direction = 'S';
        else if (direction == 'S') direction = 'W';
        else if (direction == 'W') direction = 'N';
    }

    // Function to turn the player left
    void turnLeft() {
        if (direction == 'N') direction = 'W';
        else if (direction == 'W') direction = 'S';
        else if (direction == 'S') direction = 'E';
        else if (direction == 'E') direction = 'N';
    }

bool go_wild(bool forward) {
    return true;
}

bool go_cubical(bool forward) {
    int nextHeight = height;
    int nextAnchor = anchor;
    
    if (direction == 'N') {
        if (forward) nextHeight--;
        else nextHeight++;
    } else if (direction == 'S') {
        if (forward) nextHeight++;
        else nextHeight--;
    } else if (direction == 'E') {
        if (forward) nextAnchor++;
        else nextAnchor--;
    } else if (direction == 'W') {
        if (forward) nextAnchor--;
        else nextAnchor++;
    }
    char actual_direction;

    if (forward)
    {
    actual_direction = direction;
    }
    else {
            if (direction == 'N') {
        actual_direction = 'S';
    } else if (direction == 'S') {
        actual_direction = 'N';
    } else if (direction == 'E') {
        actual_direction = 'W';
    } else if (direction == 'W') {
        actual_direction = 'E';
    }

    }

    if (nextHeight >= 0 && nextHeight < 10 && nextAnchor >= 0 && nextAnchor < 10) {
        int nextCellValue = diagram[nextHeight][nextAnchor]; 

    if (nextHeight >= 0 && nextHeight < 10 && nextAnchor >= 0 && nextAnchor < 10) {
        int nextCellValue = diagram[nextHeight][nextAnchor];

                height = nextHeight;
        anchor = nextAnchor;
        return true;
        }
        }

    return false;
    
}

bool move(bool forward) {
    int nextHeight = height;
    int nextAnchor = anchor;

    // Calculate next position based on direction
    if (direction == 'N') {
        if (forward) nextHeight--;
        else nextHeight++;
    } else if (direction == 'S') {
        if (forward) nextHeight++;
        else nextHeight--;
    } else if (direction == 'E') {
        if (forward) nextAnchor++;
        else nextAnchor--;
    } else if (direction == 'W') {
        if (forward) nextAnchor--;
        else nextAnchor++;
    }
    char actual_direction;

    if (forward)
    {
    actual_direction = direction;
    }
    else {
            if (direction == 'N') {
        actual_direction = 'S';
    } else if (direction == 'S') {
        actual_direction = 'N';
    } else if (direction == 'E') {
        actual_direction = 'W';
    } else if (direction == 'W') {
        actual_direction = 'E';
    }

    }

    if (nextHeight >= 0 && nextHeight < 10 && nextAnchor >= 0 && nextAnchor < 10) {
        int nextCellValue = diagram[nextHeight][nextAnchor];


switch (diagram[height][anchor]) {
    case 0: // Cannot move in any direction
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 0" << endl;
        return false; 
    case 1:
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 1" << endl;
        if (actual_direction == first_bit)
            break; 
        else
            return false; 
    case 2:
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 2" << endl;
        if (actual_direction == second_bit)
            break; 
        else
            return false; 
    case 3: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 3" << endl;
        if (actual_direction == second_bit || actual_direction == first_bit)
            break; 
        else
            return false; 
    case 4: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 4" << endl;
        if (actual_direction == third_bit)
            break; 
        else
            return false; 
    case 5: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 5" << endl;
        if (actual_direction == third_bit || actual_direction == first_bit)
            break; 
        else
            return false; 
    case 6: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 6" << endl;
        if (actual_direction == third_bit || actual_direction == second_bit)
            break; 
        else
            return false;
    case 7: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 7" << endl;
        if (actual_direction == third_bit || actual_direction == second_bit || actual_direction == first_bit)
            break; 
        else
            return false; 
    case 8: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 8" << endl;
        if (actual_direction == fourth_bit)
            break; 
        else
            return false; 
    case 9: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 9" << endl;
        if (actual_direction == fourth_bit || actual_direction == first_bit)
            break; 
        else
            return false; 
    case 10: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 10" << endl;
        if (actual_direction == fourth_bit || actual_direction == second_bit)
            break; 
        else
            return false; 
    case 11: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 11" << endl;
        if (actual_direction == fourth_bit || actual_direction == second_bit || actual_direction == first_bit)
            break; 
        else
            return false; 
    case 12: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 12" << endl;
        if (actual_direction == fourth_bit || actual_direction == third_bit)
            break; 
        else
            return false; 
    case 13: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 13" << endl;
        if (actual_direction == fourth_bit || actual_direction == third_bit || actual_direction == first_bit)
            break; 
        else
            return false; 
    case 14: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 14" << endl;
        if (actual_direction == fourth_bit || actual_direction == third_bit || actual_direction == second_bit)
            break; 
        else
            return false; 
    case 15: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 15" << endl;
        break; 
        case 16: 
        cout << "Actual Direction: " << actual_direction << endl;
        cout << "Case Number: 16" << endl;
        return false; 
}


        
        height = nextHeight;
        anchor = nextAnchor;
        return true; 
    }

    return false; 
}


    
    void printPosition() {
        std::cout << "Player is at position (" << height << ", " << anchor << ") facing " << direction << std::endl;
    }

    bool didiwon(int final_x, int final_y) {
        if (anchor == final_y && height == final_x) {
            return true;
        }
        cout << anchor << " is not " << final_x << " or " << height << " is not " << final_y;
        return false;
    }
};

#endif /* PLAYER_H */
