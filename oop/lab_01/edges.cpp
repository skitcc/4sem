#include "edges.h"

static error_t allocate_edges(edges_t &edges)
{
    if (edges.size <= 0)
        return ERR_EDGES_SIZE;
    
    error_t rc = SUCCESS;
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


static error_t read_amount_edges(FILE *file, int &size)
{   
    if (!file)
        return ERR_RECEIVE_FILE;

    error_t rc = SUCCESS;
    if (fscanf(file, "%d", &size) != 1)
        rc = ERR_AMOUNT;
    
    return rc;
}


static error_t read_edge(FILE *file, edge_t &edge)
{

    if (!file)
        return ERR_RECEIVE_FILE;
    
    edge_t local_edge;
    error_t rc = SUCCESS;
    if (fscanf(file, "%d %d", &local_edge.start_edge, &local_edge.end_edge) != 2)
        rc = ERR_EDGES_DATA;
    else
        edge = local_edge;
    return rc;
}


error_t read_all_edges(FILE *file, edges_t &edges)
{

    if (!file)
        return ERR_RECEIVE_FILE;

    error_t rc = SUCCESS;
    if (read_amount_edges(file, edges.size))
        rc = ERR_AMOUNT;
    else
    {
        if (allocate_edges(edges))
            rc = ERR_ALLOCATION;
        else
        {
            for (size_t i = 0; i < edges.size; i++)
            {
                error_t rc = read_edge(file, edges.array_of_edges[i]); 
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