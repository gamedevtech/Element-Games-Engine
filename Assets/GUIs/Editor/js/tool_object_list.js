define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var toolarea = require("toolarea");

    var ObjectList = toolarea.ToolArea.extend({
        data: {
            button_icon: "icon-list",
            toop_tip: "Objects List",
            tool_name: "object_list"
        },
        render_tool: function() {
            var el = this.elem;
            var i, obj;
            var ul = $("<ul>");
            for (i = 0; i < this.collection.length; i += 1) {
                obj = this.collection.at(i);
                ul.append("<li>" + obj.id + "</li>");
            }
            el.append(ul);
        },
        setup: function() {
            this.collection.bind("change", this.render());
        }
    });

    return {
        ObjectList: ObjectList
    };
});
