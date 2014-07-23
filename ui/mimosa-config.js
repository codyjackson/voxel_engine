exports.config = {
  "modules": [
    "copy",
    "server",
    "jshint",
    "csslint",
    "require",
    "minify-js",
    "minify-css",
    "live-reload",
    "bower",
    "less",
    "server-template-compile"
  ],
  "server": {
    "views": {
      "compileWith": "handlebars",
      "extension": "hbs"
    }
  }
}