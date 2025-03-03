# Weaver-Blog Compiler

The expected behavior of this program is that it's run in
a directory containing `md` and `style` subdirectories.

`md` should contain markdown files (with metadata)
that should be converted into HTML and RSS blog posts.
`style` should contain a `header.html` and `style.css`,
which will respectivelly be concatenated onto
each blog post and served as `/style.css` for the blog.


## Calling Convention

This program expects to be given a hostname,
which it uses to help create hyperlinks and
generate the RSS feed.

It will generate a `blog-router.js` that can be
included into any [weaver](https://modula.dev/weaver) project,
which exposes a `get` function
that given a filename will either return
a `serveFile` or `undefined`.

It will also generate relevant files in
a local directory, `static`, which
the blog router will serve content from.

> [!WARNING]
> The subdirectory `tmp` will be created
> and deleted during the runtime of the program

## Purpose & Usage

Idk I wanted to make a blog but without
needing a bunch of new dependencies on my website
so I wrote this.

When you write a new blogpost,
you'll run the program in project directory.
Presumably, you'll attach some kind of frontend
to this so you can write your blog and have
this program called automagically, and
have its generated router called out
from your website's actual service.