#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "ext/file.c"

#define nohost_error "expected a hostname\n"
#define argv_error "unexpected arguments. expected a hostname\n"
#define usage_error "blog v0.0.0, (C) Modula.dev 2025\nAvailable under the AGPLv3\nusage: blog <hostname>\n"
#define green "\e[0;32m"
#define white "\e[0;m"

// ================================ Include Formatter Strings
                                    // RSS
#include "strings/feed_post.h"      // fmt_feed_post
#include "strings/feed_header.h"    // fmt_feed_header
#include "strings/feed_footer.h"    // fmt_feed_footer
                                    // HTML
#include "strings/index_header.h"   // fmt_index_header
#include "strings/index_footer.h"   // fmt_index_footer
#include "strings/index_entry.h"    // fmt_index_entry
#include "strings/blog_post.h"      // fmt_blog_post
                                    // JavaScript
#include "strings/js_header.h"      // fmt_js_header
#include "strings/js_footer.h"      // fmt_js_footer
#include "strings/js_entry.h"       // fmt_js_entry
#include "strings/weaver.h"         // depend_weaver


#include "src/commands.h"
#include "src/iterator.c"
#include "src/parser.c"
#include "src/runner.c"

char FILE_BUFFER[4*1024*1024];
char WORK_BUFFER[4*1024*1024];
int main(int argc, char **argv) {
    // host should be the first argument
    if ( argc == 1) { printf(nohost_error); exit(1); }
    if ( argc != 2) { printf(argv_error); exit(1); }
    if ( argv[1][0]=='-') { printf(usage_error); exit(0); }
    char *host = argv[1];
    // Set up an iterator for files in `ls md`
        run(ls_command); iterator_t ls = iterator_init(run_buffer);
        if (&ls == &badIter) { printf("unable to initialize iterator from \"%s\"\n", ls_command); exit(1); }
        printf("[blog] init: %d posts to parse\n", ls.items);

    // OPEN TAGS: feed.xml, index.html, router.js
        run(static_rm);    
        run(mkdir_command);
        run(static_touch);
        sprintf(WORK_BUFFER, fmt_feed_header, host, host);
        file_write(filename_tmp_rss, WORK_BUFFER, sizeof(WORK_BUFFER));
        file_write(filename_tmp_index, fmt_index_header, sizeof(fmt_index_header));
        file_write(filename_router, fmt_js_header, sizeof(fmt_js_header));
        file_write(filename_weaver, depend_weaver, sizeof(depend_weaver));
        printf("[blog] init: ~tmp ready\n");

    // Then iteratively generate the blog
    char *ref; char md_path[255];
    while ( ref = iter_next(&ls) ) {
        // Read the source markdown
        snprintf(md_path, sizeof(md_path), filename_md, ref);
        int file_len = file_read(md_path, FILE_BUFFER, sizeof(FILE_BUFFER)-1); if (file_len <= 0) continue;
        char *content = FILE_BUFFER;
        printf("    " green "%s " white " %d bytes >> ", ref, file_len);
        // Ask for it to be parsed into a blogpost_t
        blogpost_t blogpost = parse_blogpost(host, ref, content);
        // Append to the RSS Feed
        int rss_len = blogpost_rss(WORK_BUFFER, blogpost);
        file_append(filename_tmp_rss, WORK_BUFFER, rss_len);
        // Append to the Index HTML
        int index_len = blogpost_link(WORK_BUFFER, blogpost);
        file_append(filename_tmp_index, WORK_BUFFER, index_len);
        // Append to the Router
        int js_len = blogpost_weaver(WORK_BUFFER, blogpost);
        file_append(filename_router, WORK_BUFFER, js_len);
        // Have Multimarkdown convert the MD to HTML
        sprintf(WORK_BUFFER, markdown_command, ref);
        run(WORK_BUFFER);
        file_len = file_read(filename_tmp_multimarkdown, FILE_BUFFER, sizeof(FILE_BUFFER)-1); FILE_BUFFER[file_len] = '\0';
        memcpy(blogpost.content, FILE_BUFFER, file_len);
        // Render the static HTML
        int body_len = blogpost_render(WORK_BUFFER, blogpost);
        file_write(tmp_post, WORK_BUFFER, body_len);
        // then tack on the user-defined header
        sprintf(WORK_BUFFER, post_wrap, blogpost.fname, blogpost.fname);
        run(WORK_BUFFER);
        // Finish our printf
        printf("%d bytes content, %d metadata\n", body_len, rss_len + index_len + js_len);
    }
    // free the iterator after we're done with it
    iter_free(&ls);
    // CLOSE TAGS: feed.xml, index.html, router.js
    file_append(filename_tmp_rss, fmt_feed_footer, sizeof(fmt_feed_footer));
    file_append(filename_tmp_index, fmt_index_footer, sizeof(fmt_index_footer));
    file_append(filename_router, fmt_js_footer, sizeof(fmt_js_footer));

    // Finish up by moving everything out of the tmp folder and killing it
    run(tmp_clean);
    run(move_command);
}