define(function(require) {
    var _ = require("underscore");
    var $ = require("jquery");
    var domReady = require("domReady");

    domReady(function() {
        $("body").html("Made It!");
    });
});
