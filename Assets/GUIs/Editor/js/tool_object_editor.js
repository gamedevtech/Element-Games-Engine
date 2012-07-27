define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var toolarea = require("toolarea");

    var ObjectEditor = toolarea.ToolArea.extend({
        data: {
            button_icon: "icon-globe",
            toop_tip: "Object Editor",
            tool_name: "object_editor"
        },
        render_tool: function() {
            var el = this.elem;
            if (_.isUndefined(this.model)) {
                el.append("<h5>No Object Selected</h5>");
            } else {
                el.append("<h5>" + this.model.id + "</h5>");
            }
        },
        setup: function() {
            //
        }
    });

    return {
        ObjectEditor: ObjectEditor
    };
});
