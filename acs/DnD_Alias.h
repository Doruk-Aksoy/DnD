#ifndef DND_ALIAS_IN
#define DND_ALIAS_IN

// in case these limits become hard to satisfy, we need to simplify the weights by dividing with their gcd in their weight table before compiling
#define ALIAS_TABLE_SCALE 8192 // this hits a good point of base weights used being 1000, supports up to 256 different elements (size of weights etc.) without overflow
#define MAX_ALIAS_TABLE_SIZE 256 // 256 elements is a good enough size -- keep maximum weight at 1000

// Bounds the SUM of the weights -- not any single weight, not the entry count. See BuildAliasTable.
#define ALIAS_TABLE_MAX_TOTALWEIGHT (bcs::INT_MAX / ALIAS_TABLE_SCALE)

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

// Every bucket full, every alias itself: samples uniformly. Used when the weights cannot be
// scaled, so a bad table degrades to something defined AND stays built -- leaving it dirty made
// Pick rebuild on every single call.
void MakeAliasTableUniform(alias_table_T? table) {
    for(int i = 0; i < table.size; i++) {
        table.probability[i] = ALIAS_TABLE_SCALE;
        table.alias[i] = i;
    }

    table.isDirty = false;
}

void BuildAliasTable(alias_table_T? table) {
    int i, s, l;
    int totalWeight = 0;

    // One scratch array, not two: the small and large stacks partition the same index set, so
    // small can grow up from the bottom and large down from the top without ever meeting.
    int workset[MAX_ALIAS_TABLE_SIZE];
    int smallCount = 0;
    int largeTop = table.size;

    // first sum the weights to get total

    for(i = 0; i < table.size; i++)
        totalWeight += table.weights[i];

    if(totalWeight <= 0) {
        MakeAliasTableUniform(table);
        return;
    }

    // past this the remainder term below wraps and the table silently samples wrong, so fail loud
    if(totalWeight > ALIAS_TABLE_MAX_TOTALWEIGHT) {
        Log(s:"Alias table weight total ", d:totalWeight, s:" is over the ", d:ALIAS_TABLE_MAX_TOTALWEIGHT, s:" limit -- scale the weights down.");
        MakeAliasTableUniform(table);
        return;
    }

    // probability holds the scaled weights while they are being worked on. The old separate
    // scratch was copied into it verbatim, and every entry the loop revisits wants the updated
    // number anyway, so the copy was the only thing the third array bought.
    for(i = 0; i < table.size; i++) {
        // Quotient + remainder rather than wn * SCALE / totalWeight. Exact, since q * SCALE is
        // whole, but bounds on totalWeight instead of on max_weight * size.
        int wn = table.weights[i] * table.size;
        table.probability[i] = ((wn / totalWeight) * ALIAS_TABLE_SCALE) + (((wn % totalWeight) * ALIAS_TABLE_SCALE) / totalWeight);

        if(table.probability[i] < ALIAS_TABLE_SCALE)
            workset[smallCount++] = i;
        else
            workset[--largeTop] = i;
    }

    // main alias loop to assign them the proper bins

    while(smallCount > 0 && largeTop < table.size) {
        s = workset[--smallCount];
        l = workset[largeTop++];

        table.alias[s] = l;

        table.probability[l] = table.probability[l] - ALIAS_TABLE_SCALE + table.probability[s];

        if(table.probability[l] < ALIAS_TABLE_SCALE)
            workset[smallCount++] = l;
        else
            workset[--largeTop] = l;
    }

    // handle whatever is left to be max probability of 1.0

    while(smallCount > 0) {
        s = workset[--smallCount];

        table.probability[s] = ALIAS_TABLE_SCALE;
        table.alias[s] = s;
    }

    while(largeTop < table.size) {
        l = workset[largeTop++];

        table.probability[l] = ALIAS_TABLE_SCALE;
        table.alias[l] = l;
    }

    table.isDirty = false;
}

// Samples an index from the built alias table
int PickFromAliasTable(alias_table_T? table) {
    if(table.isDirty)
        BuildAliasTable(table);

    int col = random(0, table.size - 1);

    //Log(s:"pick ", d:col, s:" from ", d:table.size - 1);

    if(Random(0, ALIAS_TABLE_SCALE - 1) < table.probability[col])
        return col;
    return table.alias[col];
}

#endif