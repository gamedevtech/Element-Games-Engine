define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");
    var toolarea = require("toolarea");
    var file_browser = require("file_browser");

    // ImageChooser, MaterialView does not belong here

    var ImagePreview = Backbone.View.extend({
        events: {
            "click .done_button": "done",
            "click .choose_button": "choose"
        },
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "done");
            _.bindAll(this, "choose");
            _.bindAll(this, "selected");
            this.file_path = this.options.file_path;
            this.title = this.options.title;
        },
        render: function() {
            this.$el.empty();
            var t = this;
            var m = $('<div class="modal">');
            var h = $('<div class="modal-header">');
            var b = $('<div class="modal-body">');
            var f = $('<div class="modal-footer">');

            h.append('<h4>' + this.title + '</h4>');

            b.append('<img src="../../../' + this.file_path + '" height="512px" />');
            b.append('<br>');
            b.append('<input type="text" value="' + this.file_path + '" disabled="disabled" />');
            b.append('<button class="choose_button btn-success">Choose...</button>');

            f.append('<button class="done_button btn btn-primary">Done</button>');

            m.append(h).append(b).append(f);

            this.$el.append(m);
            this.$el.modal({
                backdrop: 'static'
            }).on("hidden", function() {
                t.trigger("image_chosen", this.file_path);
            });
            return this;
        },
        choose: function() {
            var model = new file_browser.FileBrowserModel();
            var view = new file_browser.FileBrowser({
                model: model
            });
            view.bind("file_selected", this.selected);
            view.render();
            model.bind("read_complete", view.render_list);
            model.fetch();
            return false;
        },
        selected: function(path) {
            this.file_path = path.substr(1);
            this.render();
        },
        done: function() {
            this.$el.modal('hide');
            this.$el.remove();
            return false;
        }
    });

    var ImageChooser = Backbone.View.extend({
        events: {
            "click .image_chooser": "choose_image"
        },
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "image_chosen");
            _.bindAll(this, "choose_image");
            this.file_path = this.options.file_path;
            this.title = this.options.title;
        },
        image_chosen: function(p) {
            this.file_path = p;
            this.trigger("image_chosen", this.file_path);
        },
        choose_image: function(event) {
            event.preventDefault();
            var preview = new ImagePreview({
                title: this.title,
                file_path: this.file_path
            });
            preview.bind("image_chosen", this.image_chosen);
            preview.render();
            return false;
        },
        render: function() {
            this.$el.append('<a href="#" class="image_chooser"><img src="../../../' + this.file_path + '" height="50px" /></a>');
            return this;
        }
    });

    var MaterialView = Backbone.View.extend({
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "color_changed");
            _.bindAll(this, "decal_changed");
            _.bindAll(this, "normal_changed");
            _.bindAll(this, "height_changed");
            _.bindAll(this, "specular_changed");
        },
        render: function() {
            var html = "<strong>Material:</strong><br>";
            var color = this.model.get("color")
            var rgba = (color[0] * 255).toString() + ',';
            rgba += (color[1] * 255).toString() + ',';
            rgba += (color[2] * 255).toString() + ',';
            rgba += (color[3]).toString();
            html += '<div><strong>Color</strong></div><div class="input-append color" data-color="rgba(' + rgba + ')" data-color-format="rgba">&nbsp;<span class="add-on"><i style="background-color: rgba(' + rgba + ')"></i></span><input type="text" disabled="disabled" value="rgba(' + rgba + ')"></div>';
            this.$el.html(html);
            this.$(".color").colorpicker().on('changeColor', this.color_changed);

            var table = $("<table>");

            table.append("<tr><td><strong>Decal</strong></td><td><strong>Normal</strong></td><td><strong>Height</strong></td><td><strong>Specular</strong></td></tr>");

            this.decal = new ImageChooser({
                title: "Decal",
                file_path: this.model.get("decal")
            });
            this.decal.bind("image_chosen", this.decal_changed);
            this.decal.render();

            this.normal = new ImageChooser({
                title: "Normal",
                file_path: this.model.get("normal")
            });
            this.normal.bind("image_chosen", this.normal_changed);
            this.normal.render();

            this.height = new ImageChooser({
                title: "Height",
                file_path: this.model.get("height")
            });
            this.height.bind("image_chosen", this.height_changed);
            this.height.render();

            this.specular = new ImageChooser({
                title: "Specular",
                file_path: this.model.get("specular")
            });
            this.specular.bind("image_chosen", this.specular_changed);
            this.specular.render();

            var tr1 = $("<tr>");
            var decal_td = $("<td>");
            decal_td.append(this.decal.$el);
            tr1.append(decal_td);
            var normal_td = $("<td>");
            normal_td.append(this.normal.$el);
            tr1.append(normal_td);
            var height_td = $("<td>");
            height_td.append(this.height.$el);
            tr1.append(height_td);
            var specular_td = $("<td>");
            specular_td.append(this.specular.$el);
            tr1.append(specular_td);
            table.append(tr1);

            this.$el.append(table);
            return this;
        },
        color_changed: function(ev) {
            var color = ev.color.toRGB();
            var r = color.r / 255.0;
            var g = color.g / 255.0;
            var b = color.b / 255.0;
            var a = color.a;
            this.model.set({color: [r, g, b, a]});
        },
        decal_changed: function(path) {
            console.log("Decal: " + path);
            this.model.set({decal: path});
            this.render();
        },
        normal_changed: function(path) {
            this.model.set({normal: path});
            this.render();
        },
        height_changed: function(path) {
            this.model.set({height: path});
            this.render();
        },
        specular_changed: function(path) {
            this.model.set({specular: path});
            this.render();
        }
    });

    var ObjectEditor = toolarea.ToolArea.extend({
        data: {
            button_icon: "img/object_editor_icon.png",
            toop_tip: "Object Editor",
            tool_name: "object_editor"
        },
        render_materials: function() {
            var t = this;
            var el = this.elem;

            this.selected_material = null;
            var select = $('<select id="material">');
            var material_area = $("<div>");
            this.model.materials.each(function(material) {
                if (_.isNull(t.selected_material)) {
                    t.selected_material = material;
                    material_area = t.render_material(material);
                }
                select.append('<option value="' + material.id.toString() +
                    '">' + material.id.toString() + '</option>');
            });
            el.append(select);
            el.append(material_area);
            return this;
        },
        render_material: function(material) {
            var view = new MaterialView({
                model: material
            });
            view.render();
            return view.$el;
        },
        render_tool: function() {
            var el = this.elem;
            if (_.isUndefined(this.model)) {
                el.append("<h5>No Object Selected</h5>");
            } else {
                el.append("<h5>Name: " + this.model.id + "</h5>");
                this.render_materials();
            }
        },
        setup: function() {
            _.bindAll(this, "render_materials");
            _.bindAll(this, "render_material");
        }
    });

    return {
        ObjectEditor: ObjectEditor
    };
});
