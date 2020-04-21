Game::Game(){
    reset();
    controls = * new Controller();
}

void Game::render(){
    int c, r;

    clear();

    for(int c=1; c<COLS; c++) {
        mvprintw(ROWS-1, c, "_");
    }

    for(int r=0; r<ROWS; r++) {
        mvprintw(r, 0, "|");
        mvprintw(r, COLS, "|");
    }

    for(int f=0; f<field.size(); f++){
        c = f%10;
        r = floor(f/10);

        if(field[f]){
            mvprintw(r, c*2, "|X|");
        }
    }

    block.render();

    mvprintw(1, COLS + 5,  ("SCORE: " + to_string(score)).c_str());
    mvprintw(3, COLS + 5,  ("LEVEL: " + to_string(level)).c_str());
    mvprintw(5, COLS + 5,  ("LINES: " + to_string(lines)).c_str());

    // mvprintw(ROWS+2, 0, (to_string(x)+" "+to_string(y)).c_str());

    // for(int i=0; i<field.size(); i++){
    //     mvprintw(ROWS+4+floor(i/10), i%10, to_string(field[i]).c_str());
    // }

    // for(int r=0; r<rows.size(); r++){
    //     for(int c=0; c<10; c++){
    //         mvprintw(ROWS+5+r, c, to_string(rows[r][c]).c_str());
    //     }
    // }

    refresh();
}


void Game::update(){
    if(block.lockdown){
        if(counter%10==0){
            clearRows();
            block.generate();
        }
    }
}

void Game::updateScore(){
    switch(completed.size()){
        case 1:
            score += 100*level;
            break;
        case 2:
            score += 250*level;
            break;
        case 3:
            score += 500*level;
            break;
        case 4:
            score += 1000*level;
            break;
    }

    if(!cleared && all_of(field.begin(), field.end(), [](int i){return !i;})){
        cleared = true;
        score += 2000*level;
    }

    block.locked = false;
}

void Game::clearRows(){
    rows.clear();
    completed.clear();

    for(int i=0; i<ROWS; i++){
        array<int, 10> row = {};

        for(int c=0; c<10; c++){
            row[c] = field[i*10+c];
        }
        rows.push_back(row);
    }

    for(int i=0; i<rows.size(); i++){
        if(all_of(rows[i].begin(), rows[i].end(), [](int j){return j;})){
            completed.push_back(i);
        }
    }

    updateScore();

    for(int i=0; i<completed.size(); i++){
        array<int, 10> empty = {0,0,0,0,0,0,0,0,0,0};
        rows.erase(rows.begin()+completed[i], rows.begin()+completed[i]+1);
        rows.insert(rows.begin(), empty);
    }

    // for(int i=0; i<completed.size(); i++){
    //     array<int, 10> empty = {0,0,0,0,0,0,0,0,0,0};
    //     rows.insert(rows.begin(), empty);
    // }

    for(int r=0; r<rows.size(); r++){
        // for(int c=0; c<10; c++){ field[r*10+c] = rows[r][c]; }
        copy( rows[r].begin(), rows[r].end(), begin(field)+r*10 );
    }

    lines += completed.size();

    if(lines>=level*10){
        level+=1;
    }
}

void Game::pause(){
    PAUSED = !PAUSED;
}

void Game::reset(){
    level = 1;
    max_y = ROWS;
    max_x = COLS;
    next_y = 0;
    direction = 1;
    counter = 0;
    score = 0;
    lines = 0;
    cleared = true;

    rows.clear();
    completed.clear();
    field.fill(0);

    GAME_OVER = false;
    PAUSED = false;
}

void Game::loop(){
    do{
        controls.input();

        if(!PAUSED && !GAME_OVER){
            update();
            render();

            delay = DELAY-(level*5000)>5000 ? DELAY-(level*5000) : 5000;
            usleep(delay);

            next_y = y + direction;

            if(!block.active){
                block.lock();
            }
            else if(counter%10==0 && block.traverse(DOWN)){
                y += direction;
            }
            else if(!block.traverse(DOWN)){
                if(block.countdown<=0){
                    block.active = false;
                    block.locked = true;
                }
                else{
                    block.countdown--;
                }
            }

            if(counter<10000)
                counter++;
            else counter = 0;
        }
        else if(GAME_OVER){
            mvprintw(ROWS/2-1, 0,  "|-------------------|");
            mvprintw(ROWS/2, 0,  "|     GAME OVER     |");
            mvprintw(ROWS/2+1, 0,  "|-------------------|");
            usleep(DELAY);
            mvprintw(ROWS+2, 0, " PRESS SPACE TO RESET");
        }
        else if(PAUSED){
            mvprintw(ROWS/2-1, 0,  "|-------------------|");
            mvprintw(ROWS/2, 0,    "|       PAUSED      |");
            mvprintw(ROWS/2+1, 0,  "|-------------------|");
            usleep(DELAY);
        }

    }
    while( key!='q' && key!='Q' );
}