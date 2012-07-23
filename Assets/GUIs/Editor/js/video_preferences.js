define(function(require) {
    "use strict";

    var _ = require("underscore");
    var $ = require("jquery");
    var Backbone = require("backbone");
    var Mustache = require("mustache");
    var util = require("util");
    var template = require("text!html/video_preferences.html");

    var VideoPreferences = Backbone.Model.extend({
        defaults: {
            id: 0,
            bloom: false,
            normal_mapping: false,
            shadows: false,
            dof: false,
            ssao: false,
            resolution: [800, 500],
            fullscreen: false
        },
        initialize: function() {
            _.bindAll(this, "sync");
        },
        sync: function(method, model, options) {
            var t = this;
            if (method === "update") {
                util.Post("set_video_settings", model.toJSON(), function(data) {
                    model.trigger("save_complete");
                });
            } else if (method === "read") {
                util.Get("get_video_settings", function(data) {
                    console.log("\"", data.bloom, "\"");
                    model.set(data);
                    model.trigger("read_complete");
                });
            }
            // No Delete Here, and No Create
        }
    });

    var VideoPreferencesDialog = Backbone.View.extend({
        tagName: "div",
        events: {
            "click .cancel_button": "cancel",
            "click .apply_button": "apply"
        },
        initialize: function() {
            _.bindAll(this, "render");
            _.bindAll(this, "cancel");
            _.bindAll(this, "apply");
        },
        cancel: function() {
            this.$el.modal('hide');
            this.$el.remove();
        },
        apply: function() {
            var form_data = {};
            if (this.$('input:radio[name=bloom]:checked').val() === 'true') {
                form_data.bloom = true;
            } else {
                form_data.bloom = false;
            }

            if (this.$('input:radio[name=normal_mapping]:checked').val() === 'true') {
                form_data.normal_mapping = true;
            } else {
                form_data.normal_mapping = false;
            }

            if (this.$('input:radio[name=shadows]:checked').val() === 'true') {
                form_data.shadows = true;
            } else {
                form_data.shadows = false;
            }

            if (this.$('input:radio[name=ssao]:checked').val() === 'true') {
                form_data.ssao = true;
            } else {
                form_data.ssao = false;
            }

            if (this.$('input:radio[name=dof]:checked').val() === 'true') {
                form_data.dof = true;
            } else {
                form_data.dof = false;
            }
            this.model.bind("save_complete", this.cancel);
            this.model.save(form_data);
        },
        template_data: function() {
            var data = this.model.toJSON();
            return data;
        },
        render: function() {
            var out = Mustache.to_html(template, this.template_data());
            this.$el.html(out);
            if (this.model.get("bloom") === true) {
                console.log("Bloom ON");
                this.$("#bloom_on").attr('checked', 'checked');
            } else {
                console.log("Bloom OFF");
                this.$("#bloom_off").attr('checked', 'checked');
            }

            if (this.model.get("normal_mapping") === true) {
                console.log("Normal Mapping ON");
                this.$("#normal_mapping_on").attr('checked', 'checked');
            } else {
                console.log("Normal Mapping OFF");
                this.$("#normal_mapping_off").attr('checked', 'checked');
            }

            if (this.model.get("shadows") === true) {
                console.log("Shadows ON");
                this.$("#shadows_on").attr('checked', 'checked');
            } else {
                console.log("Shadows OFF");
                this.$("#shadows_off").attr('checked', 'checked');
            }

            if (this.model.get("ssao") === true) {
                console.log("SSAO ON");
                this.$("#ssao_on").attr('checked', 'checked');
            } else {
                console.log("SSAO OFF");
                this.$("#ssao_off").attr('checked', 'checked');
            }

            if (this.model.get("dof") === true) {
                console.log("Depth of Field ON");
                this.$("#dof_on").attr('checked', 'checked');
            } else {
                console.log("Depth of Field OFF");
                this.$("#dof_off").attr('checked', 'checked');
            }
            this.$el.modal({
                backdrop: 'static'
            });
        }
    });

    return {
        VideoPreferences: VideoPreferences,
        VideoPreferencesDialog: VideoPreferencesDialog
    };
});
