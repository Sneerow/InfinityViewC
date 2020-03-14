
Graph remove_successors(Graph g, int i)
{
	assert(i >= 0);
	assert(i < g->n);
	int new_len;
	int father;

	for(int j = 0; j < g->n; j++)
	{
		for(int k = 0; k < g->alist[j]->len-1; k++)
		{
			if(g->alist[j]->list[k] == i)
			{
				father = j;
			}
		}
	}

	int oldlen = g->alist[father]->len;
	g->alist[father] = realloc(g->alist[father], sizeof(struct successors) + sizeof(int) * (g->alist[father]->len + g->alist[i]->len));

	for(int z = oldlen; z < oldlen + g->alist[i]->len; z++)
	{
		g->alist[father]->list[z] = g->alist[i]->list[z - oldlen] - 1;
	}

	while (i < (g->n - 1))
	{

		if (g->alist[i]->len >= g->alist[i + 1]->len)
		{
			new_len = g->alist[i]->len;
			g->alist[i + 1] = realloc(g->alist[i + 1], 
		sizeof(struct successors) + sizeof (int) * (new_len - 1));
		}
		else
		{
			new_len = g->alist[i + 1]->len;
			g->alist[i + 1] = realloc(g->alist[i], 
		sizeof(struct successors) + sizeof (int) * (new_len - 1));
		}

		struct successors *stock = malloc(sizeof(struct successors)
		+ sizeof (int) * (new_len - 1));
		*stock = *g->alist[i + 1];
		*g->alist[i + 1] = *g->alist[i];
		*g->alist[i] = *stock;
		free(stock);
		i++;

	}

	free(g->alist[g->n -1]);
	g->n--;
	g = realloc(g, sizeof(struct graph)                                                                  + sizeof(struct successors *) * (g->n));

	for(int y = father + 1; y < g->n; y++)
	{
		for(int f = 0; f < g->alist[y]->len; f++)
		{
			g->alist[f]->list[f]--;
		}
	}

	return g;
}

