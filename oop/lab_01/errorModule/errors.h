#ifndef ERRORS_H
#define ERRORS_H


#include <QMessageBox>

enum error_t
{
    SUCCESS,
    ERR_RECEIVE_FILE,
    ERR_OPEN_FILE,
    ERR_AMOUNT,
    ERR_EDGES_DATA,
    ERR_EDGES_SIZE,
    ERR_ALLOCATION,
    ERR_POINTS_DATA,
    ERR_LOAD_DATA




};


void handle_errors(error_t &error);


#endif