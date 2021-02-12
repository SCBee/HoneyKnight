#include "main.h"

void setup(std::vector<std::vector<bool>> &board)
{
    // Populate board that is square (dim x dim)
    // Board will be dim x dim with false (0) values everywhere
    // Board will be accessed (row, col) ... (y, x)

    for (int i = 0; i < dim; ++i)
    {
        std::vector<bool> tmp;

        for (int j = 0; j < dim; ++j)
        {
            tmp.push_back(false);    
        }

        board.push_back(tmp);
    }
}

void print(std::vector<std::vector<bool>> &board)
{
    // Prints the board

    for (auto i : board)
    {
        for (auto j : i)
        {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void print2D(std::vector<std::vector<int>> vec)
{
    // Print a 2D vector
    // Mostly used for debugging pathing logic

    for (auto i : vec)
    {
        for (auto j : i)
        {
            std::cout << j << " ";
        }
        std::cout << " | ";
    }
}

void simulate(std::vector<std::vector<bool>> board, std::vector<std::vector<int>> moveHistory, int x_pos, int y_pos)
{
    // Simulate the tour using a move history
    // Move history is filled by the tour function

    board[y_pos][x_pos] = true;
    print(board);

    for (auto i : moveHistory)
    {
        int dx = i[0];
        int dy = i[1];

        std::cout << "Move: (" << dx << ", " << dy << ")\n";
        
        x_pos += dx;
        y_pos += dy;

        board[y_pos][x_pos] = true;
        print(board);

        Sleep(50);
    }
}

bool cmpPath(std::vector<int> move_a, std::vector<int> move_b)
{
    // Compare path magnitude
    // Used to sort the paths based on what has the least future possibilities

    if (move_a[2] > move_b[2])
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool isLegalMove(std::vector<int> move, int x_position, int y_position)
{
    // Check if the move indicated by 'move' is legal from the current position (x, y)
    // A legal move is a move that is within the dimensions of the board

    x_position += move[0];
    y_position += move[1];

    if (x_position >= 0 && x_position < dim && y_position >= 0 && y_position < dim)
    {
        return true;
    }

    return false;
}

bool isVisited(std::vector<std::vector<bool>> &board, std::vector<int> move, int x_position, int y_position)
{
    // Check if the move indicated by 'move' from the current position (x, y) has already been visited
    // Return TRUE if already visited, Otherwise FALSE

    x_position += move[0];
    y_position += move[1];

    if (board[y_position][x_position])
    {
        return true;
    }

    return false;
}

bool isComplete(std::vector<std::vector<bool>> &board)
{
    // Check if the board is completed
    // A completed board will have all locations have the value TRUE
    
    for (auto i : board)
    {
        for (auto j : i)
        {
            if (!j)
                return false;
        }
    }

    return true;
}

int calcPaths(std::vector<std::vector<bool>> &board, int x_position, int y_position)
{
    // Calculates number of moves that are possible from position and board state

    std::vector<std::vector<int>> moves = { {-2, 1}, {2, 1}, {-1, 2}, {1, 2}, {-2, -1}, {2, -1}, {-1, -2}, {1, -2} };

    int count = 0;

    for (auto i : moves)
    {
        if (isLegalMove(i, x_position, y_position) && !isVisited(board, i, x_position, y_position))
        {
            count++;
        }
    }

    return count - 1;
}



std::vector<std::vector<int>> findLowestMove(std::vector<std::vector<bool>> &board, int x_position, int y_position)
{
    // Implementation of Warnsdorff's Rule
    // The rule states that the move with the LOWEST subsequent path options is the best

    std::vector<std::vector<int>> moves = { {-2, 1, 8}, {2, 1, 8}, {-1, 2, 8}, {1, 2, 8}, {-2, -1, 8}, {2, -1, 8}, {-1, -2, 8}, {1, -2, 8} };
    
    for (unsigned int i = 0; i < moves.size(); ++i)
    {
        if (isLegalMove(moves[i], x_position, y_position) && !isVisited(board, moves[i], x_position, y_position))
        {
            moves[i][2] = calcPaths(board, x_position + moves[i][0], y_position + moves[i][1]);
        }
    }

    std::sort(moves.begin(), moves.end(), cmpPath);

    return moves;
}

std::vector<std::vector<bool>> tour(std::vector<std::vector<bool>> board, int x_position, int y_position, bool &finished, std::vector<std::vector<int>> &moveHistory)
{
    // Driving function to solve (recursive)

    Sleep(10); // Laptop is very old & slow. Comment this out if your PC can run this without crashing.

    board[y_position][x_position] = true;
    finished = isComplete(board);

    std::vector<std::vector<int>> moves = findLowestMove(board, x_position, y_position);

    for (auto i : moves)
    {
        int x_new = x_position + i[0];
        int y_new = y_position + i[1];

        if (isLegalMove(i, x_position, y_position) && !isVisited(board, i, x_position, y_position))
        {
            moveHistory.push_back({i[0], i[1]});
            board = tour(board, x_new, y_new, finished, moveHistory);
            if (!finished)
                moveHistory.pop_back();
        }
    }

    return board;
}

int main()
{
    std::vector<std::vector<bool>> board;
    std::vector<std::vector<int>> moveHistory;

    setup(board);
    bool finished = false;

    std::cout << "Beginning the Knight's Tour!\n\n\n";

    //board = tour(board, 4, 0, finished, moveHistory); // Starting position is (4, 0)
    tour(board, 4, 0, finished, moveHistory); // Starting position is (4, 0) ... Can be changed to solve for a different start

    std::cout << "Tour complete!\n\n\n";

    simulate(board, moveHistory, 4, 0); // Change the starting position if you changed it above

    return 0;
}