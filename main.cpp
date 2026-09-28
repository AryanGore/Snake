#include<bits/stdc++.h>
#include<conio.h>
using namespace std;

const int WORLD_WIDTH = 35;
const int WORLD_HEIGHT = 20;

struct position{
    int x;
    int y;
};

unordered_map<string, position>direction;

class Snake{
    vector<position> body;
    position direction;

    public:

    Snake(){
        body = {{5,5},{4,5},{3,5}};
        direction = {1,0};
    }

    const position& getHead() const {
        return body[0];
    }

    void grow(){
        body.push_back(body.back());
    }

    bool hasCollided() const {
        for(int i=1; i<body.size(); ++i){
            if(body[i].x == body[0].x && body[i].y == body[0].y) return true;
        }
        return false;
    }

    void changeDirection(position newDirection){
        if(direction.x + newDirection.x == 0 && direction.y + newDirection.y == 0) return;
        direction = newDirection;
    }

    const vector<position>& getBody() const{
        return body;
    }

    void move(){
        auto changeIn = direction;
        vector<position> copyBody(body.begin(), body.end());
        for(int i=0; i<body.size(); ++i){
            if(i==0){
                body[i].x = copyBody[i].x + changeIn.x;
                body[i].y = copyBody[i].y + changeIn.y;

                if(body[i].x > WORLD_WIDTH-2) body[i].x = 1;
                if(body[i].y > WORLD_HEIGHT-2) body[i].y = 1;
                if(body[i].x < 1) body[i].x = WORLD_WIDTH -2;
                if(body[i].y < 1) body[i].y = WORLD_HEIGHT-2; 

            }else{
                body[i].x = copyBody[i-1].x;
                body[i].y = copyBody[i-1].y;
            }
        }
    }
};

class Food{
    position pos;
    public:

    const position& getPosition() const{
        return pos;
    }

    void generateFood(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrX(1,WORLD_WIDTH-2);
        uniform_int_distribution<> distrY(1,WORLD_HEIGHT-2);
        this->pos.x = distrX(gen);
        this->pos.y = distrY(gen);
    }

    Food(){
        generateFood();
    }

};

void renderWorld(const Snake& snake, const Food& food){
    const auto& body = snake.getBody();
    const auto& foodPos = food.getPosition();

    for(int i=0; i< WORLD_HEIGHT; ++i){
        for(int j=0; j < WORLD_WIDTH; ++j){
            if(i == 0 || i == WORLD_HEIGHT-1) cout<<"# ";
            else if(j == 0 || j == WORLD_WIDTH-1) cout<<"# ";
            else if(foodPos.x == j && foodPos.y == i) cout<<"@ ";
            else {
                bool foundSnake = false;
                for(auto pos: body){
                    if(pos.x == j && pos.y == i) {
                        foundSnake = true;
                        break;
                    }
                    // break;
                }
                if(foundSnake) cout<<"* ";
                else cout<<"  ";

            }
        }
        cout<<'\n';
    }
}

int getDelay(int score){
    if(score < 8) return 150;
    else if(score >= 8 && score < 18) return 120;
    else if(score >= 18 && score < 25) return 100;
    
    return 80;
}

void renderScore(int score){
    cout<<"SCORE: "<<score<<'\n';
}

bool foodInSnake(const position& foodPos, const Snake& snake){
    const auto& snakeBody = snake.getBody();
    for(int i=0; i<snakeBody.size(); ++i){
        if(snakeBody[i].x == foodPos.x && snakeBody[i].y == foodPos.y) return true;
    }
    return false;
}


int main(){

    Snake snake;
    Food food;
    int score = 0;

    direction["DOWN"] = {0,1};
    direction["UP"] = {0,-1};
    direction["LEFT"] = {-1, 0};
    direction["RIGHT"] = {1, 0};

    // snake.changeDirection(direction["DOWN"]);
    cout << "\033[?25l";
    while(true){ //game loop.
        //inputs: 
        if(_kbhit()){
            char key = _getch();
            if(key == 'w' || key == 'W') snake.changeDirection(direction["UP"]);
            else if(key == 'a' || key == 'A') snake.changeDirection(direction["LEFT"]);
            else if(key == 's' || key == 'S') snake.changeDirection(direction["DOWN"]);
            else if(key == 'd' || key == 'D') snake.changeDirection(direction["RIGHT"]);
        }

        snake.move();

        if(snake.hasCollided()){
            //game OVER
            break;
        }

        const auto& foodPos = food.getPosition();
        const auto& snakeHead = snake.getHead();

        if(foodPos.x == snakeHead.x && foodPos.y == snakeHead.y) {
            score++;
            food.generateFood();
            while(foodInSnake(foodPos, snake)) food.generateFood();
            snake.grow();
        }

        cout<<"\033[H";
        renderScore(score);
        renderWorld(snake,food);
        this_thread::sleep_for(chrono::milliseconds(getDelay(score)));
    }
    cout << "\033[?25h";
    cout<<"GAME OVER";


    return 0;
}

