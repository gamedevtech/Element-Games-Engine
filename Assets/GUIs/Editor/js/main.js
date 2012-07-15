define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var domReady = require("domReady");
    var navigation = require("navigation");
    var tools = require("tools");

    domReady(function() {
        /* Navigation Menu */
        var nav = new navigation.Navigation({
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
                ]
            },
            menu_names: {
                'main': "Main",
                'new_scene': "New",
                'open_scene': "Open...",
                'object': "Object",
                'new_object': "New",
                'open_object': "Open Object...",
                'import_model': "Import Model..."
            }
        });
        nav.render();
        $("#gui_enable_link").click(function() {
            $("#gui_enable").fadeOut();
            $("#body_container").fadeIn();
        });

        /* Tools */
        var tools_view = new tools.Tools({
            left_el: $("#left"),
            right_el: $("#right")
        });
        tools_view.render();
    });
});
