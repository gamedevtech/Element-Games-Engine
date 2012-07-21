define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");

    var Navigation = Backbone.View.extend({
        events: {
            "click .menu_entry": "menu_entry_click"
        },
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "build_menu");
            _.bindAll(this, "menu_entry_click");
            this.menu_options = this.options.menu_options;
            this.menu_names = this.options.menu_names;
        },
        render: function() {
            this.$el.empty();
            var html = '';
            html += '<div class="btn-toolbar">';
            html += this.build_menu();
            html += '</div>';
            this.$el.html(html);
            return this;
        },
        menu_entry_click: function(event) {
            event.preventDefault();
            var entry = $(event.currentTarget).data("entry");
            console.log("Triggering: ", entry);
            this.trigger(entry);
        },
        build_menu: function() {
            var out = '';
            var menu_entry;
            var menu_entry_name;
            var item_entry;
            var item_entry_name;
            var items_size;
            var i;
            for (menu_entry in this.menu_options) {
                if (this.menu_options.hasOwnProperty(menu_entry)) {
                    if (this.menu_names.hasOwnProperty(menu_entry)) {
                        menu_entry_name = this.menu_names[menu_entry];
                    } else {
                        menu_entry_name = menu_entry;
                    }
                    out += '<div class="btn-group">';
                    out += '<button class="btn btn-inverse btn-mini dropdown-toggle" data-toggle="dropdown">' + menu_entry_name + '<span class="caret"></span></button>';
                    out += '<ul class="dropdown-menu">';
                    items_size = this.menu_options[menu_entry].length;
                    for (i = 0; i < items_size; i += 1) {
                        item_entry = this.menu_options[menu_entry][i];
                        if (this.menu_names.hasOwnProperty(item_entry)) {
                            item_entry_name = this.menu_names[item_entry];
                        } else {
                            item_entry_name = item_entry;
                        }
                        out += '<li><a href="#" class="menu_entry" data-entry="' + item_entry + '">' + item_entry_name + '</a></li>';
                    }
                    out += '</ul>';
                    out += '</div>';
                }
            }
            return out;
        }
    });

    return {
        Navigation: Navigation
    };
});
