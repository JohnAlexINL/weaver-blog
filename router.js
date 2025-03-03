const blogposts = {
    "feed.xml": ()=> { return weaver.serveFile("text/xml", "./static/feed.xml")},
    "index": ()=> { return weaver.serveFile("text/html", "./static/index.html")},
    "style.css": ()=> { return weaver.serveFile("text/css", "./style/style.css")},
    "HelloWorld": ()=> { return weaver.serveFile("text/html", "./static/HelloWorld")},
    };

function get(name){
    if (blogposts[name]) { return blogposts[name](); }
    return undefined;
}

module.exports = { get }