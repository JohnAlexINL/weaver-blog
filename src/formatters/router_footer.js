};

function get(name){
    if (blogposts[name]) { return blogposts[name](); }
    return undefined;
}

module.exports = { get }