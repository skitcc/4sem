#include "edges.h"

static errors allocate_edges(edges_t &edges)
{
    if (edges.size <= 0)
        return ERR_EDGES_SIZE;
    
    errors rc = SUCCESS;
    edge_t *local_array_of_edges = (edge_t *)malloc(edges.size * sizeof(edge_t));
    if (!local_array_of_edges)
        rc = ERR_ALLOCATION;
    else
        edges.array_of_edges = local_array_of_edges;

    return rc;
}


void set_default_edges(edges_t &edges)
{
    edges.array_of_edges = NULL;
    edges.size = 0;
}


void free_edges(edges &edges)
{
    free(edges.array_of_edges);
    set_default_edges(edges);
}


static errors read_amount_edges(FILE *file, int &size)
{   
    if (!file)
        return ERR_RECEIVE_FILE;

    errors rc = SUCCESS;
    if (fscanf(file, "%d", &size) != 1)
        rc = ERR_AMOUNT;
    
    return rc;
}


static errors read_edge(FILE *file, edge_t &edge)
{

    if (!file)
        return ERR_RECEIVE_FILE;
    
    edge_t local_edge;
    errors rc = SUCCESS;
    if (fscanf(file, "%d %d", &local_edge.start_edge, &local_edge.end_edge) != 2)
        rc = ERR_EDGES_DATA;
    else
        edge = local_edge;
    return rc;
}


errors read_all_edges(FILE *file, edges_t &edges)
{
    if (!file)
    {
        qDebug() << "here!!!!";
        return ERR_RECEIVE_FILE;
    }
    errors rc = SUCCESS;
    if (read_amount_edges(file, edges.size))
    {
        qDebug() << "here!" << edges.size;
        rc = ERR_AMOUNT;
    }
    else
    {
        qDebug() << "1111" << edges.size;
        if (allocate_edges(edges))
            rc = ERR_ALLOCATION;
        else
        {
            for (size_t i = 0; i < edges.size; i++)
            {
                rc = read_edge(file, edges.array_of_edges[i]); 
                if (rc)
                {
                    free_edges(edges);
                    rc = ERR_EDGES_DATA;
                }
            }
        }

    } 

    return rc;
}