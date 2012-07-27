/*global main_view:true*/
var main_view;

define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");
    var domReady = require("domReady");
    var navigation = require("navigation");
    var tools = require("tools");
    var video_preferences = require("video_preferences");
    var file_browser = require("file_browser");
    var util = require("util");
    var object_data = require("object");

    var Main = Backbone.View.extend({
        events: {
            "click #gui_toggle_link": "gui_toggle"
        },
        initialize: function() {
            _.bindAll(this, "gui_toggle");
            _.bindAll(this, "render");
            _.bindAll(this, "render_navigation");
            _.bindAll(this, "render_tools");

            _.bindAll(this, "open_video_preferences");
            _.bindAll(this, "select_model");
            _.bindAll(this, "model_selected");

            this.data = this.options.data;
            this.gui_state = true;
        },
        render: function() {
            this.render_navigation();
            this.render_tools();
            return this;
        },
        render_navigation: function() {
            this.nav = new navigation.Navigation({
                el: $("#menu"),
                menu_options: {
                    'main': [
                        'new_scene',
                        'open_scene'
                    ],
                    'object': [
                        'new_object',
                        'open_object',
                        'import_model'
                    ],
                    'options': [
                        'video_prefs'
                    ]
                },
                menu_names: {
                    'main': "Main",
                    'new_scene': "New",
                    'open_scene': "Open...",
                    'object': "Object",
                    'new_object': "New",
                    'open_object': "Open Object...",
                    'import_model': "Import Model...",
                    'options': "Options",
                    'video_prefs': "Video Preferences..."
                }
            });
            this.nav.bind("video_prefs", this.open_video_preferences);
            this.nav.bind("import_model", this.select_model);
            this.nav.render();
        },
        render_tools: function() {
            this.tools = new tools.Tools({
                left_el: this.$("#left"),
                right_el: this.$("#right"),
                data: {
                    objects: this.data.objects_collection
                }
            });
            this.tools.render();
        },
        gui_toggle: function() {
            if (this.gui_state === true) {
                this.$("#gui_toggle").html('<a href="javascript:;" id="gui_toggle_link"><i class="icon-white icon-eye-open"></i></a>');
                this.$("#body_container").fadeOut();
                this.gui_state = false;
            } else {
                this.$("#gui_toggle").html('<a href="javascript:;" id="gui_toggle_link"><i class="icon-white icon-eye-close"></i></a>');
                this.$("#body_container").fadeIn();
                this.gui_state = true;
            }
        },
        open_video_preferences: function() {
            var prefs_model = new video_preferences.VideoPreferences();
            prefs_model.bind("read_complete", function() {
                var view = new video_preferences.VideoPreferencesDialog({
                    model: prefs_model
                });
                view.render();
            });
            prefs_model.fetch();
        },
        select_model: function() {
            var model = new file_browser.FileBrowserModel();
            var view = new file_browser.FileBrowser({
                model: model
            });
            view.bind("file_selected", this.model_selected);
            view.render();
            model.bind("read_complete", view.render_list);
            model.fetch();
        },
        model_selected: function(path) {
            console.log("File Selected:");
            console.log(path);
            var t = this;
            util.Post("load_model", {path: path}, function(data) {
                console.log("Model Loaded!... i hope");
                t.data.objects_collection.fetch();
            });
        }
    });

    domReady(function() {
        // TODO: This should be fetching all init data at once!
        var objects_collection = new object_data.ObjectCollection();
        main_view = new Main({
            el: $("body"),
            data: {
                objects_collection: objects_collection
            }
        });
        objects_collection.bind("read_complete", function(c) {
            main_view.render();
        });
        objects_collection.fetch();
    });
});
