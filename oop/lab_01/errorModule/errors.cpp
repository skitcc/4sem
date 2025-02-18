#include "errors.h"



void handle_errors(error_t &error)
{
    switch (error)
    {
    case ERR_RECEIVE_FILE:
        /* code */
        break;
    
    case ERR_OPEN_FILE:
        QMessageBox::information(NULL, "Ошибка", "Не удалось открыть файл!");
    
    case ERR_AMOUNT:
        QMessageBox::information(NULL, "Ошибка", "Ошибка в записи кол-ва для точек/cвязей");

    case ERR_EDGES_DATA:
        QMessageBox::information(NULL, "Ошибка", "Ошибка в записи ребра");


    default:
        break;
    }
}