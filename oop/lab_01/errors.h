#ifndef ERRORS_H
#define ERRORS_H

enum errors
{
    SUCCESS,
    ERR_RECEIVE_FILE,
    ERR_OPEN_FILE,
    ERR_AMOUNT,
    ERR_EDGES_DATA,
    ERR_EDGES_SIZE,
    ERR_ALLOCATION,
    ERR_POINTS_DATA,
    ERR_LOAD_DATA,
    ERR_WRONG_SCENE

};


void handle_errors(errors &error);


#endif