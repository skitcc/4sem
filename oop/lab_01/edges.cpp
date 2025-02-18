#include "edges.h"

error_t allocate_edges(edges_t &edges)
{
    if (edges.size <= 0)
        return ERR_EDGES_SIZE;
    else
    {

        edge_t *local_array_of_edges = (edge_t *)malloc(edges.size * sizeof(edge_t));
        if (!local_array_of_edges)
            return ERR_ALLOCATION;
        edges.array_of_edges = local_array_of_edges;
    }
    return SUCCESS;
}


static void set_default_edges(edges_t &edges)
{
    edges.array_of_edges = NULL;
    edges.size = 0;
}


static void free_edges(edges &edges)
{
    free(edges.array_of_edges);
    set_default_edges(edges);
}


error_t read_amount_edges(FILE *file, edges_t &edges)
{   
    if (!file)
        return ERR_RECEIVE_FILE;
    else if (fscanf(file, "%d", &edges.size) != 1)
        return ERR_AMOUNT;
    
    return SUCCESS;
}


error_t read_edge(FILE *file, edge_t &edge)
{

    if (!file)
        return ERR_RECEIVE_FILE;
    
    edge_t local_edge;

    if (fscanf(file, "%d %d", &local_edge.start_edge, &local_edge.end_edge) != 2)
        return ERR_EDGES_DATA;
    
    edge = local_edge;
    return SUCCESS;
}


error_t read_all_edges(FILE *file, edges_t &edges)
{

    if (!file)
        return ERR_RECEIVE_FILE;
    else if (read_amount_edges(file, edges))
        return ERR_AMOUNT;
    else if (allocate_edges(edges))
        return ERR_ALLOCATION;
    else
    {
        for (size_t i = 0; i < edges.size; i++)
        {
            error_t rc = read_edge(file, edges.array_of_edges[i]); 
            if (rc)
            {
                free_edges(edges);
                return ERR_EDGES_DATA;
            }
        }
    }

    return SUCCESS;
}