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

        var gui_state = true;
        $("#gui_toggle_link").live('click', function() {
            if (gui_state) {
                $("#gui_toggle").html('<a href="#" id="gui_toggle_link"><i class="icon-white icon-eye-open"></i></a>');
                $("#body_container").fadeOut();
                gui_state = false;
            } else {
                $("#gui_toggle").html('<a href="#" id="gui_toggle_link"><i class="icon-white icon-eye-close"></i></a>');
                $("#body_container").fadeIn();
                gui_state = true;
            }
        });

        /* Tools */
        var tools_view = new tools.Tools({
            left_el: $("#left"),
            right_el: $("#right")
        });
        tools_view.render();
    });
});
