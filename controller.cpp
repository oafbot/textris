Controller::Controller(){}

void Controller::input(){
    game.key = getch();

    switch(game.key){
        case KEY_UP:
        case 'w':
        case 'W':
            if(y>0 && block.active){
                block.rotate();
            }
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if(block.traverse(DOWN) && !block.lockdown){
                y = y + 1;
                game.score += 1;
            }
            else{
                block.countdown = 0;
            }
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            if(block.traverse(LEFT) && !block.lockdown){
                x = x - 2;
            }
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            if(block.traverse(RIGHT) && !block.lockdown){
                x = x + 2;
            }
            break;
        case ' ':
            if(!game.GAME_OVER){
                game.pause();
            }
            else{
                game.reset();
            }
            break;
    }
};

