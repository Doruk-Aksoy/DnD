#ifndef DND_ALIAS_IN
#define DND_ALIAS_IN

// in case these limits become hard to satisfy, we need to simplify the weights by dividing with their gcd in their weight table before compiling
#define ALIAS_TABLE_SCALE 8192 // this hits a good point of base weights used being 1000, supports up to 256 different elements (size of weights etc.) without overflow
#define MAX_ALIAS_TABLE_SIZE 256 // 256 elements is a good enough size -- keep maximum weight at 1000

typedef struct {
    int size;
    bool isDirty;

    int[]? weights;
    int[]? probability;
    int[]? alias;
} alias_table_T;

#define SIZEOF_ALIASTABLE (SIZEOF_INT * 5) // bools are ints

void FreeAliasTable(alias_table_T? table) {
    // delete array of pointers like so
    bcs::arrDelete(table.weights);
    bcs::arrDelete(table.probability);
    bcs::arrDelete(table.alias);
    bcs::free(table);
    //Log(s:"Tables cleared");
}

alias_table_T* CreateAliasTable(int size) {
    if(size > MAX_ALIAS_TABLE_SIZE) {
        Log(s:"Tried to create alias table with size > ", d:MAX_ALIAS_TABLE_SIZE, s:" with request: ", d:size, s:"!");
        size = MAX_ALIAS_TABLE_SIZE;
    }

    alias_table_T* res = bcs::malloc(SIZEOF_ALIASTABLE);

    // allocate array of pointers like so -- arrNew has a 2nd parameter which determines how big the allocated element is going to be: Ex: struct of 3 ints, it'd be 3 etc.
    res.weights = bcs::arrNew(size);
    res.probability = bcs::arrNew(size);
    res.alias = bcs::arrNew(size);

    res.size = size;
    res.isDirty = true;

    return res;
}

// needs to be called right after modifying weights in it for rebuilding it later
void MarkAliasTableDirty(alias_table_T? table) {
    table.isDirty = true;
}

void BuildAliasTable(alias_table_T? table) {
    int i, s, l;
    int totalWeight = 0;

    int scaled[MAX_ALIAS_TABLE_SIZE];
    int small[MAX_ALIAS_TABLE_SIZE];
    int large[MAX_ALIAS_TABLE_SIZE];

    int smallCount = 0;
    int largeCount = 0;

    //Log(s:"Building table of size ", d:table.size);

    // first sum the weights to get total

    for(i = 0; i < table.size; i++)
        totalWeight += table.weights[i];

    //Log(s:"Weight sum: ", d:totalWeight);

    if(totalWeight <= 0)
        return;

    // now scale the weights
    for(i = 0; i < table.size; i++) {
        scaled[i] = table.weights[i] * table.size * ALIAS_TABLE_SCALE / totalWeight;

        //Log(s:"Scale for ", d:i, s:": ", d:scaled[i]);

        if(scaled[i] < ALIAS_TABLE_SCALE)
            small[smallCount++] = i;
        else
            large[largeCount++] = i;
    }

    // main alias loop to assign them the proper bins

    while(smallCount > 0 && largeCount > 0) {
        s = small[--smallCount];
        l = large[--largeCount];

        table.probability[s] = scaled[s];
        table.alias[s] = l;

        scaled[l] = scaled[l] - ALIAS_TABLE_SCALE + scaled[s];

        if(scaled[l] < ALIAS_TABLE_SCALE)
            small[smallCount++] = l;
        else
            large[largeCount++] = l;
    }

    // handle whatever is left to be max probability of 1.0

    while(largeCount > 0) {
        l = large[--largeCount];

        table.probability[l] = ALIAS_TABLE_SCALE;
        table.alias[l] = l;
    }

    while(smallCount > 0) {
        s = small[--smallCount];

        table.probability[s] = ALIAS_TABLE_SCALE;
        table.alias[s] = s;
    }

    table.isDirty = false;
}

// Samples an index from the built alias table
int PickFromAliasTable(alias_table_T? table) {
    if(table.isDirty)
        BuildAliasTable(table);

    int col = random(0, table.size - 1);

    if(Random(0, ALIAS_TABLE_SCALE - 1) < table.probability[col])
        return col;
    return table.alias[col];
}

#endif