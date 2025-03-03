const blogposts = {
    "feed.xml": ()=> { return weaver.serveFile("text/xml", "./static/feed.xml")},
    "index": ()=> { return weaver.serveFile("text/html", "./static/index.html")},
    "style.css": ()=> { return weaver.serveFile("text/css", "./style/style.css")},
