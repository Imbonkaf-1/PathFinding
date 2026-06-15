#include<iostream>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<cmath>
using namespace std;


const int ROWS = 5;
const int COLUMNS = 5;
const vector<int> START = {0, 0};
vector<int> GOAL = {4, 4};

float score[ROWS][COLUMNS][4] = {};

float LR = 0.5;
float discount = 0.9;
float explore = 1.0;

void take_step(vector<int>& pos, vector<int>& new_pos, int& action, float& reward){
    int pos_x = pos[0];
    int pos_y = pos[1];   

    int newpos_x = 0;
    int newpos_y = 0;

    if(action == 0){
        new_pos[0] = pos_x + 1;
        new_pos[1] = pos_y;
    } 
    else if(action == 1){
        new_pos[0] = pos_x;
        new_pos[1] = pos_y + 1;
    }
    else if(action == 2){
        new_pos[0] = pos_x - 1;
        new_pos[1] = pos_y;
    }
    else if(action == 3){
        new_pos[0] = pos_x;
        new_pos[1] = pos_y - 1;
    }


    if(new_pos == GOAL){
        reward+=10;
    }
    else if(new_pos[0] < 0 || new_pos[0] >= 5){
        reward-=10;
        new_pos = START;
    }
    else if(new_pos[1] < 0 || new_pos[1] >= 5){
        reward-=10;
        new_pos = START;
    }
}

int pick_action(vector<int>& pos){
    float random_number = (float)rand() / RAND_MAX;
    if(explore > random_number){
        return rand()%4;
    }

    int pos_x = pos[0];
    int pos_y = pos[1];

    int best_move = 0;
    for(int i = 1; i < 4; i++){
        if(score[pos_x][pos_y][i] > score[pos_x][pos_y][best_move]) best_move = i;
    }

    return best_move;
}

void update_score(vector<int>& pos, vector<int>& new_pos, int& action, float& reward){
    int pos_x = pos[0];
    int pos_y = pos[1];
    int nx = new_pos[0];
    int ny = new_pos[1];

    float best = max(max(score[nx][ny][0], score[nx][ny][1]), max(score[nx][ny][2], score[nx][ny][3]));
    float target = reward + discount*best;
    float error = target - score[pos_x][pos_y][action];
    score[pos_x][pos_y][action] += LR*error;

}


int main() {
    srand(time(0));
    int run = 50;

    for(int ep = 0; ep < run; ep++){
        vector<int> pos = START;
        for(int i = 0; i < 50; i++){
            vector<int> new_pos = {0, 0};
            float reward = 0;

            int action = pick_action(pos);
            take_step(pos, new_pos, action, reward);
            update_score(pos, new_pos, action, reward);

            pos = new_pos;
            if(pos == GOAL) break;
        }
        explore = 0.7*explore;
    }


    cout << "Last round: " << endl;
    vector<int> pos = START;
    for(int i = 0; i < 20; i++){
        cout << "at pos: " << pos[0] << ", " << pos[1] << " ";
        if(pos == GOAL) {cout << "GOAL\n"; break;}
        int best_move = 0;
        int pos_x = pos[0];
        int pos_y = pos[1];
        for(int i = 1; i < 4; i++){
            if(score[pos_x][pos_y][i] > score[pos_x][pos_y][best_move]){
                best_move = i;  
            }
        }
        int action = best_move;
        vector<int> new_pos = {0, 0}; float reward;
        take_step(pos, new_pos, action, reward);
        if(action == 0) cout << "moving DOWN\n";
        else if (action == 1) cout << "moving RIGHT\n";
        else if (action == 2) cout << "moving UP\n";
        else if (action == 3) cout << "moving LEFT\n";
        pos = new_pos;
    }
}
