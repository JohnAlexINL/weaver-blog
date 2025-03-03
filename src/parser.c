typedef struct {
    char category[255]; char title[255]; char author[255]; 
    char link[255]; char fname[255]; char description[255];
    char timestamp[255]; char *source; char *content;
} blogpost_t;

char *trim(char *s) { 
    while (isspace((unsigned char)*s)) s++;  
    if (!*s) return s; 
    char *e = s + strlen(s) - 1; 
    while (e > s && isspace((unsigned char)*e)) *e-- = '\0'; 
    return s; 
}

int timestamp(const char *str) {
    int value = 0;
    while (*str && *str != '\n') {
        if (isdigit((unsigned char)*str)) { value = value * 10 + (*str - '0'); } str++;
    }   return value;
}


bool parse_metadata(char *buffer, blogpost_t *post) {
    post->source = buffer;
    char *line = strtok(buffer, "\n");
    int in_metadata = 0;
    while (line) {
        char *trimmed = trim(line);
        if (strcmp(trimmed, "---") == 0) { 
            if (in_metadata) { post->content = strtok(NULL, ""); return true; } 
            in_metadata = 1; line = strtok(NULL, "\n"); continue; 
        }
        if (!in_metadata) { line = strtok(NULL, "\n"); continue; }
        char *sep = strchr(trimmed, ':'); if (!sep) { line = strtok(NULL, "\n"); continue; }
        *sep = '\0'; char *key = trim(trimmed), *value = trim(sep + 1);
        if (strcmp(key, "category") == 0) strncpy(post->category, value, sizeof(post->category) - 1);
        else if (strcmp(key, "title") == 0) strncpy(post->title, value, sizeof(post->title) - 1);
        else if (strcmp(key, "author") == 0) strncpy(post->author, value, sizeof(post->author) - 1);
        else if (strcmp(key, "timestamp") == 0) strncpy(post->timestamp, value, sizeof(post->timestamp) - 1);
        else if (strcmp(key, "description") == 0) strncpy(post->description, value, sizeof(post->description) - 1);
        line = strtok(NULL, "\n");
    }
    return false;
}

void parse_debug_post(blogpost_t *post){
    printf("    title: " green "%s " white "\n", post->title);
    printf("    author: " green "%s " white "\n", post->author);
    printf("    link: " green "%s " white "\n", post->link);
    printf("    fname: " green "%s " white "\n", post->fname);
    printf("    timestamp: " green "%s " white "\n", post->timestamp);
}

int blogpost_uri(char *host, char *ref, blogpost_t *post) {
    char *dot = strrchr(ref, '.');
    size_t len = dot ? (size_t)(dot - ref) : strlen(ref);
    for (size_t i = 0; i < len; i++) 
        post->fname[i] = isspace((unsigned char)ref[i]) ? '_' : ref[i];
    post->fname[len] = '\0';
    sprintf(post->link, "%s/%s", host, post->fname);
    return strlen(post->link);
}

blogpost_t parse_blogpost(char *host, char *ref, char *content) {
    blogpost_t post = {
        .title = "No Title",
        .author = "Unknown",
        .category = "Unknown",
    }; 
    blogpost_uri(host, ref, &post);
    if (!parse_metadata(content, &post)) return post;
    return post;
}

int blogpost_link(char *buffer, blogpost_t post) {
    sprintf(buffer, fmt_index_entry,
        post.link, post.title
    );
    return strlen(buffer);
}

int blogpost_rss(char *buffer, blogpost_t post) {
    sprintf(buffer, fmt_feed_post, 
        post.link, post.link, post.title,
        post.author, post.category, post.description,
        post.timestamp
    );
    return strlen(buffer);
}

int blogpost_render(char *buffer, blogpost_t post) {
    sprintf(buffer, fmt_blog_post,
        post.author, post.description, post.timestamp,
        post.link, post.title, post.content
    );
    return strlen(buffer);
}

int blogpost_weaver(char *buffer, blogpost_t post) {
    sprintf(buffer, fmt_js_entry, post.fname, post.fname);
    return strlen(buffer);
}