define(function(require) {
    "use strict";

    var $ = require("jquery");
    var _ = require("underscore");
    var Backbone = require("backbone");
    var util = require("util");
    var template = require("text!html/file_browser.html");

    var File = Backbone.Model.extend({
        defaults: {
            "type": "file",
            "path": ""
        }
    });

    var Directory = Backbone.Collection.extend({
        model: File
    });

    var FileBrowserModel = Backbone.Model.extend({
        defaults: {
            current_path: ""
        },
        initialize: function() {
            _.bindAll(this, "sync");
            this.collection = new Directory();
        },
        sync: function(method, model, options) {
            if (method === "read") {
                util.Post("list_directory", {path: model.get("current_path")}, function(data) {
                    model.collection.reset(data);
                    model.trigger("read_complete");
                });
            }
        }
    });

    var FileBrowser = Backbone.View.extend({
        events: {
            "click .cancel_button": "cancel",
            "click .file_link": "file_click"
        },
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "render_list");
            _.bindAll(this, "cancel");
            _.bindAll(this, "file_click");

            this.model.bind("change", this.render_list);
            this.filter = this.options.filter;
            if (_.isUndefined(this.filter)) {
                this.filter = "";
            }
        },
        cancel: function() {
            this.$el.modal('hide');
            this.$el.remove();
            return false;
        },
        file_click: function(event) {
            var elem = $(event.currentTarget);
            var path = elem.attr("path");
            var type = elem.attr("type");

            var current_path = this.model.get("current_path");
            current_path += "/" + path;
            this.model.set({current_path: current_path});

            if (type === "dir") {
                this.model.fetch();
            } else {
                this.trigger("file_selected", current_path);
                this.cancel();
            }
            return false;
        },
        render_list: function() {
            this.$(".modal-body").empty();

            var pic_regexp = new RegExp("(png|jpg|jpeg|bmp|tga)$", "i");
            var model_regexp = new RegExp("(dae|3ds|ego)$", "i");

            var well = $('<div class="well">');
            var ul = $('<ul style="list-style: none;">');
            var i, file, li, use;
            li = $('<li class="file_link">');
            li.attr("type", "up");
            li.attr("path", "..");
            li.append('<span class="path">..</span>');
            ul.append(li);
            this.model.collection.comparator = function(model) {
                return model.get("path");
            }
            this.model.collection.sort();
            var dirs = this.model.collection.where({type: "dir"});
            _.each(dirs, function(dir) {
                var li = $("<li class=\"file_link\">");
                li.attr("type", "dir");
                li.append('<i class="icon-folder-close icon-white"></i>&nbsp;');
                li.attr("path", dir.get("path"));
                li.append('<span class="path">' + dir.get("path") + '</span>');
                ul.append(li);
            }, this);
            for (i = 0; i < this.model.collection.length; i += 1) {
                use = false;
                file = this.model.collection.at(i);
                li = $("<li class=\"file_link\">");
                li.attr("type", file.get("type"));
                if (file.get("type") !== "dir") {
                    if (file.get("path").match(pic_regexp)) {
                        if (this.filter === "" || this.filter === "images") {
                            use = true;
                            li.append('<img src="../../../' + this.model.get("current_path") + '/' + file.get("path") + '" height="12px" />&nbsp;');
                        }
                    } else if (file.get("path").match(model_regexp)) {
                        if (this.filter === "" || this.filter === "models") {
                            use = true;
                            li.append('<i class="icon-plane icon-white"></i>&nbsp;');
                        }
                    } else {
                        if (this.filter === "") {
                            use = true;
                            li.append('<i class="icon-question-sign icon-white"></i>&nbsp;');
                        }
                    }
                }
                if (use === true) {
                    li.attr("path", file.get("path"));
                    li.append('<span class="path">' + file.get("path") + '</span>');
                    ul.append(li);
                }
            }

            well.append(ul);
            this.$(".modal-body").append(well);
        },
        render: function() {
            this.$el.empty();
            this.$el.html(template);
            this.render_list();
            this.$el.modal({
                backdrop: 'static'
            });
            return this;
        }
    });

    return {
        File: File,
        Directory: Directory,
        FileBrowserModel: FileBrowserModel,
        FileBrowser: FileBrowser
    };
});
