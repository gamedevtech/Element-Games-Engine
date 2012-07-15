define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var toolarea = require("toolarea");

    var ObjectList = toolarea.ToolArea.extend({
        data: {
            button_img: "img/object_label.png",
            tool_name: "object_list"
        },
        render_tool: function() {
            var el = this.elem;
            el.append("<h3>Test</h3>");
        }
    });

    return {
        ObjectList: ObjectList
    };
});
