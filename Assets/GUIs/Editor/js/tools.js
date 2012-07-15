define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");

    var tool_object_list = require("tool_object_list");

    var Tools = Backbone.View.extend({
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "render_left");
            _.bindAll(this, "render_right");
            _.bindAll(this, "add_tool");
            _.bindAll(this, "setup_tools");
            this.left_el = this.options.left_el;
            this.right_el = this.options.right_el;
            this.tool_views = {
                'left': [],
                'right': []
            };
            this.setup_tools();
        },
        add_tool: function(side, tool_view) {
            this.tool_views[side].push(tool_view);
        },
        setup_tools: function() {
            var object_list = new tool_object_list.ObjectList({
                edge: 'left'
            });
            this.add_tool('left', object_list);
        },
        render: function() {
            this.render_left();
            this.render_right();
            return this;
        },
        render_left: function() {
            this.left_el.empty();
            var view;
            var size = this.tool_views['left'].length;
            var i;
            for (i = 0; i < size; i += 1) {
                view = this.tool_views['left'][i];
                view.render();
                this.left_el.append(view.$el);
            }
        },
        render_right: function() {
            this.right_el.empty();
            var view;
            var size = this.tool_views['right'].length;
            var i;
            for (i = 0; i < size; i += 1) {
                view = this.tool_views['right'][i];
                view.render();
                this.right_el.append(view.$el);
            }
        }
    });

    return {
        Tools: Tools
    };
});
