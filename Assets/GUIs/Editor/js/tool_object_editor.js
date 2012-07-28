define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");
    var toolarea = require("toolarea");

    var Material = Backbone.Model.extend({
        defaults: {
            color: [1.0, 1.0, 1.0, 1.0]
        }
    });

    var Materials = Backbone.Collection.extend({
        model: Material
    });

    var ObjectEditor = toolarea.ToolArea.extend({
        data: {
            button_icon: "img/object_editor_icon.png",
            toop_tip: "Object Editor",
            tool_name: "object_editor"
        },
        render_tool: function() {
            var el = this.elem;
            if (_.isUndefined(this.model)) {
                el.append("<h5>No Object Selected</h5>");
            } else {
                el.append("<h5>Name: " + this.model.id + "</h5>");
                var html = '';
                if (!_.isUndefined(this.model.get("materials"))) {
                    // Show one for now.
                    var m = this.model.get("materials")[0];
                    var html = "<strong>Material:</strong><br>";
                    var rgba = (m.color[0] * 255).toString() + ',';
                    rgba += (m.color[1] * 255).toString() + ',';
                    rgba += (m.color[2] * 255).toString() + ',';
                    rgba += (m.color[3]).toString();
                    html += '<div><strong>Color</strong></div><div class="input-append color" data-color="rgba(' + rgba + ')" data-color-format="rgba">&nbsp;<span class="add-on"><i style="background-color: rgba(' + rgba + ')"></i></span><input type="text" disabled="disabled" value="rgba(' + rgba + ')"></div>';
                }
                el.append($(html));
                var t = this;
                el.find(".color").colorpicker();/*.on('changeColor', function(ev) {
                    this.model.set({materials: ev.color;
                });*/
            }
        },
        setup: function() {
            //_.bindAll(this, "save");
        }
    });

    return {
        ObjectEditor: ObjectEditor
    };
});
