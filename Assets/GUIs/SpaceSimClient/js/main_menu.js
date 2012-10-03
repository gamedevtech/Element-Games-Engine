define(function(require) {
    var $ = require("jquery");
    var _ = require("underscore");
    var Backbone = require("backbone");

    var MainMenu = Backbone.View.extend({
        tagName: "div",
        className: "row centered",
        initialize: function() {
            _.bindAll(this, "render");
        },
        render: function() {
            var template = '<ul style="list-style: none; font-size: 24px;"><li><a href="#join">Join Universe</a></li><li>&nbsp;</li><li><a href="#options">Options</a></li><li>&nbsp;</li><li><a href="#quit">Quit</a></li></ul>';

            this.$el.html(template);

            return this;
        }
    });

    return {
        MainMenu: MainMenu
    };
});
