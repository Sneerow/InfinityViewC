#define _XOPEN_SOURCE >= 700
#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"
/* basic directed graph type */
/* the implementation uses adjacency lists
 * represented as variable-length arrays */

/* these arrays may or may not be sorted: if one gets long enough
 * and you call graph_has_edge on its source, it will be */

struct graph
{
    int n; /* number of vertices */
    int m; /* number of edges */
    char label;
    struct successors
    {
        int d;
        int is_word; /* number of successors */
        char label;
        int len;        /* number of slots in array */
        char is_sorted; /* true if list is already sorted */
        int list[1];    /* actual list of successors */
    } * alist[1];
};
struct file_content
{
    char **text;
    size_t nb_line;
};
/* create a new graph with n vertices labeled 0..n-1 and no edges */
Graph graph_create(int n, char *word)
{
    Graph g;
    int i;

    g = malloc(sizeof(struct graph) + sizeof(struct successors *) * (n - 1));
    assert(g);

    g->n = n;
    g->m = 0;
    char *k;
    for (k = word, i = 0; i < n; i++, k++)
    {
        if (k == word)
        {
            g->label = *k;
        }
        g->alist[i] = malloc(sizeof(struct successors));
        assert(g->alist[i]);
        g->alist[i]->label = *k;
        g->alist[i]->is_word = i == n - 1;
        g->alist[i]->d = 0;
        g->alist[i]->len = 1;
        g->alist[i]->is_sorted = 1;
    }
    for (i = 1; i < n; i++)
    {
        graph_add_edge(g, i - 1, i);
    }

    return g;
}

Graph add_successors(Graph g)
{
    g = realloc(
        g, sizeof(struct graph) + sizeof(struct successors *) * (g->n));
    assert(g->alist[g->n]);

    g->alist[g->n] = malloc(sizeof(struct successors));
    assert(g->alist[g->n]);

    g->alist[g->n]->d = 0;
    g->alist[g->n]->len = 1;
    g->alist[g->n]->is_sorted = 1;
    g->n++;

    return g;
}

Graph remove_successors(Graph g, int i)
{
    assert(i >= 0);
    assert(i < g->n);
    int new_len;
    int father;
    int oldpos = i;

    // find the parent node of i
    for (int j = 0; j < g->n; j++)
    {
        for (int k = 0; k < g->alist[j]->len - 1; k++)
        {
            if (g->alist[j]->list[k] == i)
            {
                father = j;
            }
        }
    }

    // realloc size of adjlist father in order to add it i adjlist
    int oldlen = g->alist[father]->len;
    g->alist[father] = realloc(g->alist[father],
        sizeof(struct successors)
            + sizeof(int) * (g->alist[father]->len + g->alist[i]->len));
    for (int z = oldlen; z < oldlen + g->alist[i]->len; z++)
    {
        g->alist[father]->list[z] = g->alist[i]->list[z - oldlen] - 1;
    }

    // swap the i node to the end of the list
    while (i < (g->n - 1))
    {
        if (g->alist[i]->len >= g->alist[i + 1]->len)
        {
            new_len = g->alist[i]->len;
            g->alist[i + 1] = realloc(g->alist[i + 1],
                sizeof(struct successors) + sizeof(int) * (new_len - 1));
        }
        else
        {
            new_len = g->alist[i + 1]->len;
            g->alist[i + 1] = realloc(g->alist[i],
                sizeof(struct successors) + sizeof(int) * (new_len - 1));
        }

        struct successors *stock
            = malloc(sizeof(struct successors) + sizeof(int) * (new_len - 1));
        *stock = *g->alist[i + 1];
        *g->alist[i + 1] = *g->alist[i];
        *g->alist[i] = *stock;
        free(stock);
        i++;
    }

    // free the last node
    free(g->alist[g->n - 1]);
    g->n--;
    g = realloc(
        g, sizeof(struct graph) + sizeof(struct successors *) * (g->n));

    // replace all the nedge in the list
    for (int y = 0; y < g->n; y++)
    {
        for (int f = 0; f < g->alist[y]->len; f++)
        {
            if (g->alist[f]->list[f] > oldpos)
            {
                g->alist[f]->list[f]--;
            }
        }
    }

    return g;
}

/* free all space used by graph */
void graph_destroy(Graph g)
{
    int i;

    for (i = 0; i < g->n; i++)
        free(g->alist[i]);
    free(g);
}

/* add an edge to an existing graph */
void graph_add_edge(Graph g, int u, int v)
{
    assert(u >= 0);
    assert(u < g->n);
    assert(v >= 0);
    assert(v < g->n);

    /* do we need to grow the list? */
    while (g->alist[u]->d >= g->alist[u]->len)
    {
        g->alist[u]->len *= 2;
        g->alist[u] = realloc(g->alist[u],
            sizeof(struct successors) + sizeof(int) * (g->alist[u]->len - 1));
    }

    /* now add the new sink */
    g->alist[u]->list[g->alist[u]->d++] = v;
    g->alist[u]->is_sorted = 0;

    /* bump edge count */
    g->m++;
}

void graph_remove_edge(Graph g, int u, int v)
{
    // check if u and v exist
    assert(u >= 0);
    assert(u < g->n);
    assert(v >= 0);
    assert(v < g->n);

    // find pos of v in adjlist u
    int vpos;
    for (int b = 0; b < g->alist[u]->len; b++)
    {
        if (v == g->alist[u]->list[b])
        {
            vpos = b;
            break;
        }
    }

    // swap
    int temp;
    for (int w = vpos; w < g->alist[u]->len - 1; w++)
    {
        temp = g->alist[u]->list[w];
        g->alist[u]->list[w] = g->alist[u]->list[w + 1];
        g->alist[u]->list[w + 1] = temp;
    }

    g->alist[u]->len--;
    g->alist[u]->d--;
    g->m--;
    g->alist[u] = realloc(g->alist[u],
        sizeof(struct successors) + sizeof(int) * (g->alist[u]->len - 1))
}

/* return the number of vertices in the graph */
int graph_vertex_count(Graph g)
{
    return g->n;
}

/* return the number of vertices in the graph */
int graph_edge_count(Graph g)
{
    return g->m;
}

/* return the out-degree of a vertex */
int graph_out_degree(Graph g, int source)
{
    assert(source >= 0);
    assert(source < g->n);

    return g->alist[source]->d;
}

/* when we are willing to call bsearch */
#define BSEARCH_THRESHOLD (10)

static int intcmp(const void *a, const void *b)
{
    return *((const int *)a) - *((const int *)b);
}

/* return 1 if edge (source, sink) exists), 0 otherwise */
int graph_has_edge(Graph g, int source, int sink)
{
    int i;

    assert(source >= 0);
    assert(source < g->n);
    assert(sink >= 0);
    assert(sink < g->n);

    if (graph_out_degree(g, source) >= BSEARCH_THRESHOLD)
    {
        /* make sure it is sorted */
        if (!g->alist[source]->is_sorted)
        {
            qsort(g->alist[source]->list, g->alist[source]->d, sizeof(int),
                intcmp);
        }

        /* call bsearch to do binary search for us */
        return bsearch(&sink, g->alist[source]->list, g->alist[source]->d,
                   sizeof(int), intcmp)
               != 0;
    }
    else
    {
        /* just do a simple linear search */
        /* we could call lfind for this, but why bother? */
        for (i = 0; i < g->alist[source]->d; i++)
        {
            if (g->alist[source]->list[i] == sink)
                return 1;
        }
        /* else */
        return 0;
    }
}

/* invoke f on all edges (u,v) with source u */
/* supplying data as final parameter to f */
void graph_foreach(Graph g, int source,
    void (*f)(Graph g, int source, int sink, void *data), void *data)
{
    int i;

    assert(source >= 0);
    assert(source < g->n);

    for (i = 0; i < g->alist[source]->d; i++)
    {
        f(g, source, g->alist[source]->list[i], data);
    }
}
File_content load_file(char *filename)
{
    FILE *fp;
    size_t len = 0;
    ssize_t read;
    size_t nb_line = 1;
    File_content fc = malloc(sizeof(struct file_content));
    fc->text = malloc(sizeof(char *));
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&fc->text[nb_line - 1], &len, fp) != -1))
    {
        // printf("Retrieved line of length %zu:\n", len);
        // printf("%s\n", fc->text[nb_line - 1]);
        nb_line++;
        fc->text = realloc(fc->text, sizeof(char *) * nb_line);
        fc->text[nb_line - 1] = malloc(1024);
        if (fc->text == NULL)
            exit(EXIT_FAILURE);
    }

    fclose(fp);
    fc->nb_line = nb_line - 1;
    return fc;
}

Graph build_graph(File_content file)
{
    char *current = file->text;
    size_t count = 0;
    int is_added = 0;
    Graph *graph_list = malloc(sizeof(Graph));
    graph_list = graphe_create(strlen(current), current);
    count++;
    for (char *k = file->text + 1; k - file->text < file->count; k++)
    {
        for (size_t p = 0; p < count; p++)
        {
            if (graph_list[p]->label == *k)
            {
                // TODO add_word(graph_list[p], k);
                is_added = 1;
            }
        }
        if (!is_added)
        {
            graph_list
                = realloc(graph_list, sizeof(graph_list) + sizeof(Graph));
            graph_list[count] = grah_create(strlen(k), k);
            count++;
        }
        is_added = 0;
    }
    return graph_list;
}

void free_text(File_content file)
{
    for (size_t i = 0; i <= file->nb_line; i++)
    {
        free(file->text[i]);
    }
    free(file->text);
    free(file);
}
void print_text(File_content file)
{
    for (size_t i = 0; i < file->nb_line - 1; i++)
    {
        printf("line = %s", file->text[i]);
    }
}
void print_pointeur(File_content file)
{
    for (size_t i = 0; i < file->nb_line; i++)
    {
        printf("line = %p", file->text[i]);
    }
}

int main(int argc, char **argv)
{
    (void)argc;
    File_content file = load_file(argv[1]);
    print_text(file);
    free_text(file);
    return 0;
}

