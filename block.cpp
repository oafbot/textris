using namespace std;

Block::Block(){
    config[0] = {{ // backwards L
        {{0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0}},
        {{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0}},
        {{0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0}},
        {{0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0}}
    }};
    config[1] = {{ // L
        {{0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0}},
        {{1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0}},
        {{0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0}},
        {{0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0}},
    }};
    config[2] = {{ // Z
        {{0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0}},
        {{0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0}},
        {{0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0}},
        {{0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0}},
    }};
    config[3] = {{ // backwards Z
        {{0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0}},
        {{1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
        {{0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0}},
        {{1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
    }};
    config[4] = {{ // square
        {{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}},
        {{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}},
        {{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}},
        {{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}},
    }};
    config[5] = {{ // long
        {{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
        {{0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0}},
        {{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
        {{0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0}},
    }};
    config[6] = {{ // T
        {{0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0}},
        {{0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0}},
        {{ 0,0,0,0,
 0,1,1,1,
 0,0,1,0,
 0,0,0,0}},
        {{0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
    }};

    countdown = 0;
    active = true;
    locked = false;

    generate();
}


void Block::generate(){
    type = rand()%7;
    active = true;
    state = 0;
    y = 0;
    x = 8;
    col = 4;
    row = 0;
    locked = false;
    lockdown = false;
    countdown = 10;
}

void Block::rotate(){
    if(!locked){
        state = state<3 ? state+1 : 0;

        bool commit = false,
             right = traverse(RIGHT),
             left = traverse(LEFT),
             down = traverse(DOWN),
             up = traverse(UP);

        if(right && left && down && up){
            commit = true;
        }
        else if(!right && left){
            x -= 2;
            commit = true;
        }
        else if(!left && right){
            x += 2;
            commit = true;
        }
        // else if(!right && !left && !up && down){
        //     y += 1;
        //     commit = true;
        // }
        // else if(!right && !left && !down && up){
        //     y -= 1;
        //     commit = true;
        // }

        if(!commit)
            state = state>0 ? state-1 : 3;
    }
}

void Block::lock(){
    int size = config[type][state].size();

    for(int i=0; i<size; i++){
        if(config[type][state][i]){
            int index = convert(i);
            game.field[index] = 1;

            if(index/10<1){
                game.GAME_OVER = true;
            }
        }
    }

    active = true;
    lockdown = true;
    game.cleared = false;
}

// void Block::lock(){
//     active = false;
//     int size = config[type][state].size();

//     for(int i=0; i<size; i++){
//         if(config[type][state][i]){
//             int index = convert(i);
//             game.field[index] = 1;

//             if(index/10<1){
//                 game.GAME_OVER = true;
//             }
//         }
//     }

//     countdown = 5;
//     locked = true;
//     game.cleared = false;
// }


// void Block::lockdown(){

// }

int Block::convert(int i){
    int c = i%4, r = floor(i/4);
    return (y+r)*10 + (x-2)/2 + c;
}

void Block::render(){
    // if(active){
        int c, r;
        int size = config[type][state].size();

        for(int i=0; i<size; i++){
            c = i%4;
            r = floor(i/4);

            if(config[type][state][i]==1){
                mvprintw(y + r, x-2 + 2*c, "|_|");
            }
        }
    // }
}

bool Block::traverse(int direction){
    int c, r;
    int size = config[type][state].size();

    for(int i=0; i<size; i++){
        c = i%4;
        r = floor(i/4);

        if(config[type][state][i]){
            index = convert(i);

            if(direction==LEFT){
                if(i==0 || !config[type][state][i-1]){
                    if(index%10==0){
                        return false;
                    }
                    if(game.field[index - 1]){
                        return false;
                    }

                    if(x<=0 && index%10==9){
                        return false;
                    }
                }
            }
            else if(direction==RIGHT){
                if(!config[type][state][i+1]){
                    if(x+c*2>=COLS){
                        return false;
                    }
                    // if((index)%9==0){
                    //     blocked = "false";
                    //     return false;
                    // }

                    if(game.field[index + 1]){
                        return false;
                    }
                }
            }
            else if(direction==DOWN){
                if(i==11 || !config[type][state][i+4]){
                    if(index + 10 >= (ROWS)*10){
                        return false;
                    }

                    if(game.field[index + 10]){
                        return false;
                    }
                }
            }
            else if(direction==UP){
                if(i==11 || !config[type][state][i-4]){
                    if(game.field[index - 10]){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}