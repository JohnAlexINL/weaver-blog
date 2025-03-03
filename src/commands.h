#define ls_command "ls md"
#define mkdir_command "mkdir -p ./static && mkdir -p ./tmp"
#define filename_weaver "weaver.js"
#define filename_router "blog-router.js"
#define filename_tmp_rss "./tmp/feed.xml"
#define filename_rss "./static/feed.xml"
#define filename_tmp_index "./tmp/index.html"
#define filename_index "./static/index.html"
#define filename_tmp_post "./tmp/%s"
#define filename_post "./static/%s"
#define filename_md "./md/%s"
#define tmp_post "./tmp/tmp.html"
#define post_wrap "cat ./style/header.html " tmp_post " > ./tmp/%s"
#define static_rm "rm -rf ./tmp " filename_rss filename_index filename_router
#define static_touch "touch " filename_tmp_rss " " filename_tmp_index " " filename_router
#define filename_tmp_multimarkdown "./tmp/mmd.html"
#define markdown_command "md --snippet ./md/%s > " filename_tmp_multimarkdown
#define move_command "mv ./tmp/* ./static && rm -rf ./tmp"
#define tmp_clean "rm ./tmp/mmd.html ./tmp/tmp.html"