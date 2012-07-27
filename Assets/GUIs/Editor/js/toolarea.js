define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");

    var ToolArea = Backbone.View.extend({
        events: {
            "click .toggle_button": "toggle"
        },
        data: {
            button_img: 'default.png',
            tool_name: 'Default'
        },
        initialize: function() {
            _.bindAll(this, "toggle");
            _.bindAll(this, "render");
            _.bindAll(this, "render_tool");
            _.bindAll(this, "setup");
            this.collapsed = true;
            this.edge = this.options.edge;
            this.setup();
        },
        setup: function() {
            //
        },
        toggle: function(event) {
            event.preventDefault();
            this.collapsed = !(this.collapsed);
            this.$(".tool_area").animate({width: 'toggle'}, 150);
        },
        render: function() {
            this.$el.empty();
            var html = '<a href="javascript:;" class="vertical_text toggle_button"><i class="icon-white ' + this.data.button_icon + '"></i></a>';
            if (this.edge === 'left') {
                html += '<div class="tool_area tool_left"></div>';
            } else if (this.edge === 'right') {
                html += '<div class="tool_area tool_right"></div>';
            }
            this.$el.html(html);
            var placement = 'left';
            if (this.edge === 'left') {
                placement = 'right';
            }
            this.$(".toggle_button").tooltip({
                animation: true,
                placement: placement,
                delay: {show: 500, hide: 100},
                title: this.data.tool_tip
            });
            this.elem = this.$('.tool_area');
            this.render_tool();
            return this;
        },
        render_tool: function() {
            // Override Me
        }
    });

    return {
        ToolArea: ToolArea
    };
});
