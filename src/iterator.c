typedef struct {
    char **list;
    char *src;
    int index;
    int items;
} iterator_t;

const iterator_t badIter = { 0 };

iterator_t iterator_init(char *str) {
    if (!str || !*str) return (iterator_t){ 0 };
    int source_len = strlen(str);
    char *source = malloc((source_len + 1) * sizeof(char));
    if (!source) return (iterator_t){ 0 };
    memcpy(source, str, source_len);
    source[source_len] = '\0';

    int iter_items = 0;
    char *s = source;
    bool in_word = false;
    while (*s) {
        if (!isspace((unsigned char)*s)) { if (!in_word) { iter_items++; in_word = true; }} 
        else { *s = '\0'; in_word = false; }
        s++;
    }
    if (iter_items == 0) { free(source); return (iterator_t){ 0 }; }

    char **iter = malloc(iter_items * sizeof(char *));
    if (!iter) { free(source); return (iterator_t){ 0 }; }

    s = source;
    int i = 0;
    while (i < iter_items) { if (*s) { iter[i++] = s; while (*s) s++; } s++; }

    return (iterator_t){ .list = iter, .src = source, .index = 0, .items = iter_items };
}

char *iter_next(iterator_t *this) {
    if (this->index >= this->items) return NULL;
    return this->list[this->index++];
}

void iter_free(iterator_t *this) {
    if (this->list) free(this->list);
    if (this->src) free(this->src);
    *this = (iterator_t){ 0 };
}
