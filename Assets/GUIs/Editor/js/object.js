define(function(require) {
    "use strict";

    var $ = require("jquery");
    var _ = require("underscore");
    var Backbone = require("backbone");
    var util = require("util");

    var Material = Backbone.Model.extend({
        defaults: {
            color: [1.0, 1.0, 1.0, 1.0],
            specular: 1.0,
            decal: '',
            normal: '',
            height: '',
            specular: ''
        }
    });

    var Materials = Backbone.Collection.extend({
        model: Material
    });

    var ObjectModel = Backbone.Model.extend({
        initialize: function() {
            _.bindAll(this, "sync");
            _.bindAll(this, "prepare_data");
            this.prepare_data();
        },
        prepare_data: function() {
            this.materials = new Materials(this.get("materials"));
            this.unset("materials");
        },
        sync: function(method, model, options) {
            if (method === "create") {
                //
            } else if (method === "read") {
                util.Post("read_object", {id: model.id}, function(data) {
                    model.set(data);
                    model.trigger("read_complete", model);
                });
            } else if (method === "update") {
                //
            } else if (method === "delete") {
                //
            }
        }
    });

    var ObjectCollection = Backbone.Collection.extend({
        model: ObjectModel,
        initialize: function() {
            _.bindAll(this, "sync");
        },
        sync: function(method, collection, options) {
            if (method === "create") {
                //
            } else if (method === "read") {
                util.Get("read_objects", function(data) {
                    collection.reset(data);
                    collection.trigger("read_complete", collection);
                });
            } else if (method === "update") {
                //
            } else if (method === "delete") {
                //
            }
        }
    });

    return {
        ObjectModel: ObjectModel,
        ObjectCollection: ObjectCollection
    };
});
