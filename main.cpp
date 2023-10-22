#include "raylib.h"
#include <iostream>
#include <vector>
#include <map>
#include <random>

const int WIDTH = 500;
const int HEIGHT = 620;
const int numRow = 20;
const int numCol = 10;
const int cellSize = 30;

const Color cyan = {21, 204, 209, 255};

std::vector<Color> colors = {BLACK, YELLOW, GREEN, MAGENTA, cyan, RED, BLUE, PURPLE};

 // shape
    std::map<int, std::vector<Vector2>> LShape = {
        {0, {Vector2(0, 2), Vector2(1, 0), Vector2(1, 1), Vector2(1, 2)}},
        {1, {Vector2(0, 1), Vector2(1, 1), Vector2(2, 1), Vector2(2, 2)}},
        {2, {Vector2(1, 0), Vector2(1, 1), Vector2(1, 2), Vector2(2, 0)}},
        {3, {Vector2(0, 0), Vector2(0, 1), Vector2(1, 1), Vector2(2, 1)}}
    };

    std::map<int, std::vector<Vector2>> JShape = {
        {0, {Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(1, 2)}},
        {1, {Vector2(0, 1), Vector2(0, 2), Vector2(1, 1), Vector2(2, 1)}},
        {2, {Vector2(1, 0), Vector2(1, 1), Vector2(1, 2), Vector2(2, 2)}},
        {3, {Vector2(0, 1), Vector2(1, 1), Vector2(2, 0), Vector2(2, 1)}}
    };

    std::map<int, std::vector<Vector2>> IShape = {
        {0, {Vector2(1, 0), Vector2(1, 1), Vector2(1, 2), Vector2(1, 3)}},
        {1, {Vector2(0, 2), Vector2(1, 2), Vector2(2, 2), Vector2(3, 2)}},
        {2, {Vector2(2, 0), Vector2(2, 1), Vector2(2, 2), Vector2(2, 3)}},
        {3, {Vector2(0, 1), Vector2(1, 1), Vector2(2, 1), Vector2(3, 1)}}
    };

    std::map<int, std::vector<Vector2>> OShape = {
        {0, {Vector2(0, 0), Vector2(0, 1), Vector2(1, 0), Vector2(1, 1)}},
    };

    std::map<int, std::vector<Vector2>> SShape = {
        {0, {Vector2(0, 1), Vector2(0, 2), Vector2(1, 0), Vector2(1, 1)}},
        {1, {Vector2(0, 1), Vector2(1, 1), Vector2(1, 2), Vector2(2, 2)}},
        {2, {Vector2(1, 1), Vector2(1, 2), Vector2(2, 0), Vector2(2, 1)}},
        {3, {Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(2, 1)}}
    };
    
    std::map<int, std::vector<Vector2>> TShape = {
        {0, {Vector2(0, 1), Vector2(1, 0), Vector2(1, 1), Vector2(1, 2)}},
        {1, {Vector2(0, 1), Vector2(1, 1), Vector2(1, 2), Vector2(2, 1)}},
        {2, {Vector2(1, 0), Vector2(1, 1), Vector2(1, 2), Vector2(2, 1)}},
        {3, {Vector2(0, 1), Vector2(1, 0), Vector2(1, 1), Vector2(2, 1)}}
    };

    std::map<int, std::vector<Vector2>> ZShape = {
        {0, {Vector2(0, 0), Vector2(0, 1), Vector2(1, 1), Vector2(1, 2)}},
        {1, {Vector2(0, 2), Vector2(1, 1), Vector2(1, 2), Vector2(2, 1)}},
        {2, {Vector2(1, 0), Vector2(1, 1), Vector2(2, 1), Vector2(2, 2)}},
        {3, {Vector2(0, 1), Vector2(1, 0), Vector2(1, 1), Vector2(2, 0)}}
    };

struct Grid 
{
    int grid[numRow][numCol]; 
    Grid(){ 
        initiate();
    }

    void initiate() {
        for(int row = 0; row < numRow; row++) {
            for(int col = 0; col < numCol; col++) {
                grid[row][col] = 0;
            }
        }
    }

    void reset() {
        for(int row = 0; row < numRow; row++) {
            for(int col = 0; col < numCol; col++) {
                this->grid[row][col] = 0;
            }
        }
    }

    void draw() {
        for(int row = 0; row < numRow; row++) {
            for(int col = 0; col < numCol; col++) {
                int cellValue = grid[row][col];
                DrawRectangle(col * cellSize + 11, row * cellSize + 11, cellSize - 1, cellSize - 1, colors[cellValue]);
            }
        }
    }

    bool isCellOutside(int row, int col) {
        if(row >= 0 && row < numRow && col >= 0 && col < numCol) {
            return false;
        }
        return true;
    }

    bool isCellEmpty(int row, int col) {
        if(this->grid[row][col] == 0) return true;
        return false;
    }

    int isFullRow() {
        int completed = 0;

        for(int row = numRow - 1; row >= 0; row--) {
            if(isRowFull(row)) {
                clearRow(row);
                completed += 1;
            } else if(completed > 0) {
                moveRowDown(row, completed);
            }
        }

        return completed;
    }

    void moveRowDown(int row, int numRow) {
        for(int col = 0; col < numCol; col++) {
            this->grid[row + numRow][col] = this->grid[row][col];
            this->grid[row][col] = 0;
        }
    }

    bool isRowFull(int row) {
        for(int col = 0; col < numCol; col++) {
            if(this->grid[row][col] == 0) {
                return false;
            }
        }
        return true;
    }

    void clearRow(int row) {
        for(int col = 0; col < numCol; col++) {
            this->grid[row][col] = 0;
        }
    }
};

struct Block 
{
    int id;
    int rowOffset = 0;
    int colOffset = 0;
    int rot = 0;
    std::map<int, std::vector<Vector2>> tiles;
    
    Block(int id, std::map<int, std::vector<Vector2>> tiles) {
        this->id = id;
        this->tiles = tiles;

        // id = 3 itu adalah I-Shape
        // id = 4 itu adalah O-Shape
        if(id == 3) {
            move(3, -1); 
        } else if(id == 4) {
            move(4, 0);
        } else {
            move(3, 0);
        }
    }

    std::vector<Vector2> getCellPosition() 
    {
        std::vector<Vector2> tiles = this->tiles[this->rot];
        std::vector<Vector2> updatedTiles;
        for(Vector2 pos : tiles) {
            Vector2 position = Vector2(pos.y + rowOffset, pos.x + colOffset);
            updatedTiles.push_back(position);
        }
        return updatedTiles;
    }

    void rotate() {
        this->rot = (this->rot + 1) % this->tiles.size();
    }

    void undoRotate() {
        this->rot = (this->rot - 1 + this->tiles.size()) % this->tiles.size();
    }

    void move(int row, int col) {
        this->rowOffset += row;
        this->colOffset += col;
    }

    bool blockOutSide(Grid &grid) {
        std::vector<Vector2> tiles = getCellPosition();
        for(Vector2 pos : tiles) {
            if(grid.isCellOutside(pos.y, pos.x)) return true;
        }
        return false;
    }

    bool blockFit(Grid &grid) {
        std::vector<Vector2> tiles = getCellPosition();
        for(Vector2 pos : tiles) {
            if(!grid.isCellEmpty(pos.y, pos.x)) {
                return true;
            }
        }
        return false;
    }

    void lockBlock(Grid &grid) {
        std::vector<Vector2> tiles = getCellPosition();
        for(Vector2 pos : tiles) {
            grid.grid[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = this->id;
        }
    }

    void draw(int offsetX, int offsetY) {
        std::vector<Vector2> tiles = getCellPosition();
        for(Vector2 pos : tiles) {
            DrawRectangle(pos.x * cellSize + offsetX, pos.y * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[id]);
        }
    }
};

std::vector<Block> initializeBlocks() {
    return std::vector<Block>{Block(1, LShape), Block(2, JShape), Block(3, IShape), Block(4, OShape),
          Block(5, SShape), Block(6, TShape), Block(7, ZShape)};
}

Block getRandomBlock(std::vector<Block> &blocks) {
    if(blocks.empty()) {
        blocks = initializeBlocks();
    }

    int randomIdx = GetRandomValue(0, blocks.size() - 1);
    Block nextBlock = blocks[randomIdx];
    blocks.erase(blocks.begin() + randomIdx);
    return nextBlock;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Tetris Pog");
    SetTargetFPS(60);

    // game variable
    Grid grid = Grid();
    std::vector<Block> blocks = initializeBlocks();
    Block currentBlock = getRandomBlock(blocks); 
    Block nextBlock = getRandomBlock(blocks);
    int score = 0;
    float lastUpdate = GetTime();
    bool gameOver = false;

    while(!WindowShouldClose()) {
        
        ClearBackground(GRAY);

        if(IsKeyPressed(KEY_RIGHT)) {
            currentBlock.move(1, 0);
            if(currentBlock.blockOutSide(grid) || currentBlock.blockFit(grid)) {
                currentBlock.move(-1, 0);
            }
        };    
        if(IsKeyPressed(KEY_LEFT)) {
            currentBlock.move(-1, 0);
             if(currentBlock.blockOutSide(grid) || currentBlock.blockFit(grid)) {
                currentBlock.move(1, 0);
            }
        };    
        if(IsKeyPressed(KEY_UP)) {
            currentBlock.rotate();
            if(currentBlock.blockOutSide(grid) || currentBlock.blockFit(grid)) {
                currentBlock.undoRotate();
            }
        }

        if(GetTime() - lastUpdate > 0.2 && !gameOver) {
            currentBlock.move(0, 1);
            if(currentBlock.blockOutSide(grid) || currentBlock.blockFit(grid)) {
                currentBlock.move(0, -1);
                currentBlock.lockBlock(grid);
                currentBlock = nextBlock;
                if(currentBlock.blockFit(grid)) gameOver = true;
                nextBlock = getRandomBlock(blocks);
                if(grid.isFullRow() > 0) score += 10;
            }
            lastUpdate = GetTime();
        }

        if(IsKeyPressed(KEY_R) && gameOver) {
            grid.reset();
            currentBlock = getRandomBlock(blocks);
            score = 0;
            gameOver = false;
        }

        BeginDrawing();

        // draw grid
        grid.draw();

        // draw current block
        currentBlock.draw(11,11); 

        // draw next block
        nextBlock.draw(260, 350);

        DrawText("Next Block", 350, 300, 20, WHITE);

        if(!gameOver) {
            DrawText("Score", 360, 80, 30, WHITE);
            DrawText(std::to_string(score).c_str(), 390, 140, 30, WHITE);
        } else {
            DrawText("GAME OVER", 330, 120, 25, RED);
            DrawText("  Tekan \"R\" \nuntuk restart", 337, 180, 20, WHITE);
        }
        
        DrawText("     created by \naji mustofa @pepega90", 320, HEIGHT - 60, 15, YELLOW);

        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}