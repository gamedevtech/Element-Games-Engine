define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
	var Backbone = require("backbone");
    var domReady = require("domReady");
    var navigation = require("navigation");
    var tools = require("tools");
	var video_preferences = require("video_preferences");

	function open_video_preferences() {
		var prefs_model = new video_preferences.VideoPreferences();
		prefs_model.bind("read_complete", function() {
			if (prefs_model.get("bloom") === true) {
				console.log("Read Complete: true");
			} else {
				console.log("Read Complete: false");
			}
			var view = new video_preferences.VideoPreferencesDialog({
				model: prefs_model
			});
			view.render();
		});
		prefs_model.fetch();
	};

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
		nav.bind("video_prefs", open_video_preferences);
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
