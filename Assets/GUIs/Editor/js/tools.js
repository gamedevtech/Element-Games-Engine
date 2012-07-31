define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");

    var tool_object_list = require("tool_object_list");
    var tool_object_editor = require("tool_object_editor");
    var object_data = require("object");

    var Tools = Backbone.View.extend({
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "render_left");
            _.bindAll(this, "render_right");
            _.bindAll(this, "add_tool");
            _.bindAll(this, "setup_tools");
            _.bindAll(this, "select_object");
            this.left_el = this.options.left_el;
            this.right_el = this.options.right_el;
            this.tool_views = {
                'left': [],
                'right': []
            };
            this.data = this.options.data;
            this.setup_tools();
        },
        add_tool: function(side, tool_view) {
            this.tool_views[side].push(tool_view);
        },
        setup_tools: function() {
            /* Objects */
            this.object_list = new tool_object_list.ObjectList({
                edge: 'right',
                collection: this.data.objects
            });
            this.add_tool('right', this.object_list);
            /* Object Editor */
            this.object_editor = new tool_object_editor.ObjectEditor({
                edge: 'right'
            });
            this.add_tool('right', this.object_editor);
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
            if (size > 0) {
                var i;
                for (i = 0; i < size; i += 1) {
                    view = this.tool_views['left'][i];
                    view.render();
                    this.left_el.append(view.$el);
                }
            } else {
                this.left_el.hide();
            }
        },
        render_right: function() {
            this.right_el.empty();
            var view;
            var size = this.tool_views['right'].length;
            if (size > 0) {
                var i;
                for (i = 0; i < size; i += 1) {
                    view = this.tool_views['right'][i];
                    view.render();
                    this.right_el.append(view.$el);
                }
            } else {
                this.right_el.hide();
            }
        },
        select_object: function(object_id) {
            this.object_editor.model = this.object_list.collection.get(object_id);
            this.object_editor.render();
        }
    });

    return {
        Tools: Tools
    };
});
